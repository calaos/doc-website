+++
title = 'Services'
date = 2024-03-06T20:15:15Z
weight = 20
summary = "Voir, redémarrer et surveiller les services qui tournent sur le serveur."
+++

## De quoi on parle

Votre serveur fait tourner en permanence plusieurs programmes en arrière-plan, appelés **services** : le serveur Calaos lui-même, mais aussi les services complémentaires comme MQTT ou Zigbee, et des tâches d'entretien du système.

En fonctionnement normal, vous n'avez pas à vous en occuper : ils démarrent tout seuls à l'allumage et s'arrêtent à l'extinction. Cette page vous servira surtout le jour où quelque chose ne répond plus.

Ces réglages ne sont pas disponibles depuis l'écran tactile : connectez-vous au serveur en SSH (voir [Accès SSH]({{% relref "calaos_os/advanced/shell" %}})).

## Les services que vous rencontrerez

Voici ceux dont vous aurez besoin en pratique, avec leur nom exact :

| Service | Rôle |
|---|---|
| `calaos-server` | **Le cœur de votre domotique** : c'est lui qui pilote vos équipements et applique vos règles |
| `calaos-home` | L'interface locale affichée sur l'écran tactile |
| `calaos-container` | Le service qui démarre et surveille tous les services complémentaires |
| `mosquitto` | Messagerie MQTT |
| `zigbee2mqtt` | Passerelle Zigbee |
| `influxdb` | Historique de vos mesures |
| `grafana` | Graphiques |
| `haproxy` | Aiguillage des connexions entrantes |
| `hue2mqtt` | Passerelle Philips Hue |
| `envoy` | Onduleurs solaires Enphase |
| `olad` | Éclairage DMX |

Le service le plus important est `calaos-server` : si votre domotique ne répond plus du tout, c'est par lui qu'il faut commencer.

```sh
systemctl status calaos-server
systemctl restart calaos-server
```

{{% notice tip %}}
`calaos-server` et `calaos-home` sont deux choses différentes. Si l'écran tactile est figé mais que vos applications mobiles répondent, c'est `calaos-home` qu'il faut redémarrer, et votre domotique n'a jamais cessé de fonctionner.
{{% /notice %}}

## Voir ce qui tourne

Pour lister les services actifs :

```sh
systemctl list-units --type=service
```

La colonne de gauche donne le nom de chaque service, et les colonnes suivantes indiquent s'il fonctionne correctement. Cherchez la mention `failed` : c'est le signe d'un service en panne.

Pour ne voir que ce qui pose problème :

```sh
systemctl --failed
```

Si cette commande ne renvoie rien, tout va bien.

## Une seule façon de faire, pour tous les services

Les services complémentaires (MQTT, Zigbee, historiques, graphiques…) s'exécutent dans des containers, mais **cela ne change rien pour vous** : ils sont pilotés par le système comme tous les autres.

Autrement dit, `systemctl` les voit et les gère exactement de la même manière, et les commandes de cette page s'appliquent à eux sans distinction :

```sh
systemctl status zigbee2mqtt
systemctl restart zigbee2mqtt
```

Vous n'avez donc pas deux jeux de commandes à retenir selon le service concerné.

Ces services sont présentés en détail sur la page [Services et modules]({{% relref "calaos_os/containers" %}}).

## Redémarrer un service

Si un service ne répond plus, le redémarrer suffit souvent à régler le problème :

```sh
systemctl restart NOM_DU_SERVICE
```

en remplaçant `NOM_DU_SERVICE` par le nom relevé dans la liste.

Pour connaître l'état détaillé d'un service, et notamment la raison d'un échec :

```sh
systemctl status NOM_DU_SERVICE
```

Les dernières lignes de journal du service sont affichées à la suite, ce qui donne généralement une piste. Voir aussi [Journaux]({{% relref "calaos_os/configuration/logs" %}}).

## Redémarrer le serveur entier

Quand plusieurs choses semblent perturbées, un redémarrage complet est souvent plus rapide qu'un diagnostic service par service :

```sh
reboot
```

Comptez une minute avant que votre domotique reréponde.

## Pour aller plus loin

Calaos OS active plusieurs tâches d'entretien automatiques, qui s'exécutent seules et n'ont normalement pas besoin de votre attention :

| Tâche | Rôle |
|---|---|
| `snapper-timeline.timer` | Crée régulièrement des points de restauration |
| `snapper-cleanup.timer` | Supprime les points de restauration les plus anciens |
| `btrfs-scrub@.timer` | Vérifie périodiquement l'intégrité des données du disque |
| `fstrim.timer` | Entretient les disques SSD |
| `grub-btrfsd.service` | Tient à jour la liste des points de restauration proposée au démarrage |

Pour activer ou désactiver un service au démarrage :

```sh
systemctl enable NOM_DU_SERVICE
systemctl disable NOM_DU_SERVICE
```

{{% notice warning %}}
Ne désactivez pas les tâches d'entretien ci-dessus : ce sont elles qui rendent possibles les retours en arrière et qui évitent que le disque se remplisse de points de restauration.
{{% /notice %}}
