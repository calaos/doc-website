+++
title = 'Module Analogique'
date = 2024-03-28T14:30:46Z
+++

## Module d'entrée ou de sortie analogique

Parmi les données que l'on peut vouloir mesurer dans une maison, il y a un bon nombre de capteur qui ne se connecte pas en tout ou rien. Ces capteurs se branchent donc de différentes manière et renvoient une valeur analogique. On peut trouver par exemple des capteurs d'hygrométrie, d'ensoleillement, de luminosité, etc... De la même manière certain équipement peuvent se piloter de manière analogique en 0-10V tels que des registres de ventilation.

Le système Wago permet de connecter ce genre de capteurs grâce aux bornes analogiques. Il en existe différents types, suivant le type de données renvoyé par le capteur, ou du type d'équipement à piloter. Nous avons donc des bornes 0-10V, 4-20mA, -10V/+10V, ... le mieux est d'aller consulter le site de Wago pour trouver la borne adéquate.

![module](/en/hardware/wago/images/borne_analog_0-10v.jpg?width=10pc&classes=shadow)
![module](/en/hardware/wago/images/borne_outanalog_0-10v.jpg?width=10pc&classes=shadow)

## Câblage

Le câblage dépends du type d'équipements et de borne. Mais globalement il reste assez simple et est similaire au câblage des sondes de température. Il est conseillé de consulter la documentation Wago et de l'équipement à piloter.

## Programmation

La configuration d'une sortie s'effectue à l'aide du logiciel [Calaos Installer]({{%relref "calaos_installer/wago" %}}). Il faudra utiliser une entrée ou une sortie analogique Wago dans Calaos Installer, puis de spécifier le numéro de variable à piloter.
