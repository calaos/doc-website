+++
title = 'Revenir en arrière'
date = 2024-03-06T20:14:40Z
weight = 40
summary = "Remettre le serveur dans un état antérieur qui fonctionnait."
+++

## À quoi ça sert

Vous êtes au bon endroit si :

- une mise à jour s'est mal passée ;
- le serveur ne démarre plus ;
- quelque chose ne fonctionne plus depuis une intervention récente.

Revenir en arrière remet votre serveur dans l'état où il était à une date antérieure, celle que vous choisissez.

## Comment ça marche

Calaos OS enregistre automatiquement l'état du système : avant chaque installation ou mise à jour de logiciel, et régulièrement au fil du temps. Ces enregistrements s'appellent des **points de restauration**, et ils sont datés.

Vous pouvez demander à l'ordinateur de redémarrer sur l'un d'eux. Il retrouve alors le système exactement tel qu'il était ce jour-là.

{{% notice warning %}}
**Revenir en arrière restaure aussi votre configuration Calaos.**

Vos entrées/sorties, vos règles et vos scénarios reviennent eux aussi à la date choisie. Tout ce que vous avez créé depuis sera perdu, y compris l'historique des configurations enregistré par le serveur.

Un exemple : vous mettez à jour lundi, vous créez des règles mardi, vous revenez en arrière mercredi jusqu'à lundi — le travail de mardi disparaît.

**Récupérez donc votre configuration avant de confirmer.** L'étape 3 ci-dessous vous laisse tout le temps de le faire.
{{% /notice %}}

## 1. Choisir une date au démarrage

Redémarrez l'ordinateur. Juste après l'allumage, un menu s'affiche pendant cinq secondes.

Utilisez les flèches du clavier pour entrer dans le sous-menu qui liste les points de restauration, puis choisissez la date qui vous convient — en général la plus récente antérieure au problème. Validez avec la touche Entrée.

{{% notice tip %}}
Si le menu disparaît trop vite, redémarrez et appuyez plusieurs fois sur la flèche du haut dès l'allumage : cela arrête le compte à rebours et vous laisse tout le temps de choisir.
{{% /notice %}}

Le système démarre alors sur le point de restauration choisi.

## 2. Rien n'est encore définitif

À ce stade, le système fonctionne **en mode essai** : il vous montre à quoi ressemblerait votre serveur si vous reveniez à cette date, mais rien n'est encore écrit.

{{% notice info %}}
**Si vous changez d'avis, redémarrez simplement l'ordinateur.** Il repartira sur le système actuel, comme si de rien n'était.
{{% /notice %}}

C'est le moment de vérifier que ce point de restauration est le bon : votre domotique répond-elle correctement ? Le problème a-t-il disparu ? Si ce n'est pas le cas, redémarrez et choisissez une date antérieure.

## 3. Récupérer votre configuration

Puisque le retour en arrière restaurera aussi votre configuration Calaos, mettez-la de côté maintenant.

Le plus simple est de la rapatrier sur votre ordinateur avec **Calaos Installer** : menu **Calaos Server**, option **Charger le projet depuis Calaos Server**, puis enregistrez le projet chez vous. Le détail est décrit dans [Sauvegarder sa configuration]({{% relref "calaos_os/backup" %}}).

## 4. Confirmer

### Avec un écran tactile

Quand vous avez démarré sur un point de restauration, l'interface **Calaos Home** affiche automatiquement une fenêtre qui vous explique la situation et vous propose un bouton pour confirmer le retour en arrière. Vous n'avez rien à chercher : cliquez sur ce bouton.

### Sans écran tactile

La fenêtre ci-dessus n'apparaît que si un écran tactile est branché. Sinon, connectez-vous au serveur en SSH — cela fonctionne normalement, même démarré sur un point de restauration :

```sh
ssh root@ADRESSE_DE_VOTRE_SERVEUR
```

puis lancez :

```sh
calaos_rollback
```

{{% notice note %}}
Si la commande répond `Not booted from a readonly btrfs snapshot`, c'est que l'ordinateur n'a pas démarré sur un point de restauration mais sur le système habituel. Reprenez à l'étape 1.
{{% /notice %}}

## 5. Redémarrer

Redémarrez une dernière fois :

```sh
reboot
```

Votre serveur repart sur l'état restauré. C'est terminé.

## Ce qui n'est pas restauré

**Les journaux du système sont conservés.** Ils ne font pas partie des points de restauration : vous pouvez donc toujours consulter après coup ce qui s'est passé au moment de l'incident. Voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}).

## Si rien ne fonctionne

Si aucun point de restauration ne redonne un serveur en état de marche, il reste la réinstallation complète : refaites une clé USB, réinstallez Calaos OS en suivant [Installation]({{% relref "calaos_os/installation" %}}), puis renvoyez votre configuration depuis Calaos Installer.

## Pour aller plus loin

Les points de restauration reposent sur les instantanés Btrfs, gérés par snapper. Pour les lister depuis un terminal :

```sh
snapper list
```

Le détail du mécanisme, de la rétention automatique et de ce qui est couvert ou non est décrit dans [Sous le capot]({{% relref "calaos_os/advanced/btrfs" %}}).
