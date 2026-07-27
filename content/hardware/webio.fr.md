+++
title = 'Web IO'
date = 2024-03-06T20:05:06Z
weight = 70
summary = 'Controller des appareils utilisant des appels Web'
+++

## Introduction

Les **Web IO** permettent de piloter ou de lire n'importe quel appareil qui expose une **interface web** : une API REST, une page d'état, un fichier JSON ou XML servi en HTTP.

C'est la solution de repli universelle. Quand un équipement n'a pas de pilote dédié dans Calaos mais qu'il est joignable par le réseau, il y a de bonnes chances de l'intégrer ainsi sans écrire une ligne de code.

Quelques usages typiques :

- lire la production d'un onduleur solaire ou d'un compteur communicant ;
- récupérer une donnée météo depuis un service en ligne ;
- commander une prise connectée ou un module relais qui expose une API ;
- interroger un appareil du réseau local qui publie son état en JSON.

## Comment ça marche

Calaos interroge une **adresse** à intervalles réguliers, puis extrait la valeur qui l'intéresse à l'intérieur de la réponse.

L'extraction repose sur un **chemin** : dans un document JSON ou XML, il désigne où se trouve la donnée. Si votre appareil répond par exemple :

```json
{ "sensors": { "outdoor": { "temp": 12.4, "humidity": 68 } } }
```

le chemin mène à `temp` à l'intérieur de `sensors` puis `outdoor`.

En sortie, le principe est symétrique : Calaos envoie une requête à l'adresse indiquée pour transmettre la nouvelle valeur.

## Types d'IO supportés

**En entrée** — sonde de température, entrée analogique, entrée texte.

**En sortie** — lumière, lumière RVB, sortie analogique, sortie texte.

## Paramètres

| Nom | Description |
|---|---|
| `url` | Adresse à interroger ou à appeler |
| `path` | Emplacement de la valeur dans le document reçu |
| `file_type` | Format du document : JSON, XML, texte brut |
| `request_type` | Méthode de la requête HTTP |
| `data` | Données à envoyer, pour les sorties |
| `data_type` | Format de ces données |

{{% notice note %}}
Ces IO comptent parmi les plus souples de Calaos, et donc parmi les plus dépendantes de l'appareil visé. **L'aide intégrée de Calaos Installer** affiche la description exacte de chaque paramètre pour le type choisi — voir [Créer des IO]({{% relref "calaos_installer/io" %}}).
{{% /notice %}}

## Avant de configurer

Commencez toujours par **regarder ce que renvoie l'appareil**. Ouvrez son adresse dans un navigateur, ou depuis le serveur :

```sh
curl http://adresse-de-l-appareil/api/status
```

Vous voyez alors la structure réelle de la réponse, et vous pouvez en déduire le chemin à renseigner. Sans cette étape, la configuration se fait à l'aveugle.

## Choisir entre Web IO et MQTT

Les deux permettent d'intégrer un appareil non supporté nativement, mais ils ne se valent pas selon les cas.

| | Web IO | MQTT |
|---|---|---|
| **Fonctionnement** | Calaos interroge l'appareil | L'appareil publie ses changements |
| **Réactivité** | Dépend de la fréquence d'interrogation | Immédiate |
| **À privilégier pour** | Une donnée qui évolue lentement | Un état qui doit remonter tout de suite |

Pour une température extérieure ou une production solaire, l'interrogation périodique convient parfaitement. Pour un détecteur d'ouverture, MQTT est nettement préférable : voir [MQTT]({{% relref "hardware/mqtt" %}}).

{{% notice warning %}}
Évitez d'interroger trop fréquemment un **service en ligne**, sous peine de vous faire limiter ou bloquer. Pour une donnée météo, quelques minutes d'intervalle suffisent largement.
{{% /notice %}}

## Diagnostic

Si aucune valeur ne remonte :

1. **l'adresse répond-elle** depuis le serveur Calaos, avec `curl` ?
2. **le format déclaré** correspond-il à ce que renvoie réellement l'appareil ?
3. **le chemin** correspond-il à la structure de la réponse ? Une clé mal orthographiée renvoie une valeur vide.
4. **l'appareil demande-t-il une authentification** que la requête ne fournit pas ?

Voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}) pour suivre les échanges côté serveur.
