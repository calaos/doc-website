+++
title = 'Module de sorties'
date = 2024-03-06T20:03:00Z
weight = 40
+++

## Modules de sorties 4, 8, 16 canaux

Pour contrôler les éléments électriques tels que les luminaires, les volets/ stores électriques, les prises commandées, etc., nous utilisons un module de sorties digitales 24V DC. Cette borne de sortie tout ou rien permet de connecter jusqu'à 8 actionneurs. Habituellement, ces actionneurs sont des relais de puissance 24V/220V 16A, capables de supporter la charge des luminaires et des prises commandées dans une maison.

La borne permet de connecter 8 contacts secs et est galvaniquement isolée.

![module](/en/hardware/wago/images/borne_sortie8.jpg?width=10pc&classes=shadow)
![module](/en/hardware/wago/images/borne_sortie16.jpg?width=10pc&classes=shadow)

## Câblage

Le câblage des sorties sur relais s'effectue en basse tension comme sur le schéma ci-dessous.

![module](/en/hardware/wago/images/schema_relais.png?width=20pc&classes=shadow)

### Points lumineux, prise commandée

Le schéma présente la connexion de 2 points lumineux. Le principe est exactement le même pour une prise commandée. Les relais de puissance ayant un pouvoir de coupure de 16A, ils suffisent à être utilisé dans ces cas.

### Volet

Le cas du volet est également le même. En effet, un volet __doit être filaire avec commande montée et descente__. Il se connecte alors comme 2 points lumineux, en utilisant 2 sorties relais. La programmation utilisera alors une des 2 sorties comme montée et l'autre comme descente.

Deux modes de fonctionnements des volets sont compatibles. Le mode classique, où il faut maintenir les relais pour effectuer une montée/descente, ainsi que le modde a commandes impulsionnelles, où une impulsion suffit à donner l'ordre au volet. La programmation permettra de choisir le mode adéquat et la centrale fera le travail nécessaire.

### Chauffage

Le cas du chauffage est similaire dans certain cas ou on peut piloter une zone de chauffage grâce à une sortie tout ou rien. Habituellement, on peut facilement piloter une zone de chauffage en branchant une électrovanne sur le circuit voulu, puis connecter cette électrovanne sur une sortie relais.

## Programmation

La configuration d'une sortie s'effectue à l'aide du logiciel [Calaos Installer]({{%relref "calaos_installer/wago" %}}). Vous devrez connaître le numéro de la sortie de l'automate. Par exemple, sur le schéma ci-dessus, la lampe 1 est connectée à la sortie 0 de l'automate et la lampe 2 à la sortie 1 de l'automate. Il est donc nécessaire d'attribuer une identification pour connaître le numéro de chaque lumière, volet, etc.

Il est important de noter que la numérotation des bornes de sortie commence à partir de 0 pour la première borne, puis continue avec les bornes de sortie suivantes. Ainsi, la première borne est numérotée 0, la suivante 8, etc...
