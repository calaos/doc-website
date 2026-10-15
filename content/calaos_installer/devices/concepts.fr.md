+++
title = "Le principe d'un device"
date = 2026-10-15T09:05:00Z
weight = 10
summary = "IO possédés, liens, paramètres, capacités : comment Calaos représente une clim ou un thermostat."
+++

## Un équipement, pas vingt IO

Une clim ESPHome publie une vingtaine de valeurs en MQTT : mode, température mesurée, consigne, ventilation, oscillation, état de la connexion... Sans autre outil, il faut créer chacune de ces valeurs comme un IO MQTT, les retrouver ensuite dans l'arborescence, et ne rien oublier à la suppression.

Un **device** règle ce problème : c'est un objet qui regroupe tous les IO d'un même appareil. Il est créé en une fois depuis un modèle (voir [Créer un device]({{% relref "calaos_installer/devices/templates" %}})), il s'affiche comme **un seul équipement** dans toutes les interfaces, et il disparaît **avec tous ses IO** quand on le supprime.

Calaos connaît pour l'instant une famille de devices, le **climat** (`climate`), avec deux comportements :

| Comportement | Qui régule | Exemples |
|---|---|---|
| `proxy` | l'appareil lui-même ; Calaos relaie l'état et les commandes | split Mitsubishi ou Fujitsu via ESPHome, clim infrarouge |
| `thermostat` | Calaos, en commutant des relais | radiateurs électriques, plancher chauffant, pompe à chaleur sur contacts, ventilo-convecteurs |

## Les trois ingrédients

### Les IO possédés

Ce sont des IO ordinaires, déclarés **à l'intérieur** du device : le topic MQTT du mode, celui de la consigne, la variable interne qui garde la consigne d'un thermostat... Le device les crée et les supprime. Ils sont masqués dans les interfaces : on ne voit que le device.

### Les liens

Un **lien** donne un **rôle** à un IO : « cet IO est la température de la pièce », « celui-là est la consigne », « celui-ci est le relais de chauffage ». La cible d'un lien est soit un IO possédé, soit un **IO extérieur** déclaré ailleurs dans le projet : la sonde de température du salon, un relais Wago. Le device ne crée jamais un IO extérieur et ne le supprime jamais.

| Rôle | Sens |
|---|---|
| `mode` | mode de fonctionnement (arrêt, chaud, froid...) |
| `current_temp` | température de la pièce |
| `target_temp` | consigne unique |
| `target_temp_low`, `target_temp_high` | consigne basse et haute (plage du mode `auto`) |
| `action` | ce que fait l'appareil : chauffe, refroidit, au repos... |
| `fan_mode`, `swing_mode` | ventilation et oscillation |
| `remote_temp`, `remote_temp_source` | température de pièce relayée à la clim (voir [Clim ESPHome]({{% relref "hardware/esphome_climate" %}})) |
| `humidity` | humidité de la pièce |
| `output_heat`, `output_cool` | relais de chauffage et de refroidissement d'un thermostat |

### Les paramètres

Un **paramètre** est un réglage du device : hystérésis, durées minimales de marche et d'arrêt, modes proposés, délai avant de déclarer une sonde muette... Les valeurs par défaut viennent du modèle ; l'[éditeur de device]({{% relref "calaos_installer/devices/templates" %}}) permet de les changer après coup.

## Les capacités viennent des liens

Ce que le device sait faire dépend **uniquement** des liens présents. Pas de lien `fan_mode` : pas de réglage de ventilation. Pas de lien `target_temp_low` et `target_temp_high` : pas de plage en mode `auto`. Pas de lien `output_cool` sur un thermostat : pas de mode froid.

Les interfaces lisent ces capacités et n'affichent que ce qui existe. Une clim sans oscillation n'a donc pas de réglage d'oscillation, et un thermostat de radiateur n'a pas de mode `cool` à proposer.

## Ce que voient les règles

Le moteur de règles n'a pas changé : il ne manipule que des valeurs simples. La valeur d'un device est son **mode** : `off`, `heat`, `cool`, `auto`, `dry` ou `fan_only`. Une condition « Clim salon == `off` » fonctionne donc sur n'importe quel climat.

Pour les consignes, la température et l'action, une règle lit les IO liés, qui sont des IO ordinaires.

## Commander un device : une commande, deux formes

Qu'elle vienne d'une règle, d'un script Lua, d'une application ou de l'assistant MCP, une commande envoyée à un device est du texte, sous l'une de ces deux formes :

- **un mode seul** : `off` ou `heat` ;
- **un objet JSON**, qui change plusieurs champs d'un coup : `{"mode":"heat","target_temp":21.5}`.

| Champ | Valeurs |
|---|---|
| `mode` | un des modes que le device propose |
| `target_temp` | consigne unique, bornée et arrondie au pas du device |
| `target_temp_low`, `target_temp_high` | plage de consigne (mode `auto`) |
| `fan_mode`, `swing_mode` | un des modes de ventilation ou d'oscillation du device |

Seuls les champs présents sont appliqués. Chaque champ est vérifié par rapport aux capacités de l'instance : un champ qui n'existe pas, une valeur hors liste sont **refusés et journalisés**, et les autres champs sont appliqués quand même. Les nombres sont ramenés dans les bornes et arrondis au pas.

Dans Calaos Installer, une action de règle qui cible un device affiche un **formulaire** à la place du texte libre (voir [Règles de chauffage]({{% relref "calaos_installer/rules_heating" %}})).

## Ce que ça donne dans io.xml

Dans le fichier de configuration, un device est un élément `<calaos:device>` enfant d'une pièce, qui contient ses IO, ses liens et ses paramètres. Exemple abrégé d'un thermostat :

```xml
<calaos:device id="bureau_heat" type="climate" impl="thermostat" name="Radiateur bureau" visible="true">
  <calaos:internal id="bureau_heat_target_temp" type="InternalInt" save="true" visible="false" />
  <calaos:internal id="bureau_heat_action" type="InternalString" visible="false" />

  <calaos:link name="current_temp" io="bureau_temp" />        <!-- IO extérieur -->
  <calaos:link name="output_heat"  io="bureau_radiateur" />   <!-- IO extérieur -->
  <calaos:link name="target_temp"  io="bureau_heat_target_temp" />
  <calaos:link name="action"       io="bureau_heat_action" />

  <calaos:param name="emitter" value="electric" />
  <calaos:param name="hysteresis" value="0.3" />
  <calaos:param name="min_on_time" value="120" />
  <calaos:param name="min_off_time" value="120" />
</calaos:device>
```

Un fichier qui contient des devices porte `format="2"` sur `<calaos:home>`. Ce marquage protège vos devices : le serveur **refuse** une configuration envoyée par un Calaos Installer qui ne les connaît pas, au lieu de les effacer sans rien dire.
