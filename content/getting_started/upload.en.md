+++
title = 'Upload config'
date = 2024-03-06T19:49:22Z
weight = 50
+++

## Sending Configuration to the Server

It is essential to understand that once you have finished configuring your devices and rules in Calaos, you need to send this configuration to the server for it to take effect. This configuration consists of XML files, including `io.xml` for defining inputs/outputs (IO) and rooms, as well as `rules.xml` for defining rules. Calaos Installer is equipped with a feature that allows you to automatically send these configuration files to the server, simplifying the process of setting up your home automation system.

## Login

To send your project to the Calaos server, you need to access the **Calaos Server** option from the main menu, then select **Send Project to Calaos Server**. Next, you will need to provide the IP address of the machine where the Calaos server is installed, as well as the required login information, including the username and password.

When you send the configuration to the Calaos server, this action will automatically restart the services to apply the changes.

![upload](/en/getting_started/images/upload.png?width=20pc&classes=shadow)

{{% notice info %}}
By default, the login credentials are set as follows: username: `user` and password `pass`.
{{% /notice %}}

{{% notice note %}}
By selecting the **Load Project from Calaos Server** option, you can open the entire project from a Calaos server. This action enables the download of the necessary XML files from the server, which will then be automatically opened in Calaos Installer.
{{% /notice %}}

[Next Chapter]({{% relref "getting_started/test" %}})
