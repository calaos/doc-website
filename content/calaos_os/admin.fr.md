+++
title = "Administrer depuis l'application web"
date = 2026-10-23T06:00:00Z
weight = 22
summary = "Installer les mises à jour et redémarrer la box depuis l'application web Calaos, sans SSH."
+++

## De quoi s'agit-il

Vous pouvez installer les mises à jour et redémarrer votre box depuis l'**application web** Calaos, dans son onglet **Admin**. Ni terminal ni SSH nécessaires.

Ces actions sont protégées par un **mot de passe admin**, distinct du mot de passe que vous utilisez pour vous connecter à Calaos. Il n'est demandé que lorsque vous ouvrez l'onglet Admin.

{{% notice info %}}
Cette fonction n'existe que sous **Calaos OS**. Voir [Docker et autres installations](#docker-et-autres-installations) plus bas.
{{% /notice %}}

## Ouvrir l'application web en https

L'onglet Admin ne fonctionne que sur une connexion sécurisée. Ouvrez l'application web avec une adresse qui commence par **`https://`**, par exemple `https://192.168.1.50` (l'adresse IP de votre box), et non `http://`.

Votre navigateur affichera probablement un avertissement du type « Votre connexion n'est pas privée ». C'est normal : la box crée elle-même son certificat de sécurité, qu'aucune autorité publique n'a signé. Les échanges sont chiffrés malgré tout. Acceptez l'avertissement une fois (en général sous « Avancé », puis « Continuer vers ... »).

Si vous ouvrez l'onglet Admin sur une adresse qui ne commence pas par `https://`, l'application web vous demande d'utiliser `https://` et propose un bouton **Ouvrir en https://**.

## Première utilisation : choisir le mot de passe admin

1. Ouvrez l'onglet **Admin**.
2. Si l'onglet affiche **Changez d'abord votre mot de passe de connexion**, remplissez le formulaire qui s'y trouve (nom d'utilisateur, nouveau mot de passe deux fois) et cliquez sur **Enregistrer et continuer**. Une box qui utilise encore l'identifiant par défaut `user` / `pass` ne peut pas recevoir de mot de passe admin : ces valeurs sont connues de tous. C'est le mot de passe de Calaos Server, voir [Changer les identifiants de Calaos Server]({{% relref "calaos_os/security#2-changer-les-identifiants-de-calaos-server" %}}).
3. Saisissez un mot de passe admin d'**au moins 10 caractères**, deux fois, puis cliquez sur **Définir le mot de passe admin**.

{{% notice warning %}}
Une fois le mot de passe de connexion changé, Calaos Installer, les applications mobiles et l'application web ont toutes besoin du nouveau.
{{% /notice %}}

Le mot de passe admin n'est stocké nulle part en clair : **notez-le** dans votre gestionnaire de mots de passe. Si vous le perdez, voir [Si vous perdez le mot de passe admin](#si-vous-perdez-le-mot-de-passe-admin).

{{% notice warning %}}
Choisissez vous-même le mot de passe admin, peu après l'installation. Tant qu'il n'est pas défini, quiconque connaît votre mot de passe de connexion peut le définir le premier. Si vous constatez que quelqu'un d'autre l'a fait, la récupération ci-dessous remet les choses en ordre.
{{% /notice %}}

## Déverrouiller l'onglet Admin

Quand l'onglet Admin le demande (la première fois, puis toutes les 15 minutes, ou après un rechargement de la page), saisissez le mot de passe admin et appuyez sur **Déverrouiller**. Après plusieurs mots de passe erronés, l'application web vous fait attendre un peu (jusqu'à une minute) et indique combien de temps.

## Installer les mises à jour

1. Cliquez sur **Vérifier les mises à jour**. Cela contacte les serveurs Calaos et peut prendre jusqu'à une minute.
2. Si des mises à jour sont disponibles, cliquez sur **Installer les mises à jour**.
3. Une barre de progression montre le paquet en cours d'installation. Laissez la box allumée.

La box reste utilisable pendant la mise à jour. L'application web peut perdre la connexion un instant pendant le redémarrage du serveur, et se reconnecte d'elle-même.

Quand c'est fini, l'application web affiche **Mise à jour terminée**. Si elle affiche **Mise à jour terminée — redémarrez la box**, une partie de ce qui a été installé ne prend effet qu'après un redémarrage : cliquez sur **Redémarrer la box** dans la section **Box** de l'onglet, puis sur **Redémarrer** pour confirmer. Comptez environ une minute ; l'application web se reconnecte quand la box est revenue.

{{% notice tip %}}
Mettre à jour est aussi sûr qu'avec le terminal : un point de restauration est créé avant chaque installation. Voir [Revenir en arrière]({{% relref "calaos_os/rollback" %}}).
{{% /notice %}}

### La toute première mise à jour

Une box qui n'a pas encore l'onglet Admin se met à jour avec les outils habituels : `calaos-os upgrade`, `apt upgrade` (voir [Mise à jour]({{% relref "calaos_os/update" %}})), ou le bouton de mise à jour de l'écran tactile. Il n'y a rien d'autre à faire : une fois cette mise à jour terminée, l'onglet Admin fonctionne. Pendant un court moment, l'application web peut afficher **Activation de l'administration en cours** le temps que la box termine : elle réessaie toute seule. Si elle affiche encore **Redémarrez la box** au bout de quelques minutes, redémarrez la box vous-même, car l'onglet Admin ne peut pas le faire dans cet état : débranchez-la puis rebranchez-la, ou tapez `reboot` en SSH (voir [Services]({{% relref "calaos_os/configuration/services" %}})).

## Changer le mot de passe admin

Dans l'onglet Admin déverrouillé, section **Box**, **Mot de passe admin**, cliquez sur **Changer**. Vous restez déverrouillé avec le nouveau mot de passe.

## Si vous perdez le mot de passe admin

Connectez-vous à la box en SSH (voir [Accès SSH]({{% relref "calaos_os/advanced/shell" %}})) en `root`, et tapez :

```sh
calaos-os admin set
```

La commande demande le nouveau mot de passe admin, deux fois. Il remplace l'ancien, que vous vous en souveniez ou non. Toute personne connectée à l'onglet Admin doit se déverrouiller à nouveau.

Pour supprimer complètement le mot de passe admin, afin que l'application web propose d'en choisir un nouveau :

```sh
calaos-os admin reset
```

Après `calaos-os admin reset`, quiconque connaît votre mot de passe de connexion pourrait choisir le nouveau mot de passe admin avant vous. Changez d'abord le mot de passe de Calaos Server, puis choisissez le nouveau mot de passe admin dans l'application web.

Ces deux commandes exigent l'accès `root` à la box, c'est pourquoi aucun écran web ne peut faire la même chose.

{{% notice note %}}
Un [retour en arrière]({{% relref "calaos_os/rollback" %}}) restaure aussi le mot de passe admin (et le mot de passe de connexion) qui existaient à la date choisie.
{{% /notice %}}

## Autoriser d'autres noms : `api_allowed_hosts`

Tant que le mot de passe admin n'est pas défini, rien n'est vérifié. Une fois défini, l'application web n'est acceptée que si vous l'ouvrez avec l'une des adresses IP de la box (y compris sur le port 5454), ou avec n'importe quel nom en `https://`. Sans https, par exemple `http://calaos.local:5454`, tout autre nom, y compris `calaos.local` ou le nom d'hôte de la box, est refusé tant que vous ne l'ajoutez pas à l'option `api_allowed_hosts` : une liste de noms séparés par des virgules. Une entrée peut se terminer par un port, par exemple `calaos.example.net:8080` ; sans port, tous les ports sont acceptés.

```sh
calaos_config set api_allowed_hosts "calaos.example.net,box.local"
```

(voir [calaos_config]({{% relref "calaos_os/configuration/calaos_config" %}}).) Si possible, faites-le **avant** de choisir le mot de passe admin : l'écran de choix du mot de passe affiche la liste en vigueur pour que vous puissiez la vérifier.

{{% notice note %}}
Si vous avez modifié le fichier `haproxy.cfg` à la main, conservez la ligne `http-request set-header X-Forwarded-Proto` dans chaque frontend. Sans elle, la box ne peut pas savoir que la connexion est sécurisée.
{{% /notice %}}

## Derrière un reverse proxy externe

*Pour utilisateurs avancés.* Si une autre machine de votre réseau se place devant la box (HAProxy, nginx...), elle doit rediriger vers le HAProxy de la box sur le port **443 en TLS**, et non vers le port 5454 en http simple : avec 5454, l'application web affiche « Use https:// », car le mot de passe admin circulerait sinon en clair sur votre réseau. Votre navigateur ne voit que le certificat du proxy externe ; le certificat de la box n'est vérifié que par ce proxy. L'en-tête `Host` d'origine doit être transmis tel quel, donc `api_allowed_hosts` n'est pas nécessaire.

Si la box a un certificat Let's Encrypt valide via [calaos_ddns]({{% relref "calaos_os/configuration/calaos_ddns" %}}) (par exemple `mabox.calaos.fr`), vérifiez-le avec les certificats du système et le nom DDNS de la box. Exemple HAProxy :

```
backend calaos
    server calaos-server <ip-box>:443 ssl sni str(mabox.calaos.fr) verify required ca-file ca-certificates.crt verifyhost mabox.calaos.fr check check-sni mabox.calaos.fr
```

Ici `ca-file ca-certificates.crt` suppose `ca-base /etc/ssl/certs` dans la section `global` ; `@system-ca` ne fonctionne pas dans ce cas. Sans DDNS, la box a un certificat auto-signé : remplacez les options `ssl ...` par `ssl verify none` (ou donnez le certificat de la box à `ca-file` pour l'épingler).

L'équivalent nginx :

```
location / {
    proxy_pass https://<ip-box>:443;
    proxy_ssl_server_name on;
    proxy_ssl_name mabox.calaos.fr;
    proxy_ssl_verify on;
    proxy_ssl_trusted_certificate /etc/ssl/certs/ca-certificates.crt;
    proxy_set_header Host $host;
    proxy_http_version 1.1;
    proxy_set_header Upgrade $http_upgrade;
    proxy_set_header Connection "upgrade";
}
```

(avec un certificat auto-signé, utilisez `proxy_ssl_verify off;` et supprimez les deux autres lignes `proxy_ssl_` du nom et de l'AC.)

{{% notice note %}}
Si la box n'arrive pas à renouveler son certificat Let's Encrypt, elle retombe sur un certificat auto-signé et le proxy refuse la connexion. C'est voulu : pas de dégradation silencieuse de la vérification.
{{% /notice %}}

## Docker et autres installations

Il n'y a **pas d'administration web** sur les installations Docker ni sur les systèmes autres que Calaos OS. L'onglet Admin affiche **Administration indisponible**, et rien d'autre ne change : tout le reste fonctionne comme avant. Mettez ces installations à jour avec leurs outils habituels (pour Docker, récupérez les nouvelles images).

## Calaos Installer

Si votre box a un mot de passe admin, **Charger le projet** et **Envoyer le projet** (menu **Calaos Server**) dans Calaos Installer le demandent aussi : un champ facultatif **Mot de passe admin** apparaît dans les deux fenêtres. Laissez-le vide si la box n'en a pas. Il n'est jamais enregistré : saisissez-le à chaque ouverture de la fenêtre. Voir [Calaos Installer]({{% relref "calaos_installer" %}}).
