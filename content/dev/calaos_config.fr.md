+++
title = 'Calaos Config'
date = 2024-03-06T20:18:16Z
weight = 60
summary = "Les fichiers de configuration : format, emplacement et cycle de vie."
+++

## Trois fichiers

Toute la configuration d'une installation tient dans trois fichiers XML, rangés dans `/mnt/calaos/config` :

| Fichier | Contenu |
|---|---|
| `io.xml` | Les entrées/sorties, organisées par pièce |
| `rules.xml` | Les règles et les scénarios |
| `local_config.xml` | Les réglages du serveur et de l'interface locale |

Les deux premiers décrivent **votre maison** et sont produits par Calaos Installer. Le troisième contient les **réglages de la machine** et se manipule avec l'outil `calaos_config`.

## io.xml et rules.xml

Ces fichiers sont écrits par Calaos Installer, puis envoyés au serveur qui les recharge. Ce sont eux que vous éditez indirectement chaque fois que vous ajoutez un équipement ou une règle.

Côté serveur, `CalaosConfig` les charge au démarrage : `IOFactory` instancie une classe pour chaque IO déclarée, à partir de son attribut de type, et le moteur de règles construit ses conditions et ses actions à partir de `rules.xml`.

C'est pourquoi ajouter un nouveau type d'IO au serveur suffit à le rendre déclarable dans ces fichiers — voir [Calaos Server]({{% relref "dev/calaos_server" %}}).

{{% notice warning %}}
**N'éditez pas ces fichiers à la main pendant que le serveur tourne.** Le prochain envoi depuis Calaos Installer écrasera vos modifications, et un fichier mal formé empêchera le serveur de démarrer.
{{% /notice %}}

## local_config.xml

Ce fichier stocke des couples clé/valeur, sans structure imposée. Il est lu et écrit :

- par **`calaos_server`**, via `Utils::get_config_option()` et `set_config_option()` ;
- par **l'outil `calaos_config`**, qui n'est qu'une interface en ligne de commande sur ces mêmes fonctions ;
- par **Calaos Home**, qui y range ses propres réglages d'affichage ;
- par **`calaos_ddns`**, qui y conserve son jeton.

La liste des clés est décrite dans [calaos_config]({{% relref "calaos_os/configuration/calaos_config" %}}).

L'implémentation de l'outil tient en une centaine de lignes, dans `src/bin/tools/calaos_config.cpp`. Il expose quatre actions — `list`, `get`, `set`, `del` — et appelle `sync()` après écriture pour garantir que la valeur atteint le disque.

{{% notice tip %}}
Cette simplicité est délibérée : **ajouter un réglage ne demande aucune modification de l'outil**. Il suffit de lire la nouvelle clé là où on en a besoin, et `calaos_config set` sait déjà l'écrire.
{{% /notice %}}

## Historique des configurations

À chaque envoi d'une nouvelle configuration depuis Calaos Installer, le serveur archive la précédente avant de la remplacer, dans une arborescence datée :

```text
/mnt/calaos/config/backups/2026/07/01-07-2026_16-56-00/
├── io.xml
├── local_config.xml
└── rules.xml
```

Ce mécanisme permet de revenir à une version antérieure. Il ne constitue pas une sauvegarde pour autant, puisqu'il vit sur la même machine et dans le même dossier — voir [Sauvegarder sa configuration]({{% relref "calaos_os/backup" %}}).

## Identifiants

Les identifiants de connexion au serveur sont stockés sous les clés `cn_user` et `cn_pass`. Les clés historiques `calaos_user` et `calaos_password` sont encore lues en secours, et supprimées dès que les identifiants sont modifiés — voir `JsonApi::changeCredentials`.

Le changement peut se faire de trois façons : depuis l'interface Calaos Home, par l'API avec `change_cred`, ou en ligne de commande avec `calaos_config set`. Voir [Changer les mots de passe]({{% relref "calaos_os/security" %}}).

## Emplacement

Tout vit sous `/mnt/calaos`, aux côtés des données des services complémentaires. Ce dossier est **le seul à sauvegarder**, et il n'est jamais touché par une mise à jour du système.

Il fait en revanche partie du sous-volume racine, donc d'un point de restauration : un retour en arrière rejoue aussi la configuration. Voir [Sous le capot]({{% relref "calaos_os/advanced/btrfs" %}}).
