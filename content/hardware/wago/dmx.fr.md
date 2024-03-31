+++
title = 'Variation DMX'
date = 2024-03-06T20:03:15Z
weight = 60
+++

## Module d'interface DMX

### DMX 512

Parmi les méthodes de contrôle d'éclairage on retrouve plusieurs normes bien définit et utilisé depuis longtemps. C'est le cas du [DMX512](http://fr.wikipedia.org/wiki/DMX_(%C3%A9clairage)) plus communément appelé DMX (Digital Multiplexing).

Il est principalement utilisé dans le monde de la scène (concerts, plateaux télé, spectacle son & lumière) pour le contrôle de l'éclairage dynamique. Ce procédé a quelques avantages pour l'utiliser dans le milieu de la domotique résidentiel. Le DMX512 est à ce jour le protocole le plus répandu et le plus universel, (utilisés partout et par tous les fabricants de matériel d'éclairage scénique).

On peut donc trouver des blocs de puissance gradateur capable de varier plusieurs équipements à des prix très abordable. Ces blocs peuvent aussi supporter des puissances supérieures à ce qu'on pourrait faire en DALI.

## Mise en oeuvre

Afin de faciliter la mise en oeuvre avec un système Calaos, nous avons choisi d'utiliser une passerelle simple DMX ↔ Ethernet. Cette passerelle s'occupera de convertir les ordres en provenance du réseau Ethernet et de les transmettre sur le lien DMX.

La passerelle utilisé est disponible chez [DMX4ALL](https://shop.dmx4all.de/de/dmx4all-produkte/dmx-interfaces/usb-dmx-interfaces/) sous la référence LAN-DMX-STAGE-PROFI.

![module](/en/hardware/wago/images/lan_dmx_stage_profi.jpg?width=10pc&classes=shadow)

## Câblage

Cette passerelle devra être connecté sur le réseau automate, soit en passant par un switch, soit en passant par le double port réseau de la tête 750-849. Les blocs DMX seront connectés au bus DMX. Il est préférable de placer sur le dernier appareil DMX une résistance de fin de ligne d'une valeur de 120Ω pour terminer le bus.

![module](/en/hardware/wago/images/schema_dmx.png?width=20pc&classes=shadow)

## Programmation

### Adresses

Etant donné que la passerelle DMX communique avec l'automate sur le réseau Ethernet, elle devra donc avoir l'adresse IP suivante par défaut: **10.0.0.120**. Il est possible de changer cette IP sur l'automate en utilisant Calaos Installer.

![module](/en/hardware/wago/images/calaos_wago_dmx.jpg?width=10pc&classes=shadow)

Pour plus de simplicité dans la gestion du système Calaos, un équipement DMX est vu comme un équipement DALI. Cela va permettre à l'automate de gérer de la même façon un éclairage DALI et DMX. La programmation devient totalement transparente. Il faudra cependant noter que l'adresse DMX 1 équivaut à l'adresse DALI 101, DMX 2 vaudra DALI 102 et ainsi de suite, …

La programmation s'effectue également avec le logiciel [Calaos Installer]({{%relref "calaos_installer/wago" %}}). On pourra jouer avec l'intensité des éclairages exactement comme les éléments de lumière DALI.

### Programmation

La programmation s'effectue également avec le logiciel [Calaos Installer]({{%relref "calaos_installer/wago" %}}). On pourra cependant jouer avec l'intensité des éclairages contrairement aux éléments de lumière classique.
