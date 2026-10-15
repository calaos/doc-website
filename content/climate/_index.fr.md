+++
title = "Chauffage et climatisation"
date = 2026-10-15T09:00:00Z
weight = 25
chapter = true
+++

### Piloter le confort

# Chauffage et climatisation

Calaos sait piloter le chauffage et la climatisation de deux façons, qui se retrouvent à l'identique dans toutes les interfaces (application web, application mobile, écrans muraux) et dans les règles :

- **une clim ou une pompe à chaleur qui régule seule** (par exemple un split Mitsubishi ou Fujitsu piloté par un module ESPHome) : Calaos affiche son état, envoie les consignes, et peut lui relayer la température d'une sonde de la pièce. C'est le mode `proxy` ;
- **un thermostat que Calaos fait lui-même** : Calaos lit une sonde de température, compare à la consigne, et commute un ou deux relais (radiateurs, plancher chauffant, vannes, pompe à chaleur sur contacts). C'est le mode `thermostat`.

Dans les deux cas, l'équipement est un **device** : un objet unique qui regroupe tous les IO d'un même appareil, au lieu de vingt IO MQTT à créer à la main.

## Par où commencer

| Vous voulez... | Page |
|---|---|
| comprendre ce qu'est un device | [Le principe d'un device]({{% relref "climate/concepts" %}}) |
| ajouter une clim ESPHome | [Clim ESPHome]({{% relref "climate/esphome" %}}) |
| faire réguler Calaos avec des relais | [Thermostat interne]({{% relref "climate/thermostat" %}}) |
| créer ou modifier un device dans l'installer | [Créer un device avec l'assistant]({{% relref "climate/templates" %}}) |
| voir le résultat sur vos écrans | [Afficher et régler]({{% relref "climate/clients" %}}) |
| programmer le chauffage | [Règles de chauffage]({{% relref "climate/rules" %}}) |
| comprendre un problème | [Dépannage]({{% relref "climate/troubleshooting" %}}) |

{{% notice note %}}
Les captures de cette documentation viennent d'une maison de démonstration inventée (salon, chambre, bureau, cuisine, séjour). Les noms, les adresses et les valeurs n'ont rien de réel.
{{% /notice %}}

{{% notice info %}}
Les devices demandent un serveur Calaos, une application web, une application mobile, un écran mural et un Calaos Installer à jour. Un Calaos Installer plus ancien refuse d'ouvrir un projet qui contient des devices, plutôt que de les supprimer sans prévenir.
{{% /notice %}}

{{% children description="true" %}}
