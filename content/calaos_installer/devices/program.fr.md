+++
title = "Programme horaire"
date = 2026-11-26T09:00:00Z
weight = 35
summary = "Programmer la semaine d'un thermostat ou d'une clim : plages, niveaux de température, priorités, copie d'un jour, mise en pause, et où l'éditer."
+++

Un climat peut porter son **programme hebdomadaire**, sans règle à écrire. Il demande calaos_base 4.9.0, calaos-web-app 4.3.0 et calaos_mobile 4.6.0 au minimum. Le chauffage (`heat`) et la clim (`cool`) ont chacun leur programme, car leurs horaires et leurs consignes diffèrent.

## Plages et niveaux

- Une **plage** est un créneau d'un jour de la semaine, avec **sa propre température**. Plusieurs plages peuvent avoir la même température.
- Une semaine, pour un même programme, compte au plus **10 températures différentes**. Chaque température est un **niveau**, numéroté de 1 à 10.
- Un créneau qui passe minuit est coupé en deux : par exemple jeudi 22 h 30 à 24 h, puis vendredi 0 h à 6 h.
- Les consignes vont de 7 à 30 °C, par pas de 0,5.

### Priorités

Quand deux plages se recouvrent, le **niveau le plus bas l'emporte** : le niveau 1 passe avant le niveau 2. Cette priorité n'est pas réglable.

### Consigne par défaut

Chaque programme a une **consigne par défaut**, appliquée quand aucune plage n'est active. Pour la clim, vous pouvez choisir **« Clim arrêtée hors créneau »** : hors des plages, la consigne est poussée au maximum (30 °C) et le mode reste `cool`.

## Ce que fait le programme

Le programme ne génère aucune règle. Le device écrit lui-même sa consigne :

- quand une plage commence ou finit ;
- quand le mode change ;
- quand vous activez ou mettez en pause le programme ;
- quand vous modifiez la consigne d'un niveau ;
- au démarrage du serveur.

Une consigne réglée à la main **tient jusqu'au prochain de ces événements**.

| Mode | Effet du programme |
|---|---|
| `heat`, `cool` | écrit la consigne du niveau actif |
| `auto` | écrit la consigne basse avec le programme chauffage et la haute avec le programme clim ; l'écart minimal est respecté (2 °C par défaut) |
| `off`, `dry`, `fan_only` | n'écrit rien |

## Mise en pause

Un interrupteur **Programme activé** existe par climat. En pause, la dernière consigne reste en place, et les écrans indiquent « Programme en pause : niveau n (xx °C) ». À la réactivation, la plage en cours est réappliquée. Cet interrupteur peut aussi être lu et commandé par une [règle]({{% relref "calaos_installer/rules" %}}).

## Modifier le programme

### Application web, mobile et écran mural

Ouvrez le climat, puis **Modifier le programme**. Choisissez l'onglet **Chauffage** ou **Clim**, puis le jour (Lun à Dim). Une frise de 24 h montre les plages du jour.

- **+ Créneau** (ou **Ajouter une plage**) ajoute une plage : heure de début, de fin, température. Le champ d'heure avance par 15 minutes.
- Sur ordinateur et sur écran mural, on peut aussi **tirer sur la frise** pour créer une sélection, par pas de 15 minutes. L'application web n'a pas ce glisser.
- Une 11e température différente est refusée : « Cette semaine utilise déjà 10 températures différentes. Choisissez l'une d'elles : », avec les températures existantes à toucher.
- **Copier ce jour…** recopie le jour affiché vers d'autres jours, pour l'onglet actif seulement. La copie **remplace** les plages des jours cibles ; une confirmation est demandée si l'un d'eux en contient déjà. Un jour source vide est refusé.
- En mode `auto`, l'aide « i » explique les deux programmes ; la zone hachurée entre les deux consignes est l'écart minimal.

Une session en lecture seule (service ou hors ligne) ne peut rien modifier.

Les écrans de commande à distance (ESP32) affichent seulement l'état, par exemple « Niveau 2 jusqu'à 22:30 · 20.5° », « Programme en pause… » ou « Clim arrêtée jusqu'à 12:00 ». Ils ne permettent pas l'édition.

Sur la ligne du climat, une pastille indique le prochain changement, par exemple « ▶ 17:30 ».

## Dans Calaos Installer

À l'étape 0 de la recette, la case **Programme** (dans Chauffage et/ou le groupe Clim) active le programme horaire. La décocher **supprime** les consignes, plages et niveaux du programme. Le nombre de niveaux n'est plus un réglage : il est de 10 dès que le programme est activé.

- Les IO du programme appartiennent au device et se trouvent dans le sous-nœud **Programme**.
- Après avoir ajouté un programme à un device existant, **redémarrez le serveur** : c'est lui qui crée les IO.
- Dans une règle, le sélecteur propose par exemple « Salon › programme actif ».
- Les mois réglés dans Calaos Installer sont conservés par les modifications faites depuis les applications.

L'assistant MCP refuse d'écrire les IO du programme.

## Programme ou règles ?

Le programme couvre les semaines types. Gardez les [règles de chauffage]({{% relref "calaos_installer/rules_heating" %}}) pour le reste : absence, fenêtre ouverte, hors-gel.
