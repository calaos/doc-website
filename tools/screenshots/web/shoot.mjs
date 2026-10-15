// Takes the climate screenshots of calaos-web-app against the demo house.
// Usage: node shoot.mjs <web-app-tree> <fixtures-dir> <dist-dir> <out-dir>
// Starts the tree's mock server (with the demo fixtures) and `vite preview`
// itself, once per scenario.
import { spawn } from 'node:child_process';
import { cpSync, mkdirSync, mkdtempSync, readFileSync, rmSync, symlinkSync, writeFileSync } from 'node:fs';
import { createRequire } from 'node:module';
import { tmpdir } from 'node:os';
import { join, resolve } from 'node:path';

const [tree, fixtures, dist, out] = process.argv.slice(2).map((p) => resolve(p));
const MOCK_PORT = Number(process.env.MOCK_PORT) || 5601;
const APP_PORT = Number(process.env.APP_PORT) || 4601;
const { chromium } = createRequire(join(tree, 'package.json'))('@playwright/test');
const fr = JSON.parse(readFileSync(join(tree, 'app/src/i18n/fr.json'), 'utf8')).climate;

mkdirSync(out, { recursive: true });

async function waitFor(url, ms = 60_000) {
    const end = Date.now() + ms;
    for (;;) {
        try {
            if ((await fetch(url)).status < 500) return;
        } catch {
            /* not up yet */
        }
        if (Date.now() > end) throw new Error(`timeout waiting for ${url}`);
        await new Promise((r) => setTimeout(r, 200));
    }
}

async function withStack(scenario, fn) {
    // The mock reads fixtures/home.json next to its own files: run a copy.
    const dir = mkdtempSync(join(tmpdir(), `doc5-web-${scenario}-`));
    cpSync(join(tree, 'mock-server'), join(dir, 'mock-server'), { recursive: true });
    cpSync(join(fixtures, scenario, 'home.json'), join(dir, 'mock-server/fixtures/home.json'));
    symlinkSync(join(tree, 'node_modules'), join(dir, 'node_modules'));
    writeFileSync(join(dir, 'package.json'), '{"type":"module"}');
    const env = { ...process.env, PORT: String(MOCK_PORT), VITE_CONFIG_NATIVE_IGNORE_WARNING: 'true' };
    const mock = spawn('node', [join(dir, 'mock-server/index.mjs')], { env, stdio: 'ignore' });
    const preview = spawn(
        join(tree, 'node_modules/.bin/vite'),
        ['preview', '--outDir', dist, '--port', String(APP_PORT), '--strictPort'],
        { cwd: tree, env: { ...env, CALAOS_SERVER: `http://127.0.0.1:${MOCK_PORT}` }, stdio: 'ignore' },
    );
    try {
        await waitFor(`http://127.0.0.1:${MOCK_PORT}/control`);
        await waitFor(`http://localhost:${APP_PORT}/`);
        await fn();
    } finally {
        mock.kill();
        preview.kill();
        rmSync(dir, { recursive: true, force: true });
    }
}

async function session(browser, viewport, room) {
    const ctx = await browser.newContext({ viewport, locale: 'fr-FR', deviceScaleFactor: 1 });
    const page = await ctx.newPage();
    await page.goto(`http://localhost:${APP_PORT}/`);
    await page.getByLabel(/utilisateur|identifiant|user/i).first().fill('demo');
    await page.locator('input[type=password]').fill('demo');
    await page.locator('button[type=submit]').click();
    await page.locator('.room-tile').first().waitFor();
    await page.locator('.room-tile').filter({ hasText: room }).first().click();
    await page.locator('.io-row').first().waitFor();
    return page;
}

const row = (page, name) =>
    page.locator('.io-row').filter({ has: page.locator('.io-row__name', { hasText: name }) });

async function openSettings(page, name) {
    await row(page, name).click();
    await page.locator('.climate-detail').waitFor();
}

async function shoot(page, file) {
    await page.waitForTimeout(500); // fade-in of panel and dialogs
    await page.mouse.move(0, 0);
    await page.screenshot({ path: join(out, file) });
    console.log('wrote', file);
}

const browser = await chromium.launch();
const wide = { width: 1280, height: 800 };
try {
    await withStack('showcase', async () => {
        const page = await session(browser, { width: 1280, height: 900 }, 'Salon');
        await shoot(page, 'web-room-list.png');
        await page.context().close();
    });

    await withStack('house', async () => {
        let page = await session(browser, wide, 'Salon');
        await openSettings(page, 'Clim salon');
        await shoot(page, 'web-settings-panel.png');

        await page.locator('.climate-detail__pill').click();
        await page.getByRole('dialog', { name: fr.modeTitle }).waitFor();
        await shoot(page, 'web-mode-choice.png');
        await page.keyboard.press('Escape');

        await page.locator('.climate-detail').getByRole('button', { name: fr.more }).click();
        await page.getByRole('dialog', { name: fr.more }).waitFor();
        await shoot(page, 'web-more-settings.png');
        await page.keyboard.press('Escape');

        // Walk the low end up to the min_range stop: the next press is refused.
        const raiseLow = page.getByRole('button', { name: fr.raiseLow.replace('{name}', 'Clim salon') });
        for (let i = 0; i < 6; i++) {
            await raiseLow.click();
            await page.waitForTimeout(150);
        }
        await page.locator('.climate-detail__stop').waitFor();
        await shoot(page, 'web-range-stop.png');
        await page.context().close();

        page = await session(browser, wide, 'Chambre');
        await openSettings(page, 'Clim chambre');
        await shoot(page, 'web-settings-single.png');
        await page.context().close();

        page = await session(browser, wide, 'Séjour');
        await openSettings(page, 'Plancher séjour');
        await shoot(page, 'web-thermostat.png');
        await page.context().close();
    });

    await withStack('fault', async () => {
        const page = await session(browser, wide, 'Bureau');
        await openSettings(page, 'Radiateur bureau');
        await shoot(page, 'web-fault.png');
        await page.context().close();
    });
} finally {
    await browser.close();
}
