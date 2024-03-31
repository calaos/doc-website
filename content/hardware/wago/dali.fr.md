+++
title = 'Variation DALI'
date = 2024-03-06T20:03:07Z
weight = 50
+++

## Module d'interface DALI

Un des principaux atouts de la domotique se trouve être la gestion d'éclairage. Pour correctement la mettre en oeuvre, la possibilité de varier l'intensité des points lumineux est obligatoire. Une technologie émergente de variation de lumière utilise le standard [DALI (Digital Addressable Lighting Interface)](http://fr.wikipedia.org/wiki/Protocole_Dali)

![module](/en/hardware/wago/images/borne_dali.jpg?width=20pc&classes=shadow)

Le regroupement des fournisseurs autour d'un standard a permis d'obtenir une interopérabilité dans le pilotage des ballasts électroniques. Ce nouveau standard remplace peu à peu l'ancienne interface de gradation 0-10V. La borne Wago 750-641 est une module maître DALI, qui est capable de piloter jusqu'à 64 ballasts esclaves. Les ballasts sont de différents types tels que:

- Transformateurs pour spots
- Transformateurs pour éclairage LED
- Transformateurs RVB tricolore pour des éclairages à LED
- Ballasts électronique pour tubes fluocompacts

## Câblage

![module](/en/hardware/wago/images/wago-288-895.jpg?width=5pc&classes=shadow)

Le câblage des éléments DALI diffère totalement des points lumineux classique. En effet il faut cette fois mettre en place un bus de communication 2 fils. De plus ce bus nécessite une alimentation 18V dédié spécifique.

Comme spécifié plus haut, le DALI est une interface permettant de piloter des ballasts et transformateurs électroniques d'éclairage. Il faudra donc équiper les éclairages voulus avec des transformateurs compatible DALI.

![module](/en/hardware/wago/images/schema_dali.png?width=20pc&classes=shadow)

Chaque ballasts pourra être branché sur le bus. Le bus DALI ne demande pas de polarité particulière de branchement, il faudra cependant faire attention de ne pas faire de bouclage ou encore de court circuit. Un maximum de 64 esclaves peuvent être branchés sur la borne. Attention, certains transformateurs RVB utilisent 3 adresses (une pour chaque couleur).

Il est préconisé d'utiliser du câble 1.5mm² minimum pour le bus 2 fils DALI. Une section plus petite pourrait entraîner du parasitage dans le signal.

## Programmation

### Adresses

Une fois les ballasts connectés au bus, il va falloir les identifier. Pour cela il va falloir utiliser le logiciel [Calaos Installer]({{%relref "calaos_installer/wago" %}}) et faire une recherche d'adresses. Le principe du bus DALI est simple, chaque élément reçoit une adresse comprise entre 1 et 64. La centrale et l'automate ont besoin de savoir l'adresse à laquelle l’équipement pourra être commander.

### Programmation

La programmation s'effectue également avec le logiciel [Calaos Installer]({{%relref "calaos_installer/wago" %}}). On pourra cependant jouer avec l'intensité des éclairages contrairement aux éléments de lumière classique.
