+++
title = 'Zone de Musique'
date = 2024-03-06T20:05:25Z
weight = 100
summary = 'Zones de musique (Squeezebox)'
+++

## Introduction

Une **zone de musique** est un lecteur audio installé dans une pièce, que Calaos pilote comme le reste de la maison : lancer une playlist, régler le volume, passer au morceau suivant, tout arrêter en partant.

Chaque pièce équipée devient une zone indépendante, avec sa propre lecture et son propre volume.

## Systèmes supportés

| Système | Description |
|---|---|
| **Squeezebox** | Lecteurs Logitech Squeezebox et compatibles, pilotés par un serveur Lyrion / Logitech Media Server |
| **Roon** | Lecteurs gérés par un serveur Roon |

Dans les deux cas, Calaos ne lit pas la musique lui-même : il **commande le système de diffusion** déjà en place, et récupère en retour l'état de lecture et les informations sur le morceau en cours.

## Ce que Calaos affiche

Une fois la zone déclarée, les interfaces présentent un lecteur complet : morceau en cours, pochette d'album, contrôles de lecture et de volume, accès aux playlists, aux artistes, aux albums et aux radios.

Cette bibliothèque vient du serveur de musique, pas de Calaos : les morceaux que vous voyez sont ceux qu'il connaît.

## Prérequis

**Le serveur de musique doit être installé et fonctionner**, avec ses lecteurs déjà déclarés et audibles. Calaos vient par-dessus une installation qui marche, il ne la remplace pas.

Vérifiez avant toute chose que vous savez lancer un morceau dans une pièce depuis l'interface native du système. Si ce n'est pas le cas, le problème est en amont de Calaos.

{{% notice tip %}}
Donnez une **adresse IP fixe** au serveur de musique, comme aux lecteurs. C'est la cause la plus fréquente de zones qui disparaissent après une coupure de courant.
{{% /notice %}}

## Ajouter une zone

Dans Calaos Installer, **Ajouter → Zone de musique**, puis indiquez le système et le lecteur concerné.

Calaos Installer sait **détecter les lecteurs Squeezebox** présents sur le réseau, ce qui évite de saisir les adresses à la main.

## Utilisation dans les règles

Une zone de musique s'utilise comme n'importe quelle IO :

- **couper la musique partout** dans le scénario « Départ » ;
- **lancer une playlist** au réveil, avec une programmation horaire ;
- **baisser le volume** quand la sonnette retentit ;
- **arrêter la lecture** dans une pièce quand elle se vide, à partir d'un détecteur de présence.

Voir [Créer des règles]({{% relref "calaos_installer/rules" %}}), [Scenario]({{% relref "calaos_installer/scenario" %}}) et [Programmation horaire]({{% relref "calaos_installer/time" %}}).

## Diagnostic

Si une zone ne répond pas :

1. **le lecteur fonctionne-t-il** depuis l'interface native du système de musique ?
2. **le serveur de musique est-il joignable** depuis le serveur Calaos ?
3. **les adresses ont-elles changé** depuis la configuration ?
4. **le lecteur est-il allumé** ? Certains lecteurs se coupent complètement et ne répondent plus.

Voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}).

## Voir aussi

Pour piloter un ampli home cinéma plutôt qu'un lecteur multipièce, voyez [Ampli Audio/Video]({{% relref "hardware/avr" %}}).
