+++
title = "Projets d'exemple"
date = 2026-10-15T10:00:00Z
weight = 50
summary = "Quatre projets complets à télécharger, du radiateur électrique au plancher réversible sur pompe à chaleur : les ouvrir, les essayer, les adapter."
+++

Ces projets sont complets : un `io.xml` (pièces, IO, devices) et un `rules.xml` (règles). Ils s'ouvrent dans Calaos Installer sans erreur. Les exemples 1, 3 et 4 fonctionnent même **sans matériel** : vous pouvez les essayer avant de les adapter à votre maison. L'exemple 2 a besoin de sa clim.

| Projet | Ce qu'il montre | Télécharger |
|---|---|---|
| **1. Un radiateur électrique** | un thermostat interne sur un relais, une programmation semaine, l'arrêt fenêtre ouverte ([recettes 1 et 3]({{% relref "calaos_installer/rules_heating" %}})) | [1-radiateur-electrique.zip](/examples/climat/1-radiateur-electrique.zip) |
| **2. Une clim ESPHome** | une clim qui régule seule (device `proxy`), la sonde de la pièce relayée à la clim, l'absence et le hors-gel ([Clim ESPHome]({{% relref "hardware/esphome_climate" %}}), recette 4) | [2-clim-esphome.zip](/examples/climat/2-clim-esphome.zip) |
| **3. Un plancher chauffant, 3 zones** | trois thermostats de plancher, une vanne par zone, un circulateur commun commandé par règles (recette 5) | [3-plancher-chauffant-3-zones.zip](/examples/climat/3-plancher-chauffant-3-zones.zip) |
| **4. Un plancher réversible, 9 zones** | neuf zones à une vanne, le mode de la maison, les deux contacts de la pompe à chaleur, l'eau chaude sanitaire (recettes 6 et 7) | [4-plancher-reversible-pac.zip](/examples/climat/4-plancher-reversible-pac.zip) |

Chaque archive contient un dossier avec `io.xml`, `rules.xml` et un `LISEZMOI.txt` qui liste ce qu'il faut remplacer. Les noms sont en français, comme sur les captures de cette documentation (« PAC » pour pompe à chaleur, « ECS » pour eau chaude sanitaire).

## Ouvrir un exemple

1. Décompressez l'archive : vous obtenez un dossier.
2. Dans Calaos Installer, menu **Fichier**, **Ouvrir un projet...**, et choisissez ce dossier.

Le projet s'ouvre sans message d'erreur. Les devices se modifient comme les vôtres : voir [Créer un device]({{% relref "calaos_installer/devices/templates" %}}).

## Les IO à remplacer

Les IO dont le nom finit par **« (à remplacer) »** sont des [variables internes]({{% relref "calaos_installer/internal_var" %}}) : une sonde est une variable numérique, un relais ou une vanne une variable booléenne. Elles tiennent la place de votre matériel, pour que le projet fonctionne tel quel.

Pour essayer, envoyez le projet sur une box de test, puis changez la température d'une sonde depuis une interface : la sortie du thermostat suit. Une sonde « à remplacer » ne se met à jour que quand vous la changez. Au bout de `sensor_timeout` (2 h 30) sans changement, le thermostat la juge muette et coupe sa sortie : c'est la sûreté normale.

Pour mettre votre matériel à la place, deux façons :

- **dans Calaos Installer** : créez le vrai IO (sonde, relais), ouvrez le device et choisissez-le pour le lien concerné (voir [Créer un device]({{% relref "calaos_installer/devices/templates" %}})). Corrigez les règles qui utilisaient l'ancien IO, puis supprimez l'IO « à remplacer » ;
- **dans `io.xml`**, avec un éditeur de texte : remplacez la ligne de l'IO par celle de votre matériel, **en gardant le même `id`**. Le device et les règles suivent sans autre changement.

L'exemple 2 est différent : ses IO sont les vrais IO MQTT d'une clim ESPHome, mais ils visent le broker `127.0.0.1` et le nœud `clim_salon`. Le plus simple est de recréer le device avec le modèle **Clim ESPHome** de l'assistant et sa recherche sur le broker, en gardant le même nom, puis de vérifier les règles.

## Envoyer le projet à une box

Menu **Calaos Server**, **Envoyer le projet à Calaos Server...** : voir [Envoyer la config]({{% relref "getting_started/upload" %}}).

{{% notice warning %}}
Le projet **remplace** toute la configuration de la box. Si vous voulez garder la vôtre, récupérez-la d'abord avec **Charger le projet depuis Calaos Server...**, ou essayez l'exemple sur une box de test.
{{% /notice %}}

## Ce qui est vérifié

Chaque exemple est chargé par `calaos_server` et ouvert par Calaos Installer avant d'être publié : le serveur charge toutes les règles, sans avertissement au chargement. Seule exception, attendue : dans l'exemple 4, une ligne « cooling has no dew-point cut » par zone qui rafraîchit sans sonde d'humidité (voir [le point de rosée]({{% relref "calaos_installer/devices/thermostat" %}})).
