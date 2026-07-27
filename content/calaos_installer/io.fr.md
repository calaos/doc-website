+++
title = 'Créer des IO'
date = 2024-03-06T20:12:23Z
weight = 20
summary = "Déclarer vos équipements : lumières, volets, interrupteurs, sondes."
+++

## Ce qu'est une IO

Une **IO** — pour entrée/sortie — représente un élément de votre maison : une lampe, un volet, un interrupteur, une sonde de température, une prise commandée.

Calaos distingue deux familles, et cette distinction structure tout le reste :

| Famille | Rôle | Exemples |
|---|---|---|
| **Entrée** | Calaos **lit** une information | interrupteur, sonde, détecteur, compteur |
| **Sortie** | Calaos **agit** sur quelque chose | lampe, volet, prise, chauffage |

Une lampe qui s'allume quand on appuie sur un interrupteur, ce sont donc **deux IO** — une entrée et une sortie — reliées par une règle. Elles ne sont pas liées d'office : c'est vous qui décidez du lien, ce qui permet ensuite à n'importe quel interrupteur de commander n'importe quelle lampe.

## Ajouter une IO

Sélectionnez la pièce concernée, puis ouvrez le menu **Ajouter**. Vous choisissez d'abord **la famille de matériel**, puis **le type d'équipement**.

![Menu lumière](/en/getting_started/images/menu_light.png?width=20pc&classes=shadow)

Par exemple, pour une lampe pilotée par un automate Wago : **Ajouter → Wago PLC → Lumière**. Il ne reste qu'à donner un nom et à indiquer l'adresse de la sortie sur l'automate.

![Nouvelle lumière](/en/getting_started/images/wago_light.png?width=20pc&classes=shadow)

L'IO apparaît alors dans la pièce sélectionnée.

![Liste des IO](/en/getting_started/images/io.png?width=20pc&classes=shadow)

## Les familles de matériel

Chaque famille correspond à une façon de communiquer avec vos équipements, et possède sa page dans le chapitre [Matériel supporté]({{% relref "hardware" %}}) :

| Famille | Documentation |
|---|---|
| Automate Wago | [Automate Wago]({{% relref "hardware/wago" %}}) |
| KNX | [KNX]({{% relref "hardware/knx" %}}) |
| MQTT | [MQTT]({{% relref "hardware/mqtt" %}}) |
| Zigbee | [Zigbee]({{% relref "hardware/zigbee" %}}) |
| 1-Wire | [1-Wire]({{% relref "hardware/1wire" %}}) |
| DMX / OLA | [DMX]({{% relref "hardware/dmx_ola" %}}) |
| Philips Hue | [Hue]({{% relref "hardware/hue2mqtt" %}}) |
| GPIO | [GPIO]({{% relref "hardware/gpio" %}}) |
| WebIO | [WebIO]({{% relref "hardware/webio" %}}) |
| Caméras | [Caméras]({{% relref "hardware/camera" %}}) |
| Écrans Remote UI | [Remote UI]({{% relref "hardware/remote_ui" %}}) |

Certaines IO ne dépendent d'aucun matériel : les [variables internes]({{% relref "calaos_installer/internal_var" %}}), les [scénarios]({{% relref "calaos_installer/scenario" %}}) et les objets de [programmation horaire]({{% relref "calaos_installer/time" %}}) se manipulent exactement comme les autres IO dans les règles.

## Les types d'équipement

Quelle que soit la famille, on retrouve les mêmes grands types :

**En entrée** — interrupteur simple, interrupteur à appui long, interrupteur triple, entrée analogique, sonde de température, valeur numérique, chaîne de caractères.

**En sortie** — lumière simple, lumière variable (dimmer), lumière RVB, volet, volet intelligent, sortie analogique, chaîne de caractères.

{{% notice tip %}}
**L'appui long est un type d'interrupteur à part entière.** Plutôt que de bricoler une règle avec un minuteur, choisissez le type adapté à la création : Calaos distingue alors l'appui court de l'appui long, et vous pouvez affecter une action différente à chacun.
{{% /notice %}}

Le choix du type détermine aussi **comment l'équipement s'affiche** dans les interfaces : un volet aura des boutons montée/descente, une lumière variable un curseur.

## Les propriétés d'une IO

Une fois l'IO créée, sa fenêtre de propriétés donne accès à des réglages communs à tous les types :

| Propriété | Effet |
|---|---|
| **Nom** | Le libellé affiché partout |
| **Visible** | Affiche ou masque l'IO dans les interfaces |
| **Historique** | Enregistre les changements d'état dans le journal d'événements |
| **Enregistrement des valeurs** | Envoie les mesures à la base d'historique, pour tracer des courbes |

Masquer une IO ne la désactive pas : elle continue de fonctionner dans les règles, elle n'encombre simplement pas les interfaces. C'est utile pour les entrées techniques dont personne n'a besoin au quotidien.

{{% notice note %}}
Chaque type d'IO possède en plus ses propres paramètres — adresse, port, identifiant d'équipement. **Calaos Installer affiche une aide intégrée pour chacun** : c'est la description la plus à jour, puisqu'elle vient directement du serveur.
{{% /notice %}}

## Nommer ses IO

Comme pour les pièces, ces noms seront lus tous les jours.

Le nom n'a pas besoin de rappeler la pièce : les interfaces affichent déjà l'IO **dans** sa pièce. « Cuisine — Lumière cuisine » est redondant ; « Plafonnier » suffit.

En revanche, distinguez clairement les équipements semblables d'une même pièce : « Plafonnier » et « Spots plan de travail » plutôt que « Lumière 1 » et « Lumière 2 ».

## Et ensuite

Vos équipements déclarés, il reste à les faire réagir : [Créer des règles]({{% relref "calaos_installer/rules" %}}).
