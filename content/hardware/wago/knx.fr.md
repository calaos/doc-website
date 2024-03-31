+++
title = 'KNX avec Wago'
date = 2024-03-28T14:23:14Z
+++

## Installation KNX

Il est possible d'utiliser l'automate Wago pour piloter une installation KNX et faire office de passerelle KNX.

KNX est un protocole standardisé de communication pour bâtiments intelligents. KNX est né de la fusion des standards EHS, EIB (ou Instabus) et Bâtibus.

Au contraire d'une installation électrique standard, il n'existe aucune connexion filaire entre les unités de commande et l'alimentation: par exemple, un interrupteur n'est pas directement relié à la lampe correspondante. En effet, les appareils et les équipements électriques sont connectés par le biais du BUS, qui fonctionne sous une 28 V. Tous les appareils du BUS peuvent être programmés à l'aide d'un instrument commun. Ainsi, le BUS KNX permet d'obtenir une installation simple et très flexible. Les modifications ultérieures peuvent en outre être réalisées sans toucher aux câblages.

Un des avantages d'utiliser l'automate Wago avec du KNX, c'est de pouvoir avoir le mode dégradé où l'automate pourra allumer une lumiere sur un relais avec un bouton poussoir KNX par ex.

## Prérequis

Le système Calaos nécessite quelques composants pour fonctionner avec le bus KNX:

- Une tête de station Wago 750-849
- Une borne Wago 753-646
- Une Alimentation KNX 30Vdc pour alimenter le bus KNX
- Des modules KNX
- Le logiciel ETS4 / ETS5 pour le paramétrage

## Architecture

Le bus KNX communique sur un liaison constitué une paire torsadé (TP).

Il est cependant possible de communiquer sur d'autre support :

- Par courant porteur (CPL)
- Par radio fréquence (RF)
- Par Infra-rouge
- Par Ethernet (KNX sur IP)

Chaque élément connecté au bus KNX est indépendant des autres éléments. Il est capable d'envoyer des informations qui sera reçu par les autres éléments, mais traité uniquement par l'élément concerné.

![pic](/en/hardware/wago/images/bus_knx_.png?width=20pc&classes=shadow)

PL : Power Line TP : Twisted Pair (paire torsadée)

### Alimentation

Le bus doit être alimenté avec une tension continue nominale de 29Vcc. La plupart des composants sont alimenter directement par le bus. La limite inférieure de la tension d'alimentation est de 21Vcc. La consommation d'un composant est, en moyenne, de l'ordre de 10 mA.

### Limites du Bus

Comme tous les bus, le KNX à des limites de câble à respecter.

- Distance maximale entre deux modules : 700 m
- Distance maximale entre un module et son alimentation : 350 m
- Longueur maximale du bus : 1000 m
- Distance maximale entre deux alimentation : 200 m

![pic](/en/hardware/wago/images/wago.png?width=20pc&classes=shadow)

## Topologies

Une installation peut être divisée en lignes et en zones. Une ligne contient un maximum de **64 participants** (modules). Une zone comporte un maximum de **15 lignes** reliées à la ligne principale par des coupleurs de ligne Une dorsale relie un maximum de **15 zones** par l'intermédiaire des coupleurs de zone.

![pic](/en/hardware/wago/images/topologie.png?width=20pc&classes=shadow)

## Programmation

TODO!
