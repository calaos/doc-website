+++
title = 'Zigbee'
date = 2024-03-06T20:04:35Z
weight = 40
summary = 'Support du Zigbee avec zigbee2mqtt'
+++

## Zigbee avec Zigbee2MQTT sur Calaos

### Introduction

**Zigbee** est un protocole de communication sans fil conçu pour les applications domotiques et de domotique industrielle, offrant une faible consommation d'énergie et une portée étendue grâce à son maillage réseau. Il permet à divers dispositifs tels que des ampoules connectées, des capteurs et des interrupteurs de communiquer entre eux.

**Zigbee2MQTT** est une passerelle qui permet de contrôler des appareils Zigbee via MQTT sans avoir besoin d'une passerelle propriétaire. Il traduit les messages Zigbee en messages MQTT, facilitant ainsi l'intégration avec des systèmes domotiques comme Calaos.

#### Fonctionnement de Zigbee2MQTT

- **Adaptateur Zigbee** : Un dongle USB ou un module Ethernet qui se connecte au réseau Zigbee.
- **Serveur Zigbee2MQTT** : Logiciel qui tourne sur votre serveur Calaos et gère la communication entre les dispositifs Zigbee et MQTT.
- **MQTT Broker** : Serveur de messagerie qui relaie les messages entre Zigbee2MQTT et Calaos. Il faudra l'activer au préalable [MQTT]({{% relref "hardware/mqtt" %}})

#### Rôle des Routeurs Zigbee

Les dispositifs Zigbee peuvent agir en tant que **routeurs** ou **coordinateurs** pour étendre la portée du réseau. En ajoutant des dispositifs secteur (comme des prises intelligentes ou des ampoules connectées), vous pouvez augmenter la couverture du signal Zigbee dans toute votre maison.

---

### Activation et Configuration de Zigbee2MQTT sur Calaos

#### Prérequis

- Un adaptateur Zigbee compatible :

  - **Soit un adaptateur USB (comme le ConBee II)**. L'adaptateur USB devra être connecté sur votre serveur Calaos. Si il se trouve dans une cave, la communication peut être difficile avec les équipements aux étages.
  - **Soit un adaptateur Réseau (comme le SMLIGHT SLZB-06)**. Peut etre placé n'importe où dans la maison si il y a une prise réseau. De plus le SLZB-06 est POE (power over ethernet)
  
- Consultez la liste complète des adaptateurs supportés : [Zigbee2MQTT Adapters](https://www.zigbee2mqtt.io/guide/adapters/)

> Le type d'adaptateur déterminera la manière de configurer zigbee2mqtt.

#### Étapes d'Activation

1. **Configuration de Zigbee2MQTT**

Le fichier de configuration se trouve à l'emplacement :

```bash
/mnt/calaos/zigbee2mqtt/configuration.yaml
```

> Avec l'éditeur `nano` en ligne de commande il est possible d'éditer ce fichier.

La configuration ressemble à ca:

```yaml
# Home Assistant integration (MQTT discovery)
homeassistant: false

# allow new devices to join
permit_join: true

# MQTT settings
mqtt:
  # MQTT base topic for zigbee2mqtt MQTT messages
  base_topic: zigbee2mqtt
  # MQTT server URL
  server: 'mqtt://127.0.0.1'
  # MQTT server authentication, uncomment if required:
  # user: my_user
  # password: my_password

# Serial settings
serial:
  # Location of CC2531 USB sniffer
  port: /dev/ttyACM0

frontend: true
```

Selon l'adaptateur choisi il faudra configurer l'option `port` correctement afin de permettre à zigbee2mqtt de l'utiliser:

- **Pour le ConBee II (USB)** :

```yaml
serial:
  port: '/dev/ttyACM0'
```

- **Pour le SMLIGHT SLZB-06 (Ethernet POE)** :

```yaml
serial:
  port: 'tcp://slzb-06.local:6638'
```

Il faudra également modifier les paramètres MQTT si besoin, en particulier `username` et `password`.

> Par défaut dans Calaos-OS, MQTT est configuré sans authentification. Si vous souhaitez plus de sécurité il est préférable de configurer un nom d'utilisateur et mot de passe.

2. **Activation de Zigbee2MQTT**

Zigbee2MQTT est déjà inclus dans **Calaos OS**. Pour l'activer, exécutez la commande suivante dans le terminal de votre serveur Calaos :

```bash
systemctl enable zigbee2mqtt.service
systemctl restart zigbee2mqtt.service
```

3. **Redémarrage du Service**

Après avoir modifié la configuration, redémarrez le service pour appliquer les changements :

```bash
systemctl restart zigbee2mqtt.service
```

---

### Utilisation de Zigbee2MQTT via la Page Web

#### Accès à l'Interface Web

- Ouvrez un navigateur web.
- Accédez à `http://<adresse-ip-de-calaos>:8080`.
- Vous verrez le tableau de bord de Zigbee2MQTT avec la liste des dispositifs connectés.

Sur cette interface il vous faudra faire l'appairage et configuration des dispositifs Zigbee avant de pouvoir les ajouter dans Calaos.

---

### Appairage d'un Dispositif et Configuration dans Zigbee2MQTT

#### Appairage du Dispositif

1. **Activer le Mode Appairage**

   Sur l'interface web de Zigbee2MQTT, cliquez sur le bouton **"Permit join (All devices)"** pour autoriser l'appairage de nouveaux dispositifs.

2. **Mettre le Dispositif en Mode Appairage**

   Suivez les instructions du fabricant pour mettre votre dispositif en mode appairage. Il est aussi possible de consulter la page du dispositif sur le site de Zigbee2MQTT pour savoir comment faire l'appairage.

   > Exemple: Pour une ampoule connecté TRADFRI, l'explication se trouve ici: [Zigbee2mqtt TRADFRI](https://www.zigbee2mqtt.io/devices/LED1546G12.html)

3. **Vérification de l'Appairage**

   Une fois le dispositif détecté, il apparaîtra dans la liste avec un identifiant par défaut.

### Configuration du Dispositif

1. **Renommer le Dispositif**

   - Cliquez sur le dispositif dans la liste.
   - Attribuez-lui un nom convivial pour une identification facile dans Calaos.

2. **Ajouter des Routeurs pour Étendre le Réseau**

   - Installez des dispositifs secteur comme des prises connectées ou des ampoules.
   - Ces dispositifs agiront comme des routeurs Zigbee, étendant la portée du réseau.

---

### Ajout du Dispositif dans Calaos avec Calaos Installer

Calaos utilise MQTT pour communiquer avec Zigbee2MQTT. Il faudra donc ajouter un IO MQTT pour chaque appareils que vous souhaitez piloter dans Calaos. Il faut prendre en compte qu'un IO dans Calaos représente une **valeur**. Prenons l'exemple d'une sonde de températeur et humidité [Aquara](https://www.zigbee2mqtt.io/devices/WSDCGQ12LM.html). On voit dans la doc que cet appareil va **exposer** plusieurs valeurs: `temperature`, `humidity`, `pressure`, `battery`, ... Dans Calaos il faudra créer un IO pour chacune des propriétés: Un IO pour la température, un IO pour l'humidité, etc...

Dans Calaos Installer il y a un outil dédié qui est capable de se connecter au Broker MQTT et de détecter les appareils zigbee disponibles sur Zigbee2MQTT. Il faudra sélectionner la propriété avec laquelle vous souhaitez créer un IO, et l'assistant fera la configuration pour vous.

![zigbee2mqtt](/en/hardware/images/z2q_01.png?width=10pc&classes=shadow)

![zigbee2mqtt discovery](/en/hardware/images/z2q_02.png?width=20pc&classes=shadow)

---

## Ressources Supplémentaires

- **Documentation Zigbee2MQTT** : [zigbee2mqtt.io](https://www.zigbee2mqtt.io/)
- **Adaptateurs Compatibles** : [Zigbee2MQTT Adapters](https://www.zigbee2mqtt.io/guide/adapters/)
- **Appareils zigbee supportés** : [Zigbee2MQTT Devices](https://www.zigbee2mqtt.io/supported-devices/)
- **Documentation MQTT dans Calaos** : [Calaos MQTT]({{% relref "hardware/mqtt" %}})
