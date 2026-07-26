+++
title = 'calaos_config'
date = 2024-03-06T20:15:29Z
weight = 40
summary = "L'outil qui lit et modifie les réglages du serveur Calaos."
+++

## À quoi sert cet outil

`calaos_config` est l'outil qui permet de consulter et de modifier les réglages du serveur Calaos depuis un terminal. C'est lui qu'utilisent la plupart des pages de ce chapitre.

Il travaille sur la configuration locale du serveur, rangée dans le dossier `/mnt/calaos/config`.

{{% notice info %}}
Cet outil ne concerne **que les réglages du serveur** : identifiants, notifications, options de fonctionnement. Vos entrées/sorties, vos règles et vos scénarios se gèrent depuis **Calaos Installer**, pas ici.
{{% /notice %}}

## Utilisation

Connectez-vous au serveur en SSH (voir [Accès SSH]({{% relref "calaos_os/advanced/shell" %}})), puis utilisez l'une des quatre commandes suivantes.

| Commande | Effet |
|---|---|
| `calaos_config list` | Affiche tous les réglages et leur valeur |
| `calaos_config get CLÉ` | Affiche la valeur d'un réglage |
| `calaos_config set CLÉ VALEUR` | Modifie un réglage |
| `calaos_config del CLÉ` | Supprime un réglage |

Chaque réglage est identifié par une **clé**, c'est-à-dire un nom court sans espace.

## Voir tous les réglages

La commande la plus utile pour commencer :

```sh
calaos_config list
```

Elle affiche l'intégralité des réglages actuels de votre serveur, un par ligne, sous la forme `clé: valeur`. C'est le meilleur moyen de découvrir ce qui est configuré sur votre installation.

Cette commande **ne modifie rien**, vous pouvez la lancer sans crainte.

## Lire un réglage précis

```sh
calaos_config get cn_user
```

affiche la valeur du réglage `cn_user`, c'est-à-dire le nom d'utilisateur de connexion au serveur.

## Modifier un réglage

```sh
calaos_config set cn_user mon_utilisateur
```

Le changement est enregistré immédiatement. Certains réglages ne sont pris en compte qu'après un redémarrage du serveur Calaos.

## Supprimer un réglage

```sh
calaos_config del NOM_DU_RÉGLAGE
```

Le réglage disparaît de la configuration, et le serveur reprend son comportement par défaut pour cette option.

## Quelques clés utiles

| Clé | Rôle | Page concernée |
|---|---|---|
| `cn_user` | Nom d'utilisateur de connexion au serveur | [Changer les mots de passe]({{% relref "calaos_os/security" %}}) |
| `cn_pass` | Mot de passe de connexion au serveur | [Changer les mots de passe]({{% relref "calaos_os/security" %}}) |
| `hwid` | Identifiant matériel de la machine, en lecture seule | — |

Les autres clés disponibles dépendent des fonctions que vous utilisez ; `calaos_config list` vous montre celles qui sont définies chez vous.

{{% notice warning %}}
**N'éditez pas les fichiers de configuration à la main** pendant que le serveur tourne. Vos modifications risquent d'être écrasées, et un fichier mal formé empêchera le serveur de démarrer. Passez toujours par `calaos_config`.
{{% /notice %}}

## Pour aller plus loin

Les réglages sont stockés dans le fichier `local_config.xml`, dans `/mnt/calaos/config`. Ce dossier contient aussi l'historique des configurations envoyées depuis Calaos Installer, décrit dans [Sauvegarder sa configuration]({{% relref "calaos_os/backup" %}}).

Un appel à `calaos_config` sans argument affiche l'aide de l'outil.
