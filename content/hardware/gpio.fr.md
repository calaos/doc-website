+++
title = 'GPIO'
date = 2024-03-06T20:05:12Z
weight = 110
summary = 'GPIO grâce au support du kernel Linux'
+++

## Introduction

Les **GPIO** sont les broches d'entrée/sortie présentes sur certaines cartes — Raspberry Pi, cartes industrielles, mini-PC équipés d'un connecteur d'extension. Calaos peut les lire et les commander directement, sans passerelle ni bus.

C'est le moyen le plus simple de connecter quelques équipements quand on n'a pas d'automate : un contact de porte, un relais, un bouton.

{{% notice info %}}
Les GPIO conviennent à **quelques points** de commande. Pour une installation complète, l'automate Wago reste préférable : il apporte le nombre d'entrées/sorties, la protection électrique et le [mode de secours]({{% relref "hardware/wago/failsafe" %}}) qu'un connecteur d'extension n'offre pas — voir [Prérequis]({{% relref "hardware/requirements" %}}).
{{% /notice %}}

## Identifier une broche

Chaque broche porte un **numéro**, qui est celui utilisé par le noyau Linux — pas nécessairement celui imprimé sur le connecteur ni celui du brochage physique.

{{% notice warning %}}
La numérotation est la principale source d'erreurs. Sur une même carte coexistent souvent trois numérotations différentes : le numéro de broche physique, le numéro du contrôleur et celui du noyau. C'est **ce dernier** qu'attend Calaos.

Reportez-vous à la documentation de votre carte, et vérifiez sur une broche connue avant de câbler l'ensemble.
{{% /notice %}}

## Le niveau inversé

Beaucoup de montages sont **actifs à l'état bas** : le contact est fermé quand la broche est à zéro, et non à un. C'est notamment le cas des cartes à relais courantes, où un relais s'active quand on met la sortie à zéro.

Sans réglage, tout fonctionne à l'envers : votre lampe s'allume quand elle devrait s'éteindre. Le paramètre `active_low` corrige cela sans rien changer au câblage.

## Types d'IO supportés

**En entrée** — interrupteur, interrupteur à appui long, interrupteur triple.

**En sortie** — lumière, volet, volet intelligent.

## Paramètres

### Entrées et sorties simples

| Nom | Type | Requis | Description |
|---|---|---|---|
| `gpio` | int | oui | Numéro de la broche, de 0 à 65535 |
| `active_low` | bool | non | À activer si le niveau est inversé. Par défaut `false` |

### Volets

Un volet occupe deux broches, une par sens de rotation.

| Nom | Type | Requis | Description |
|---|---|---|---|
| `gpio_up` | int | oui | Numéro de la broche de montée |
| `gpio_down` | int | oui | Numéro de la broche de descente |
| `active_low_up` | bool | non | Niveau inversé pour la montée. Par défaut `false` |
| `active_low_down` | bool | non | Niveau inversé pour la descente. Par défaut `false` |

## Créer une IO GPIO

Dans Calaos Installer, **Ajouter → GPIO**, puis le type voulu. Renseignez le numéro de broche, et cochez le niveau inversé si nécessaire.

## Précautions électriques

{{% notice warning %}}
**Les GPIO fonctionnent en très basse tension et ne supportent aucune charge.** On ne branche jamais un circuit 230 V directement dessus : il faut passer par un relais ou un module de commande adapté.

Une erreur de câblage sur un connecteur d'extension détruit généralement la carte, et parfois davantage.
{{% /notice %}}

Pour les entrées, vérifiez la tension attendue par votre carte : appliquer 5 V sur une entrée prévue pour 3,3 V l'endommage.

## Diagnostic

Si une IO ne réagit pas :

1. **le numéro de broche** est-il celui du noyau, et non celui du connecteur ?
2. **le comportement est-il simplement inversé** ? Dans ce cas, `active_low` est la réponse.
3. **la broche est-elle déjà utilisée** par une autre fonction de la carte — port série, bus I2C — auquel cas elle n'est pas disponible ?

Les journaux du serveur signalent les erreurs d'accès aux broches : voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}).
