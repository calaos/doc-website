+++
title = 'Under the hood'
date = 2026-07-25T10:00:00Z
weight = 30
summary = "Disk layout, restore points and how rollback works."
+++

## Disk layout

The installer partitions the machine differently depending on its boot mode.

| Mode | Partitions |
|---|---|
| **UEFI** (GPT table) | EFI partition in FAT32 (1 → 513 MiB) · swap (513 MiB → 2.5 GiB) · Btrfs root (2.5 GiB → 100%) |
| **BIOS** (MBR table) | swap (1 → 2049 MiB) · Btrfs root (2049 MiB → 100%) |

The root partition is labelled `calaos-os` and mounted with the options `noatime,compress=zstd`.

On UEFI, GRUB is installed in *removable* mode — into `/EFI/BOOT/BOOTX64.EFI` — to stay compatible with firmwares that handle custom boot entries poorly.

## Btrfs subvolumes

The installer creates the following layout:

```text
@                              main volume
@/.snapshots                   restore points
@/.snapshots/1/snapshot        the actually booted root (default subvolume)
@/boot/grub
@/opt
@/root
@/srv
@/tmp
@/usr/local
@/var/cache
@/var/log
@/var/spool
@/var/tmp
```

Copy-on-write is disabled on the `@/var/*` subvolumes, whose contents change constantly.

The system boots not from `@` but from **`@/.snapshots/1/snapshot`**, set as the default subvolume. This indirection is what lets `snapper` point the root at another restore point without moving anything.

## What a restore point covers

A restore point photographs **the root subvolume**. Separately mounted subvolumes are not part of it.

| Covered | Not covered |
|---|---|
| The system and installed programs | `/var/log` — the logs |
| System configuration files | `/var/cache`, `/var/spool`, `/var/tmp` |
| **`/mnt/calaos` — your whole Calaos configuration** | `/opt`, `/srv`, `/root`, `/usr/local` |
| | `/boot/grub` |

{{% notice warning %}}
`/mnt/calaos` is an **ordinary folder inside the root subvolume**, not a separate subvolume. It is therefore captured by restore points and **replayed during a rollback**.

This is why a rollback also undoes the Calaos configuration made since the chosen date. See [Rolling back]({{% relref "calaos_os/rollback" %}}).
{{% /notice %}}

Conversely, since `/var/log` sits on a separate subvolume, the logs **survive** a rollback: this is what allows an incident to be analysed after working around it.

## How restore points are created

Two mechanisms feed them:

- **dpkg hooks** — a restore point is created before every package operation, so on every `calaos-os upgrade` as well as every `apt`;
- **`snapper-timeline.timer`** — periodic points, independent of updates.

Point number 1 is created during installation, with the description *First Root Filesystem Created at Installation*.

## Automatic retention

`snapper-cleanup.timer` does the housekeeping according to the configuration set at installation:

| Setting | Value | Meaning |
|---|---|---|
| `NUMBER_LIMIT` | 10 | Maximum number of numbered points kept |
| `NUMBER_MIN_AGE` | 5400 | Minimum age in seconds (1 h 30) before a point may be deleted |
| `TIMELINE_LIMIT_DAILY` | 14 | Daily points kept |
| `TIMELINE_LIMIT_WEEKLY` | 4 | Weekly points kept |
| `TIMELINE_LIMIT_MONTHLY` | 6 | Monthly points kept |
| `TIMELINE_LIMIT_YEARLY` | 2 | Yearly points kept |

To read or change these values:

```sh
snapper -c root get-config
snapper -c root set-config "NUMBER_LIMIT=10"
```

## Inspecting restore points

```sh
snapper list                 # list with dates and descriptions
snapper delete NUMBER        # delete one specific point
btrfs filesystem usage /     # real disk usage
```

{{% notice tip %}}
The space taken by a restore point is not visible immediately: when created it costs almost nothing, then grows as the system diverges from the photographed state. `btrfs filesystem usage /` gives the real picture, where `df` can be misleading.
{{% /notice %}}

## Boot menu

`grub-btrfsd` watches the restore points and automatically regenerates the submenu offered at boot. The GRUB timeout is deliberately kept at 5 seconds to leave time to choose.

When you boot on a restore point, the root is mounted **read-only**, and an overlay layer receives temporary writes. Nothing is therefore modified until you confirm.

## How rollback works

The `calaos_rollback` script:

1. checks that the machine really booted on a read-only restore point — otherwise it stops with the message `Not booted from a readonly btrfs snapshot`;
2. reads the restore point number from `/proc/cmdline`, in the form `subvol=@/.snapshots/<number>/snapshot`;
3. temporarily mounts the main volume and the restore points subvolume;
4. calls `snapper rollback <number>`, which makes that point the new root;
5. unmounts everything.

All that remains is to reboot onto the restored root.

The script is started either by the Calaos Home interface, which automatically displays a window in this context, or by hand from a terminal.

## Maintenance tasks

| Unit | Purpose |
|---|---|
| `snapper-timeline.timer` | Creates the periodic restore points |
| `snapper-cleanup.timer` | Applies the retention policy |
| `grub-btrfsd.service` | Updates the boot menu |
| `btrfs-scrub@.timer` | Checks data integrity |
| `fstrim.timer` | SSD maintenance |

## Sources

This information comes from the scripts in the [calaos-container](https://github.com/calaos/calaos-container) repository, in particular `scripts/calaos_install` and `scripts/calaos_rollback`.
