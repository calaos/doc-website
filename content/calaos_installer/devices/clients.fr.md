+++
title = "Afficher et régler"
date = 2026-10-15T09:25:00Z
weight = 40
summary = "Les climats dans l'application web, l'application mobile, les écrans muraux et l'assistant MCP."
+++

Tous les clients lisent les mêmes informations du device et suivent les mêmes principes :

- l'écran principal montre la **consigne**, la **température de la pièce**, le **mode** et un **statut** ;
- le mode est une pastille : un appui ouvre le choix du mode ;
- la ventilation, l'oscillation et l'humidité sont rangées sous **Plus de réglages** et n'apparaissent que si le device les propose ;
- le cadran est un arc presque fermé ; en mode `auto`, il porte **deux curseurs** et deux paires de boutons − / + ;
- si les deux consignes sont à l'écart minimal, les boutons intérieurs sont refusés et l'écran indique « Écart minimum 2° ».

Les couleurs ont un sens : **jaune** quand l'appareil chauffe, **cyan** quand il refroidit, **ambre** quand il attend, **rouge** quand il est hors ligne ; un appareil arrêté, inconnu ou en veille reste neutre.

## Le statut affiché

| Statut | Signification |
|---|---|
| Chauffe, Refroidit, Déshumidifie, Ventile | l'action en cours rapportée par l'appareil ou le thermostat |
| En veille | l'appareil est allumé mais ne fait rien pour l'instant |
| Démarre dans N min | une sortie voulue attend la fin de sa durée minimale d'arrêt |
| Hors ligne | l'appareil relayé est injoignable (ESPHome déconnecté) |
| Sonde muette, Sonde déconnectée, En attente de la sonde, Sonde absente, Sonde hors limites | sûreté d'un thermostat : voir [Thermostat interne]({{% relref "calaos_installer/devices/thermostat" %}}) |
| Humidité trop haute, Sonde d'humidité muette | refroidissement d'un plancher coupé (point de rosée) |
| Arrêté | mode `off` |
| Mode inconnu | le device n'a encore reçu aucun mode : voir [Dépannage]({{% relref "calaos_installer/devices/troubleshooting" %}}) |

## Application web

Les climats forment des **rangées** dans la liste de la pièce : un mini-cadran, le nom, le statut, la consigne, la température de la pièce.

![Liste des climats de la pièce dans l'application web](/en/calaos_installer/devices/images/web-room-list.png?width=45pc&classes=shadow)
*Une rangée par device : ici une clim qui chauffe, un plancher qui refroidit, une pompe à chaleur en veille, un radiateur qui attend, une clim arrêtée et une clim hors ligne.*

Un clic sur une rangée ouvre le **panneau de réglage** à droite de l'écran (en dessous de 1024 px de large, une fenêtre centrée). On y règle la consigne avec les boutons − / +, et on voit le statut et la température de la pièce.

![Panneau de réglage en mode auto](/en/calaos_installer/devices/images/web-settings-panel.png?width=45pc&classes=shadow)
*Une clim en mode auto : deux consignes, 19,5° et 24,5°.*

![Panneau de réglage d'une clim à consigne unique](/en/calaos_installer/devices/images/web-settings-single.png?width=45pc&classes=shadow)
*Une clim à consigne unique, en mode Froid.*

La pastille de mode ouvre le choix du mode, et **Plus de réglages** ouvre la ventilation, l'oscillation et l'humidité.

![Choix du mode](/en/calaos_installer/devices/images/web-mode-choice.png?width=45pc&classes=shadow)
*Le choix du mode : seuls les modes proposés par le device apparaissent.*

![Plus de réglages](/en/calaos_installer/devices/images/web-more-settings.png?width=45pc&classes=shadow)
*Plus de réglages : ventilation, oscillation, humidité.*

Un thermostat interne s'affiche de la même façon ; seuls les modes qu'il possède sont proposés.

![Thermostat d'un plancher](/en/calaos_installer/devices/images/web-thermostat.png?width=45pc&classes=shadow)
*Le plancher du séjour, en refroidissement.*

L'écart minimal entre les deux consignes est imposé à l'écran :

![Écart minimum](/en/calaos_installer/devices/images/web-range-stop.png?width=45pc&classes=shadow)
*Les boutons intérieurs sont refusés quand l'écart minimum est atteint.*

Un défaut de sûreté se voit dans la rangée comme dans le panneau, en rouge :

![Défaut de sonde](/en/calaos_installer/devices/images/web-fault.png?width=45pc&classes=shadow)
*Un radiateur dont la sonde est muette : le chauffage est coupé, la consigne reste réglable.*

## Application mobile

Sur téléphone, la liste montre pour chaque climat le nom (en jaune quand l'appareil agit), la température de la pièce, la pastille de mode, le statut et la consigne (« 19,5 – 24,5° » en `auto`).

![Liste des climats sur mobile](/en/calaos_installer/devices/images/mobile-list.png?width=18pc&classes=shadow)
*La liste sur téléphone : un climat par statut.*

Un appui ouvre une **fenêtre de réglage** en bas de l'écran (sur ordinateur, un dialogue).

![Réglage sur mobile](/en/calaos_installer/devices/images/mobile-sheet.png?width=18pc&classes=shadow)
*Le réglage d'une clim en mode auto.*

![Choix du mode sur mobile](/en/calaos_installer/devices/images/mobile-sheet-mode.png?width=18pc&classes=shadow)
*Le choix du mode, par-dessus le réglage.*

![Plus de réglages sur mobile](/en/calaos_installer/devices/images/mobile-sheet-more.png?width=18pc&classes=shadow)
*Plus de réglages.*

![Réglage d'un thermostat sur mobile](/en/calaos_installer/devices/images/mobile-sheet-thermostat.png?width=18pc&classes=shadow)
*Le plancher du séjour.*

Sur ordinateur, la même liste et un dialogue à la place de la fenêtre du bas :

![Liste sur ordinateur](/en/calaos_installer/devices/images/mobile-desktop-list.png?width=22pc&classes=shadow)
*La liste, version ordinateur.*

![Dialogue de réglage sur ordinateur](/en/calaos_installer/devices/images/mobile-desktop-dialog.png?width=26pc&classes=shadow)
*Le dialogue de réglage.*

## Écran mural

Sur l'écran mural (voir [Remote UI]({{% relref "hardware/remote_ui" %}})), le contenu dépend de la **taille du widget** dans la grille. Dans Calaos Installer, ajoutez un widget de type **Climate** et liez-le au device.

Les **petits widgets** (1×1, 2×1, 1×2) affichent la température de la pièce, la consigne, l'icône et le mot de statut, et le nom. Il n'y a pas de boutons − / + : un appui ouvre la **vue plein écran**.

![Petits widgets climat sur l'écran mural](/en/calaos_installer/devices/images/rui-small-widgets.png?width=30pc&classes=shadow)
*Plusieurs petits widgets sur une page de l'écran mural.*

![Vue plein écran](/en/calaos_installer/devices/images/rui-fullscreen.png?width=30pc&classes=shadow)
*La vue plein écran : cadran, consignes, pastille de mode, Plus de réglages.*

Le mode et les réglages s'ouvrent en fenêtres superposées :

![Fenêtre de choix du mode](/en/calaos_installer/devices/images/rui-mode-popup.png?width=30pc&classes=shadow)
*Le choix du mode.*

![Fenêtre Plus de réglages](/en/calaos_installer/devices/images/rui-more-popup.png?width=30pc&classes=shadow)
*Plus de réglages.*

Un **grand widget** (2×2, 3×3 et plus) contient le climat complet, sans vue intermédiaire : la pastille de mode et `⋯` en haut, le cadran et les boutons − / + au milieu, le nom en bas comme pour les autres widgets.

![Grand widget 2×2](/en/calaos_installer/devices/images/rui-large-2x2.png?width=30pc&classes=shadow)
*Un widget 2×2.*

![Grand widget 3×3](/en/calaos_installer/devices/images/rui-large-3x3.png?width=30pc&classes=shadow)
*Un widget 3×3 en mode auto.*

![Écran 800×480](/en/calaos_installer/devices/images/rui-800x480.png?width=30pc&classes=shadow)
*Sur un panneau 800×480.*

{{% notice tip %}}
Si un widget lié à un climat s'affiche en rouge « Unsupported », c'est un type de widget que le logiciel du panneau ne connaît pas : voir [Dépannage]({{% relref "calaos_installer/devices/troubleshooting" %}}).
{{% /notice %}}

## L'assistant MCP

L'assistant MCP de Calaos (sidecar de [Calaos Server]({{% relref "dev/calaos_server" %}})) voit un device comme un IO ordinaire dont la valeur est le mode. Il peut le commander avec `set_io_state`, en mode simple (`off`) ou en JSON (`{"mode":"cool","target_temp":24}`) ; la description de l'outil indique les champs de chaque device. Il peut aussi lire l'objet complet du device (capacités, état). Les **relais** d'un thermostat sont masqués et ne peuvent pas être écrits : l'assistant règle le thermostat, jamais ses sorties.
