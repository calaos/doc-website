+++
title = "Clim ESPHome"
date = 2026-10-15T09:15:00Z
weight = 20
summary = "Brancher un split Mitsubishi ou Fujitsu piloté par ESPHome : YAML, topics MQTT, température de pièce relayée, une ou deux consignes."
+++

Ce guide décrit une clim qui **régule seule** et que Calaos pilote par MQTT : un module ESP32 relié à la prise de service de l'unité intérieure, qui tourne sous [ESPHome](https://esphome.io). L'exemple utilise un split Mitsubishi relié par son connecteur CN105 ; la démarche est la même pour un Fujitsu.

```text
Unité intérieure ──(série)── ESP32 / ESPHome ──(MQTT)── broker Mosquitto ── Calaos
                                                              ▲
                              température de la pièce ────────┘ (relayée par Calaos)
```

Calaos ne régule pas : la clim garde sa propre modulation. Calaos lit son état, envoie le mode, les consignes, la ventilation et l'oscillation, et peut lui **relayer la température d'une sonde de la pièce**, plus juste que celle de l'unité installée haut sur le mur.

{{% notice info %}}
Calaos n'a pas de découverte automatique ESPHome. Le modèle `esphome_climate` de l'assistant crée en une fois tous les IO MQTT nécessaires ; il demande le **préfixe de topic** et l'**identifiant de l'entité** du module (voir plus bas pour les trouver).
{{% /notice %}}

## Le module ESPHome : exemple Mitsubishi CN105

Le composant utilisé est [MitsubishiCN105ESPHome](https://github.com/echavet/MitsubishiCN105ESPHome). Voici le YAML d'un module, sans ses secrets :

```yaml
esphome:
  name: clim-salon

esp32:
  board: esp32dev
  framework:
    type: esp-idf

# INFO : au niveau DEBUG le composant CN105 est très bavard.
logger:
  level: INFO

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password
  reboot_timeout: 2min

ota:
  - platform: esphome
    password: !secret ota_password

mqtt:
  broker: !secret mqtt_broker
  port: !secret mqtt_port
  username: !secret mqtt_username
  password: !secret mqtt_password
  discovery: false          # Calaos n'utilise pas la découverte Home Assistant
  topic_prefix: "clim_salon"
  log_topic: null           # ne pas publier les journaux du module sur le broker
  id: mqtt_client

external_components:
  - source: github://echavet/MitsubishiCN105ESPHome

uart:
  - id: uart_clim
    tx_pin: GPIO32
    rx_pin: GPIO33
    baud_rate: 2400
    rx_full_threshold: 120

sensor:
  # Température de la pièce, publiée par Calaos (voir « Température de pièce relayée »).
  - platform: mqtt_subscribe
    id: calaos_room_temp
    topic: clim_salon/calaos/remote_temp
    internal: true

climate:
  - platform: cn105
    id: clim_salon_hp
    name: Clim salon
    uart_id: uart_clim
    remote_temperature_source:
      sensor_id: calaos_room_temp
    remote_temperature_timeout: 15min
    update_interval: 3500ms
    debounce_delay: 100ms
    supports:
      dual_setpoint: true
      mode: [HEAT_COOL, COOL, HEAT, DRY, FAN_ONLY]
      fan_mode: [AUTO, QUIET, LOW, MEDIUM, HIGH]
      swing_mode: ["OFF", VERTICAL, HORIZONTAL, BOTH]
    visual:
      min_temperature: 17
      max_temperature: 28
      temperature_step:
        target_temperature: 0.5
        current_temperature: 0.1
```

Les capteurs de diagnostic du composant (fréquence du compresseur, puissance, énergie, température extérieure, signal Wi-Fi...) sont facultatifs et ne servent pas à Calaos ; ajoutez ceux que vous voulez.

### Les lignes qui comptent pour Calaos

| Ligne | Pourquoi |
|---|---|
| `mqtt:` avec `discovery: false` et un `topic_prefix` | ESPHome publie sous `<préfixe>/climate/<id>/...` ; ce préfixe est demandé par l'assistant. |
| `logger: level: INFO` et `log_topic: null` | Au niveau `DEBUG`, le composant publie ses journaux en MQTT en continu : le broker est noyé, et le module ralentit. Gardez `INFO`, et coupez la publication des journaux sur le broker. |
| `supports: dual_setpoint: true` | Le climat publie une consigne **basse** et une consigne **haute** au lieu d'une seule. Requis pour le mode `HEAT_COOL`. |
| `mode: [HEAT_COOL, ...]` | `HEAT_COOL` (chaud ou froid automatiquement entre deux consignes) correspond au mode **`auto`** de Calaos. |
| `remote_temperature_source` et le capteur `mqtt_subscribe` | C'est ce qui permet à Calaos de fournir la température de la pièce. |
| `remote_temperature_timeout` | Au bout de ce délai sans nouvelle valeur, le composant revient à la sonde de l'unité. Il doit être **plus long** que l'intervalle de rappel de Calaos (120 s par défaut). |

### La température de pièce relayée

Le capteur `mqtt_subscribe` lit le topic `<préfixe>/calaos/remote_temp`. Dans Calaos, le device écrit sur ce topic la valeur de la **sonde de la pièce** que vous avez choisie dans l'assistant (champ « Sonde de la pièce »), à chaque changement **et** toutes les `remote_temp_keepalive` secondes (120 s par défaut). Le rappel périodique est obligatoire : sans nouvelle valeur pendant `remote_temperature_timeout`, la clim repasse sur sa sonde interne.

Calaos cesse de relayer une sonde qui ne dit plus rien depuis `sensor_timeout` secondes (9000 s par défaut) ou qui se déclare déconnectée : la clim retombe alors d'elle-même sur sa sonde, sans régler sur une valeur périmée.

{{% notice note %}}
Seuls les modules qui acceptent une température distante (ici le CN105 à liaison série) en profitent. Une clim **infrarouge** (`climate_ir`) ne peut pas en recevoir : voir le modèle `esphome_climate_ir` dans [Créer un device]({{% relref "calaos_installer/devices/templates" %}}).
{{% /notice %}}

## Exemple Fujitsu

Le composant [esphome-fujitsu-halcyon](https://github.com/Omniflux/esphome-fujitsu-halcyon) pilote les unités Fujitsu AirStage-H (Halcyon) par leur bus série :

```yaml
esphome:
  name: clim-fuji

esp32:
  board: esp32dev
  framework:
    type: arduino

logger:
  level: INFO

mqtt:
  broker: !secret mqtt_broker
  port: !secret mqtt_port
  username: !secret mqtt_username
  password: !secret mqtt_password
  discovery: false
  topic_prefix: "clim_fuji"
  log_topic: null

external_components:
  - source: github://Omniflux/esphome-fujitsu-halcyon

uart:
  - id: uart_clim
    tx_pin: GPIO17
    rx_pin: GPIO16
    baud_rate: 500
    parity: EVEN
    rx_full_threshold: 120

climate:
  - platform: fujitsu-halcyon
    id: clim_fuji_hp
    name: Clim Fuji
    uart_id: uart_clim
    controller_address: 1      # 0 = principal, 1 = secondaire
```

Ce composant publie une **consigne unique** : dans l'assistant, laissez « Deux consignes » décoché. Son mode `AUTO` est envoyé en `auto` : réglez le paramètre `auto_payload` du device sur `auto` (par défaut, Calaos écrit `heat_cool`). La température distante dépend de ce que le composant accepte (il possède une option `temperature_sensor_id` pour utiliser un capteur ESPHome) : consultez sa documentation, puis alimentez ce capteur avec un `mqtt_subscribe` comme ci-dessus.

## Trouver le préfixe et l'identifiant de l'entité

L'assistant demande deux valeurs qui se lisent dans les topics MQTT du module :

```text
<préfixe>/climate/<identifiant>/mode/state
```

- le **préfixe** est le `topic_prefix` du bloc `mqtt:` du module (par défaut, le nom du module) ;
- l'**identifiant** est le nom de l'entité climate (`name:`) en minuscules, avec les espaces et la ponctuation remplacés par des tirets bas. « Clim salon » donne `clim_salon`, « Clim Mitsu Buanderie » donne `clim_mitsu_buanderie`.

{{% notice warning %}}
Ne confondez pas l'identifiant de l'entité avec le nom du module : pour un module `clim-mitsu` dont l'entité s'appelle « Clim Mitsu Buanderie », le préfixe est `clim_mitsu` et l'identifiant `clim_mitsu_buanderie`. Un identifiant faux ne produit aucune erreur : le device reste simplement muet.
{{% /notice %}}

Le plus sûr est d'écouter ce que le module publie. Depuis un poste qui a `mosquitto-clients` (ou avec un outil graphique comme MQTTX, voir [MQTT]({{% relref "hardware/mqtt" %}})) :

```sh
mosquitto_sub -h 192.168.1.10 -p 1883 -u utilisateur -P motdepasse -v -t 'clim_salon/#'
```

Après une ou deux minutes, vous voyez défiler :

```text
clim_salon/status online
clim_salon/climate/clim_salon/mode/state heat_cool
clim_salon/climate/clim_salon/current_temperature/state 19.2
clim_salon/climate/clim_salon/target_temperature_low/state 19.5
clim_salon/climate/clim_salon/target_temperature_high/state 24.5
clim_salon/climate/clim_salon/action/state heating
clim_salon/climate/clim_salon/fan_mode/state auto
clim_salon/climate/clim_salon/swing_mode/state off
```

Pour ne lister que les climats d'un broker : `mosquitto_sub -h ... -v -t '+/climate/#' -W 60`.

## Une ou deux consignes

Cela dépend du **module**, pas de la clim, et se lit dans les topics :

| Topics publiés | Dans l'assistant |
|---|---|
| `.../target_temperature/state` | « Deux consignes » **décoché** |
| `.../target_temperature_low/state` et `.../target_temperature_high/state` | « Deux consignes » **coché** |

Avec `dual_setpoint: true` dans le YAML, le module publie la consigne basse et la consigne haute et **plus** la consigne unique. Cocher la mauvaise case n'affiche aucune erreur : le device ne reçoit jamais la consigne qu'il attend et reste « Inconnu ». Une clim à consigne unique peut tout de même offrir une plage dans Calaos : voir la double consigne émulée plus bas.

## Les vitesses de ventilation et l'oscillation

Calaos ne traduit rien : il envoie et reçoit les chaînes **d'ESPHome**, en minuscules. La liste cochée dans l'assistant doit correspondre à `supports: fan_mode:` du YAML :

| YAML ESPHome | Dans l'assistant |
|---|---|
| `AUTO`, `QUIET`, `LOW`, `MEDIUM`, `HIGH`, `MIDDLE` | Auto, Silencieux, Bas, Moyen, Haut, Milieu |
| `swing_mode: ["OFF", VERTICAL, HORIZONTAL, BOTH]` | Arrêt, Vertical, Horizontal, Les deux |

Un mode coché dans Calaos mais absent du YAML est refusé par le module ; un mode du YAML non coché n'est simplement pas proposé. Pour vérifier une valeur, regardez le topic `fan_mode/state` pendant que vous changez la ventilation à la télécommande.

## Le device correspondant dans io.xml

Le modèle `esphome_climate` écrit, pour le module Mitsubishi ci-dessus avec **une consigne** (extrait ; les attributs de connexion `host`, `port`, `data` et `connected_status_*` sont répétés sur chaque IO et omis ici) :

```xml
<calaos:device id="salon_ac" type="climate" impl="proxy" name="Clim salon" visible="true">
  <calaos:output id="salon_ac_mode" type="MqttOutputString" visible="false"
      topic_pub="clim_salon/climate/clim_salon/mode/command"
      topic_sub="clim_salon/climate/clim_salon/mode/state" />
  <calaos:input id="salon_ac_current_temp" type="MqttInputTemp" visible="false"
      topic_sub="clim_salon/climate/clim_salon/current_temperature/state" />
  <calaos:output id="salon_ac_target_temp" type="MqttOutputAnalog" visible="false"
      topic_pub="clim_salon/climate/clim_salon/target_temperature/command"
      topic_sub="clim_salon/climate/clim_salon/target_temperature/state"
      value_min="17" value_max="28" step="0.5" unit="°C" />
  <calaos:input id="salon_ac_action" type="MqttInputString" visible="false"
      topic_sub="clim_salon/climate/clim_salon/action/state" />
  <calaos:output id="salon_ac_fan_mode" type="MqttOutputString" visible="false"
      topic_pub="clim_salon/climate/clim_salon/fan_mode/command"
      topic_sub="clim_salon/climate/clim_salon/fan_mode/state" />
  <calaos:output id="salon_ac_remote_temp" type="MqttOutputAnalog" visible="false"
      topic_pub="clim_salon/calaos/remote_temp"
      topic_sub="clim_salon/calaos/remote_temp" />

  <calaos:link name="mode" io="salon_ac_mode" />
  <calaos:link name="current_temp" io="salon_ac_current_temp" />
  <calaos:link name="target_temp" io="salon_ac_target_temp" />
  <calaos:link name="action" io="salon_ac_action" />
  <calaos:link name="fan_mode" io="salon_ac_fan_mode" />
  <calaos:link name="remote_temp" io="salon_ac_remote_temp" />
  <calaos:link name="remote_temp_source" io="salon_temp" />   <!-- sonde de la pièce, IO extérieur -->

  <calaos:param name="modes" value="off,heat,cool,auto,dry,fan_only" />
  <calaos:param name="auto_payload" value="heat_cool" />
  <calaos:param name="fan_modes" value="auto,quiet,low,medium,high" />
  <calaos:param name="remote_temp_keepalive" value="120" />
  <calaos:param name="sensor_timeout" value="9000" />
</calaos:device>
```

Avec **deux consignes** (`dual_setpoint: true`), la consigne unique est remplacée par deux IO et deux liens :

```xml
  <calaos:output id="salon_ac_target_temp_low" type="MqttOutputAnalog" visible="false"
      topic_pub="clim_salon/climate/clim_salon/target_temperature_low/command"
      topic_sub="clim_salon/climate/clim_salon/target_temperature_low/state"
      value_min="17" value_max="28" step="0.5" unit="°C" />
  <calaos:output id="salon_ac_target_temp_high" type="MqttOutputAnalog" visible="false"
      topic_pub="clim_salon/climate/clim_salon/target_temperature_high/command"
      topic_sub="clim_salon/climate/clim_salon/target_temperature_high/state"
      value_min="17" value_max="28" step="0.5" unit="°C" />

  <calaos:link name="target_temp_low" io="salon_ac_target_temp_low" />
  <calaos:link name="target_temp_high" io="salon_ac_target_temp_high" />
```

Les interfaces affichent alors deux curseurs en mode `auto`, séparés au minimum de `min_range`.

## Double consigne émulée

{{% notice warning %}}
Cette fonction (ticket CLIM-8) est **en cours de développement**. La description ci-dessous suit la décision de conception ; les noms exacts peuvent évoluer avant la publication.
{{% /notice %}}

Beaucoup de clims n'ont qu'**une** consigne et pas de mode `auto` entre deux consignes. Le paramètre `emulate_range` du device (désactivé par défaut) donne à Calaos le rôle manquant : le device **possède** une consigne basse et une consigne haute, mémorisées par Calaos, et les interfaces affichent la plage comme pour une clim à deux consignes (l'option du modèle s'appelle « double consigne émulée »).

En mode `auto`, Calaos pilote l'unité en **mode simple** :

| Température de la pièce | Ce que Calaos fait faire à l'unité |
|---|---|
| sous `basse − hystérésis` | passe en `heat`, avec la consigne basse |
| au-dessus de `haute + hystérésis` | passe en `cool`, avec la consigne haute |
| revenue dans la bande | passe en `off` : **rien ne consomme entre les deux consignes** |

Pour ménager le compresseur, Calaos applique une hystérésis, des durées minimales de marche et d'arrêt, et un délai entre un passage en chaud et un passage en froid. La référence est la sonde de la pièce (celle relayée à l'unité si vous en avez choisi une) ; une sonde muette ne provoque aucun changement. Calaos n'envoie jamais `auto` à l'unité.

Si quelqu'un change le mode **à la télécommande**, l'émulation est suspendue (un message est journalisé) : Calaos ne reprend pas la main sur un choix fait sur place.

## Et si ça ne marche pas

La page [Dépannage]({{% relref "calaos_installer/devices/troubleshooting" %}}) explique les symptômes classiques : état « Inconnu », « Hors ligne », device invisible.
