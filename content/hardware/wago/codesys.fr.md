+++
title = 'Installation Calaos'
date = 2024-03-06T20:02:42Z
weight = 10
+++

## Programmation de Calaos sur les automates Wago

Afin de faire fonctionner un automate Wago avec Calaos, il doit être chargé au prealable avec le programme Calaos. Deux solutions existent pour programmer un automate avec Calaos:

- avec Calaos Installer
- avec Codesys

## Adresse IP

Dans tous les cas, il est nécessaire de configurer l'adresse IP de l'automate au préalable. Il est préférable de consulter la documentation Wago pour effectuer le paramétrage de l'adresse IP en mode fixe. Par défaut, un automate est réglé en DHCP. Pour passer à une adresse IP fixe, le logiciel Wago Ethernet Settings doit être utilisé. Ce logiciel permet également de rechercher l'automate sur le réseau.

Pour les automates équipés de DIP Switch en façade pour le réglage de l'IP, il est nécessaire de suivre ces étapes :

- Mettre le sélecteur de l'automate dans la position du milieu.
- Choisir l'adresse IP via les sélecteurs. L'adresse sera de la forme 192.168.1.XXX.
- Mettre le sélecteur de l'automate dans la position du haut.

Pour vérifier si l'automate est correctement configuré sur le réseau, il suffit d'accéder à la page web de l'automate via http://192.168.1.XXX. La page web doit s'ouvrir.

## Programme Calaos-Wago

Pour commencer, il faut récupérer le code Calaos sur [Github](https://github.com/calaos/calaos_wago/archive/master.zip). L'archive contient tout le code de tous les automates supportés ainsi que les librairies utilisées. Il suffit d'extraire le zip quelque part.

### Versions de calaos_wago

|Version|Notes|
|-|-|
|1.7 - 2.2| Ancienne version, ne plus utiliser|
|2.3| Version stable. Fonctionne uniquement si pas de borne DALI Master 750-647|
|3.0| Version stable à utiliser uniquement avec une borne DALI Master 750-647|

## Programmer avec Calaos Installer

La solution la plus simple est d'utiliser Calaos Installer. Pour cela il suffit d'ouvrir l'outil **Flash PLC with custom firmware** dans le menu Wago. Entrez ensuite l'IP de l'automate que vous souhaiter installer. Calaos Installer vous demandera ensuite de sélectionner 2 fichiers (utiliser CTRL+sélectionner), un fichier `.PRG` et un fichier `.CHK`. Veillez à bien prendre la bonne version qui correspond à votre automate et à la version souhaitée.

![img](/en/hardware/wago/images/wago_flash.png?width=20pc&classes=shadow)

{{% notice note %}}
La programmation de l'automate peut prendre plusieurs minutes, patientez meme si vous ne voyez rien qui bouge.
{{% /notice %}}

## Programmer avec Codesys

CodeSys est l'outil de développement des automates. Ce tutoriel utilise la version 2.3.9.xx. Il est recommandé d'utiliser la version complète de CodeSys, car la version de démonstration ne permet pas de conserver le code dans l'automate après un redémarrage.

{{% notice note %}}
Il est conseillé d'utiliser CodeSys en anglais, car la version française présente des traductions parfois incompréhensibles. Pour changer la langue, accédez à Menu Projet, Options, Environnement de travail, puis choisissez Anglais.
{{% /notice %}}

### Ouvrir le projet dans Codesys

Il faut commencer par executer CodeSys puis ouvrir le projet correspondant à son automate (wago_881.pro pour un 750-881). Il convient de prendre la derniere version en date qui se trouvent dans le dossier. Ex: Wago_3.0 pour la version 3.0 du programme.

Une fois le projet ouvert il ne doit pas y avoir d'erreur. Une message d'information peut s'afficher expliquant que des bibliotheques ont ete modifiés, il faut juste valider.

![img](/en/hardware/wago/images/codesys01.jpg?width=20pc&classes=shadow)

### Compiler le projet

Pour recompiler le projet il faut se rendre sur le menu **Project** puis **Rebuild All**

![img](/en/hardware/wago/images/codesys02.jpg?width=20pc&classes=shadow)

Si tout se passe bien, il doit y avoir **0 Erreurs** en bas dans la console.

![img](/en/hardware/wago/images/codesys03.jpg?width=20pc&classes=shadow)

### Sélection de l'IP de l'automate

Il faut ensuite aller dans **Online**, **Communication parameters**.

![img](/en/hardware/wago/images/codesys04.jpg?width=20pc&classes=shadow)

La fenetre suivante s'ouvre:

![img](/en/hardware/wago/images/codesys05.jpg?width=20pc&classes=shadow)

L'addresse IP est à changer dans cette fenêtre en double cliquant sur celle ci, puis en cliquant sur OK.

### Chargement

Pour charger l'automate il faut aller sur **Online**, **Login**. Le projet sera compilà si nécessaire automatiquement, puis la connection avec l'automate établie. Si la connection fonctionne, CodeSys va demander si on veut charger le nouveau programme. Il faut bien entendu répondre **Yes**.

Le chargement peut prendre du temps (plusieurs minutes). Une fois terminé, codesys se met en mode debogueur, mais le programme n'est pas executé. On voit si le programme est lancé en bas a droite.

![img](/en/hardware/wago/images/codesys07.jpg?width=20pc&classes=shadow)

Avant d'executer le programme, il faut que le programme soit ecrit dans la mémoire non-volatile. Sans ca, au prochain reboot de l'automate, tout aura disparu. Pour le faire, une fois en mode **Online** il faut aller dans le menu **Online**, **Create boot project**.

![img](/en/hardware/wago/images/codesys08.jpg?width=20pc&classes=shadow)

Cela va durer un certain temps, il faut laisser codesys faire son travail.

## Calaos Installer

Pour que calaos installer fonctionne ensuite avec l'automate il faut que la led de l'automate soit en orange fixe. Il ne faut pas se connecter sur l'automate avec calaos_installer en meme temps que `calaos_server`. Il faut imperativement déconnecter `calaos_server` du reseau dans ce cas la.

Dans calaos installer:

- Menu automate → **Se connecter**, donner l'IP de l'automate.
- Une fois que c'est connecté en bas a droite de la fenetre: `connecté (2.2)` avec la version de l'automate.
- Si c'est ok, la config du mode dégradé peut etre envoyé avec Menu automate → **Programmer l'automate**.
- Les regles fonctionnent de suite sans devoir redémarrer l'automate.

Pour la detection des interrupteurs:

- Il faut être en mode connecté a l'automate dans calaos installer!
- Si la detection ne marche pas, vérifier que la led sur la borne d'entrée s'allume bien.
- Si ca ne marche toujours pas, c'est un pb avec les firewall sous windows qui ne laissent pas passer les trames UDP correctement… Essayer de couper le firewall.
- Si ca ne marche quand meme pas. Ce n'est pas grave, les regles peuvent quand même être crées en donnant le numero de l'entrée qu'il faut utiliser pour l'interrupteur. Il faut compter le numéro de l'entrée sur la borne directement.
