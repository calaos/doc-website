+++
title = 'MQTT'
date = 2024-03-06T20:04:27Z
weight = 30
summary = 'Appareils utilisant le protocole MQTT'
+++

## Introduction à MQTT

MQTT (Message Queuing Telemetry Transport) est un protocole de messagerie léger, idéal pour les appareils à ressources limitées et les réseaux de faible bande passante. MQTT fonctionne sur un modèle de publication/abonnement, où les clients peuvent publier des messages sur des "topics" spécifiques et s'abonner à ces topics pour recevoir des messages.

### Qu'est-ce qu'un Broker MQTT ?

Un broker MQTT est un serveur qui reçoit les messages publiés par les clients et les distribue aux abonnés correspondants. Mosquitto est un exemple populaire de broker MQTT, qui peut être utilisé avec Calaos.

### Activation de Mosquitto dans Calaos

Mosquitto est déjà intégré dans Calaos OS. Il suffit de l'activer pour l'utiliser.

Pour activer Mosquitto dans Calaos, utilisez la commande suivante :
```bash
systemctl enable --now mosquitto
```
Il est possible de configurer l'authentification et d'autres paramètres dans le fichier de configuration situé à `/mnt/calaos/mosquitto/config/mosquitto.conf`.

Pour appliquer les modifications de configuration, redémarrez Mosquitto avec :
```bash
systemctl restart mosquitto
```

## Débogage

Pour déboguer ce qui se passe avec MQTT, vous pouvez utiliser un outil comme MQTTX. MQTTX permet de se connecter au broker MQTT et de surveiller les messages échangés, ce qui est utile pour tester et vérifier les configurations des topics et des payloads.

## Utilisation de MQTT dans Calaos

Dans Calaos, MQTT peut être utilisé pour différents types d'entrées et sorties (IO). Chaque IO MQTT doit être configuré avec `topic_sub`, `topic_pub` et `path`.

### Types d'IO MQTT supportés

- Sonde de température
- Interrupteur
- Entrée analogique
- Entrée texte
- Lumière On/Off
- Lumière à variation
- Lumière RGB
- Sortie analogique

## Exemple de configuration d'une sonde de température MQTT

Pour configurer une sonde de température MQTT dans Calaos, supposons que la sonde envoie sa valeur au format JSON dans le topic `mqtt_devices/temp_01` avec le payload JSON suivant : `{ "temp": 24.15, "humidity": 43, "battery": 85 }`. Pour cette configuration, nous nous concentrerons sur la valeur de la température.

### Paramètres de configuration :

Voici la configuration à adopter dans Calaos Installer lors de la création de l'IO MQTT Température:

| Nom         | Valeur                                                                                 |
|-------------|---------------------------------------------------------------------------------------------|
| name        | `Sonde Température`                                    |
| topic_sub   | `mqtt_devices/temp_01`                                     |
| path        | `temp`         |
| host        | `127.0.0.1` |
| port        | `1883`|
| precision   | `1` |

En configurant ces paramètres dans Calaos, vous pourrez recevoir et afficher les valeurs de température envoyées par la sonde de température MQTT.

### Paramètres généraux pour les IO MQTT

| Nom         | Type   | Obligatoire | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| topic_sub   | string | oui         | Topic sur lequel s'abonner.                                                                                                                      |
| topic_pub   | string | oui         | Topic sur lequel publier.                                                                                                                        |
| path        | string | oui         | Chemin où trouver la valeur dans le payload MQTT. Si le payload est JSON, les informations seront extraites en fonction du chemin.                |
| user        | string | non         | Utilisateur pour l'authentification avec le broker MQTT. Le mot de passe doit être défini dans ce cas.                                            |
| keepalive   | int    | non         | Temps de keepalive en secondes. Temps entre deux PING MQTT.                                                                                       |
| port        | int    | non         | Port TCP du broker MQTT. La valeur par défaut est 1883.                                                                                           |
| password    | string | non         | Mot de passe pour l'authentification avec le broker MQTT. L'utilisateur doit être défini dans ce cas.                                             |
| host        | string | non         | Adresse IP du broker MQTT. La valeur par défaut est 127.0.0.1.                                                                                    |
| enabled     | bool   | non         | Activer l'Entrée/Sortie. La valeur par défaut est true. Ce paramètre est ajouté s'il n'est pas trouvé dans la configuration.                        |
| log_history | bool   | non         | Si activé, écrire une entrée dans l'historique des événements pour cet IO.                                                                        |
| visible     | bool   | non         | Afficher l'Entrée/Sortie sur toutes les interfaces utilisateur si défini. Par défaut, true.                                                       |
| logged      | bool   | non         | Si activé, et si influxdb est activé dans la configuration locale, envoyer la valeur à influxdb pour cet IO.                                      |
| gui_type    | string | non         | Type graphique interne pour tous les objets Calaos. Paramètre en lecture seule, défini automatiquement.                                           |
| io_type     | string | oui         | Type d'IO, peut être "input", "output", "inout".                                                                                                  |
| id          | string | oui         | ID unique identifiant l'Entrée/Sortie dans calaos-server.                                                                                        |

### Paramètres spécifiques pour chaque type d'IO MQTT

#### MqttInputAnalog

Entrée analogique utilisée pour lire des valeurs analogiques, les afficher et les utiliser dans les règles.

| Nom         | Type   | Obligatoire | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| topic_sub   | string | oui         | Topic sur lequel s'abonner.                                                                                                                      |
| topic_pub   | string | oui         | Topic sur lequel publier.                                                                                                                        |
| precision   | int    | non         | Précision de la valeur retournée. Représente le nombre de décimales après la virgule.                                                            |
| name        | string | oui         | Nom de l'Entrée/Sortie.                                                                                                                          |
| path        | string | oui         | Chemin où trouver la valeur dans le payload MQTT.                                                                                                |
| io_style    | list   | oui         | Style d'affichage de l'interface utilisateur.                                                                                                    |
| coeff_a     | float  | non         | Utilisé avec coeff_b pour appliquer une équation de la forme `valeur_affichée = coeff_a * valeur_brute + coeff_b`. La valeur par défaut est 1.0.  |
| coeff_b     | float  | non         | Utilisé avec coeff_a pour appliquer une équation de la forme `valeur_affichée = coeff_a * valeur_brute + coeff_b`. La valeur par défaut est 0.0.  |
| unit        | string | non         | Unité affichée sur l'interface utilisateur en tant que suffixe.                                                                                  |

#### MqttInputString

Entrée de texte utilisée pour lire des valeurs de type chaîne de caractères.

| Nom         | Type   | Obligatoire | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| topic_sub   | string | oui         | Topic sur lequel s'abonner.                                                                                                                      |
| topic_pub   | string | oui         | Topic sur lequel publier.                                                                                                                        |
| name        | string | oui         | Nom de l'Entrée/Sortie.                                                                                                                          |
| path        | string | oui         | Chemin où trouver la valeur dans le payload MQTT.                                                                                                |

#### MqttInputSwitch

Interrupteur avec états appuyé/libéré.

| Nom         | Type   | Obligatoire | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| on_value    | string | oui         | Valeur à interpréter comme ON.                                                                                                                   |
| off_value   | string | oui         | Valeur à interpréter comme OFF.                                                                                                                  |

#### MqttInputTemp

Capteur de température utilisé pour afficher la température et contrôler les dispositifs de chauffage via des règles basées sur la valeur de température.

| Nom         | Type   | Obligatoire | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| topic_sub   | string | oui         | Topic sur lequel s'abonner.                                                                                                                      |
| topic_pub   | string | oui         | Topic sur lequel publier.                                                                                                                        |
| precision   | int    | non         | Précision de la valeur retournée.                                                                                                                |
| name        | string | oui         | Nom de l'Entrée/Sortie.                                                                                                                          |
| path        | string | oui         | Chemin où trouver la valeur dans le payload MQTT.                                                                                                |

#### MqttOutputAnalog

Sortie analogique utilisée pour contrôler des dispositifs de sortie analogiques connectés à Calaos.

| Nom         | Type   | Obligatoire | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| data        | string | oui         | Les données envoyées lors de la publication sur le topic.                                                                                       |
| step        | float  | non         | Définir un pas pour augmenter/diminuer la valeur. La valeur par défaut est 1.0.                                                                 |

#### MqttOutputLight

Lumière de base avec seulement deux états, ON ou OFF. Peut également être utilisée pour contrôler des relais simples.

| Nom         | Type   | Obligatoire | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| data        | string | oui         | Les données envoyées lors de la publication sur le topic.                                                                                       |
| on_value    | string | oui         | Valeur à interpréter comme ON.                                                                                                                   |
| off_value   | string | oui         | Valeur à interpréter comme OFF.                                                                                                                  |

#### MqttOutputLightDimmer

Lumière avec contrôle de variation. L'intensité de la lumière peut être modifiée.

| Nom         | Type   | Obligatoire | Description                                                                                                                                      |
|-------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| data        | string | oui         | Les données envoyées lors de la publication sur le topic.                                                                                       |

#### MqttOutputLightRGB

Lumière RGB. Choisissez une couleur à définir pour cette lumière.

| Nom              | Type   | Obligatoire | Description                                                                                                                                      |
|------------------|--------|-------------|--------------------------------------------------------------------------------------------------------------------------------------------------|
| path_brightness  | string | oui         | Chemin où trouver la valeur de luminosité dans le payload MQTT.                                                                                 |
| path_y           | string | oui         | Chemin où trouver la valeur Y (espace couleur X/Y) dans le payload MQTT.                                                                         |
| path_x           | string | oui         | Chemin où trouver la valeur X (espace couleur X/Y) dans le payload MQTT.                                                                         |
| data             | string | oui         | Les données envoyées lors de la publication de la couleur sur le topic.                                                                          |
