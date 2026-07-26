+++
title = "Qu'est-ce que Calaos OS ?"
date = 2026-07-25T10:00:00Z
weight = 5
summary = "Ce qu'est Calaos OS et ce qu'il apporte."
+++

## En quelques mots

Calaos OS est un système d'exploitation complet, prêt à l'emploi, que vous installez sur un ordinateur dédié à votre domotique. Cet ordinateur devient votre **serveur Calaos** : c'est lui qui pilote vos éclairages, vos volets, votre chauffage, et qui répond aux applications mobiles.

Vous n'avez pas besoin d'installer un système d'exploitation puis les logiciels Calaos par-dessus : tout est déjà assemblé et préconfiguré. Vous copiez le système sur une clé USB, vous démarrez l'ordinateur dessus, et vous installez.

## Ce que ça change pour vous

**Tout est déjà là.** Le serveur Calaos et les services complémentaires (MQTT, Zigbee, historiques de mesures, graphiques, éclairage DMX…) sont installés et configurés pour fonctionner ensemble. Vous n'avez rien à assembler vous-même.

**Les mises à jour sont réversibles.** Avant chaque installation de logiciel, Calaos OS enregistre automatiquement l'état du système. Si une mise à jour se passe mal, vous pouvez redémarrer sur l'état précédent. C'est expliqué dans [Revenir en arrière]({{% relref "calaos_os/rollback" %}}).

**Votre configuration est regroupée à un seul endroit.** Tous vos réglages — ceux de Calaos et ceux des services — vivent dans un unique dossier, ce qui rend la sauvegarde simple. Voir [Sauvegarder sa configuration]({{% relref "calaos_os/backup" %}}).

## Ce qu'il vous faut

Un ordinateur qui restera allumé en permanence, une clé USB, et de quoi le relier à votre réseau. Le détail est sur la page [De quoi ai-je besoin ?]({{% relref "calaos_os/requirements" %}}).

Un écran tactile est **optionnel**. Avec un écran, vous disposez de l'interface locale Calaos Home, qui affiche votre maison et permet quelques réglages. Sans écran, tout se configure à distance depuis un autre ordinateur.

{{% notice note %}}
Cette documentation ne concerne que **Calaos v4**. Les versions précédentes (v1, v2, v3) ne sont pas couvertes.
{{% /notice %}}

## Par où commencer

Si vous découvrez Calaos, suivez le chapitre [Démarrage]({{% relref "getting_started" %}}) : il vous guide de l'installation jusqu'à votre première règle domotique.

Si vous voulez installer le serveur maintenant, allez directement à [Installation]({{% relref "calaos_os/installation" %}}).

## Pour aller plus loin

Calaos OS est construit sur **Debian**. Les composants applicatifs (serveur Calaos, MQTT, Zigbee, bases de données…) s'exécutent dans des **containers Podman**, isolés les uns des autres, ce qui permet de les mettre à jour indépendamment.

Le disque est formaté en **Btrfs**, un système de fichiers capable de photographier l'état du système à un instant donné pour presque rien en espace disque. C'est ce qui rend les retours en arrière possibles. Le détail est décrit dans [Sous le capot]({{% relref "calaos_os/advanced/btrfs" %}}).

L'architecture logicielle complète est présentée dans le chapitre [Pile logicielle Calaos]({{% relref "dev" %}}).
