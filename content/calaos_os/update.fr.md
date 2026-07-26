+++
title = 'Mise à jour'
date = 2024-03-06T20:14:30Z
weight = 20
summary = "Mettre à jour le système et les services, sans risque."
+++

## Mettre à jour est sans risque

Avant chaque installation de logiciel, Calaos OS enregistre automatiquement l'état complet du système. Si une mise à jour se passe mal, vous pouvez redémarrer sur l'état d'avant et retrouver un serveur qui fonctionne.

Vous n'avez rien à faire pour que ces points de restauration existent : ils sont créés tout seuls. Voir [Revenir en arrière]({{% relref "calaos_os/rollback" %}}).

Les mises à jour de Calaos OS sont **incrémentales** : seuls les composants qui ont changé sont téléchargés et remplacés. Il n'y a jamais besoin de refaire une clé USB ni de réinstaller.

{{% notice info %}}
**Votre configuration n'est pas touchée par une mise à jour.** Vos entrées/sorties, vos règles, vos scénarios et l'historique de vos mesures sont conservés tels quels.
{{% /notice %}}

## Avant de commencer

Faites une sauvegarde de votre configuration : c'est rapide et cela vous évitera bien des ennuis un jour. La marche à suivre est sur [Sauvegarder sa configuration]({{% relref "calaos_os/backup" %}}).

Prévoyez aussi un moment où l'interruption ne gênera personne. Pendant la mise à jour, votre domotique peut cesser de répondre quelques instants, et un redémarrage est parfois nécessaire à la fin.

## Se connecter au serveur

La mise à jour se lance depuis le serveur. Le plus simple est de s'y connecter à distance depuis votre ordinateur habituel.

Ouvrez un terminal — l'application **Terminal** sur macOS et Linux, **PowerShell** ou **Windows Terminal** sur Windows — et tapez :

```sh
ssh root@ADRESSE_DE_VOTRE_SERVEUR
```

en remplaçant `ADRESSE_DE_VOTRE_SERVEUR` par l'adresse IP de votre serveur. Le mot de passe est celui que vous avez défini dans [Changer les mots de passe]({{% relref "calaos_os/security" %}}).

{{% notice tip %}}
Vous pouvez aussi brancher un clavier et un écran directement sur le serveur et taper les commandes dessus. C'est moins pratique, mais cela fonctionne exactement pareil.
{{% /notice %}}

## 1. Voir ce qui est disponible

Tapez :

```sh
calaos-os check-update
```

Cette commande **ne modifie rien** : elle se contente d'aller voir si de nouvelles versions existent et de vous les afficher. Vous pouvez la lancer aussi souvent que vous voulez, sans aucun risque.

Si rien n'est disponible, votre serveur est à jour, vous n'avez rien d'autre à faire.

## 2. Lancer la mise à jour

Tapez :

```sh
calaos-os upgrade
```

Le téléchargement démarre, puis l'installation. Beaucoup de lignes défilent à l'écran : c'est normal, c'est le détail de ce qui est installé. Vous n'avez pas à les lire.

**Ne fermez pas la fenêtre et ne coupez pas l'alimentation** tant que la commande n'est pas terminée. Selon le nombre de composants à mettre à jour et la vitesse de votre connexion, comptez de quelques minutes à une vingtaine de minutes.

C'est terminé quand le défilement s'arrête et que le terminal vous rend la main, en réaffichant une ligne d'invite qui attend votre prochaine commande.

## 3. Vérifier que tout va bien

Reprenez l'usage normal de votre installation : allumez une lumière depuis l'application mobile, vérifiez qu'un scénario se déclenche. Si tout répond comme avant, la mise à jour est réussie.

Si le serveur a besoin d'un redémarrage, redémarrez-le :

```sh
reboot
```

## Si quelque chose ne va pas

Ne réinstallez rien, ne paniquez pas : c'est exactement la situation pour laquelle les points de restauration existent. Rendez-vous sur [Revenir en arrière]({{% relref "calaos_os/rollback" %}}), qui vous ramène en quelques minutes à l'état d'avant la mise à jour.

Si le serveur démarre encore mais qu'un service précis ne fonctionne plus, la page [En cas de problème]({{% relref "calaos_os/troubleshooting" %}}) vous aidera à identifier lequel.

## Pour aller plus loin

**Voir ce qui est installé**, versions comprises :

```sh
calaos-os list
```

**Mettre à jour un seul composant**, plutôt que la totalité :

```sh
calaos-os upgrade NOM_DU_PAQUET
```

**Passer par les outils Debian.** Calaos OS étant basé sur Debian, l'outil `apt` fonctionne également et agit sur le même dépôt de paquets :

```sh
apt update
apt full-upgrade
```

Le résultat est le même, et les points de restauration sont créés dans les deux cas.

L'ensemble des commandes de l'outil est détaillé dans [L'outil calaos-os]({{% relref "calaos_os/advanced/cli" %}}).
