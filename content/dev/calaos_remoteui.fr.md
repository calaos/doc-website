+++
title = 'Calaos Remote UI'
date = 2026-03-06T20:17:52Z
weight = 35
summary = "Le firmware des écrans muraux, et son portage sur de nouvelles cartes."
+++

## Rôle

Remote UI est le firmware des **écrans muraux autonomes** : de petits panneaux tactiles encastrés dans la maison, qui se connectent au serveur par le réseau et affichent une interface configurée depuis Calaos Installer.

Contrairement à [Calaos Home]({{% relref "dev/calaos_home" %}}), qui tourne sur le serveur lui-même, un écran Remote UI est un appareil indépendant, alimenté séparément, sans système d'exploitation généraliste.

Dépôt : [calaos_remote_ui](https://github.com/calaos/calaos_remote_ui), écrit en **C**.

{{% notice info %}}
Pour l'installation, le câblage et l'appairage d'un écran, voyez la page matériel [Remote UI]({{% relref "hardware/remote_ui" %}}). La présente page traite du firmware lui-même.
{{% /notice %}}

## Plateforme

Le firmware cible le microcontrôleur **ESP32-P4** et se construit avec l'environnement ESP-IDF, piloté par CMake. Les fichiers `sdkconfig.defaults` et `sdkconfig.defaults.esp32p4` fournissent la configuration de base.

## Cartes supportées

Le dossier `boards/` regroupe une définition par carte, chacune avec son fichier CMake :

| Carte | Format |
|---|---|
| `waveshare-86-panel` | Panneau mural 86 mm |
| `luckfox-86-panel` | Panneau mural 86 mm |
| `waveshare-touchlcd-7` | Écran tactile 7 pouces |
| `waveshare-touchlcd-8` | Écran tactile 8 pouces |
| `waveshare-touchlcd-10` | Écran tactile 10 pouces |
| `linux-dev` | Cible de développement, sur ordinateur |

**Porter le firmware sur une nouvelle carte** consiste donc à ajouter une définition dans `boards/` et à décrire son écran, son tactile et son réseau, sans toucher à la logique applicative. Le fichier `boards/ci-boards.json` liste les cartes construites automatiquement par l'intégration continue.

## Organisation du code

| Emplacement | Contenu |
|---|---|
| `main/` | Point d'entrée du firmware |
| `hal/` | Couche d'abstraction matérielle |
| `network/` | Connexion réseau et dialogue avec le serveur |
| `flux/` | Gestion de l'état de l'interface |
| `components/` | Composants ESP-IDF |
| `smooth_ui_toolkit/` | Bibliothèque d'interface |
| `fonts/` | Polices embarquées |
| `boards/` | Définitions des cartes |
| `patches/` | Correctifs appliqués aux dépendances |
| `docs/` | Documentation interne, dont `BOARD.md` |

## Développer sans matériel

La cible `linux-dev`, avec le script `build_linux.sh`, permet de **compiler et exécuter l'interface sur un ordinateur**. C'est le moyen le plus rapide d'itérer sur l'affichage sans reflasher une carte à chaque essai.

Pour tout ce qui touche au matériel — écran, tactile, réseau — le test sur carte réelle reste indispensable, la couche `hal/` étant précisément ce que cette cible simule.

## Cycle de vie d'un écran

Du point de vue du firmware, la mise en service se déroule ainsi :

1. **Connexion réseau** — Wi-Fi ou Ethernet, selon ce qui a été inscrit dans le firmware au moment du flash.
2. **Synchronisation de l'heure** par NTP. Cette étape est **bloquante** : l'authentification auprès du serveur repose sur l'heure, un écran mal daté ne se connectera pas.
3. **Découverte du serveur** sur le réseau local, ou connexion à l'adresse configurée.
4. **Appairage** — l'écran génère et affiche un code, qu'il faut déclarer dans Calaos Installer. Tant que le code n'est pas connu du serveur, la connexion est refusée et l'écran affiche l'invite correspondante.
5. **Fonctionnement normal** — l'écran reçoit sa configuration d'interface et les changements d'état du serveur.

Côté serveur, cela se traduit par un type d'IO dédié : voir `IO/RemoteUI/` et `RemoteUI/` dans [Calaos Server]({{% relref "dev/calaos_server" %}}).

## Mises à jour

Le flash par USB n'est nécessaire qu'une seule fois, à la mise en service. Les mises à jour suivantes se font **par le réseau (OTA)**.

C'est aussi pourquoi le domaine `ota` apparaît parmi ceux que l'on peut activer dans les journaux de diagnostic du serveur — voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}) :

```sh
calaos_config set debug_domains ota:5,remote_ui:5,remoteui:5
```
