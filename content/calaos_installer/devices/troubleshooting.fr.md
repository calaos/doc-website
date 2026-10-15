+++
title = "Dépannage"
date = 2026-10-15T09:35:00Z
weight = 60
summary = "« Inconnu », « Hors ligne », widget « Unsupported », device invisible : les causes et les commandes de diagnostic."
+++

## Les trois commandes de diagnostic

**Ce que le module publie sur MQTT** (depuis un poste avec `mosquitto-clients`, ou avec MQTTX) :

```sh
mosquitto_sub -h 192.168.1.10 -p 1883 -u utilisateur -P motdepasse -v -t 'clim_salon/#'
```

**Ce que dit le serveur Calaos** (en SSH sur la box, voir [Accès SSH]({{% relref "calaos_os/advanced/shell" %}})) :

```sh
journalctl -u calaos-server -f
journalctl -u calaos-server --since "1 hour ago" | grep -i device
```

Les messages concernant les devices (lien introuvable, commande refusée, sortie déjà prise, écriture sur une sortie protégée) portent le mot `device`.

**Ce que le serveur envoie aux interfaces** : la réponse `get_home` contient, pour chaque device, l'objet `device` (capacités, état, IO liés). Avec `curl` et `jq` :

```sh
curl -s -X POST http://192.168.1.10:5454/api.php \
  -d '{"action":"get_home","cn_user":"user","cn_pass":"pass"}' \
  | jq '.. | objects | select(.gui_type? == "climate")'
```

Un device absent de cette sortie n'est pas chargé ou pas visible (voir plus bas). La clé `state` d'un device donne le mode ; `device.state` donne la température, les consignes, l'action, et `connected`, `fault` ou `lockout` quand ils s'appliquent.

## « Mode inconnu » : le device n'a reçu aucun mode

Le device lit son mode sur le topic `<préfixe>/climate/<identifiant>/mode/state`. Rien n'arrive :

1. Écoutez `mosquitto_sub -v -t '<préfixe>/#'`. Si le module ne publie rien, le problème est côté ESPHome (Wi-Fi, broker, `topic_prefix`).
2. Si le module publie, comparez le **préfixe** et l'**identifiant** de l'entité avec ceux du device (éditeur de device, IO « mode », champ `topic_sub`). Une faute ou la confusion entre le nom du module et le nom de l'entité est la cause la plus fréquente : voir [Clim ESPHome]({{% relref "hardware/esphome_climate" %}}).
3. Vérifiez « Deux consignes » : si le module publie `target_temperature_low/state` et `target_temperature_high/state`, le device doit avoir deux consignes ; s'il publie `target_temperature/state`, une seule.
4. Vérifiez que le broker, le port, l'utilisateur et le mot de passe du device sont ceux du module.
5. Redémarrez le module pour qu'il republie son état.

## « Hors ligne »

Le device est « hors ligne » quand le topic `<préfixe>/status` ne vaut pas `online`. ESPHome le publie à la connexion et le fait passer à `offline` à la déconnexion (message de dernière volonté). Vérifiez avec `mosquitto_sub -v -t '<préfixe>/status'` : module éteint, perdu sur le Wi-Fi, ou autre `topic_prefix` que celui du device.

## « Sonde muette », « Sonde déconnectée »... sur un thermostat

Ce sont des sûretés, pas des pannes du thermostat : voir le tableau de [Thermostat interne]({{% relref "calaos_installer/devices/thermostat" %}}). Vérifiez la sonde liée (valeur, connexion) ; dès qu'elle livre une lecture valide, la régulation reprend d'elle-même.

## Le thermostat ne chauffe pas

1. Le statut affiché donne la raison : « Démarre dans N min » (durée minimale d'arrêt en cours), une sûreté, « En veille » (la température est dans la bande, c'est normal), « Arrêté » (mode `off`).
2. Regardez l'IO **action** du thermostat : `heating`, `idle` ou `off`.
3. Si l'action est `heating` mais que le relais ne bouge pas, le problème est en aval : le relais ou son pilote (Wago...).
4. Si le relais s'éteint juste après avoir été allumé, une règle ou un script l'écrit aussi : le journal contient des avertissements « sortie protégée ». Commandez le mode, pas le relais.

## La clim ne suit pas la température de la pièce

Le topic `<préfixe>/calaos/remote_temp` doit recevoir une valeur toutes les 120 s :

```sh
mosquitto_sub -h 192.168.1.10 -v -t 'clim_salon/calaos/remote_temp'
```

- Rien : la sonde de la pièce n'a rien lu depuis le démarrage, est muette depuis `sensor_timeout`, ou le lien « Sonde de la pièce » manque. Calaos ne relaie jamais une valeur périmée.
- Des valeurs arrivent mais la clim les ignore : vérifiez dans le YAML le topic du capteur `mqtt_subscribe`, `remote_temperature_source` et `remote_temperature_timeout` (supérieur à 120 s).

Pour tester le côté ESPHome sans Calaos : `mosquitto_pub -h ... -t 'clim_salon/calaos/remote_temp' -m 21.5`.

## Une commande ne fait rien ou revient en arrière

Une consigne hors des bornes est ramenée dans les bornes. Un champ ou une valeur que le device ne propose pas (une ventilation absente du paramètre `fan_modes`, `cool` sans sortie de refroidissement) est **refusé et journalisé** : cherchez la ligne dans `journalctl -u calaos-server`. Un écart trop faible entre consigne basse et haute (`min_range`) est refusé de même.

## Le widget de l'écran mural affiche « Unsupported »

Le type de widget écrit par Calaos Installer n'est pas connu du panneau. Mettez à jour Calaos Installer, ouvrez la page de l'écran dans l'éditeur Remote UI (le widget reprend le type **Climate**), envoyez la configuration, puis mettez à jour le logiciel du panneau si le problème subsiste : un panneau trop ancien n'a pas le widget Climate.

## Le device n'apparaît pas dans l'application web, le mobile ou Calaos Home

1. Le serveur est-il à jour ? Un serveur ancien n'écrivait pas l'attribut `visible` d'un device, que les applications exigent. Sans mise à jour, ajoutez `visible="true"` à l'élément `<calaos:device>`.
2. Le device est-il dans `get_home` (commande `curl` plus haut) ? Sinon, regardez le journal au démarrage du serveur : un fichier mal formé ou une erreur de chargement y est signalé.
3. A-t-il bien été envoyé ? Le device n'existe sur le serveur qu'après l'[envoi du projet]({{% relref "getting_started/upload" %}}).

## Calaos Installer refuse d'envoyer ou d'ouvrir le projet

- « ce io.xml supprimerait N device(s) » : le serveur protège ses devices d'un Calaos Installer trop ancien. Mettez-le à jour.
- Le projet s'ouvre en lecture seule : il a été enregistré par une version plus récente du format. Mettez Calaos Installer à jour.
