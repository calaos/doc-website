+++
title = 'Accès SSH'
date = 2026-07-25T10:00:00Z
weight = 20
summary = "Se connecter au serveur à distance et commandes utiles."
+++

## Se connecter

SSH permet de prendre la main sur votre serveur depuis un autre ordinateur du réseau. C'est la façon normale d'administrer Calaos OS.

Ouvrez un terminal — l'application **Terminal** sur macOS et Linux, **PowerShell** ou **Windows Terminal** sur Windows — puis :

```sh
ssh root@ADRESSE_DE_VOTRE_SERVEUR
```

Le mot de passe est celui que vous avez défini dans [Changer les mots de passe]({{% relref "calaos_os/security" %}}). Par défaut il vaut `calaos`, ce qui n'est acceptable que le temps de le changer.

{{% notice tip %}}
À la première connexion, votre ordinateur vous demande de confirmer l'empreinte de la machine : répondez `yes`. Et pendant la saisie du mot de passe, **rien ne s'affiche** — c'est normal.
{{% /notice %}}

Pour vous déconnecter :

```sh
exit
```

## Authentification par clé

Plus sûre et plus pratique que le mot de passe, elle est recommandée dès que votre serveur est accessible depuis l'extérieur.

Sur votre ordinateur, si vous n'avez pas encore de clé :

```sh
ssh-keygen -t ed25519
```

Puis installez-la sur le serveur :

```sh
ssh-copy-id root@ADRESSE_DE_VOTRE_SERVEUR
```

Les connexions suivantes ne demandent plus de mot de passe.

Une fois la clé validée, vous pouvez interdire la connexion par mot de passe en ajoutant `PasswordAuthentication no` dans `/etc/ssh/sshd_config`, puis en redémarrant le service :

```sh
systemctl restart sshd
```

{{% notice warning %}}
Vérifiez que votre clé fonctionne **avant** de désactiver le mot de passe, en ouvrant une seconde session sans fermer la première. Sinon, une erreur de configuration vous laisserait dehors, et il faudrait un écran et un clavier branchés sur la machine pour vous rattraper.
{{% /notice %}}

## Sans réseau

Si le serveur n'est plus joignable, branchez un écran et un clavier directement dessus : vous obtenez un terminal local, où toutes les commandes de cette documentation fonctionnent à l'identique.

## Commandes utiles

### État du système

```sh
systemctl --failed          # services en échec
systemctl status SERVICE    # état détaillé d'un service
systemctl restart SERVICE   # redémarrer un service
reboot                      # redémarrer la machine
```

### Journaux

```sh
journalctl -f               # suivre les journaux en direct
journalctl -u SERVICE       # journaux d'un service
journalctl -b               # journaux du dernier démarrage
```

Voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}).

### Containers

Les services en containers se pilotent avec `systemctl`, comme tous les autres. Podman n'offre qu'une vue complémentaire, pratique pour du diagnostic :

```sh
podman ps                   # containers en cours d'exécution
podman images               # images disponibles localement
```

Voir [Services et modules]({{% relref "calaos_os/containers" %}}).

### Disque et points de restauration

```sh
df -h /                     # espace libre
btrfs filesystem usage /    # occupation détaillée du disque
snapper list                # liste des points de restauration
```

Voir [Sous le capot]({{% relref "calaos_os/advanced/btrfs" %}}).

### Système et réseau

```sh
calaos-os list              # versions installées
calaos-os network list      # interfaces réseau
calaos_config list          # réglages du serveur Calaos
hostname                    # nom de la machine
timedatectl                 # date, heure et fuseau horaire
```

## Copier des fichiers

Pour récupérer un fichier depuis le serveur :

```sh
scp root@ADRESSE_DE_VOTRE_SERVEUR:/tmp/journal.txt .
```

Pour en envoyer un vers le serveur :

```sh
scp fichier.txt root@ADRESSE_DE_VOTRE_SERVEUR:/tmp/
```

## Bonnes pratiques

{{% notice warning %}}
**Vos modifications manuelles peuvent être écrasées par une mise à jour.** Les fichiers du système appartiennent aux paquets installés : si vous éditez un fichier de configuration à la main, la prochaine mise à jour du paquet concerné peut le remplacer.

Deux exceptions : le dossier `/mnt/calaos`, qui contient vos données et n'est jamais touché, et les réglages effectués avec `calaos_config` ou `calaos-os`, qui sont faits pour durer.
{{% /notice %}}

Rappelez-vous enfin qu'un retour en arrière restaure **tout le système, `/mnt/calaos` compris** : il réparera donc une mauvaise manipulation, mais en annulant du même coup la configuration faite depuis le point de restauration choisi. Voir [Revenir en arrière]({{% relref "calaos_os/rollback" %}}) et [Sauvegarder sa configuration]({{% relref "calaos_os/backup" %}}).
