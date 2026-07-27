+++
title = 'Ampli Audio/Video'
date = 2024-03-06T20:05:33Z
weight = 80
summary = 'Récepteur Audio Video avec port Ethernet (Denon, Marantz, Yamaha, ...)'
+++

## Introduction

Calaos pilote les **amplis home cinéma** raccordés au réseau : allumer et éteindre, régler le volume, changer de source. L'ampli devient un équipement comme un autre, utilisable dans vos règles et vos scénarios.

C'est ce qui permet à un scénario « Cinéma » d'allumer l'ampli sur la bonne entrée en même temps qu'il baisse l'éclairage et ferme les volets.

## Marques supportées

| Marque | Identifiant |
|---|---|
| Denon | `denon` |
| Marantz | `marantz` |
| Onkyo | `onkyo` |
| Pioneer | `pioneer` |
| Yamaha | `yamaha` |
| HiFi Rose | `hifirose` |

Ces fabricants exposent tous un protocole de commande réseau, que Calaos utilise directement. Aucune passerelle ni boîtier supplémentaire n'est nécessaire.

## Prérequis

**L'ampli doit être raccordé au réseau**, en Ethernet de préférence, et joignable depuis le serveur Calaos.

{{% notice tip %}}
Donnez-lui une **adresse IP fixe**, ou réservez-la sur votre box. Un ampli qui change d'adresse après une coupure devient injoignable, et le symptôme — « les commandes ne passent plus » — n'oriente pas naturellement vers le réseau.
{{% /notice %}}

Vérifiez également dans les réglages de l'appareil que la **commande réseau est activée**. Beaucoup d'amplis la désactivent par défaut, ou la coupent en veille pour économiser l'énergie : dans ce dernier cas, Calaos ne pourra pas les rallumer.

## Paramètres

| Nom | Description |
|---|---|
| `host` | Adresse IP ou nom de l'ampli sur le réseau |
| `model` | Marque de l'appareil, parmi la liste ci-dessus |
| `output` | Zone de sortie de l'ampli, pour les modèles multizones |

## Le paramètre de zone

Beaucoup d'amplis pilotent **plusieurs zones** : la pièce principale et une ou deux zones secondaires alimentant d'autres pièces.

Chaque zone se déclare séparément dans Calaos, avec la même adresse mais une sortie différente. Vous obtenez ainsi un contrôle indépendant du salon et de la terrasse, depuis le même appareil.

## Utilisation dans les règles

Une fois déclaré, l'ampli s'utilise comme n'importe quelle IO :

- **dans un scénario** — « Cinéma » allume l'ampli sur l'entrée du vidéoprojecteur ;
- **en action de règle** — couper l'ampli quand le scénario « Départ » se déclenche ;
- **en condition** — n'éteindre les lumières du salon que si l'ampli est en marche.

Voir [Créer des règles]({{% relref "calaos_installer/rules" %}}) et [Scenario]({{% relref "calaos_installer/scenario" %}}).

## Diagnostic

Si l'ampli ne répond pas :

1. **répond-il au ping** depuis le serveur Calaos ?
2. **la commande réseau est-elle activée** dans ses réglages ?
3. **reste-t-elle active en veille** ? Sinon, l'ampli ne pourra jamais être rallumé à distance.
4. **son adresse a-t-elle changé** depuis la configuration ?

Voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}) pour les erreurs de communication.

## Voir aussi

Pour la diffusion de musique multipièce, voyez [Zone de Musique]({{% relref "hardware/music" %}}).
