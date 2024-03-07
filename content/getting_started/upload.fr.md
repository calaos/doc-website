+++
title = 'Envoyer la config'
date = 2024-03-06T19:49:00Z
weight = 50
+++

## Envoyer la configuration sur le serveur

Il est essentiel de comprendre qu'une fois que vous avez terminé de configurer vos dispositifs et vos règles dans Calaos, vous devez envoyer cette configuration vers le serveur pour qu'elle soit prise en compte. Cette configuration est composée de fichiers XML, notamment `io.xml` pour la définition des entrées/sorties (IO) et des pièces, ainsi que `rules.xml` pour la définition des règles. Calaos Installer est équipé d'une fonctionnalité qui vous permet d'envoyer ces fichiers de configuration automatiquement vers le serveur, simplifiant ainsi le processus de mise en place de votre système domotique.

## Login

Pour envoyer votre projet vers le serveur Calaos, vous devez accéder depuis le bandeau principal à l'option **Calaos Server**, puis sélectionner **Envoyer le projet à Calaos Server**. Ensuite, vous devrez fournir l'adresse IP de la machine où est installé le serveur Calaos, ainsi que les informations de connexion requises, comprenant le nom d'utilisateur et le mot de passe.

Lorsque vous envoyez la configuration vers le serveur Calaos, cette action entraînera automatiquement le redémarrage des services pour prendre en compte les modifications apportées.

![upload](/en/getting_started/images/upload.png?width=20pc&classes=shadow)

{{% notice info %}}
Par défaut, les identifiants de connexion sont définis comme suit : nom d'utilisateur: `user` et mot de passe `pass`.
{{% /notice %}}

{{% notice note %}}
En sélectionnant l'option **Charger le projet depuis Calaos Server**, vous pouvez ouvrir l'intégralité du projet à partir d'un serveur Calaos. Cette action permet le téléchargement des fichiers XML nécessaires depuis le serveur, qui seront ensuite ouverts automatiquement dans Calaos Installer.
{{% /notice %}}

[Chapitre suivant]({{% relref "getting_started/test" %}})
