+++
title = 'Créer une IO'
date = 2024-03-06T19:48:49Z
weight = 30
+++

## Les IO dans Calaos

Dans cette étape, nous allons introduire nos premières entrées/sorties (I/O) dans Calaos Installer. Dans Calaos, une I/O représente divers éléments tels qu'une lampe, un interrupteur, un volet, etc. Pour ce faire, nous utilisons une hiérarchie basée sur des pièces auxquelles nous ajoutons des I/O. Vous pouvez commencer en créant une pièce dans Calaos Installer. Le type de pièce que vous choisissez affectera son icône dans les interfaces.

## Créer une pièce

Pour créer une pièce, commencez par cliquer sur **Ajouter** dans le menu, puis sélectionnez **Pièce**. Ensuite, saisissez le nom de la pièce que vous souhaitez créer et choisissez le type correspondant parmi les options disponibles.

![room menu](/en/getting_started/images/calaos_installer_room_menu.png?width=20pc&classes=shadow)
![new room](/en/getting_started/images/calaos_installer_new_room.png?width=20pc&classes=shadow)

## Créer une lumière

Pour ajouter une lumière, commencez par cliquer sur **Ajouter** dans le menu, puis choisissez **Wago PLC** suivi de **Lumière**. Ensuite, donnez un nom à la lumière et spécifiez l'adresse de sortie sur l'automate. Une fois terminé, appuyez sur **OK**. La lumière sera ajoutée à la pièce sélectionnée.

![new light](/en/getting_started/images/menu_light.png?width=20pc&classes=shadow)
![new light](/en/getting_started/images/wago_light.png?width=20pc&classes=shadow)

{{% notice note %}}
Gardez à l'esprit que cet exemple est basé sur l'utilisation d'un automate Wago, mais vous pouvez également appliquer la même procédure pour d'autres types de luminaires si nécessaire.
{{% /notice %}}

## Créer un interrupteur

Pour ajouter un interrupteur, dirigez-vous vers l'option **Ajouter** dans le menu, puis sélectionnez **Wago PLC** suivi de **Interrupteur**. Ensuite, attribuez un nom à votre interrupteur et indiquez l'adresse de sortie appropriée sur l'automate. Une fois ces étapes terminées, validez en cliquant sur **OK**.

{{% notice note %}}
Tout comme pour l'ajout d'une lumière, rappelez-vous que cet exemple est spécifique à un automate Wago, mais le processus reste le même pour d'autres types d'interrupteurs si nécessaire.
{{% /notice %}}

![rule](/en/getting_started/images/io.png?width=20pc&classes=shadow)

Une fois que vous avez ajouté vos entrées/sorties, vous pouvez passer à l'étape suivante: [la création de règles]({{%relref "getting_started/create_rule" %}}).
