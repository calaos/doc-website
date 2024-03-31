+++
title = 'Prérequis'
date = 2024-03-31T16:29:23Z
weight = 5
summary = 'Prérequis'
+++

## Installation

Bienvenue sur la rubrique dédiée à l'aspect technique de la domotique Calaos. Vous y trouverez tout ce dont un auto-installateur doit savoir pour réaliser une installation Calaos dans son intégralité. Vous retrouverez ci-contre un schéma global d'installation reprenant tous les composants d'une installation domotique Calaos ainsi qu'une liste des éléments les plus importants.

A travers les différents liens vous aurez accès aux caractéristiques de chaque produit, aux explications de câblages ainsi qu'à chaque étape de la programmation. Il ne vous reste plus qu'à lire attentivement chaque point pour ne sauter aucune étape de l'installation.

Bon courage!

## Base

![schema](/en/hardware/images/schema.png?width=40pc)

### Prérequis

Pour une installation domotique dans une maison neuve ou dans le cadre de rénovations importantes, nous recommandons fortement l'approche Calaos avec une base d'automate Wago pour sa durabilité et son efficacité économique. Voici quelques conseils clés pour démarrer sur de bonnes bases :

- Topologie de câblage spécifique : L'architecture de câblage pour Calaos avec un automate Wago diffère des installations électriques classiques. Il est essentiel de prévoir une topologie adaptée qui facilitera l'intégration de la domotique et la rendra plus efficace.

- Compatibilité des matériaux : Tous les composants électriques ne se valent pas pour une utilisation avec Calaos. Il est crucial de sélectionner des matériaux compatibles pour garantir le bon fonctionnement de votre système domotique.

Bien que Calaos soit compatible avec des technologies populaires comme Zigbee, pour une installation pérenne et fiable, privilégier un câblage et des équipements dédiés dès le départ est la meilleure approche. Cette méthode minimise les risques de dysfonctionnement et assure une meilleure longévité de votre système domotique.

Adopter Calaos dès la conception de votre maison ou au début d'une grande rénovation vous permettra de bénéficier d'un système stable, durable et économiquement avantageux sur le long terme.

#### Câblage du circuit de commande

Le circuit de commande est séparé et provient directement du tableau de communication ou d'une boîte de dérivation. Le câblage est alors effectué par bouclage arborescent (quatre ou cinq interrupteurs à la suite selon le type de câble) avec du câble de type téléphone quatre paires (section 0,6 mm²) ou plus. 

{{% notice info %}}
Toutes les commandes sont des boutons poussoirs simples
{{% /notice %}}

Pour exemple, voici un schéma de câblage des boutons poussoirs. Un câble 4 paires permet de câbler jusqu'à 7 boutons poussoirs étant donné qu'il faut un fil pour faire le +24v commun. Cependant, il est préférable dans certains cas de conserver des fils de réserve pour pouvoir rajouter des interrupteurs par la suite.

![schema](/en/hardware/images/bp.png?width=20pc)

#### Câblage du circuit de puissance

Le circuit de puissance provient directement du tableau de répartition ou d'une boîte de dérivation. Chaque circuit est séparé pour pouvoir être piloté séparément. Le câble à utiliser est de section traditionnelle ( 3×1,5mm² pour les lumières et 4×1,5mm² pour des volets roulants). 

{{% notice info %}}
Pour un soucis de compatibilité avec le système Calaos, tous les volets roulants doivent être filaires et commandés en 230v avec pour simple action montée/descente.
{{% /notice %}}

Pour exemple, voici un schéma de câblage des lumières.

![schema](/en/hardware/images/lum.jpg?width=20pc)

Et un autre exemple avec cette fois ci un schéma de câblage des volets.

![schema](/en/hardware/images/vol.jpg?width=20pc)

### Tableau électrique

Lors de la mise en place d'un système domotique avec Calaos, notamment en utilisant des automates Wago et des relais, il est important de tenir compte de l'espace nécessaire pour l'installation. Ces équipements, en raison de leur taille et de leur fonctionnalité, peuvent occuper un espace conséquent dans votre tableau électrique.

Nous recommandons donc de prévoir une enveloppe de distribution suffisamment spacieuse pour accueillir à la fois la partie électrique traditionnelle de votre maison et les composants supplémentaires liés à la domotique. La qualité et les dimensions du tableau électrique sont cruciales pour assurer une installation organisée et sécurisée.

Il est conseillé de choisir un tableau électrique qui offre un espace ample pour l'ensemble du système, en prenant en compte non seulement les dimensions actuelles de votre installation mais aussi la possibilité d'extensions futures. Assurez-vous que le tableau choisi peut supporter l'encombrement des automates Wago et des relais, tout en laissant suffisamment de place pour une maintenance aisée et l'ajout de nouveaux composants si nécessaire.

Voici un example de tableau, à gauche la partie disjoncteur classique et à droite la partie basse tension avec les relais, l'automate et même le réseau informatique:

![schema](/en/hardware/images/tableau.jpg?width=60pc)