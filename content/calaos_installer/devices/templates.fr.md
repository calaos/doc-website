+++
title = "Créer un device avec l'assistant"
date = 2026-10-15T09:20:00Z
weight = 15
summary = "Les cinq modèles, leurs valeurs par défaut, l'assistant de création et l'éditeur de device de Calaos Installer."
+++

Dans Calaos Installer, un device ne se construit pas IO par IO : on choisit un **modèle**, on remplit quelques champs, et l'assistant crée le device, tous ses IO, ses liens et ses paramètres.

## Les cinq modèles

| Modèle | Comportement | Pour quoi faire |
|---|---|---|
| Clim ESPHome (`esphome_climate`) | relayé (`proxy`) | split ou pompe à chaleur ESPHome qui régule seul ; température de pièce relayée en option |
| Clim ESPHome infrarouge (`esphome_climate_ir`) | relayé (`proxy`) | clim pilotée en infrarouge (`climate_ir`) : pas d'action remontée, pas de température distante |
| Thermostat chauffage (`thermostat_heat`) | thermostat | une sortie de chauffage commutée par Calaos |
| Thermostat chaud / froid (`thermostat_heat_cool`) | thermostat | deux sorties (chaud, froid) : pompe à chaleur réversible sur contacts, ventilo-convecteurs |
| Plancher chaud / froid (`thermostat_floor_heat_cool`) | thermostat | plancher chauffant et rafraîchissant, avec coupure avant le point de rosée |

## Créer un device

1. Dans l'arborescence, sélectionnez la **pièce** qui recevra le device.
2. Ouvrez le menu **Ajouter** et choisissez **Device depuis un template…**.

   ![Menu Ajouter](/en/calaos_installer/devices/images/inst-add-menu.png?width=30pc&classes=shadow)
   *Le menu Ajouter d'une pièce.*

3. Choisissez le modèle, puis remplissez les champs. Les champs obligatoires sont marqués ; le bouton **Créer** reste inactif tant qu'un champ est invalide.
4. Cliquez sur **Créer** : le device apparaît dans la pièce, avec ses IO dessous.

### Clim ESPHome

Champs principaux : le nom, la pièce, le **préfixe de topic** et l'**identifiant** de l'entité (voir [Clim ESPHome]({{% relref "hardware/esphome_climate" %}}) pour les trouver), l'adresse et le port du broker MQTT (par défaut `127.0.0.1:1883`), les modes proposés, « Deux consignes », les bornes et le pas de la consigne, la remontée de l'action, les modes de ventilation et d'oscillation, et la **sonde de la pièce** à relayer.

![Assistant Clim ESPHome](/en/calaos_installer/devices/images/inst-wizard-esphome-climate.png?width=36pc&classes=shadow)
*Le modèle Clim ESPHome rempli pour « Clim salon ».*

Valeurs par défaut : tous les modes, une consigne, de 16 à 30 °C par pas de 0,5 °C, action remontée, aucune ventilation ni oscillation. Lorsqu'une sonde de la pièce est choisie, le device relaie sa valeur toutes les 120 s (`remote_temp_keepalive`) et `sensor_timeout` vaut 9000 s.

### Clim ESPHome infrarouge

Même principe, avec les modes `off`, `heat`, `cool` et `auto`, un pas de 1 °C, et une température de pièce facultative. Une clim infrarouge n'a ni action ni température distante.

![Assistant Clim infrarouge](/en/calaos_installer/devices/images/inst-wizard-esphome-climate-ir.png?width=36pc&classes=shadow)
*Le modèle infrarouge rempli pour « Clim chambre ».*

### Thermostat chauffage

Champs : le nom, la pièce, la **sonde de température** (un IO existant), la **sortie de chauffage** (un relais existant) et l'**émetteur**. L'émetteur choisit l'hystérésis et les durées minimales (voir [Thermostat interne]({{% relref "calaos_installer/devices/thermostat" %}})). Le device crée sa consigne et son action, qui sont des variables internes.

![Assistant thermostat chauffage](/en/calaos_installer/devices/images/inst-wizard-thermostat-heat.png?width=36pc&classes=shadow)
*Le modèle Thermostat chauffage rempli pour « Radiateur bureau ».*

### Thermostat chaud / froid

Même chose avec une **sortie de refroidissement** en plus. Le device crée en plus une consigne basse et une consigne haute pour le mode `auto`, séparées d'au moins 2 °C pour une pompe à chaleur (1 °C pour des ventilo-convecteurs).

![Assistant thermostat chaud froid](/en/calaos_installer/devices/images/inst-wizard-thermostat-heat-cool.png?width=36pc&classes=shadow)
*Le modèle Thermostat chaud / froid rempli pour « PAC cuisine ».*

### Plancher chaud / froid

Comme le précédent, avec une **sonde d'humidité** obligatoire et l'**humidité maximale** (60 % par défaut, de 40 à 90 %) à partir de laquelle le refroidissement est coupé.

![Assistant plancher](/en/calaos_installer/devices/images/inst-wizard-thermostat-floor.png?width=36pc&classes=shadow)
*Le modèle Plancher chaud / froid rempli pour « Plancher séjour ».*

### Un relais, un seul thermostat

Les listes de sorties ne proposent pas un relais déjà utilisé par un autre thermostat : une sortie n'a qu'un propriétaire (voir [Thermostat interne]({{% relref "calaos_installer/devices/thermostat" %}})). Si le relais est pris, le refus est expliqué à l'écran.

![Sortie refusée](/en/calaos_installer/devices/images/inst-output-refused.png?width=36pc&classes=shadow)
*Le relais est déjà piloté par un autre thermostat.*

## Le device dans l'arborescence

Le device apparaît dans sa pièce, avec ses IO possédés dessous. Un clic droit ouvre le menu du device ; **supprimer le device supprime aussi tous ses IO**. Si un IO extérieur encore lié à un device est supprimé, ou si la suppression touche des règles, l'installer le signale et liste ce qui est concerné avant de continuer.

![Device dans l'arborescence](/en/calaos_installer/devices/images/inst-tree-device.png?width=30pc&classes=shadow)
*Les devices d'une maison : pièce, device, IO possédés.*

## Modifier un device existant

Pas besoin de recréer un device pour changer la sonde ou l'hystérésis : l'**éditeur de device** (clic droit sur le device, **Propriétés**) permet de changer les **liens** (par exemple la sonde de la pièce) et les **paramètres** (hystérésis, durées minimales, modes...) sans toucher aux IO possédés ni aux règles qui les utilisent. Les rôles et les paramètres affichés viennent de la documentation embarquée du serveur : chacun a son libellé, sa description, son type, ses bornes et sa valeur par défaut.

![Éditeur de device](/en/calaos_installer/devices/images/inst-device-editor.png?width=36pc&classes=shadow)
*L'éditeur de device de « Clim salon ».*

## Envoyer au serveur

Comme le reste du projet, le device n'existe sur le serveur qu'après l'[envoi de la configuration]({{% relref "getting_started/upload" %}}). Calaos Installer déclare au serveur qu'il comprend les devices (`io_format` 2) ; un serveur trop ancien pour les devices refuse l'envoi plutôt que de recevoir un fichier qu'il ne comprend pas.
