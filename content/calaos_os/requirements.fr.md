+++
title = 'De quoi ai-je besoin ?'
date = 2026-07-25T10:00:00Z
weight = 7
summary = "Le matériel nécessaire pour faire tourner Calaos OS."
+++

## Liste de courses

Pour installer Calaos OS, il vous faut :

- **un ordinateur** de type PC, qui restera allumé en permanence ;
- **une clé USB** d'au moins 4 Go, qui servira à l'installation — **prenez-la rapide**, voir ci-dessous ;
- **un câble réseau** pour relier l'ordinateur à votre box ou à votre switch.

Et de façon optionnelle :

- **un écran tactile**, si vous voulez une interface murale dans la maison ;
- un clavier et un écran, le temps de l'installation, si vous préférez ne pas configurer à distance.

## Quel ordinateur choisir

Calaos OS s'installe sur un **PC 64 bits** (architecture x86_64). Les machines anciennes comme récentes conviennent : l'installeur gère aussi bien les ordinateurs modernes (UEFI) que les plus anciens (BIOS), et vous n'avez rien à choisir.

En pratique, un **mini-PC sans ventilateur** est le meilleur choix pour une installation permanente : il est silencieux, consomme peu, et n'a pas de pièce mécanique qui s'use. Un ancien ordinateur portable ou un PC de bureau récupéré fait parfaitement l'affaire pour découvrir Calaos.

{{% notice tip %}}
Pour simplement essayer Calaos OS, vous n'avez besoin d'aucun matériel dédié : le mode **Live** permet de démarrer le système depuis la clé USB sans rien installer, et sans toucher au disque de l'ordinateur. Voir [Installation]({{% relref "calaos_os/installation" %}}).
{{% /notice %}}

## La clé USB

N'importe quelle clé d'au moins 4 Go fera l'affaire, mais **sa vitesse détermine le temps que durera votre installation**.

Au premier démarrage, Calaos OS décompresse et installe tous ses services depuis la clé. Sur une clé rapide, cela prend quelques minutes ; sur une clé lente ou d'entrée de gamme, **cela peut demander de 30 à 60 minutes**.

Prenez donc une **clé USB 3.0 de marque**, et branchez-la de préférence sur un port USB 3.0 de l'ordinateur — souvent reconnaissable à sa couleur bleue. Vous gagnerez ce temps deux fois : à la copie de l'image, puis au premier démarrage.

## Le disque

Prévoyez de la marge. Calaos OS occupe peu de place au départ, mais trois choses grandissent avec le temps :

- **les points de restauration**, ces copies de l'état du système qui vous permettent de revenir en arrière ;
- **les images des services** qui tournent sur votre serveur ;
- **l'historique de vos mesures**, si vous enregistrez des températures, des consommations, etc.

L'installeur réserve environ 2 à 2,5 Go en début de disque pour le démarrage et la mémoire virtuelle, puis utilise tout le reste. Un disque de petite capacité fonctionnera, mais vous serez rapidement à l'étroit. Un SSD est préférable à un disque mécanique, pour le silence et la rapidité.

## Le réseau

**Reliez le serveur en câble** plutôt qu'en Wi-Fi. Votre domotique doit répondre instantanément et de façon fiable, ce que le câble garantit mieux.

Donnez à votre serveur une **adresse IP fixe**, pour le retrouver toujours au même endroit sur votre réseau. Sans cela, son adresse peut changer au fil des redémarrages, et vos applications ne le trouveront plus. La marche à suivre est sur la page [Réseau]({{% relref "calaos_os/configuration/network" %}}).

## L'écran tactile

Un écran tactile branché sur le serveur affiche **Calaos Home**, l'interface locale qui présente votre maison et permet quelques réglages.

Il est **facultatif**. Sans écran, vous configurez le serveur à distance depuis un autre ordinateur, et vous pilotez votre maison depuis les applications mobiles ou la web app. Cette documentation indique systématiquement les deux façons de faire.

Pour des écrans muraux répartis dans la maison, regardez plutôt du côté des [écrans Remote UI]({{% relref "hardware/remote_ui" %}}), conçus pour cet usage.

{{% notice note %}}
Cette page traite du **matériel informatique** de votre serveur. Pour tout ce qui concerne l'installation électrique de la maison — câblage, tableau, automate — voyez [Prérequis]({{% relref "hardware/requirements" %}}) dans le chapitre matériel.
{{% /notice %}}

## Pour aller plus loin

L'installeur partitionne le disque de la façon suivante :

| Machine | Partitions créées |
|---|---|
| Moderne (UEFI) | une partition de démarrage de 512 Mo, une partition d'échange de 2 Go, puis le reste du disque pour le système |
| Ancienne (BIOS) | une partition d'échange de 2 Go, puis le reste du disque pour le système |

La partition système est formatée en **Btrfs** avec compression, et porte l'étiquette `calaos-os`. Le détail est décrit dans [Sous le capot]({{% relref "calaos_os/advanced/btrfs" %}}).
