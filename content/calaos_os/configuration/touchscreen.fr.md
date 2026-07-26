+++
title = 'Ecran tactile'
date = 2024-03-06T20:15:36Z
weight = 50
summary = "L'écran branché sur le serveur et l'interface Calaos Home."
+++

## À quoi sert l'écran tactile

Un écran tactile branché directement sur le serveur affiche **Calaos Home**, l'interface locale de Calaos. Elle présente votre maison — pièces, éclairages, volets, températures — et permet de la piloter au doigt.

Elle sert aussi à quelques opérations sur le système lui-même :

- configurer le [réseau]({{% relref "calaos_os/configuration/network" %}}) ;
- changer les [identifiants de connexion]({{% relref "calaos_os/security" %}}) ;
- changer sa propre [langue]({{% relref "calaos_os/configuration/lang" %}}) ;
- [installer Calaos OS]({{% relref "calaos_os/installation" %}}) sur le disque, depuis le mode Live ;
- confirmer un [retour en arrière]({{% relref "calaos_os/rollback" %}}).

{{% notice info %}}
**L'écran tactile est optionnel.** Sans lui, votre serveur fonctionne exactement pareil : vous pilotez votre maison depuis les applications mobiles ou la web app, et vous configurez le serveur à distance en SSH.
{{% /notice %}}

## Écran sur le serveur ou écran mural ?

Ne confondez pas deux choses différentes :

- **l'écran branché sur le serveur**, dont parle cette page. Il n'y en a qu'un, là où se trouve la machine ;
- **les écrans muraux Remote UI**, répartis dans les pièces de la maison, qui se connectent au serveur par le réseau.

Si votre objectif est d'avoir des commandes murales dans plusieurs pièces, c'est vers les écrans Remote UI qu'il faut vous tourner : voyez [Remote UI]({{% relref "hardware/remote_ui" %}}).

## Mise en service

Branchez l'écran sur la sortie vidéo du serveur, et son câble tactile sur un port USB, avant d'allumer la machine. Au démarrage, Calaos Home s'affiche automatiquement.

Si vous branchez l'écran alors que le serveur est déjà allumé, redémarrez-le pour qu'il soit détecté :

```sh
reboot
```

## Si l'écran reste noir

Vérifiez dans l'ordre :

1. **l'écran est-il alimenté et allumé ?** Beaucoup d'écrans ont leur propre interrupteur ;
2. **le câble vidéo est-il bien branché** des deux côtés, et sur la bonne sortie si la machine en a plusieurs ;
3. **l'écran est-il sur la bonne entrée ?** Un écran réglé sur HDMI 2 alors que vous êtes branché en HDMI 1 reste noir ;
4. **le serveur fonctionne-t-il malgré tout ?** Essayez de vous y connecter en SSH : si cela marche, le problème est purement lié à l'affichage.

Pour comprendre ce qui se passe côté affichage, consultez les [journaux]({{% relref "calaos_os/configuration/logs" %}}) :

```sh
journalctl -b | grep -i -E "drm|display|xorg"
```

## Calibrer le tactile

Si le toucher fonctionne mais tombe à côté — vous appuyez sur un bouton et c'est le voisin qui réagit — l'écran a besoin d'être calibré.

Connectez-vous en SSH et lancez :

```sh
xinput_calibrator
```

Un écran de calibration s'affiche : touchez successivement les croix qui apparaissent dans les coins. À la fin, **les valeurs sont enregistrées automatiquement au bon endroit**, dans `/mnt/calaos/xorg.conf.d`. Vous n'avez aucun fichier à recopier ni à éditer.

Redémarrez ensuite pour que la nouvelle calibration soit prise en compte :

```sh
reboot
```

{{% notice tip %}}
Touchez les croix le plus précisément possible, avec la pointe du doigt ou un stylet, et en regardant l'écran bien en face. Une calibration faite de biais donne un tactile décalé sur les bords.
{{% /notice %}}

## Si le tactile ne répond pas du tout

Si l'image s'affiche mais que le toucher ne fait rien, c'est la partie tactile qui n'est pas reconnue. Elle passe généralement par un câble **distinct** du câble vidéo : vérifiez qu'il est bien branché.

Pour voir si le système détecte un périphérique tactile :

```sh
libinput list-devices
```

### Écran tactile relié par un adaptateur USB / série

Certains écrans, notamment les modèles à dalle résistive, ne se branchent pas directement en USB : ils utilisent une liaison série, reliée à l'ordinateur par un **adaptateur USB/série**. Ces écrans ne sont pas détectés automatiquement, il faut activer le service correspondant.

Repérez d'abord le nom du périphérique créé par l'adaptateur :

```sh
ls /dev/ttyUSB*
```

Il s'agit le plus souvent de `ttyUSB0`. Activez alors le service en lui passant ce nom, puis démarrez-le :

```sh
systemctl enable usb-serial-touchscreen@ttyUSB0.service
systemctl start usb-serial-touchscreen@ttyUSB0.service
```

Si votre adaptateur apparaît sous un autre nom — `ttyUSB1` par exemple — remplacez-le dans les deux commandes.

Une fois le service actif, le tactile est reconnu et vous pouvez le calibrer comme décrit plus haut.

Pour vérifier que le service tourne correctement :

```sh
systemctl status usb-serial-touchscreen@ttyUSB0.service
```

## Mise en veille de l'écran

Un écran mural allumé en permanence s'use et éclaire la pièce la nuit. Calaos OS peut donc l'éteindre automatiquement après un moment sans utilisation, et le rallumer dès qu'on le touche.

Ces réglages se font avec l'outil [calaos_config]({{% relref "calaos_os/configuration/calaos_config" %}}) :

```sh
calaos_config set dpms_enable true
calaos_config set dpms_standby 2
```

`dpms_enable` active la mise en veille, et `dpms_standby` définit le délai d'inactivité avant qu'elle se déclenche.

Pour empêcher totalement l'écran de s'éteindre — utile pour un écran de supervision qui doit rester visible en permanence :

```sh
calaos_config set dpms_block true
```

## Le curseur de la souris

Sur un écran purement tactile, la flèche de la souris n'a aucune utilité et traîne au milieu de l'affichage. Pour la masquer :

```sh
calaos_config set show_cursor false
```

Repassez la valeur à `true` si vous branchez une souris sur la machine.

## Forcer la connexion à un serveur précis

Par défaut, l'interface se connecte au serveur Calaos de la machine sur laquelle elle tourne. Vous pouvez lui indiquer un autre serveur, par exemple pour utiliser un écran déporté :

```sh
calaos_config set calaos_server_host ws://192.168.1.50:5454/api
```

Respectez bien cette forme : le préfixe `ws://`, l'adresse du serveur, le port `5454`, et le chemin `/api`.

## Pour aller plus loin

Les réglages d'affichage et de calibration propres à votre installation sont conservés dans `/mnt/calaos/xorg.conf.d`. Ce dossier faisant partie de vos données, **son contenu survit aux mises à jour** — voir [Sauvegarder sa configuration]({{% relref "calaos_os/backup" %}}).

{{% notice note %}}
Les écrans tactiles étant très variés, certains réglages fins comme la rotation de l'affichage dépendent du modèle que vous utilisez. Si vous rencontrez une difficulté avec un écran particulier, le forum Calaos est le meilleur endroit pour trouver une configuration déjà éprouvée.
{{% /notice %}}
