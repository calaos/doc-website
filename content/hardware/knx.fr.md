+++
title = 'KNX'
date = 2024-03-06T20:04:18Z
weight = 20
summary = 'Appareils KNX avec le service knxd'
+++

## Introduction

KNX est un standard de bus domotique très répandu, notamment dans le tertiaire et sur les installations réalisées par des intégrateurs. Calaos sait piloter un bus KNX existant, ce qui permet de conserver une installation en place tout en lui ajoutant les interfaces et l'automatisation de Calaos.

Calaos ne parle pas directement au bus : il passe par **knxd**, un service qui fait la passerelle entre le réseau IP et le bus KNX.

## Le service knxd

`knxd` doit être joignable depuis le serveur Calaos. Il tourne le plus souvent sur la machine Calaos elle-même, auquel cas les IO utilisent l'adresse par défaut `127.0.0.1`.

Il a besoin d'une interface physique vers le bus, qui peut être :

- une **passerelle IP / KNX** sur le réseau ;
- une **interface USB** branchée sur le serveur ;
- un **coupleur** intégré à un automate, comme les têtes Wago compatibles KNX — voir [Installation KNX]({{% relref "hardware/wago/knx" %}}).

## Les adresses de groupe

Tout repose sur les **adresses de groupe**, notées sous la forme `x/y/z`. Chaque fonction du bus — allumer une lampe, lire une température — possède la sienne, définie lors de la conception de l'installation KNX, généralement avec le logiciel ETS.

{{% notice tip %}}
Munissez-vous du **projet ETS de l'installation**, ou à défaut de la liste des adresses de groupe. Sans elle, il faut deviner à quoi correspond chaque adresse, ce qui est fastidieux et risqué sur une installation en service.
{{% /notice %}}

## Paramètres communs

Toutes les IO KNX partagent les paramètres suivants :

| Nom | Type | Requis | Description |
|---|---|---|---|
| `knx_group` | string | oui | Adresse de groupe à commander ou à lire, de la forme `x/y/z` |
| `listen_knx_group` | string | non | Adresse de groupe à écouter pour connaître l'état réel |
| `eis` | int | non | Type de donnée KNX (EIS), de 0 à 15 |
| `read_at_start` | bool | non | Envoie une requête de lecture au démarrage pour connaître l'état courant. Par défaut `false` |
| `host` | string | oui | Adresse de knxd. Par défaut `127.0.0.1` |

### L'adresse d'écoute

`listen_knx_group` mérite une explication, car son oubli est la cause la plus fréquente d'incohérences d'affichage.

Sur un bus KNX, l'adresse qui **commande** un équipement et celle qui **remonte son état** sont souvent distinctes. Si vous ne renseignez que `knx_group`, Calaos sait ce qu'il a demandé mais ignore ce qui s'est réellement passé : une lampe allumée depuis un bouton KNX classique apparaîtra éteinte dans les interfaces.

Renseignez `listen_knx_group` avec l'adresse de retour d'état, et l'affichage suivra la réalité quelle que soit l'origine de la commande.

### Le paramètre `eis`

KNX encode ses valeurs selon des types de données. Le paramètre `eis` indique à Calaos comment interpréter ce qui circule sur l'adresse : un booléen, un pourcentage, une température.

Si les valeurs remontées sont aberrantes — une température de 6000 degrés, un variateur qui saute de 0 à 100 — c'est presque toujours ce paramètre qui ne correspond pas à ce qu'émet l'équipement.

## Volets

Les volets font exception : ils utilisent deux adresses distinctes plutôt qu'une.

| Nom | Type | Requis | Description |
|---|---|---|---|
| `knx_group_up` | string | oui | Adresse de groupe pour la montée |
| `knx_group_down` | string | oui | Adresse de groupe pour la descente |

## Types d'IO supportés

**En entrée** — interrupteur, interrupteur à appui long, interrupteur triple, entrée analogique, sonde de température.

**En sortie** — lumière, lumière à variation, lumière RVB, volet, volet intelligent, sortie analogique.

## Créer une IO KNX

Dans Calaos Installer, **Ajouter → KNX**, puis le type voulu. Renseignez l'adresse de groupe, et l'adresse d'écoute si elle existe.

Le détail de chaque paramètre est affiché directement dans Calaos Installer — voir [Créer des IO]({{% relref "calaos_installer/io" %}}).

## Diagnostic

Si une IO ne réagit pas, vérifiez dans l'ordre :

1. **knxd est-il joignable** depuis le serveur, à l'adresse indiquée dans `host` ?
2. **l'adresse de groupe** correspond-elle bien à la fonction visée dans le projet ETS ?
3. **le type de donnée** `eis` correspond-il à ce qu'attend l'équipement ?

Les échanges avec le bus apparaissent dans les journaux du serveur, sous le domaine `knx` :

```sh
calaos_config set debug_enabled true
calaos_config set debug_domains knx:5
systemctl restart calaos-server
journalctl -u calaos-server -f
```

Pensez à remettre `debug_enabled` à `false` ensuite — voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}).
