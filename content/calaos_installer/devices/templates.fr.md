+++
title = "Créer un device avec l'assistant"
date = 2026-10-15T09:20:00Z
weight = 15
summary = "Les six modèles, leurs valeurs par défaut, l'assistant de création et l'éditeur de device de Calaos Installer."
+++

Dans Calaos Installer, un device ne se construit pas IO par IO : on choisit un **modèle**, on remplit quelques champs, et l'assistant crée le device, tous ses IO, ses liens et ses paramètres.

## Les six modèles

| Modèle | Comportement | Pour quoi faire |
|---|---|---|
| Climat ESPHome (MQTT) (`esphome_climate`) | relayé (`proxy`) | split ou pompe à chaleur ESPHome qui régule seul ; température de pièce relayée en option |
| Climat IR ESPHome (MQTT) (`esphome_climate_ir`) | relayé (`proxy`) | clim pilotée en infrarouge (`climate_ir`) : pas d'action remontée, pas de température distante |
| Thermostat de chauffage (`thermostat_heat`) | thermostat | une sortie de chauffage commutée par Calaos |
| Thermostat chauffage et refroidissement (`thermostat_heat_cool`) | thermostat | deux sorties (chaud, froid) : pompe à chaleur réversible sur contacts, ventilo-convecteurs |
| Thermostat de plancher réversible (`thermostat_floor_heat_cool`) | thermostat | plancher chauffant et rafraîchissant à deux sorties, avec coupure avant le point de rosée |
| Zone de plancher réversible (`thermostat_floor_zone`) | thermostat | une zone de plancher avec **une seule vanne** pour le chaud et le froid ; la pompe à chaleur est commandée par des règles |

## Créer un device

1. Dans l'arborescence, sélectionnez la **pièce** qui recevra le device.
2. Ouvrez le menu **Ajouter** et choisissez **Device depuis un template…**.

   ![Menu Ajouter](/en/calaos_installer/devices/images/inst-add-menu.png?width=30pc&classes=shadow)
   *Le menu Ajouter d'une pièce.*

3. Choisissez le modèle, puis remplissez les champs. Les champs obligatoires sont marqués ; le bouton **Créer** reste inactif tant qu'un champ est invalide.

   Sous chaque champ, une ligne « Lien : libellé (identifiant) » ou « Paramètre : libellé (identifiant) » dit ce que le champ devient dans le device. Les listes de choix ne proposent que les IO du bon type : un relais n'apparaît pas à la place d'une sonde de température.
4. Cliquez sur **Créer** : le device apparaît dans la pièce, avec ses IO dessous.

### Climat ESPHome

Champs principaux : le nom, la pièce, le **préfixe de topic** et l'**identifiant** de l'entité, l'adresse et le port du broker MQTT (par défaut `127.0.0.1:1883`), les modes proposés, « Deux consignes » ou « Double consigne émulée », les bornes et le pas de la consigne, la remontée de l'action, les modes de ventilation et d'oscillation, et la **sonde de la pièce** à relayer.

#### Le trouver sur le broker MQTT

Plutôt que de deviner le préfixe et l'identifiant, cliquez sur **Rechercher** dans le panneau **« Le trouver sur le broker MQTT »** : l'assistant écoute le broker, liste les climats qu'il y voit, et le choix d'une ligne remplit le formulaire (préfixe, identifiant, nombre de consignes, ventilation, oscillation...).

![Panneau de recherche sur le broker](/en/calaos_installer/devices/images/inst-discovery-found.png?width=36pc&classes=shadow)
*La recherche a trouvé trois climats ; le dernier, grisé, est un ancien nœud.*

Le détail (broker utilisé, lignes grisées, erreurs) est dans [Clim ESPHome]({{% relref "hardware/esphome_climate" %}}).

#### Double consigne émulée

Pour une clim à une seule consigne (« Deux consignes » décoché), « Double consigne émulée » donne à Calaos une consigne basse et une consigne haute et le mode `auto` : voir [Clim ESPHome]({{% relref "hardware/esphome_climate" %}}). Le modèle règle alors l'hystérésis à 0,5 °C, les durées minimales de marche et d'arrêt à 300 s, le délai de bascule à 600 s et l'écart minimal des consignes à 2 °C.

![Assistant Clim ESPHome](/en/calaos_installer/devices/images/inst-wizard-esphome-climate.png?width=36pc&classes=shadow)
*Le modèle Clim ESPHome rempli pour « Clim salon ».*

Valeurs par défaut : tous les modes, une consigne, de 16 à 30 °C par pas de 0,5 °C, action remontée, aucune ventilation ni oscillation. Lorsqu'une sonde de la pièce est choisie, le device relaie sa valeur toutes les 120 s (`remote_temp_keepalive`) et `sensor_timeout` vaut 9000 s.

### Climat IR ESPHome

Même principe, avec les modes `off`, `heat`, `cool` et `auto`, un pas de 1 °C, et une température de pièce facultative. Une clim infrarouge n'a ni action ni température distante.

![Assistant Clim infrarouge](/en/calaos_installer/devices/images/inst-wizard-esphome-climate-ir.png?width=36pc&classes=shadow)
*Le modèle infrarouge rempli pour « Clim chambre ».*

### Thermostat de chauffage

Champs : le nom, la pièce, la **sonde de température** (un IO existant), la **sortie de chauffage** (un relais existant) et l'**émetteur**. L'émetteur choisit l'hystérésis et les durées minimales (voir [Thermostat interne]({{% relref "calaos_installer/devices/thermostat" %}})). Le device crée sa consigne et son action, qui sont des variables internes. L'assistant écrit aussi les bornes de vraisemblance de la sonde (`valid_min` = -30 °C, `valid_max` = 60 °C), modifiables ensuite dans l'éditeur de device : voir [Sonde hors limites]({{% relref "calaos_installer/devices/thermostat" %}}).

![Assistant thermostat chauffage](/en/calaos_installer/devices/images/inst-wizard-thermostat-heat.png?width=36pc&classes=shadow)
*Le modèle Thermostat de chauffage rempli pour « Radiateur bureau ».*

### Thermostat chauffage et refroidissement

Même chose avec une **sortie de refroidissement** en plus. Le device crée en plus une consigne basse et une consigne haute pour le mode `auto`, séparées d'au moins 2 °C pour une pompe à chaleur (1 °C pour des ventilo-convecteurs).

![Assistant thermostat chaud froid](/en/calaos_installer/devices/images/inst-wizard-thermostat-heat-cool.png?width=36pc&classes=shadow)
*Le modèle Thermostat chauffage et refroidissement rempli pour « PAC cuisine ».*

### Thermostat de plancher réversible

Comme le précédent (une sortie de chauffage et une sortie de refroidissement), avec une **sonde d'humidité** obligatoire et l'**humidité maximale** (60 % par défaut, de 40 à 90 %) à partir de laquelle le refroidissement est coupé.

![Assistant plancher](/en/calaos_installer/devices/images/inst-wizard-thermostat-floor.png?width=36pc&classes=shadow)
*Le modèle Thermostat de plancher réversible rempli pour « Plancher séjour ».*

### Zone de plancher réversible

Pour un plancher dont **une même vanne** sert au chaud et au froid, et dont la pompe à chaleur change le sens de l'eau. Champs : le nom, la pièce, la **sonde de température**, la **vanne de zone** (un relais existant), les **modes** de la zone, une **sonde d'humidité** facultative et l'**humidité maximale** (60 % par défaut) qui ne sert qu'avec cette sonde.

![Assistant zone de plancher](/en/calaos_installer/devices/images/inst-wizard-floor-zone.png?width=36pc&classes=shadow)
*Une zone qui chauffe et rafraîchit, avec sa sonde d'humidité : « Plancher séjour ».*

- Les modes cochés sont ceux de la zone : `off` est toujours offert. Une zone qui ne rafraîchit pas (salle de bain, entrée) ne coche que **Chauffage** : elle s'éteint quand la maison passe en froid.
- Sans sonde d'humidité, le refroidissement est permis sans protection contre la condensation : l'installation doit garder l'eau au-dessus du point de rosée. Le journal du serveur l'avertit au chargement. Avec une sonde, la coupure sur l'humidité s'applique (voir [Thermostat interne]({{% relref "calaos_installer/devices/thermostat" %}})).
- Valeurs par défaut : hystérésis 0,5 °C, marche et arrêt minimaux 900 s, pas d'écart de consignes (une zone n'a pas de mode `auto`).

![Zone sans refroidissement](/en/calaos_installer/devices/images/inst-wizard-floor-zone-heat.png?width=36pc&classes=shadow)
*Une zone en chauffage seul : « Plancher salle de bain ».*

Le modèle ne crée pas de device « pompe à chaleur » : la demande au générateur, ses deux contacts chaud et froid et le mode de la maison se font par des règles. Voir [Plancher réversible : neuf zones et une pompe à chaleur]({{% relref "calaos_installer/rules_heating" %}}).

### Un relais, un seul thermostat

Les listes de sorties ne proposent pas un relais déjà utilisé par un autre thermostat : une sortie n'a qu'un propriétaire (voir [Thermostat interne]({{% relref "calaos_installer/devices/thermostat" %}})). Si le relais est pris, le refus est expliqué à l'écran.

![Sortie refusée](/en/calaos_installer/devices/images/inst-output-refused.png?width=36pc&classes=shadow)
*Le relais est déjà piloté par un autre thermostat.*

## Le device dans l'arborescence

Le device apparaît dans sa pièce, avec ses IO possédés dessous. Un clic droit ouvre le menu du device ; **supprimer le device supprime aussi tous ses IO**. Si un IO extérieur encore lié à un device est supprimé, ou si la suppression touche des règles, l'installer le signale et liste ce qui est concerné avant de continuer.

![Device dans l'arborescence](/en/calaos_installer/devices/images/inst-tree-device.png?width=30pc&classes=shadow)
*Les devices d'une maison : pièce, device, IO possédés.*

## Modifier un device existant

Pas besoin de recréer un device pour changer la sonde ou l'hystérésis : l'**éditeur de device** (clic droit sur le device, **Propriétés**) permet de changer les **liens** (par exemple la sonde de la pièce) et les **paramètres** (hystérésis, durées minimales, modes...) sans toucher aux IO possédés ni aux règles qui les utilisent. Les rôles et les paramètres affichés viennent de la documentation embarquée du serveur : chacun a son libellé (l'identifiant technique est dans l'infobulle), sa description, son type, ses bornes et sa valeur par défaut. Les IO proposés pour un lien sont filtrés par type, et un lien existant vers un IO d'un autre type est affiché « d'un autre type » et conservé.

![Éditeur de device](/en/calaos_installer/devices/images/inst-device-editor.png?width=36pc&classes=shadow)
*L'éditeur de device de « Clim salon ».*

## Envoyer au serveur

Comme le reste du projet, le device n'existe sur le serveur qu'après l'[envoi de la configuration]({{% relref "getting_started/upload" %}}). Calaos Installer déclare au serveur qu'il comprend les devices (`io_format` 2) ; un serveur trop ancien pour les devices refuse l'envoi plutôt que de recevoir un fichier qu'il ne comprend pas.
