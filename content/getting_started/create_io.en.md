+++
title = 'Create an IO'
date = 2024-03-06T19:49:13Z
weight = 30
+++

## I/O in Calaos

In this step, we will introduce our first inputs/outputs (IO) in Calaos Installer. In Calaos, an IO represents various elements such as a lamp, a switch, a blind, etc. To do this, we use a hierarchy based on rooms to which we add I/O. You can start by creating a room in Calaos Installer. The type of room you choose will affect its icon in the interfaces.

## Creating a Room

To create a room, start by clicking on **Add** in the menu, then select **Room**. Next, enter the name of the room you want to create and choose the corresponding type from the available options.

![room menu](/en/getting_started/images/calaos_installer_room_menu.png?width=20pc&classes=shadow)
![new room](/en/getting_started/images/calaos_installer_new_room.png?width=20pc&classes=shadow)

## Creating a Light

To add a light, start by clicking on **Add** in the menu, then choose **Wago PLC** followed by **Light**. Next, give the light a name and specify the output address on the PLC. Once done, press **OK**. The light will be added to the selected room.

![new light](/en/getting_started/images/menu_light.png?width=20pc&classes=shadow)
![new light](/en/getting_started/images/wago_light.png?width=20pc&classes=shadow)

{{% notice note %}}
Keep in mind that this example is based on using a Wago PLC, but you can also apply the same procedure for other types of light fixtures if needed.
{{% /notice %}}

## Creating a Switch

To add a switch, navigate to the Add option in the menu, then select Wago PLC followed by Switch. Next, assign a name to your switch and specify the appropriate output address on the PLC. Once these steps are completed, confirm by clicking OK.

{{% notice note %}}
Just like adding a light, remember that this example is specific to a Wago PLC, but the process remains the same for other types of switches if needed.
{{% /notice %}}

![rule](/en/getting_started/images/io.png?width=20pc&classes=shadow)

Once you have added your inputs/outputs, you can proceed to the next step: [Creating Rules]({{%relref "getting_started/create_rule" %}}).
