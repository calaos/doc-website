+++
title = 'Sous le capot'
date = 2026-07-25T10:00:00Z
weight = 30
summary = "Organisation du disque, points de restauration et retour en arrière."
+++

## Organisation du disque

L'installeur partitionne la machine différemment selon son type de démarrage.

| Mode | Partitions |
|---|---|
| **UEFI** (table GPT) | partition EFI en FAT32 (1 → 513 Mio) · swap (513 Mio → 2,5 Gio) · racine Btrfs (2,5 Gio → 100 %) |
| **BIOS** (table MBR) | swap (1 → 2049 Mio) · racine Btrfs (2049 Mio → 100 %) |

La partition racine porte l'étiquette `calaos-os` et est montée avec les options `noatime,compress=zstd`.

En UEFI, GRUB est installé en mode *removable* — dans `/EFI/BOOT/BOOTX64.EFI` — afin de rester compatible avec les BIOS qui gèrent mal les entrées de démarrage personnalisées.

## Sous-volumes Btrfs

L'installeur crée l'arborescence suivante :

```text
@                              volume principal
@/.snapshots                   points de restauration
@/.snapshots/1/snapshot        racine réellement démarrée (sous-volume par défaut)
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

La copie sur écriture est désactivée sur les sous-volumes `@/var/*`, dont le contenu change constamment.

Le système démarre non pas sur `@` mais sur **`@/.snapshots/1/snapshot`**, défini comme sous-volume par défaut. C'est cette indirection qui permet à `snapper` de faire pointer la racine vers un autre point de restauration sans rien déplacer.

## Ce qui est couvert par un point de restauration

Un point de restauration photographie **le sous-volume racine**. Les sous-volumes montés séparément n'en font pas partie.

| Couvert | Non couvert |
|---|---|
| Le système et les programmes installés | `/var/log` — les journaux |
| Les fichiers de configuration système | `/var/cache`, `/var/spool`, `/var/tmp` |
| **`/mnt/calaos` — toute votre configuration Calaos** | `/opt`, `/srv`, `/root`, `/usr/local` |
| | `/boot/grub` |

{{% notice warning %}}
`/mnt/calaos` est un **dossier ordinaire du sous-volume racine**, et non un sous-volume séparé. Il est donc capturé par les points de restauration et **rejoué lors d'un retour en arrière**.

C'est la raison pour laquelle un retour en arrière annule aussi la configuration Calaos réalisée depuis la date choisie. Voir [Revenir en arrière]({{% relref "calaos_os/rollback" %}}).
{{% /notice %}}

À l'inverse, `/var/log` étant sur un sous-volume séparé, les journaux **survivent** à un retour en arrière : c'est ce qui permet d'analyser un incident après l'avoir contourné.

## Création des points de restauration

Deux mécanismes les alimentent :

- **les hooks dpkg** — un point de restauration est créé avant chaque opération sur les paquets, donc à chaque `calaos-os upgrade` comme à chaque `apt` ;
- **`snapper-timeline.timer`** — des points périodiques, indépendants des mises à jour.

Le point numéro 1 est créé pendant l'installation, avec la description *First Root Filesystem Created at Installation*.

## Rétention automatique

`snapper-cleanup.timer` fait le ménage selon la configuration posée à l'installation :

| Réglage | Valeur | Signification |
|---|---|---|
| `NUMBER_LIMIT` | 10 | Nombre maximum de points numérotés conservés |
| `NUMBER_MIN_AGE` | 5400 | Âge minimum en secondes (1 h 30) avant qu'un point puisse être supprimé |
| `TIMELINE_LIMIT_DAILY` | 14 | Points quotidiens conservés |
| `TIMELINE_LIMIT_WEEKLY` | 4 | Points hebdomadaires conservés |
| `TIMELINE_LIMIT_MONTHLY` | 6 | Points mensuels conservés |
| `TIMELINE_LIMIT_YEARLY` | 2 | Points annuels conservés |

Pour consulter ou modifier ces valeurs :

```sh
snapper -c root get-config
snapper -c root set-config "NUMBER_LIMIT=10"
```

## Consulter les points de restauration

```sh
snapper list                 # liste avec dates et descriptions
snapper delete NUMÉRO        # supprimer un point précis
btrfs filesystem usage /     # occupation réelle du disque
```

{{% notice tip %}}
L'espace occupé par un point de restauration n'est pas visible immédiatement : au moment de sa création il ne coûte quasiment rien, puis il grossit à mesure que le système diverge de l'état photographié. `btrfs filesystem usage /` donne l'image réelle de l'occupation, là où `df` peut être trompeur.
{{% /notice %}}

## Menu de démarrage

`grub-btrfsd` surveille les points de restauration et régénère automatiquement le sous-menu proposé au démarrage. Le délai de GRUB est volontairement maintenu à 5 secondes pour laisser le temps de choisir.

Quand vous démarrez sur un point de restauration, la racine est montée **en lecture seule**, et une couche overlay accueille les écritures temporaires. Rien n'est donc modifié tant que vous n'avez pas confirmé.

## Mécanisme du retour en arrière

Le script `calaos_rollback` :

1. vérifie qu'on a bien démarré sur un point de restauration en lecture seule — sinon il s'arrête avec le message `Not booted from a readonly btrfs snapshot` ;
2. lit le numéro du point de restauration dans `/proc/cmdline`, sous la forme `subvol=@/.snapshots/<numéro>/snapshot` ;
3. monte temporairement le volume principal et le sous-volume des points de restauration ;
4. appelle `snapper rollback <numéro>`, qui fait de ce point la nouvelle racine ;
5. démonte le tout.

Il reste à redémarrer pour repartir sur la racine restaurée.

Le script est lancé soit par l'interface Calaos Home, qui affiche automatiquement une fenêtre dans ce contexte, soit à la main depuis un terminal.

## Tâches d'entretien

| Unité | Rôle |
|---|---|
| `snapper-timeline.timer` | Crée les points de restauration périodiques |
| `snapper-cleanup.timer` | Applique la politique de rétention |
| `grub-btrfsd.service` | Met à jour le menu de démarrage |
| `btrfs-scrub@.timer` | Vérifie l'intégrité des données |
| `fstrim.timer` | Entretien des SSD |

## Sources

Ces informations proviennent des scripts du dépôt [calaos-container](https://github.com/calaos/calaos-container), en particulier `scripts/calaos_install` et `scripts/calaos_rollback`.
