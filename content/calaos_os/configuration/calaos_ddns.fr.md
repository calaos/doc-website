+++
title = 'DNS Dynamique'
date = 2024-03-06T20:16:09Z
weight = 80
summary = "Accéder à votre domotique depuis l'extérieur, avec une adresse en calaos.fr."
+++

## Le problème que ça résout

Chez vous, vos applications trouvent le serveur Calaos sur le réseau local. Depuis l'extérieur — au travail, en vacances — il faut passer par votre connexion Internet, et donc connaître l'adresse publique de votre box.

Le souci est que cette adresse **change régulièrement**, sans prévenir : la plupart des fournisseurs d'accès ne garantissent pas une adresse fixe aux particuliers. Une application configurée avec l'ancienne adresse ne trouve plus rien.

Calaos fournit un service de **DNS dynamique** qui résout cela : vous obtenez une adresse fixe de la forme `mamaison.calaos.fr`, et votre serveur signale tout seul sa nouvelle adresse au service dès qu'elle change. Vous configurez vos applications une fois pour toutes avec ce nom.

En prime, un **certificat de sécurité Let's Encrypt** est obtenu automatiquement, ce qui vous donne une connexion chiffrée depuis l'extérieur.

## Avant de commencer

**Votre box doit rediriger les connexions vers votre serveur.** Par défaut, une box bloque tout ce qui vient de l'extérieur. Il faut lui demander d'envoyer les connexions entrantes vers votre serveur Calaos : c'est ce qu'on appelle une **redirection de port**, à configurer dans l'interface de votre box.

**Votre serveur doit avoir une adresse fixe sur le réseau local**, sinon la redirection pointera un jour vers un autre appareil. Voir [Réseau]({{% relref "calaos_os/configuration/network" %}}).

{{% notice warning %}}
Ouvrir un accès depuis Internet rend votre domotique **joignable par n'importe qui**, pas seulement par vous.

Avant de le faire, assurez-vous d'avoir changé les mots de passe par défaut — voir [Changer les mots de passe]({{% relref "calaos_os/security" %}}) — et choisissez un mot de passe long, que vous n'utilisez nulle part ailleurs. C'est le minimum absolu.
{{% /notice %}}

## Enregistrer votre nom de domaine

Connectez-vous au serveur en SSH (voir [Accès SSH]({{% relref "calaos_os/advanced/shell" %}})), puis choisissez le nom que vous voulez utiliser :

```sh
calaos_ddns register mamaison
```

Cette commande enregistre **`mamaison.calaos.fr`** et l'associe à l'adresse publique de votre connexion.

Choisissez un nom qui vous identifie mais qui reste discret : il sera visible de l'extérieur, autant éviter votre nom de famille ou votre adresse.

### Le certificat de sécurité

Lors du premier enregistrement, l'outil vous demande **une adresse email**. Elle sert uniquement à obtenir le certificat Let's Encrypt, qui chiffre les échanges entre vos applications et votre serveur.

```text
To generate a Let's Encrypt certificate, you need to set a user email address.
Enter your email address:
```

Saisissez une adresse valide et validez. La génération du certificat prend quelques instants.

{{% notice note %}}
Si vous laissez le champ vide, l'enregistrement fonctionne quand même, mais **aucun certificat n'est généré** et votre connexion depuis l'extérieur ne sera pas chiffrée. C'est déconseillé.
{{% /notice %}}

## Ajouter des sous-domaines

Vous pouvez enregistrer des noms supplémentaires sous votre domaine principal, par exemple pour joindre une caméra :

```sh
calaos_ddns register mamaison camera1
```

crée `camera1.mamaison.calaos.fr` en plus de `mamaison.calaos.fr`.

Mieux : en précisant l'adresse et le port de l'équipement, le serveur se charge d'aiguiller la connexion vers lui.

```sh
calaos_ddns register mamaison camera1=192.168.0.20:4444
```

Une visite de `camera1.mamaison.calaos.fr` sera alors dirigée vers l'équipement situé en `192.168.0.20:4444` sur votre réseau local. Vous pouvez indiquer plusieurs sous-domaines à la suite, séparés par des espaces.

{{% notice tip %}}
Cet aiguillage est pratique, mais rappelez-vous que **l'équipement visé devient joignable depuis Internet**. Ne l'utilisez que pour des appareils dont vous maîtrisez le mot de passe, et jamais pour une caméra restée sur ses identifiants d'usine.
{{% /notice %}}

## La mise à jour est automatique

Une fois l'enregistrement fait, vous n'avez plus rien à faire : votre serveur vérifie **toutes les deux heures** que son adresse publique n'a pas changé, et prévient le service le cas échéant. Le certificat est renouvelé de la même façon.

Pour forcer une vérification immédiate, par exemple juste après une coupure de votre box :

```sh
calaos_ddns update
```

## Vérifier que ça fonctionne

Le test se fait **depuis l'extérieur de chez vous** : coupez le Wi-Fi de votre téléphone pour passer en réseau mobile, puis essayez de vous connecter à votre serveur avec votre nom en `.calaos.fr`.

{{% notice tip %}}
Beaucoup de box ne savent pas rediriger une connexion vers l'extérieur puis la ramener vers l'intérieur. Autrement dit, tester votre nom depuis chez vous peut échouer alors que tout fonctionne parfaitement depuis l'extérieur. Testez toujours depuis une connexion mobile.
{{% /notice %}}

## Les autres commandes

| Commande | Effet |
|---|---|
| `calaos_ddns register DOMAINE [SOUS-DOMAINE...]` | Enregistre un nom et obtient un certificat |
| `calaos_ddns update` | Force la mise à jour de l'adresse |
| `calaos_ddns update -f` | Force en plus le renouvellement du certificat |
| `calaos_ddns unregister` | Supprime l'enregistrement |
| `calaos_ddns reset` | Efface le jeton enregistré sur le serveur |

## Le jeton n'est plus valide

Votre serveur conserve un **jeton d'identification**, obtenu lors de l'enregistrement, qui lui permet de prouver au service que le domaine est bien le sien. Si votre serveur est resté hors ligne très longtemps, ce jeton peut devenir invalide.

Vous vous en apercevez au fait que les commandes `calaos_ddns` **retournent une erreur** au lieu de fonctionner, y compris `unregister` — puisque celui-ci commence par présenter le jeton au service, qui le refuse.

La solution est de **supprimer le vieux jeton** de la configuration de votre serveur :

```sh
calaos_config del ddns_token
```

Puis de refaire un enregistrement normal :

```sh
calaos_ddns register mamaison
```

{{% notice note %}}
`calaos_ddns reset` fait exactement la même chose que la commande `calaos_config del` ci-dessus. Si `reset` échoue lui aussi, passez directement par `calaos_config del ddns_token`, qui se contente d'effacer le réglage localement sans rien demander au service.
{{% /notice %}}

{{% notice warning %}}
Cette manipulation efface la trace locale de l'enregistrement, mais **pas l'enregistrement lui-même côté service**. Si vous reprenez le même nom de domaine, tout rentre dans l'ordre. Si vous en changez, l'ancien nom peut rester réservé quelque temps.
{{% /notice %}}

## Si ça ne marche pas

Vérifiez dans l'ordre :

- votre serveur est-il joignable normalement **depuis le réseau local** ? Si non, le problème est en amont ;
- la redirection de port est-elle bien configurée sur votre box, vers l'adresse locale actuelle du serveur ?
- l'adresse locale du serveur a-t-elle changé depuis que vous avez configuré la redirection ?
- votre fournisseur d'accès vous attribue-t-il une adresse publique partagée ? Certaines offres, notamment en fibre ou en 4G, ne permettent pas de recevoir de connexions entrantes. Dans ce cas, aucun réglage ne fonctionnera et il faut demander une adresse publique à votre fournisseur.

Pour voir ce que fait le service de mise à jour :

```sh
systemctl status calaos-ddns.timer
journalctl -u calaos-ddns.service
```

## Pour aller plus loin

Le service de mise à jour est déclenché par `calaos-ddns.timer`, une minute après le démarrage puis toutes les deux heures.

L'aiguillage des sous-domaines est assuré par HAProxy, dont la configuration est générée automatiquement dans `/mnt/calaos/haproxy`, avec le certificat dans `server.pem`. Ces fichiers font partie de vos données et sont donc conservés lors des mises à jour.

Le jeton d'identification et l'adresse email sont enregistrés dans la configuration du serveur, sous les clés `ddns_token` et `ddns_le_email` — voir [calaos_config]({{% relref "calaos_os/configuration/calaos_config" %}}).

## Voir aussi

Pour configurer vos applications mobiles avec votre accès distant, voyez [Envoyer le projet]({{% relref "getting_started/upload" %}}).
