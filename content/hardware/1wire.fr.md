+++
title = '1-Wire'
date = 2024-03-06T20:04:56Z
weight = 60
summary = 'Sondes de température 1-Wire'
+++

## 1-Wire avec des sondes de température sur Calaos

### Introduction

**1-Wire** est un protocole de communication développé par Dallas Semiconductor (aujourd'hui Maxim Integrated) qui permet la communication entre un maître (tel qu'un microcontrôleur ou un ordinateur) et plusieurs dispositifs 1-Wire à l'aide d'un seul fil de données, plus une connexion à la terre. Le protocole est souvent utilisé pour des capteurs de température, tels que les **DS18B20**, qui offrent une mesure précise et abordable.

La **passerelle USB DS9490R** permet de relier un réseau 1-Wire à un système tel que **Calaos** pour la surveillance des températures, en utilisant des capteurs 1-Wire comme les sondes **DS18B20**.

![1wire](/en/hardware/images/one_wire1.png?width=30pc&classes=shadow)

---

### Prérequis

- **Adaptateur DS9490R** : Un adaptateur USB pour relier le réseau 1-Wire à un serveur Calaos.
![1wire](/en/hardware/images/one_wire6.png?width=15pc&classes=shadow)
- **Sondes DS18B20** : Capteurs de température compatibles 1-Wire.
  
  Ces sondes peuvent être câblées de deux façons : en mode alimenté (3 fils) ou en mode parasite (2fils, alimentation via la ligne de données).

---

### Type de sonde

- **Modèle cablé**: La sonde est protégée dans une enveloppe inox étanche. Elle possède 3 fils.
  - Fil Rouge : Power (5v). Indiqué VDD, VCC, V+ suivant les documentations.
  - Fil Jaune : 1-wire data (données).
  - Fil Noir : GND, 1-wire Ground (0v).

![1wire](/en/hardware/images/one_wire2.png?width=15pc&classes=shadow)

- **Modèle "nu"**:
  
![1wire](/en/hardware/images/one_wire3.png?width=20pc&classes=shadow)

---

### Câblage

![1wire](/en/hardware/images/one_wire4.png?width=30pc&classes=shadow)

#### Mode Alimenté

Dans ce mode, la sonde DS18B20 utilise une alimentation dédiée en plus de la ligne de données et de la masse :

- **Données (DQ)** : Connecté à la ligne de données 1-Wire.
- **Alimentation (Vdd)** : Connecté à une alimentation de 3.3V ou 5V.
- **Masse (GND)** : Connecté à la masse commune.

Ce mode est recommandé car il permet une communication plus stable, surtout sur des distances plus longues.

#### Mode Parasite

En mode parasite, la sonde DS18B20 utilise la ligne de données pour son alimentation, éliminant ainsi le besoin de la connexion à Vdd :

- **Données (DQ)** : Connecté à la ligne de données 1-Wire.
- **Masse (GND)** : Connecté à la masse commune.

Ce mode est plus simple à câbler mais peut être moins fiable, notamment sur de longues distances ou avec plusieurs sondes.

#### Cablâge du DS9490R

![1wire](/en/hardware/images/one_wire7.png?width=30pc&classes=shadow)

---

### Identification des sondes

Une fois vos sondes 1-Wire correctement connectées à l'adaptateur DS9490R, il faudra récupérer les ID unique des sondes pour les affecter à une pièce.

> Il est conseillé de connecter les sondes 1 apres l'autre et de récupérer son ID unique une fois la nouvelle connecté. Cela permettra de ne pas mélanger les ID en cas d'un nombre important de sondes connectés.

#### Scanner les sondes

Dans Calaos-OS, l'outil `calaos_1wire` sert à scanner le bus 1wire pour lister les sondes et leur températures.

> Attention il est nécessaire d'arrêter temporairement le service `calaos-server` pour pouvoir scanner le bus. Les 2 ne peuvent pas fonctionner en meme temps et `calaos_1wire` affichera une erreur.

En console utilisez la commande suivante:

```bash
# on coupe calaos-server
systemctl stop calaos-server

calaos_1wire --scan -u

# on relance calaos-server
systemctl start calaos-server
```

> l'option `-u` demande à l'outil d'utiliser la passerelle USB, si vous utilisez une autre passerelle il faudra utiliser une autre option de connection. Se référer à la documentation de OWFS dans ce cas.

```shell
[INF] extern_process (OWExternProc_main.cpp:281) OW Library initialization ok
Scanning for 1wire devices... found 6 devices.
Device: 28.74DDB4040000 (DS18B20) --> value: 22.875
Device: 28.3C2DB4040000 (DS18B20) --> value: 21.8125
Device: 28.BE3BB4040000 (DS18B20) --> value: 23.625
Device: 28.F53416122101 (DS18B20) --> value: 21.8125
Device: 28.FF31563A0400 (DS18B20) --> value: 19.375
Device: 28.FFED9A3C0400 (DS18B20) --> value: 19.5625
```

Cette commande retournera les adresses uniques des sondes connectées, sous forme de numéro d’identification unique (ID) pour chaque sonde. Notez ces identifiants, car ils seront nécessaires pour l'ajout dans Calaos Installer.

> Exemple: `28.74DDB4040000` pour la 1ere sonde.

---

### Ajouter les sondes dans Calaos Installer

Pour ajouter une sonde de température dans Calaos Installer il suffit de donner l'ID de la sonde et l'option de connection à la passerelle 1Wire (ici `-u` dans l'exemple pour la passerelle **DS9490R**)

![1wire](/en/hardware/images/1w_01.png?width=10pc&classes=shadow)
