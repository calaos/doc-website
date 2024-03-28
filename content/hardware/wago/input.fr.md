+++
title = "Module d'entrées"
date = 2024-03-06T20:02:50Z
weight = 30
+++

## Modules d'entrée 4, 8, 16 canaux

Associés au contrôleur de bus de terrain, les modules d'entrées digitales à 4, 8, 16 canaux 24V DC permettent de connecter des boutons-poussoirs ainsi que tout signal d'entrée tout ou rien.

## Modules Wago

La borne permet de connecter 4, 8 ou 16 contacts secs et est galvaniquement isolée.

- 750-402: 4 entrées 24V
- 750-430: 8 entrées 24V
- 750-1405: 16 entrées 24V

![module](/en/hardware/wago/images/borne_entree8.jpg?width=20pc&classes=shadow)


Le câblage des interrupteurs s'effectue en basse tension 24V et en injectant du +24V dans la borne d'entrée. Pour ce faire, on peut donc câbler comme présenté sur la figure ci-dessous, en utilisant l’alimentation 24V de l'automate.

![module](/en/hardware/wago/images/schema_inter.png?width=20pc&classes=shadow)

## Programmation

La configuration d'un interrupteur s'effectue à l'aide du logiciel [Calaos Installer]({{%relref "calaos_installer/wago" %}}). Vous devrez éventuellement connaître le numéro de l'entrée de l'automate de l'interrupteur. Par exemple, sur le schéma ci-dessus, l'interrupteur 1 est connecté à l'entrée 0 de l'automate et l'interrupteur 2 à l'entrée 1 de l'automate.

Il est important de noter que la numérotation des bornes d'entrée commence à partir de 0 pour la première borne, puis continue avec les bornes d'entrée suivantes. Ainsi, la première borne est numérotée 0, la suivante 8 (pour des bornes de 8 canaux), etc...
