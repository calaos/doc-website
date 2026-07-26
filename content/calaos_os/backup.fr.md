+++
title = 'Sauvegarder sa configuration'
date = 2026-07-25T10:00:00Z
weight = 45
summary = "Mettre votre configuration à l'abri, et la restaurer si besoin."
+++

## Ce qu'il faut sauvegarder

Toute la configuration de votre serveur vit dans un seul dossier : `/mnt/calaos`. On y trouve :

- vos réglages Calaos : entrées/sorties, règles, scénarios, pièces ;
- l'historique de vos mesures, si vous enregistrez des températures ou des consommations ;
- les réglages des services complémentaires : MQTT, Zigbee, graphiques, éclairage DMX, etc.

C'est aussi le dossier qui **n'est jamais touché par une mise à jour**.

## La méthode la plus simple : Calaos Installer

Calaos Installer sait relire toute votre configuration depuis le serveur et l'enregistrer sur votre ordinateur. Aucune ligne de commande n'est nécessaire.

Dans Calaos Installer, menu **Calaos Server**, choisissez **Charger le projet depuis Calaos Server**. Les fichiers de configuration sont téléchargés et le projet s'ouvre. Enregistrez-le ensuite sur votre ordinateur, à un endroit dont vous vous souviendrez.

Vous obtenez un exemplaire complet de votre configuration, **stocké ailleurs que sur le serveur** : c'est exactement le but d'une sauvegarde.

La manipulation est décrite en détail dans [Envoyer le projet]({{% relref "getting_started/upload" %}}).

## Quand faire une sauvegarde

- **Avant une mise à jour**, par précaution.
- **Avant de confirmer un retour en arrière**, car celui-ci restaure aussi votre configuration — voir [Revenir en arrière]({{% relref "calaos_os/rollback" %}}).
- **Après une grosse session de configuration**, pour ne pas risquer de refaire deux heures de travail.

## Où la stocker

**Ailleurs que sur le serveur.** Une sauvegarde qui reste sur la machine ne sert à rien le jour où c'est le disque de cette machine qui lâche. Enregistrez le projet sur votre ordinateur, et idéalement recopiez-le sur un disque externe ou un espace de stockage en ligne.

## L'historique automatique du serveur

À chaque fois que vous envoyez une nouvelle configuration depuis Calaos Installer, le serveur Calaos range **une copie datée** de l'ancienne configuration, dans un dossier organisé par année et par mois :

```text
/mnt/calaos/config/backups/2026/07/01-07-2026_16-56-00/
├── io.xml
├── local_config.xml
└── rules.xml
```

Chaque envoi laisse ainsi une trace, ce qui permet de revenir à une version antérieure de votre configuration.

{{% notice warning %}}
**Cet historique n'est pas une sauvegarde.** Il se trouve sur le serveur lui-même, dans `/mnt/calaos`. Il ne vous protège donc ni d'une panne de disque, ni d'un retour en arrière — qui le restaurera comme le reste et fera disparaître les versions récentes.

Considérez-le comme un filet de sécurité pratique, pas comme votre sauvegarde.
{{% /notice %}}

## Point de restauration ou sauvegarde ?

Les deux existent, et ils ne protègent pas de la même chose :

| | Protège de | Stocké |
|---|---|---|
| **Point de restauration** | une mise à jour ratée, un système cassé | sur le serveur |
| **Sauvegarde** | une panne de disque, un vol, une erreur de configuration | ailleurs |

Un point de restauration ne remplace pas une sauvegarde : si le disque du serveur meurt, les deux disparaissent ensemble.

## Restaurer

Après une réinstallation de Calaos OS, ouvrez votre projet enregistré dans Calaos Installer, puis renvoyez-le au serveur avec **Envoyer le projet à Calaos Server**. Votre installation domotique repart comme avant.

## Pour aller plus loin

Pour sauvegarder également l'historique de vos mesures et les réglages des services complémentaires, copiez le dossier `/mnt/calaos` en entier. Depuis votre ordinateur :

```sh
scp -r root@ADRESSE_DE_VOTRE_SERVEUR:/mnt/calaos ./sauvegarde-calaos
```

Cette copie est plus lourde et plus longue, mais elle est exhaustive.
