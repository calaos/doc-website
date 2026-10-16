+++
title = "Devices : chauffage et climatisation"
date = 2026-10-15T09:00:00Z
weight = 65
summary = "Clim, pompe à chaleur, radiateurs, plancher chauffant : créer et régler un device de climat."
+++

Calaos sait piloter le chauffage et la climatisation de deux façons, qui se retrouvent à l'identique dans toutes les interfaces (application web, application mobile, écrans muraux) et dans les règles :

- **une clim ou une pompe à chaleur qui régule seule** (par exemple un split Mitsubishi ou Fujitsu piloté par un module ESPHome) : Calaos affiche son état, envoie les consignes, et peut lui relayer la température d'une sonde de la pièce. C'est le mode `proxy` ;
- **un thermostat que Calaos fait lui-même** : Calaos lit une sonde de température, compare à la consigne, et commute un ou deux relais (radiateurs, plancher chauffant, vannes, pompe à chaleur sur contacts). C'est le mode `thermostat`.

Dans les deux cas, l'équipement est un **device** : un objet unique qui regroupe tous les IO d'un même appareil, au lieu de vingt IO MQTT à créer à la main.

## Par où commencer

| Vous voulez... | Page |
|---|---|
| comprendre ce qu'est un device | [Le principe d'un device]({{% relref "calaos_installer/devices/concepts" %}}) |
| ajouter une clim ESPHome | [Clim ESPHome]({{% relref "hardware/esphome_climate" %}}) |
| faire réguler Calaos avec des relais | [Thermostat interne]({{% relref "calaos_installer/devices/thermostat" %}}) |
| créer ou modifier un device dans l'installer | [Créer un device avec l'assistant]({{% relref "calaos_installer/devices/templates" %}}) |
| voir le résultat sur vos écrans | [Afficher et régler]({{% relref "calaos_installer/devices/clients" %}}) |
| partir d'un projet complet | [Projets d'exemple]({{% relref "calaos_installer/devices/examples" %}}) |
| programmer le chauffage | [Règles de chauffage]({{% relref "calaos_installer/rules_heating" %}}) |
| comprendre un problème | [Dépannage]({{% relref "calaos_installer/devices/troubleshooting" %}}) |

{{% children description="true" %}}
