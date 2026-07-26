+++
title = 'Changer les mots de passe'
date = 2026-07-25T10:00:00Z
weight = 15
summary = "À faire juste après l'installation : remplacer les deux mots de passe par défaut."
+++

## Pourquoi c'est important

Calaos OS sort de l'installation avec **deux comptes protégés par des mots de passe par défaut**. Ces mots de passe sont les mêmes sur toutes les installations, et ils sont écrits dans cette page : n'importe qui peut les connaître.

Tant que vous ne les avez pas changés, toute personne ayant accès à votre réseau peut prendre la main sur votre domotique. C'est l'affaire de quelques minutes, et c'est à faire **maintenant**, juste après l'installation.

## Les deux comptes

| Compte | À quoi il sert | Valeur par défaut |
|---|---|---|
| `root` | Administrer le système : mises à jour, configuration, dépannage | utilisateur `root`, mot de passe `calaos` |
| Calaos Server | Se connecter au serveur depuis Calaos Installer, les applications mobiles et la web app | utilisateur `user`, mot de passe `pass` |

Ce sont bien **deux comptes différents**, avec deux usages différents. Changez les deux.

## Se connecter au serveur

Les deux changements se font depuis le serveur, dans une seule session. Si vous avez un écran et un clavier branchés sur la machine, vous pouvez travailler directement dessus. Sinon, connectez-vous à distance depuis votre ordinateur habituel.

Ouvrez un terminal — l'application **Terminal** sur macOS et Linux, **PowerShell** ou **Windows Terminal** sur Windows — et tapez :

```sh
ssh root@ADRESSE_DE_VOTRE_SERVEUR
```

en remplaçant `ADRESSE_DE_VOTRE_SERVEUR` par l'adresse IP de votre serveur Calaos. Le mot de passe demandé est `calaos`.

{{% notice tip %}}
Lors de la toute première connexion, votre ordinateur vous demande de confirmer qu'il ne connaît pas encore cette machine. Répondez `yes`. Et ne soyez pas surpris : quand vous tapez un mot de passe dans un terminal, **rien ne s'affiche**, pas même des étoiles. C'est normal, tapez et validez.
{{% /notice %}}

Plus de détails sur cette connexion à distance dans [Accès SSH]({{% relref "calaos_os/advanced/shell" %}}).

## 1. Changer le mot de passe `root`

Une fois connecté, tapez :

```sh
passwd
```

Le système vous demande le nouveau mot de passe, puis vous le fait confirmer une seconde fois. Là encore, rien ne s'affiche pendant la saisie.

Choisissez un mot de passe long, que vous ne réutilisez pas ailleurs, et **notez-le** : il vous servira à chaque mise à jour et à chaque intervention sur le serveur.

## 2. Changer les identifiants de Calaos Server

Ce sont ceux que réclament Calaos Installer et les applications mobiles quand elles se connectent à votre serveur.

### Depuis l'écran tactile

Si votre serveur a un écran tactile, l'interface **Calaos Home** permet de les modifier directement dans son écran de configuration.

### Sans écran tactile

Dans la même session que tout à l'heure, tapez ces deux commandes en remplaçant les valeurs par les vôtres :

```sh
calaos_config set cn_user mon_utilisateur
calaos_config set cn_pass mon_mot_de_passe
```

Vous pouvez vérifier le résultat avec :

```sh
calaos_config get cn_user
```

L'outil `calaos_config` est présenté en détail sur sa [page dédiée]({{% relref "calaos_os/configuration/calaos_config" %}}).

{{% notice warning %}}
Après ce changement, **toutes vos applications vont refuser de se connecter** tant que vous ne leur aurez pas donné les nouveaux identifiants : Calaos Installer, les applications mobiles, la web app. Pensez à les mettre à jour dans la foulée, sinon vous croirez à une panne.
{{% /notice %}}

## Si vous oubliez un mot de passe

**Les identifiants de Calaos Server** peuvent être redéfinis depuis le serveur avec les commandes `calaos_config set` ci-dessus : il suffit d'avoir accès au compte `root`.

**Le mot de passe `root`**, lui, n'est récupérable qu'avec un accès physique à la machine, en démarrant sur la clé USB de Calaos OS. Notez-le dans votre gestionnaire de mots de passe plutôt que de compter sur votre mémoire.

## Pour aller plus loin

Pour un serveur exposé à Internet, remplacez le mot de passe `root` par une **authentification par clé SSH**, nettement plus sûre, puis désactivez la connexion par mot de passe. La marche à suivre est décrite dans [Accès SSH]({{% relref "calaos_os/advanced/shell" %}}).

Si vous accédez à votre domotique depuis l'extérieur, lisez aussi [DNS dynamique]({{% relref "calaos_os/configuration/calaos_ddns" %}}) avant d'ouvrir des ports sur votre box.
