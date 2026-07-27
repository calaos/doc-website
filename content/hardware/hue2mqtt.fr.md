+++
title = 'Hue2mqtt'
date = 2024-03-11T17:05:27Z
summary = 'Lampes Philips Hue via MQTT'
+++

## Introduction

Les ampoules et bandeaux **Philips Hue** se pilotent depuis Calaos : allumage, variation, couleur. Ils apparaissent dans les interfaces et s'utilisent dans les règles comme n'importe quel éclairage.

Calaos passe par le **pont Hue**, le boîtier fourni par Philips auquel les ampoules sont appairées. Les ampoules elles-mêmes ne sont jamais adressées directement.

## Deux façons de procéder

| Méthode | Principe | À privilégier quand |
|---|---|---|
| **IO Hue directe** | Calaos interroge le pont Hue | Vous ne pilotez que des lampes |
| **Passerelle hue2mqtt** | Un service traduit le Hue en MQTT | Vous utilisez déjà MQTT, ou voulez remonter d'autres appareils Hue |

La première est la plus simple à mettre en place. La seconde s'intègre mieux dans une installation où MQTT est déjà le point de passage commun — voir [MQTT]({{% relref "hardware/mqtt" %}}).

## Méthode 1 — IO Hue directe

Calaos Installer propose un type de sortie **lumière RVB pilotée par un pont Hue**.

| Nom | Type | Requis | Description |
|---|---|---|---|
| `host` | string | oui | Adresse IP du pont Hue |
| `api` | string | oui | Clé d'API renvoyée par le pont lors de l'association |
| `id_hue` | string | oui | Identifiant unique de la lampe sur le pont |

{{% notice tip %}}
Ne cherchez pas à obtenir la clé d'API et les identifiants à la main : **Calaos Installer intègre un assistant Hue** qui les récupère automatiquement. Il faut simplement appuyer sur le bouton du pont au moment où il le demande, comme pour toute association Hue.
{{% /notice %}}

## Méthode 2 — La passerelle hue2mqtt

`hue2mqtt` est un service qui se connecte au pont Hue et publie tout ce qu'il voit sur MQTT. Calaos consomme ensuite ces messages avec des IO MQTT classiques.

Il est fourni avec Calaos OS et se configure dans `/mnt/calaos/hue2mqtt`. Le fichier de configuration comporte deux sections :

```toml
[mqtt]
host = "127.0.0.1"
port = 1883
topic_prefix = "hue2mqtt"

[hue]
ip = "192.168.1.25"
username = "la clé fournie par le pont"
```

La section `[mqtt]` indique où publier — le broker Mosquitto local par défaut — et la section `[hue]` comment joindre le pont.

Le service accepte également l'authentification MQTT et le chiffrement TLS, si votre broker les exige.

Après modification, redémarrez le service :

```sh
systemctl restart hue2mqtt
```

Les IO se créent ensuite comme des IO MQTT, en s'abonnant aux topics publiés sous le préfixe configuré. Pour découvrir ces topics, un outil comme MQTTX permet d'observer ce qui circule réellement — voir [MQTT]({{% relref "hardware/mqtt" %}}).

## Prérequis communs

**Le pont Hue doit être joignable** depuis le serveur Calaos, et de préférence à une adresse fixe.

Les ampoules doivent déjà être **appairées au pont** via l'application Philips Hue. Calaos les pilote, il ne les appaire pas.

{{% notice note %}}
Une ampoule Hue coupée par un interrupteur mural devient injoignable : c'est le fonctionnement normal de ces produits, qui doivent rester alimentés en permanence. Si vos lampes disparaissent régulièrement, vérifiez d'abord ce point avant de chercher du côté de la configuration.
{{% /notice %}}

## Diagnostic

1. **le pont répond-il** depuis le serveur Calaos ?
2. **la clé d'API est-elle toujours valide** ? Une réinitialisation du pont l'invalide.
3. **l'ampoule est-elle alimentée** et visible dans l'application Philips Hue ?
4. Avec hue2mqtt : **des messages arrivent-ils** sur le broker MQTT ?

Voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}), et [Services]({{% relref "calaos_os/configuration/services" %}}) pour l'état du service `hue2mqtt`.
