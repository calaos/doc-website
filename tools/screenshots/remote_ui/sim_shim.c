/*
 * LD_PRELOAD shim for the calaos_remote_ui SDL simulator.
 *   - SDL_RenderPresent: keeps a copy of the last presented frame.
 *   - A unix-socket thread (SIMSHOT_SOCK) takes line commands:
 *       ping            -> "ok <frames presented> <crc32 of the last frame>"
 *       tap X Y [ms]    -> injects press/release as SDL mouse events
 *       shot PATH       -> writes the last frame as PNG, "ok W H"
 *     and answers each one with a single line.
 */
#define _GNU_SOURCE
#include <SDL2/SDL.h>
#include <dlfcn.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <zlib.h>

static pthread_mutex_t frame_lock = PTHREAD_MUTEX_INITIALIZER;
static uint8_t *frame;
static int frame_w, frame_h;
static unsigned long frame_count;
static unsigned long frame_crc;
static SDL_Window *main_window;

SDL_Window *SDL_CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags)
{
    static SDL_Window *(*real)(const char *, int, int, int, int, Uint32);
    if (!real)
        real = dlsym(RTLD_NEXT, "SDL_CreateWindow");
    SDL_Window *win = real(title, x, y, w, h, flags);
    if (!main_window)
        main_window = win;
    return win;
}

void SDL_RenderPresent(SDL_Renderer *renderer)
{
    static void (*real)(SDL_Renderer *);
    if (!real)
        real = dlsym(RTLD_NEXT, "SDL_RenderPresent");

    int w = 0, h = 0;
    if (SDL_GetRendererOutputSize(renderer, &w, &h) == 0 && w > 0 && h > 0) {
        pthread_mutex_lock(&frame_lock);
        if (!frame || w != frame_w || h != frame_h) {
            free(frame);
            frame = malloc((size_t)w * h * 4);
            frame_w = w;
            frame_h = h;
        }
        /* ABGR8888 is R,G,B,A in memory on little-endian: what PNG RGBA wants. */
        if (frame && SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ABGR8888, frame, w * 4) == 0)
            frame_crc = crc32(0, frame, (uInt)((size_t)w * h * 4)), frame_count++;
        pthread_mutex_unlock(&frame_lock);
    }
    real(renderer);
}

static void put32(uint8_t *p, uint32_t v)
{
    p[0] = v >> 24; p[1] = v >> 16; p[2] = v >> 8; p[3] = v;
}

static int write_chunk(FILE *f, const char *type, const uint8_t *data, uint32_t len)
{
    uint8_t hdr[8];
    put32(hdr, len);
    memcpy(hdr + 4, type, 4);
    uint32_t crc = crc32(0, hdr + 4, 4);
    if (len)
        crc = crc32(crc, data, len);
    uint8_t tail[4];
    put32(tail, crc);
    return fwrite(hdr, 1, 8, f) == 8 && (!len || fwrite(data, 1, len, f) == len) &&
           fwrite(tail, 1, 4, f) == 4;
}

/* Alpha is forced opaque: the window framebuffer carries no meaningful alpha. */
static int write_png(const char *path, const uint8_t *rgba, int w, int h)
{
    size_t row = (size_t)w * 3 + 1;
    uint8_t *raw = malloc(row * h);
    if (!raw)
        return -1;
    for (int y = 0; y < h; y++) {
        uint8_t *d = raw + y * row;
        const uint8_t *s = rgba + (size_t)y * w * 4;
        *d++ = 0;
        for (int x = 0; x < w; x++, s += 4, d += 3) {
            d[0] = s[0]; d[1] = s[1]; d[2] = s[2];
        }
    }
    uLongf clen = compressBound(row * h);
    uint8_t *comp = malloc(clen);
    if (!comp || compress2(comp, &clen, raw, row * h, 6) != Z_OK) {
        free(raw); free(comp);
        return -1;
    }
    FILE *f = fopen(path, "wb");
    int ok = f != NULL;
    if (ok) {
        static const uint8_t sig[8] = {0x89, 'P', 'N', 'G', 13, 10, 26, 10};
        uint8_t ihdr[13] = {0};
        put32(ihdr, w);
        put32(ihdr + 4, h);
        ihdr[8] = 8;
        ihdr[9] = 2;
        ok = fwrite(sig, 1, 8, f) == 8 && write_chunk(f, "IHDR", ihdr, 13) &&
             write_chunk(f, "IDAT", comp, clen) && write_chunk(f, "IEND", NULL, 0);
        ok = fclose(f) == 0 && ok;
    }
    free(raw); free(comp);
    return ok ? 0 : -1;
}

static void push_mouse(Uint32 type, int x, int y)
{
    SDL_Event e;
    memset(&e, 0, sizeof e);
    Uint32 win = main_window ? SDL_GetWindowID(main_window) : 0;
    e.type = type;
    if (type == SDL_MOUSEMOTION) {
        e.motion.windowID = win;
        e.motion.x = x;
        e.motion.y = y;
    } else {
        e.button.windowID = win;
        e.button.button = SDL_BUTTON_LEFT;
        e.button.state = type == SDL_MOUSEBUTTONDOWN ? SDL_PRESSED : SDL_RELEASED;
        e.button.clicks = 1;
        e.button.x = x;
        e.button.y = y;
    }
    SDL_PushEvent(&e);
}

static void handle(int fd, char *line)
{
    char reply[512];
    int x, y, ms = 80;
    char path[1024];

    if (!strcmp(line, "ping")) {
        pthread_mutex_lock(&frame_lock);
        snprintf(reply, sizeof reply, "ok %lu %lx\n", frame_count, frame_crc);
        pthread_mutex_unlock(&frame_lock);
    } else if (sscanf(line, "tap %d %d %d", &x, &y, &ms) >= 2) {
        push_mouse(SDL_MOUSEMOTION, x, y);
        usleep(30000);
        push_mouse(SDL_MOUSEBUTTONDOWN, x, y);
        usleep(ms * 1000);
        push_mouse(SDL_MOUSEBUTTONUP, x, y);
        snprintf(reply, sizeof reply, "ok\n");
    } else if (!strncmp(line, "shot ", 5)) {
        snprintf(path, sizeof path, "%s", line + 5);
        pthread_mutex_lock(&frame_lock);
        if (!frame)
            snprintf(reply, sizeof reply, "err no frame yet\n");
        else if (write_png(path, frame, frame_w, frame_h) != 0)
            snprintf(reply, sizeof reply, "err cannot write the PNG\n");
        else
            snprintf(reply, sizeof reply, "ok %d %d\n", frame_w, frame_h);
        pthread_mutex_unlock(&frame_lock);
    } else {
        snprintf(reply, sizeof reply, "err unknown command\n");
    }
    if (write(fd, reply, strlen(reply)) < 0)
        perror("sim_shim write");
}

static void *server_thread(void *arg)
{
    const char *sock_path = arg;
    int srv = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr = {.sun_family = AF_UNIX};
    snprintf(addr.sun_path, sizeof addr.sun_path, "%s", sock_path);
    unlink(sock_path);
    if (srv < 0 || bind(srv, (struct sockaddr *)&addr, sizeof addr) || listen(srv, 1)) {
        perror("sim_shim bind");
        return NULL;
    }
    for (;;) {
        int fd = accept(srv, NULL, NULL);
        if (fd < 0)
            continue;
        char buf[1024];
        size_t n = 0;
        for (;;) {
            ssize_t r = read(fd, buf + n, 1);
            if (r <= 0)
                break;
            if (buf[n] == '\n') {
                buf[n] = 0;
                handle(fd, buf);
                n = 0;
            } else if (n < sizeof buf - 1) {
                n++;
            }
        }
        close(fd);
    }
    return NULL;
}

__attribute__((constructor)) static void shim_init(void)
{
    const char *sock = getenv("SIMSHOT_SOCK");
    if (!sock)
        return;
    pthread_t t;
    pthread_create(&t, NULL, server_thread, strdup(sock));
    pthread_detach(t);
}
