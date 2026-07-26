+++
title = 'Configuration'
date = 2024-03-06T20:14:55Z
weight = 30
summary = "Régler le réseau, les services, la date, la langue et les notifications."
+++

## Deux façons de configurer votre serveur

**Depuis l'écran tactile.** Si un écran tactile est branché sur le serveur, l'interface **Calaos Home** propose un écran de configuration.

**À distance, en SSH.** Depuis votre ordinateur habituel, vous vous connectez au serveur et vous tapez des commandes. C'est la méthode disponible dans tous les cas.

{{% notice info %}}
**L'écran tactile est optionnel, et toutes les options n'y sont pas encore disponibles.** Aujourd'hui, Calaos Home permet de configurer le réseau, les identifiants de connexion et sa propre langue. Tout le reste passe par la ligne de commande.

Si vous n'avez pas d'écran, vous ne perdez rien d'essentiel : la voie SSH couvre l'intégralité des réglages.
{{% /notice %}}

Chaque page ci-dessous indique laquelle des deux méthodes est disponible pour le réglage concerné.

## Avant de commencer

Pour la voie SSH, vous aurez besoin du mot de passe `root` de votre serveur : voyez [Changer les mots de passe]({{% relref "calaos_os/security" %}}) si vous ne l'avez pas encore défini, et [Accès SSH]({{% relref "calaos_os/advanced/shell" %}}) pour la connexion elle-même.

La plupart des réglages passent par l'outil `calaos_config`, présenté sur sa [page dédiée]({{% relref "calaos_os/configuration/calaos_config" %}}).

{{% children description="true" %}}
