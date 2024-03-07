+++
title = 'Installation'
date = 2024-02-03T16:26:37Z
weight = 20
+++

## Calaos Image Creator

Calaos Installer provides a tool to download and create an image on a bootable USB key. From Calaos Installer, in the **Project Management** menu, select **Create an Image**.

![Create Image menu](/en/getting_started/images/calaos_installer_creator_menu.png?width=20pc&classes=shadow)
![Create Image](/en/getting_started/images/calaos_installer_creator.png?width=20pc&classes=shadow)

Choose the latest stable version of Calaos OS from the dropdown list, then click **Continue**. The image is then downloaded from the Calaos server. This operation may take a few minutes and depends on the speed of your internet connection. Once the download is complete, insert a USB key into your computer. It will be formatted, and the image files will be copied onto it. The USB key should appear in the application window. Select the USB key and click **Continue**.

## First Boot

Once the image is copied to the USB key, you can insert it into the PC that will serve as the Calaos server. Restart the machine and boot from the USB key.

When Calaos OS starts, you have two options:

- Use Calaos OS in Live mode to test the system without installing it.
- Directly install Calaos OS on the internal disk of the machine.

{{% notice info %}}
The **Live** mode allows you to configure the system, test the features, and check if everything is working correctly. If you are satisfied, you can then install Calaos OS on the internal disk of the server. You will keep all the configurations you made in Live mode. See more details on the [dedicated page].
{{% /notice %}}

When the **Live** mode has started, the graphical interface of Calaos OS appears on the screen. You can then configure your system, add I/Os, rules, and scenarios. For now, the interface is empty, but we will see how to configure it in the next step.

[Next Chapter]({{% relref "getting_started/create_io" %}})