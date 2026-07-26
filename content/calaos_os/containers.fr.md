+++
title = 'Services et modules'
date = 2026-07-25T10:00:00Z
weight = 50
summary = "Les logiciels complémentaires livrés avec Calaos OS."
+++

## De quoi on parle

Calaos OS ne contient pas seulement le serveur Calaos. Il embarque aussi une série de logiciels complémentaires, qui apportent chacun une fonction : parler à des équipements Zigbee, enregistrer l'historique de vos mesures, tracer des graphiques, piloter de l'éclairage DMX…

Chacun tourne **isolé des autres**, dans ce qu'on appelle un container. Concrètement, cela veut dire que :

- un module qui plante n'entraîne pas le reste de votre installation ;
- chacun se met à jour indépendamment ;
- chacun range ses données dans son propre dossier.

Vous n'avez rien à installer : ils sont déjà là. Certains ne servent que si vous utilisez le matériel correspondant.

## Ce qui est fourni

| Service | À quoi il sert | Documentation |
|---|---|---|
| Mosquitto | Fait circuler les messages MQTT entre vos équipements et Calaos | [MQTT]({{% relref "hardware/mqtt" %}}) |
| Zigbee2MQTT | Fait le lien entre vos équipements Zigbee et Calaos | [Zigbee]({{% relref "hardware/zigbee" %}}) |
| InfluxDB | Enregistre l'historique de vos mesures : températures, consommations… | — |
| Grafana | Affiche ces mesures sous forme de graphiques | — |
| OLA | Pilote l'éclairage DMX | [DMX / OLA]({{% relref "hardware/dmx_ola" %}}) |
| hue2mqtt | Fait le lien avec les ampoules Philips Hue | [Hue]({{% relref "hardware/hue2mqtt" %}}) |
| Envoy | Récupère les données des onduleurs solaires Enphase | [Enphase]({{% relref "hardware/enphase" %}}) |
| HAProxy | Aiguille les connexions réseau entrantes vers le bon service | — |

## Ils se gèrent comme les autres services

{{% notice info %}}
**Le fait qu'ils tournent dans des containers ne change rien à leur utilisation.** Chacun est piloté par le système au même titre que les autres services : vous les démarrez, les arrêtez et consultez leur état avec les mêmes commandes.
{{% /notice %}}

Connectez-vous au serveur en SSH (voir [Accès SSH]({{% relref "calaos_os/advanced/shell" %}})), puis utilisez `systemctl` comme pour n'importe quel service :

```sh
systemctl status zigbee2mqtt
systemctl restart zigbee2mqtt
```

Pour voir l'ensemble des services actifs, y compris ceux-ci :

```sh
systemctl list-units --type=service
```

Tout cela est détaillé sur la page [Services]({{% relref "calaos_os/configuration/services" %}}).

Pour connaître les versions installées et savoir si des mises à jour existent :

```sh
calaos-os list
```

## Mettre à jour

Les services complémentaires se mettent à jour en même temps que le reste du système :

```sh
calaos-os upgrade
```

Voir [Mise à jour]({{% relref "calaos_os/update" %}}) pour la procédure complète.

Pour ne mettre à jour qu'un service précis, sans toucher au reste :

```sh
calaos-os upgrade NOM_DU_SERVICE
```

## Où sont rangées vos données

Chaque service stocke ses données dans son propre sous-dossier de `/mnt/calaos` :

```text
/mnt/calaos/
├── config/          réglages de Calaos
├── influxdb/        historique de vos mesures
├── grafana/         graphiques et tableaux de bord
├── mosquitto/       données MQTT
├── zigbee2mqtt/     équipements Zigbee appairés
├── ola/             configuration DMX
├── hue2mqtt/        configuration Hue
├── envoy/           configuration Enphase
├── haproxy/         configuration réseau
└── xorg.conf.d/     réglages d'affichage
```

Sauvegarder ce dossier, c'est sauvegarder l'ensemble : voir [Sauvegarder sa configuration]({{% relref "calaos_os/backup" %}}).

{{% notice info %}}
Le dossier `zigbee2mqtt` mérite une attention particulière : il contient la liste de vos équipements Zigbee appairés. Le perdre vous obligerait à tout réappairer un par un.
{{% /notice %}}

## Quand un service ne fonctionne plus

Consultez d'abord son état et ses journaux, qui indiquent presque toujours la cause :

```sh
systemctl status NOM_DU_SERVICE
journalctl -u NOM_DU_SERVICE
```

Ces commandes fonctionnent pour ces services comme pour tous les autres.

Voir aussi [Journaux]({{% relref "calaos_os/configuration/logs" %}}) et [Services]({{% relref "calaos_os/configuration/services" %}}).

## Pour aller plus loin

Les services s'exécutent avec **Podman**, une alternative à Docker qui n'a pas besoin de service central pour fonctionner.

Chaque container est décrit par une unité systemd, ce qui explique qu'ils se pilotent tous avec `systemctl` : pour le système, un container est un service comme un autre.

C'est aussi pourquoi **`journalctl` voit les journaux de tous les services**, containers compris : inutile de passer par Podman pour les lire.

Si vous voulez regarder les choses depuis l'angle des containers plutôt que des services, Podman offre ses propres commandes :

```sh
podman ps                   # containers en cours d'exécution
podman images               # images disponibles localement
```

Elles donnent une vue complémentaire, utile pour du diagnostic fin, mais ne remplacent ni `systemctl` pour démarrer et arrêter un service, ni `journalctl` pour consulter ses journaux.

Les images des services sont décrites par des fichiers `.source` rangés dans `/usr/share/calaos/`. Lors de l'installation, elles sont exportées vers un cache local, dans `/var/lib/cache/containers` : c'est ce qui permet à votre serveur de démarrer une première fois **sans accès à Internet**.
