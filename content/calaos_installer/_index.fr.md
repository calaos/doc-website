+++
title = "Calaos Installer"
date = 2024-03-06T19:56:46Z
weight = 30
chapter = true
+++

### La configuration

# Calaos Installer

Calaos Installer est le logiciel avec lequel vous décrivez votre installation : les pièces de la maison, les équipements qui s'y trouvent, et les règles qui les font réagir.

Il s'installe sur **votre ordinateur habituel**, pas sur le serveur. Vous y préparez votre projet, puis vous l'envoyez au serveur Calaos, qui l'applique.

## Comment ça s'articule

```text
Votre ordinateur                          Le serveur Calaos
┌────────────────────┐                    ┌────────────────────┐
│  Calaos Installer  │  ── envoi ──▶      │   calaos_server    │
│  votre projet      │  ◀── lecture ──    │   pilote la maison │
└────────────────────┘                    └────────────────────┘
```

Vous travaillez toujours sur une copie locale du projet. Rien ne change chez vous tant que vous n'avez pas **envoyé** la configuration au serveur, ce qui vous laisse libre d'expérimenter sans risque.

Le serveur, de son côté, conserve une copie datée de chaque configuration reçue — voir [Sauvegarder sa configuration]({{% relref "calaos_os/backup" %}}).

## L'ordre des choses

La configuration se construit dans un ordre naturel, chaque étape s'appuyant sur la précédente :

1. **[Créer des pièces]({{% relref "calaos_installer/rooms" %}})** — la structure de votre maison.
2. **[Créer des IO]({{% relref "calaos_installer/io" %}})** — les équipements : lumières, volets, interrupteurs, sondes.
3. **[Créer des règles]({{% relref "calaos_installer/rules" %}})** — ce qui doit se passer, et quand.

Les autres pages de ce chapitre couvrent des besoins plus spécifiques : les [variables internes]({{% relref "calaos_installer/internal_var" %}}) pour mémoriser un état, les [scénarios]({{% relref "calaos_installer/scenario" %}}) pour enchaîner des actions, la [programmation horaire]({{% relref "calaos_installer/time" %}}) pour déclencher sur l'heure, et les [scripts Lua]({{% relref "calaos_installer/scripts" %}}) quand les règles ne suffisent plus.

{{% notice tip %}}
Si vous découvrez Calaos, ne commencez pas par ce chapitre : suivez d'abord [Démarrage]({{% relref "getting_started" %}}), qui vous fait créer une pièce, une lumière, un interrupteur et une première règle. Vous reviendrez ici pour approfondir.
{{% /notice %}}

## Se connecter au serveur

Deux opérations relient Calaos Installer à votre serveur, dans le menu **Calaos Server** :

- **Envoyer le projet** — votre configuration part vers le serveur, qui redémarre ses services pour la prendre en compte ;
- **Charger le projet** — vous récupérez la configuration qui tourne actuellement sur le serveur.

La seconde est aussi le moyen le plus simple de **sauvegarder** votre installation : voir [Envoyer le projet]({{% relref "getting_started/upload" %}}).

Ces opérations demandent l'adresse du serveur et les identifiants de connexion, dont les valeurs par défaut sont `user` / `pass` — à changer, voir [Changer les mots de passe]({{% relref "calaos_os/security" %}}).

## Au-delà de la configuration

Calaos Installer sert aussi à quelques tâches qui débordent du projet lui-même :

- **créer la clé USB d'installation** de Calaos OS — voir [Installation]({{% relref "calaos_os/installation" %}}) ;
- **flasher et configurer les écrans muraux** Remote UI — voir [Remote UI]({{% relref "hardware/remote_ui" %}}) ;
- **programmer l'automate Wago** — voir [Configuration Wago]({{% relref "calaos_installer/wago" %}}).

{{% children description="true" %}}
