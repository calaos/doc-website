+++
title = 'Variation DALI'
date = 2024-03-06T20:03:07Z
weight = 50
+++

## Module d'interface DALI

Un des principaux atouts de la domotique se trouve être la gestion d'éclairage. Pour correctement la mettre en oeuvre, la possibilité de varier l'intensité des points lumineux est obligatoire. Une technologie de variation de lumière utilise le standard [DALI (Digital Addressable Lighting Interface)](http://fr.wikipedia.org/wiki/Protocole_Dali)

![module](/en/hardware/wago/images/borne_dali.jpg?width=20pc&classes=shadow)

Le regroupement des fournisseurs autour d'un standard a permis d'obtenir une interopérabilité dans le pilotage des ballasts électroniques. Ce nouveau standard remplace peu à peu l'ancienne interface de gradation 0-10V. La borne Wago 750-641 est une module maître DALI, qui est capable de piloter jusqu'à 64 ballasts esclaves. Les ballasts sont de différents types tels que:

- Transformateurs pour spots
- Transformateurs pour éclairage LED
- Transformateurs RVB tricolore pour des éclairages à LED
- Ballasts électronique pour tubes fluocompacts

Deux bornes DALI sont disponibles : la borne DALI de génération ancienne, référencée **750-641**, et la borne DALI Master, référencée **750-647**. Il est important de noter que ces bornes offrent des fonctionnalités différentes en termes de configuration dans Calaos.

La borne DALI de génération ancienne, modèle **750-641**, est conçue pour fonctionner uniquement en tant qu'esclave DALI. Elle est adaptée à des applications traditionnelles où une simple intégration avec d'autres maîtres n'est pas nécessaire.

En revanche, la borne DALI Master, modèle **750-647**, offre des fonctionnalités avancées en tant que maître DALI. Elle prend en charge non seulement le contrôle de plusieurs périphériques DALI en tant que maître, mais elle est également capable de fonctionner en tant que maître multimaster, permettant le support de détecteur de mouvement ou de luminosité DALI (Comme le Tridonic M-Sensor2 par exemple).

## Installation de la borne dans l'automate

La borne DALI doit imperativement etre installé directement apres les modules I/O digitales. Il est imperatif d'installer n'importe quelle autre borne analogique **apres la borne DALI**. Le programme calaos_wago ne supporte qu'une seule et unique borne DALI. On ne peut donc pas mettre plusieurs borne DALI (maitre ou non).

## Câblage

![module](/en/hardware/wago/images/wago-288-895.jpg?width=5pc&classes=shadow)

Le câblage des éléments DALI diffère totalement des points lumineux classique. En effet il faut cette fois mettre en place un bus de communication 2 fils. De plus ce bus nécessite une alimentation 18V dédié spécifique.

Comme spécifié plus haut, le DALI est une interface permettant de piloter des ballasts et transformateurs électroniques d'éclairage. Il faudra donc équiper les éclairages voulus avec des transformateurs compatible DALI.

![module](/en/hardware/wago/images/schema_dali.png?width=20pc&classes=shadow)

Chaque ballasts pourra être branché sur le bus. Le bus DALI ne demande pas de polarité particulière de branchement, il faudra cependant faire attention de ne pas faire de bouclage ou encore de court circuit. Un maximum de 64 esclaves peuvent être branchés sur la borne. Attention, certains transformateurs RVB utilisent 3 adresses (une pour chaque couleur).

Il est préconisé d'utiliser du câble 1.5mm² minimum pour le bus 2 fils DALI. Une section plus petite pourrait entraîner du parasitage dans le signal.

## Programmation

### Adresses

Une fois les ballasts connectés au bus, il va falloir les identifier.

#### 750-641

Il va falloir utiliser le logiciel [Calaos Installer]({{%relref "calaos_installer/wago" %}}) et faire une recherche d'adresses. Le principe du bus DALI est simple, chaque élément reçoit une adresse comprise entre 1 et 64. La centrale et l'automate ont besoin de savoir l'adresse à laquelle l’équipement pourra être commander.

#### 750-647

L'adressage des equipements DALI devra se faire avec le logiciel **Wago Dali Configurator**. Ce logiciel ne fonctionne qu'avec la borne 753-647. Sur le bus DALI avec la borne maitre, 64 addresses de ballasts sont supportées. A cela s'ajoute 14 addresses pour les detecteurs. Attention cependant, un detecteur de type Tridonic M-Sensor2 a besoin de 3 addresses. Il faut faire attention à ne pas depasser ces limites (qui sont liés au bus DALI).

##### Wago Dali Configurator

- Débrancher le serveur Calaos
- Positionner le Wago en mode STOP via l'interrupteur (position centrale)
- Lancer Wago Dali Configurator
- Lancer une recherche des équipements non adressés
- Pour trouver la couleur correspondante à l'adresse, on peut allumer la sélection
- Configurer les ballasts et écrire la configuration (Dans l'exemple, on retrouve un ballast RGBW (⇒ 4 adresses))

{{% notice note %}}
Il est conseillé de changer l'adresse des ballasts par une adresse supérieure à 3. En effet, si un ballast est ajouté par la suite, il peut rentrer en conflit avec les ballasts existants.
{{% /notice %}}

![module](/en/hardware/wago/images/daliconfiguratorparametres.png?width=20pc&classes=shadow)

{{% notice note %}}
La position de la borne ne correspond pas à la position de la carte sur le bus mais au nombre de carte DALI (1)
{{% /notice %}}

![module](/en/hardware/wago/images/daliconfiguratoradressage.png?width=20pc&classes=shadow)

![module](/en/hardware/wago/images/daliconfiguratorpoweron.png?width=20pc&classes=shadow)

![module](/en/hardware/wago/images/daliconfiguratorwriteparam.png?width=20pc&classes=shadow)

##### Configuration dans Calaos Installer

Une fois dans Calaos Installer, il ne suffit pas d'ajouter des equipements Wago Dali pour que ca fonctionne avec la borne 750-647. Il faut également déclarer tous les équipement du bus, et envoyer cette configuration dans l'automate.

Pour ca, il faut se connecter a l'automate avec calaos installer. Puis se rendre sur la page **Automate Wago** → **Upload Dali Master 647 Configuration**

![pic](/en/hardware/wago/images/calaos_installer_dali_master.png?width=20pc&classes=shadow)

Dans cet outil dédié a la borne Dali Master, il faut lister tous les équipements Dali sur le bus ainsi que leur adresses respectivent. Le bouton **Load from PLC** va permettre de recharger une configuration stocké dans l'automate. Le bouton **Send to PLC** fera l'inverse et va envoyer la configuration dans l'automate. Un redemarrage de l'automate sera effectué automatiquement.

Une fois cette configuration terminé, on pourra utiliser calaos installer d'une maniere traditionnelle en ajoutant des objets Lumiere Dali.

##### Adresses

##### Ballasts

Les adresses des ballasts DALI commencent a 1 dans calaos. Ce n'est pas le cas dans Wago Dali Configurator. Il faut donc faire attention.

##### Détecteurs

Pour utiliser des detecteurs de presence ou des interrupteurs Dali, il faut utiliser des Interrupteurs Wago classique dans calaos installer. Leurs addresses commencent à **6656**.

Ex:

| Adresse DALI | Adresse Wago Dali Configurator | Adresse Calaos installer inter |
|----------|------|------|
| 1 | 0 | 6656 |
| 2 | 1 | 6657 |
| 3 | 2 | 6658 |
| 4 | 3 | 6659 |
| ... | ... | ... |

Lors de la configuration des detecteurs dans calaos installer il est possible de specifier un _Off Timeout_. Cela correspond au temps que va mettre le detecteur avant de rebasculer a l'etat bas.

##### Luxmetre

Il est possible de recuperer la valeur des luxmetres dans calaos. Pour cela, il faut creer un IO _Wago Input Analog_ dans calaos installer et lui donner l'addresse DALI + 255.

Ex:

| Adresse DALI | Adresse Wago Dali Configurator | Adresse Calaos installer inter |
|----------|------|------|
| 1 | 0 | 256 |
| 2 | 1 | 257 |
| 3 | 2 | 258 |
| 4 | 3 | 259 |
| ... | ... | ... |

#### Exemple avec 750-647

Avec le detecteur suivant:

{{% notice note %}}
Multi Sensor Type 2: Presence address: 1 Light address: 2 IR address: 3 Offtimeout: 10000ms
{{% /notice %}}

On pourra declarer dans calaos installer les IO suivants:

- Wago Switch address: 6656
- Wago Switch address: 6658
- Wago Analog Input address: 257

Pour le detecteur on pourra creer la regle suivante:

    IF DetecteurPresence == True
    IF DetecteurLux <= 50
    Then LumiereDali = impulse 600000

Cette regle aura pour effet d'allumer la lumiere pendant 10min si le detecteur detecte une presence et que le luxmetre est en dessous de 50 Lux.

### Programmation

La programmation s'effectue également avec le logiciel [Calaos Installer]({{%relref "calaos_installer/wago" %}}). On pourra cependant jouer avec l'intensité des éclairages contrairement aux éléments de lumière classique.
