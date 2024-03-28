+++
title = 'Automate Wago'
date = 2024-03-06T20:02:31Z
weight = 10
summary = 'Automate Wago de type 750-841, 750-880, ...'
+++

## Automate Wago

L'automate Wago constitue le socle d'une installation Calaos. Doté de composants industriels pour assurer qualité et durabilité, cet automate est chargé de piloter tous les éléments électriques de la maison, ainsi que d'acquérir l'état des interrupteurs, des sondes de température, des capteurs, et bien d'autres. Il joue ainsi un rôle crucial en faisant le lien entre le monde informatique de la centrale et le monde électrique.

### Sécurité

L'automate intègre également un programme de secours en cas de défaillance du système informatique principal. En cas de rupture de communication avec Calaos, il prend le relais en mode autonome. Cela garantit une fiabilité accrue du système, permettant à l'utilisateur d'interagir avec sa maison comme avec une installation électrique traditionnelle. Ainsi, les préoccupations liées aux pannes informatiques sont écartées.

### Gamme d'automates

L'avantage de l'automate Wago réside dans sa modularité. Il est donc évolutif et adaptable à tous les types d'installations. Par exemple, il est toujours possible d'ajouter ultérieurement un module pour contrôler une pièce supplémentaire. Le module principal intègre l'intelligence du système ainsi qu'un port de communication réseau pour interagir avec le système informatique et recevoir les commandes du serveur Calaos. Selon la version, ce module peut éventuellement inclure un port Ethernet supplémentaire pour le protocole KNX ou DMX. L'automate permet de connecter sur son bus interne des bornes d'entrées/sorties analogiques et digitales, ainsi que des bornes spécifiques à d'autres bus de communication (DALI, KNX, etc.).

Nous utilisons principalement ces têtes d'automate:

- La tête 750-841 (Remplacé par 750-881, 750-880, 750-891, 750-893)
- La tête 750-849 pour les installations nécessitants une compatibilité KNX. (Remplacé par 750-889)

Pour plus d'informations sur une installation Calaos compatible KNX, vous pouvez aller voir cette page: [Installation KNX]({{%relref "hardware/wago/knx" %}})

## Sécurité électrique

Du point de vue de la sécurité, l'automate Wago respecte la norme de protection IP20 en intègrant une séparation galvanique de l'électronique interne et du champs. Cette protection contre les courts circuits est assurée notamment par des optocoupleurs.

## Modules et Extensions

Pour recueillir les informations des interrupteurs, nous utilisons des bornes d'entrées digitales. Ces bornes opèrent avec des courants faibles de 24V DC et fonctionnent sur le principe du tout ou rien [TOR](http://fr.wikipedia.org/wiki/Tout_ou_rien). Les modules de sortie, quant à eux, permettent de contrôler, par exemple, l'éclairage ou les volets motorisés.

Voici la liste des différents modules et extensions pouvant être ajoutés à un automate Wago.

### Interrupteurs, capteurs TOR

- [Module d'entrées 4,8,16 canaux 24V]({{%relref "hardware/wago/input" %}})

### Points lumineux, volets, stores, sortie tout ou rien (TOR)

- [Module de sorties 4,8,16 canaux 24V]({{%relref "hardware/wago/output" %}})

### Variation de lumière

- [Module d'interface DALI]({{%relref "hardware/wago/dali" %}})
- [Module DMX]({{%relref "hardware/wago/dmx" %}})

### Chauffage, divers, ...

- [Module d'entrée de sonde de température PT100/PT1000]({{%relref "hardware/wago/pt1000" %}})
- [Module d'entrée/sortie analogique]({{%relref "hardware/wago/analog" %}})

### Prolongateur de bus

- [Prolonger un bus Wago]({{%relref "hardware/wago/750-627" %}})
