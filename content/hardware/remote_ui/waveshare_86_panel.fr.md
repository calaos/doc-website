+++
title = 'Waveshare ESP32-P4 86 Panel'
date = 2026-03-28T14:32:10Z
+++

## Présentation

La carte **Waveshare ESP32-P4 Smart 86 Box** est un panneau mural tactile au format standard 86 mm, conçu pour s'intégrer directement dans vos boîtiers d'encastrement muraux. Elle embarque un écran IPS tactile 4 pouces (720 × 720 pixels), basé sur le microcontrôleur ESP32-P4 haute performance.

Vous pouvez l'acheter directement sur le site Waveshare (version avec port Ethernet + 2 relais recommandée pour Calaos) :
👉 [Waveshare ESP32-P4 Smart 86 Box — version Ethernet + 2ch Relay](https://www.waveshare.com/esp32-p4-wifi6-touch-lcd-4b.htm?sku=31570)

![Remote UI installé sur un mur](/en/hardware/remote_ui/images/IMG_7580.jpg?width=25pc&classes=shadow)

## Caractéristiques principales

- **Format 86 mm** : s'encastre dans les boîtiers muraux standard de type 86 (boîte d'encastrement non fournie, profondeur minimale recommandée : 33 mm)
- **Alimentation** : DC 6 V à 30 V via bornier à vis
- **Réseau** : Wi-Fi ou Ethernet 100 Mbps (port RJ45), au choix
- **2 relais intégrés** : 10 A / 230 V AC avec isolation optocoupeur — utilisables directement comme sorties dans Calaos (éclairage, etc.)
- **Écran** : IPS 4 pouces, 720 × 720 px, tactile capacitif 5 points, angle de vision 170°
- **Microcontrôleur** : ESP32-P4 RISC-V double cœur, 32 MB Flash, 32 MB PSRAM

## Installation du firmware Calaos

Avant la première utilisation, il faut flasher le firmware Calaos sur la carte via le **port USB**. Cette opération se réalise depuis **Calaos Installer** :

1. Connectez la carte à votre ordinateur via le câble USB-C (port USB TO UART)
2. Ouvrez Calaos Installer et accédez à la section de flash du firmware Remote UI (Menu **Tools** > **Flash Remote UI Device** )
3. Sélectionnez le port série correspondant à la carte et lancez le flash
4. Une fois le flash terminé, débranchez l'USB et alimentez la carte en DC

![Flash tool étape 1](/en/hardware/remote_ui/images/flash_tool_01.png?width=30pc&classes=shadow)

L'outil de flash permet de configurer plusieurs paramètres qui seront intégrés directement dans le firmware :

- **Réseau** : choix entre Wi-Fi (SSID + mot de passe) ou Ethernet, avec une adresse IP en DHCP ou statique
- **Serveur Calaos** : découverte automatique (`calaos_server` sur le réseau local), ou adresse IP/hostname et port renseignés manuellement, avec option SSL
- **Effacement complet** (*Erase entire flash before writing*) : remet la carte en état d'usine et génère un nouveau code d'appairage — utile si vous souhaitez réassigner la carte à un autre serveur

![Flash tool étape 2](/en/hardware/remote_ui/images/flash_tool_02.png?width=30pc&classes=shadow)

> Le reflash manuel n'est nécessaire qu'une seule fois. Les mises à jour ultérieures se font en OTA (voir section [Mises à jour](#mises-à-jour-ota)).

## Processus d'appairage

Une fois la carte alimentée et le firmware Calaos installé, voici ce qui se passe :

1. **Connexion réseau** — La carte se connecte automatiquement au réseau (Wi-Fi ou Ethernet selon la configuration).

2. **Synchronisation de l'heure** — La carte se synchronise avec un serveur NTP (Internet requis). **Cette étape est obligatoire** : l'authentification sécurisée avec Calaos repose sur l'heure système. Si l'heure est incorrecte, la carte ne pourra pas se connecter au serveur.

3. **Découverte du serveur** — La carte tente de détecter automatiquement `calaos_server` sur le réseau local, ou se connecte à l'adresse de serveur configurée manuellement.

4. **Génération du code d'appairage** — La carte génère et affiche un **code d'appairage unique** à configurer dans Calaos Installer.

5. **Vérification du code** :
   - Si le code est **déjà présent** dans Calaos Server (ajouté via Calaos Installer), la carte se connecte immédiatement de façon sécurisée.
   - Sinon, l'écran affiche une invite vous demandant d'ajouter ce code dans Calaos Installer avant de réessayer.

6. **Configuration de l'interface** — Une fois la carte appairée, vous pouvez configurer son interface directement depuis Calaos Installer.

## Configuration dans Calaos Installer

### Création de l'IO Remote UI

Pour ajouter un nouvel écran Remote UI :

1. Dans Calaos Installer, créez un nouvel IO de type **RemoteUI**
2. Renseignez le **code d'appairage** affiché sur l'écran de la carte
3. Donnez-lui un nom (ex. : « Salon », « Entrée »)

![Ajout d'un IO Remote UI](/en/hardware/remote_ui/images/add_remote_ui.png?width=20pc&classes=shadow)

![IO Remote UI dans la liste](/en/hardware/remote_ui/images/remoteui_io.png?width=25pc&classes=shadow)

### Éditeur d'interface Remote UI

Une fois l'IO créé, ouvrez l'**éditeur Remote UI** pour concevoir l'interface de l'écran :

![Menu éditeur Remote UI](/en/hardware/remote_ui/images/edit_remote_ui_menu.png?width=20pc&classes=shadow)

- L'interface est organisée sous forme de **grille** (par défaut 3 × 3 sur la carte Waveshare)
- Chaque cellule de la grille peut recevoir un **widget** :
  - Widgets Calaos : **Lumière**, **Volet**, **Scénario**, **Température**, etc.
  - Widget **Horloge** (Clock)
- Vous pouvez créer **plusieurs pages** et naviguer entre elles depuis l'écran

![Éditeur Remote UI](/en/hardware/remote_ui/images/remote_ui_editor.png?width=40pc&classes=shadow)

### Écran de veille (screensaver)

Un **screensaver** peut être configuré pour éteindre ou réduire la luminosité de l'écran après une période d'inactivité, afin de préserver l'affichage et réduire la consommation.

### Actions depuis les règles Calaos

Dans Calaos Installer, vous pouvez utiliser des **actions sur un IO Remote UI** au sein de vos règles d'automatisation :

| Action | Description |
|---|---|
| `set_brightness` | Régler la luminosité de l'écran (ex. : dimmer la nuit) |
| `set_page` | Changer la page affichée (ex. : afficher la page « Alarme » en cas d'alerte) |

Cela permet, par exemple, de baisser automatiquement la luminosité la nuit en fonction d'une plage horaire.

### Utiliser les relais comme sorties Calaos

Pour contrôler les 2 relais intégrés de la carte depuis Calaos :

1. Créez un nouvel IO de type **RemoteUI / Relay Output** dans Calaos Installer
2. Associez-le à la carte et au numéro de relais souhaité
3. Cet IO se comporte ensuite comme une **sortie lumière classique** dans Calaos : vous pouvez l'utiliser dans vos règles, scénarios, et l'afficher dans vos interfaces

![Ajout d'un IO relais Remote UI](/en/hardware/remote_ui/images/add_io_relay.png?width=30pc&classes=shadow)

## Mises à jour OTA

Calaos Remote UI supporte les **mises à jour de firmware en OTA** (Over The Air) depuis Calaos Server, à condition d'utiliser **Calaos OS**.

Lors des mises à jour de Calaos OS, tous les écrans Remote UI connectés reçoivent automatiquement la nouvelle version du firmware si une mise à jour est disponible. Aucune intervention manuelle n'est nécessaire.
