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

Calaos OS enregistre automatiquement l'état du système : avant chaque installation ou mise à jour de logiciel, et régulièrement au fil du temps. Ces enregistrements s'appellent des **points de restauration**, et ils sont datés. L'application web les appelle des **snapshots** : c'est la même chose.

Vous pouvez demander à l'ordinateur de redémarrer sur l'un d'eux. Il retrouve alors le système exactement tel qu'il était ce jour-là.

{{% callout warning %}}
**Revenir en arrière restaure aussi votre configuration Calaos.**

Vos entrées/sorties, vos règles et vos scénarios reviennent eux aussi à la date choisie. Tout ce que vous avez créé depuis sera perdu, y compris l'historique des configurations enregistré par le serveur.

Un exemple : vous mettez à jour lundi, vous créez des règles mardi, vous revenez en arrière mercredi jusqu'à lundi — le travail de mardi disparaît.

**Récupérez donc votre configuration avant de confirmer.** L'étape 3 ci-dessous vous laisse tout le temps de le faire.
{{% /callout %}}

## 1. Choisir un point de restauration

Il y a deux façons de redémarrer sur un point de restauration. Elles mènent au même résultat.

### Depuis l'application web

Il faut un serveur qui fonctionne et le [mot de passe admin]({{% relref "calaos_os/admin" %}}).

1. Ouvrez l'application web en `https://`, puis l'onglet **Admin**, et déverrouillez-le.
2. Ouvrez la tuile **Snapshots**. Les points de restauration sont listés, du plus récent au plus ancien, avec leur date, leur type et une description. Deux pastilles vous aident : **Actuel** (celui sur lequel le système tourne) et **Par défaut** (celui d'un démarrage normal).
3. Cliquez sur **Redémarrer dessus** sur la ligne voulue, en général la plus récente antérieure au problème, puis confirmez avec **Redémarrer**.

Le serveur redémarre de lui-même sur ce point de restauration, une seule fois. Il reste hors ligne de 1 à 3 minutes, et la page se reconnecte toute seule. N'envoyez pas la demande une seconde fois si la page indique qu'elle attend le serveur.

Un point de restauration marqué **Non bootable** ne peut pas être démarré ainsi. C'est notamment le cas des plus anciens, enregistrés avant que le système sache démarrer sur un point de restauration en mode d'essai : l'application web les refuse comme trop anciens. Ils ne sont pas proposés en mode d'essai depuis l'application web, et il n'est pas garanti qu'ils démarrent correctement depuis le menu de démarrage.

### Depuis le menu de démarrage

Redémarrez l'ordinateur. Juste après l'allumage, un menu s'affiche pendant cinq secondes.

Utilisez les flèches du clavier pour entrer dans le sous-menu qui liste les points de restauration, puis choisissez la date qui vous convient — en général la plus récente antérieure au problème. Validez avec la touche Entrée.

{{% callout tip %}}
Si le menu disparaît trop vite, redémarrez et appuyez plusieurs fois sur la flèche du haut dès l'allumage : cela arrête le compte à rebours et vous laisse tout le temps de choisir.
{{% /callout %}}

Le système démarre alors sur le point de restauration choisi.

## 2. Rien n'est encore définitif

À ce stade le système tourne en **mode d'essai** (ou **mode de secours**) : il vous montre à quoi ressemblerait votre serveur si vous reveniez à cette date, mais rien n'est conservé sur le disque. Tout ce que vous modifiez dans ce mode est perdu au prochain redémarrage.

L'application web affiche sur toutes ses pages un bandeau **Mode secours** avec le numéro du point de restauration. L'écran tactile affiche la fenêtre **Système de récupération**.

{{% callout info %}}
**Si vous changez d'avis, redémarrez simplement l'ordinateur.** Il repartira sur le système actuel, comme si rien ne s'était passé. Dans l'application web, **Admin**, **Snapshots**, puis **Revenir au système normal** fait la même chose.
{{% /callout %}}

Dans le mode d'essai, **les mises à jour sont refusées** depuis l'application web et l'écran tactile tant que vous n'avez pas quitté ce mode.

C'est le moment de vérifier que ce point de restauration est le bon : votre domotique répond-elle correctement ? Le problème a-t-il disparu ? Sinon, redémarrez et choisissez une date plus ancienne.

## 3. Récupérer votre configuration

Puisque le retour en arrière restaurera aussi votre configuration Calaos, mettez-la de côté maintenant.

Le plus simple est de la rapatrier sur votre ordinateur avec **Calaos Installer** : menu **Calaos Server**, option **Charger le projet depuis Calaos Server**, puis enregistrez le projet chez vous. Le détail est décrit dans [Sauvegarder sa configuration]({{% relref "calaos_os/backup" %}}).

## 4. Confirmer

### Depuis l'application web

En mode d'essai, le bandeau **Mode secours** explique la situation. Sa fenêtre d'information (**Système de récupération**) et le panneau **Admin**, **Snapshots** proposent **Rollback : rendre permanent**. Le mot de passe admin est demandé si l'onglet Admin est verrouillé. Cochez **Je comprends que c'est définitif**, puis cliquez sur **Rollback**.

Le serveur redémarre de lui-même. Il reste hors ligne de 1 à 3 minutes, et la page se reconnecte toute seule.

### Avec un écran tactile

Quand vous avez démarré sur un point de restauration, l'interface **Calaos Home** propose une fenêtre **Système de récupération** qui vous explique la situation, avec un bouton **Retour en arrière** : cliquez dessus. L'option est aussi accessible depuis le menu **Configuration** du panneau de configuration. **Fermer** masque la fenêtre sans rien changer.

Le serveur redémarre de lui-même.

Si un message indique qu'une mise à jour ou un redémarrage est déjà en cours, patientez quelques minutes et réessayez. Si la connexion est perdue, le serveur est probablement en train de redémarrer : attendez qu'il se reconnecte avant de réessayer.

### En SSH

Si vous n'avez ni écran tactile ni application web, connectez-vous au serveur en SSH — cela fonctionne normalement, même démarré sur un point de restauration :

```sh
ssh root@ADRESSE_DE_VOTRE_SERVEUR
```

puis lancez :

```sh
calaos_rollback
```

{{% callout note %}}
Si la commande répond `Not booted from a readonly btrfs snapshot`, c'est que l'ordinateur n'a pas démarré sur un point de restauration mais sur le système habituel. Reprenez à l'étape 1.
{{% /callout %}}

## 5. Redémarrer

Avec l'application web ou l'écran tactile, le serveur redémarre de lui-même deux secondes après la confirmation : il n'y a rien à faire. En SSH, redémarrez vous-même une dernière fois :

```sh
reboot
```

Votre serveur repart sur l'état restauré. C'est terminé.

## Limites et précautions

- **Le retour en arrière est définitif.** Votre configuration depuis cette date est perdue : voir l'avertissement plus haut.
- **Une opération à la fois.** Pendant une mise à jour ou un retour en arrière, l'application web et l'écran tactile en refusent une nouvelle. Attendez la fin, qui peut prendre quelques minutes.
- **Un retour en arrière qui échoue laisse le serveur comme avant.** Si quelque chose échoue avant la fin, le système remet l'état précédent et le serveur redémarre normalement dessus ; le message affiché nomme la cause. Vous pouvez réessayer une fois le problème réglé.
- **Si un message dit _do not reboot_**, arrêtez-vous là et ne redémarrez pas : le retour en arrière n'a pas pu être annulé proprement et la réparation est manuelle, depuis un shell sur le serveur. Conservez le message affiché.
- **Sans clavier ni écran, un point de restauration qui plante peut boucler.** Sur un serveur qui démarre en BIOS classique (ou sans partition EFI), un point de restauration lancé une fois depuis l'application web qui plante très tôt peut être relancé à chaque redémarrage. GRUB attend alors 3 secondes sans afficher son menu : appuyer sur **Échap** pendant ce délai affiche le menu normal, où vous choisissez le système habituel. Un serveur sans clavier ni écran a alors besoin d'une clé USB externe.
- **L'application web liste les 200 points de restauration les plus récents.**

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
