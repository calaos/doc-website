+++
title = 'Installation'
date = 2024-02-03T16:26:42Z
weight = 20
+++

## Calaos Image Creator

Calaos Installer fournit un outil pour télécharger et créer une image sur une clé USB amorçable. A partir de Calaos Installer, dans le menu **Gestion du projet** sélectionnez **Créez une image**.

![Create Image menu](/en/getting_started/images/calaos_installer_creator_menu.png?width=20pc&classes=shadow)
![Create Image](/en/getting_started/images/calaos_installer_creator.png?width=20pc&classes=shadow)

Choisir la dernier version stable de Calaos OS dans la liste déroulante, puis cliquer sur **Continuer**. L'image est alors téléchargée depuis le serveur de Calaos. Cette opération peut prendre quelques minutes et depends de la vitesse de votre connexion internet. Une fois le téléchargement terminé, insérer une clef USB dans votre ordinateur. Celle-ci sera formatée et les fichiers de l'image seront copiés dessus. La clé USB devrait apparraitre dans la fenêtre de l'application. Choisir la clé USB puis cliquer sur **Continuer**.

## Premier démarrage

Une fois l'image copiée sur la clé USB, vous pouvez l'insérer dans le PC qui servira de serveur Calaos. Redémarrer la machine et démarrer sur la clé USB.

Lorsque Calaos OS démarre vous avez le choix entre deux possibilités:

- Utiliser Calaos OS en mode **Live** pour tester le système sans l'installer
- Installer directement Calaos OS sur le disque interne de la machine

{{% notice info %}}
Le mode **Live** permet de configurer le systeme, de tester les fonctionnalités et de voir si tout fonctionne correctement. Si vous êtes satisfait, vous pouvez alors installer Calaos OS sur le disque interne du serveur. Vous garderez alors toutes les configurations que vous avez faites en mode **Live**. Voir plus de détails sur la [page dédié]({{%relref "calaos_os/installation" %}})
{{% /notice %}}

Lorsque le le mode **Live** a demarré, l'interface graphique de Calaos OS apparait sur l'écran. Vous pouvez alors configurer votre systeme, ajouter des I/O, des règles, des scénarios. Pour le moment l'interface est vide, mais nous allons voir comment la configurer dans la prochaine étape.

[Chapitre suivant]({{% relref "getting_started/create_io" %}})