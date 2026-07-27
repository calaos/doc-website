+++
title = 'Calaos DDNS'
date = 2024-03-06T20:18:21Z
weight = 50
summary = "Le client de DNS dynamique, les certificats et l'aiguillage HAProxy."
+++

## Rôle

Calaos DDNS résout un problème d'accès distant : l'adresse publique d'une connexion domestique change régulièrement, ce qui casse toute configuration reposant sur une adresse fixe.

Le client tourne sur le serveur, déclare un nom en `.calaos.fr` auprès du service Calaos, et le maintient à jour. Il obtient au passage un **certificat Let's Encrypt** et génère la configuration **HAProxy** qui aiguille les connexions entrantes.

| Composant | Dépôt | Rôle |
|---|---|---|
| Client | [calaos_ddns](https://github.com/calaos/calaos_ddns) | Tourne sur le serveur de l'utilisateur |
| Service | [calaos_dns](https://github.com/calaos/calaos_dns) | Hébergé par Calaos, tient la zone DNS |

Les deux sont écrits en **Go**. Le service de référence est joignable sur `https://ns1.calaos.fr/`.

{{% notice info %}}
Pour l'usage courant — enregistrer un domaine, ouvrir un port, diagnostiquer — voyez [DNS dynamique]({{% relref "calaos_os/configuration/calaos_ddns" %}}).
{{% /notice %}}

## Commandes

| Commande | Effet |
|---|---|
| `calaos_ddns register DOMAINE [SOUS-DOMAINE...]` | Enregistre un nom et demande un certificat |
| `calaos_ddns update [-f]` | Met à jour l'adresse ; `-f` force le renouvellement du certificat |
| `calaos_ddns unregister` | Supprime l'enregistrement auprès du service |
| `calaos_ddns reset` | Efface le jeton local |

## Le jeton

Toute la relation avec le service repose sur un **jeton** obtenu à l'enregistrement, puis stocké dans la configuration du serveur sous la clé `ddns_token`. L'adresse email utilisée pour Let's Encrypt est conservée sous `ddns_le_email`.

Cela explique un comportement qui peut dérouter : `unregister` commence par présenter le jeton au service pour supprimer l'enregistrement, et **ne nettoie la configuration locale que si le service accepte**. Si le jeton n'est plus valide, la commande échoue sans rien effacer, et l'on se retrouve bloqué.

C'est à cela que sert `reset`, qui se contente de supprimer la clé localement — l'équivalent exact de :

```sh
calaos_config del ddns_token
```

## Sous-domaines et aiguillage

L'argument d'un sous-domaine accepte deux formes :

```sh
calaos_ddns register mamaison camera1
calaos_ddns register mamaison camera1=192.168.0.20:4444
```

La première crée simplement `camera1.mamaison.calaos.fr`. La seconde y ajoute une **entrée HAProxy** dirigeant les connexions vers l'équipement indiqué sur le réseau local.

La configuration HAProxy est générée dans `/mnt/calaos/haproxy`, et le certificat écrit dans `server.pem`. Ces chemins sont fournis au client par des variables d'environnement définies dans son unité systemd :

| Variable | Valeur |
|---|---|
| `CALAOS_CONFIG` | `/mnt/calaos/config` |
| `CALAOS_HAPROXY_PATH` | `/mnt/calaos/haproxy` |
| `CALAOS_CERT_FILE` | `/mnt/calaos/haproxy/server.pem` |
| `CALAOS_HAPROXY_TEMPLATE_PATH` | `/usr/share/calaos-ddns` |
| `CALAOSDNS_CACHE_DIR` | `/mnt/calaos/calaos-ddns` |

Tout étant sous `/mnt/calaos`, la configuration et le certificat font partie des données de l'utilisateur : ils survivent aux mises à jour et sont couverts par une sauvegarde du dossier.

## Mise à jour périodique

Le client n'est pas un service permanent : c'est une tâche ponctuelle, déclenchée par un timer systemd.

```ini
[Timer]
OnBootSec=1min
OnUnitActiveSec=2h
AccuracySec=30min
```

Soit une première vérification une minute après le démarrage, puis toutes les deux heures. À chaque passage, le client compare l'adresse publique à celle enregistrée, renouvelle le certificat s'il approche de l'expiration, et redémarre HAProxy quand la configuration a changé.

Pour observer son fonctionnement :

```sh
systemctl status calaos-ddns.timer
journalctl -u calaos-ddns.service
```
