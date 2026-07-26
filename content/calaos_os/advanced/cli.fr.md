+++
title = "L'outil calaos-os"
date = 2026-07-25T10:00:00Z
weight = 10
summary = "Référence des commandes de l'outil calaos-os."
+++

## Rôle

`calaos-os` est l'outil d'administration du système. Il gère les mises à jour et la configuration réseau, et remplace avantageusement les commandes Debian équivalentes, dont il connaît les particularités de Calaos OS.

Il s'utilise depuis un terminal sur le serveur, ou à distance en SSH — voir [Accès SSH]({{% relref "calaos_os/advanced/shell" %}}).

## Référence des commandes

| Commande | Effet |
|---|---|
| `calaos-os list` | Liste les images et paquets installés, ainsi que les mises à jour disponibles |
| `calaos-os check-update` | Vérifie si des mises à jour sont disponibles |
| `calaos-os upgrade` | Met à jour tous les composants |
| `calaos-os upgrade PAQUET` | Met à jour un seul composant |
| `calaos-os network list` | Liste les interfaces réseau |
| `calaos-os network configure dhcp INTERFACE` | Configure une interface en adresse automatique |
| `calaos-os network configure static INTERFACE IPV4 MASQUE [PASSERELLE] [DNS...]` | Configure une interface en adresse fixe |

## Mises à jour

Un cycle complet de mise à jour ressemble à ceci :

```sh
# Voir l'état actuel du système
calaos-os list

# Vérifier ce qui est disponible, sans rien modifier
calaos-os check-update

# Tout mettre à jour
calaos-os upgrade
```

Pour ne mettre à jour qu'un composant, par exemple pour tester une nouvelle version d'un service sans toucher au reste :

```sh
calaos-os upgrade NOM_DU_PAQUET
```

Un point de restauration est créé automatiquement avant chaque opération, quelle que soit la commande utilisée. Voir [Revenir en arrière]({{% relref "calaos_os/rollback" %}}).

{{% notice info %}}
Calaos OS étant basé sur Debian, `apt` fonctionne également et agit sur le même dépôt de paquets. `calaos-os upgrade` et `apt full-upgrade` produisent le même résultat.
{{% /notice %}}

La procédure complète, expliquée pas à pas, est sur la page [Mise à jour]({{% relref "calaos_os/update" %}}).

## Configuration réseau

Commencez toujours par relever le nom de l'interface :

```sh
calaos-os network list
```

Puis configurez-la, en adresse automatique :

```sh
calaos-os network configure dhcp eth0
```

ou en adresse fixe :

```sh
calaos-os network configure static eth0 192.168.1.50 255.255.255.0 192.168.1.1 192.168.1.1
```

Les arguments sont, dans l'ordre : l'interface, l'adresse IPv4, le masque de réseau, la passerelle, puis un ou plusieurs serveurs DNS séparés par des espaces. La passerelle et les DNS sont facultatifs, mais sans eux le serveur n'aura pas accès à Internet.

{{% notice warning %}}
Si vous êtes connecté en SSH et que vous modifiez l'interface par laquelle vous êtes connecté, **votre session sera coupée**. Reconnectez-vous à la nouvelle adresse.
{{% /notice %}}

Les explications détaillées sont sur la page [Réseau]({{% relref "calaos_os/configuration/network" %}}).

## À ne pas confondre

Deux outils portent un nom voisin et n'ont rien à voir :

- **`calaos-os`** — avec un tiret — administre **le système** : mises à jour, réseau ;
- **`calaos_config`** — avec un tiret bas — gère les **réglages du serveur Calaos** : identifiants, notifications. Voir sa [page dédiée]({{% relref "calaos_os/configuration/calaos_config" %}}).
