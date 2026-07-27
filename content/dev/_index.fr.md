+++
title = "Pile logicielle Calaos"
date = 2024-03-06T19:59:28Z
weight = 100
chapter = true
+++

### Pour les développeurs

# Pile logicielle Calaos

Ce chapitre décrit le fonctionnement interne de Calaos, composant par composant. Il s'adresse aux développeurs, aux contributeurs et aux utilisateurs avancés qui veulent comprendre comment les morceaux s'assemblent, écrire un client, ou porter Calaos sur un nouveau matériel.

{{% notice info %}}
**Rien de ce chapitre n'est nécessaire pour utiliser Calaos.** Pour installer et configurer votre installation, voyez [Démarrage]({{% relref "getting_started" %}}) et [Calaos OS]({{% relref "calaos_os" %}}).
{{% /notice %}}

## Vue d'ensemble

Toute l'installation gravite autour d'un seul processus, **calaos_server**. C'est lui qui parle au matériel, applique les règles et expose une API. Tous les autres composants — interfaces graphiques, écrans muraux, applications mobiles — sont des **clients de cette API**, et n'ont aucun accès direct au matériel.

{{< mermaid >}}
graph TD
    subgraph clients[Clients]
        HOME[Calaos Home<br/>écran tactile local]
        MOBILE[Calaos Mobile<br/>iOS / Android]
        WEB[Calaos WebApp<br/>navigateur]
        RUI[Remote UI<br/>écrans muraux ESP32]
        INST[Calaos Installer<br/>configuration]
    end

    subgraph server[Serveur]
        SRV[calaos_server]
        RULES[Moteur de règles<br/>et scénarios]
        IO[Pilotes d'entrées / sorties]
    end

    subgraph hw[Matériel et services]
        WAGO[Wago / KNX / GPIO]
        MQTT[MQTT / Zigbee]
        ONEWIRE[1-Wire / DMX / Hue]
        DB[(InfluxDB<br/>historique)]
    end

    HOME -->|JSON / WebSocket| SRV
    MOBILE -->|JSON / WebSocket| SRV
    WEB -->|JSON / WebSocket| SRV
    RUI -->|JSON / WebSocket| SRV
    INST -->|JSON / HTTP| SRV

    SRV --- RULES
    SRV --- IO
    IO --> WAGO
    IO --> MQTT
    IO --> ONEWIRE
    SRV --> DB
{{< /mermaid >}}

Cette architecture a une conséquence pratique : **écrire un nouveau client ne demande de toucher à rien d'autre**. L'API est la même pour tous, documentée par le code de `calaos_server`, et les clients existants en sont autant d'exemples.

## Les composants

| Composant | Rôle | Technologie | Dépôt |
|---|---|---|---|
| [Calaos Server]({{% relref "dev/calaos_server" %}}) | Cœur du système : matériel, règles, API | C++ / libuv | [calaos_base](https://github.com/calaos/calaos_base) |
| [Calaos Home]({{% relref "dev/calaos_home" %}}) | Interface locale sur écran tactile | Qt / QML | [calaos_mobile](https://github.com/calaos/calaos_mobile) |
| [Calaos Mobile]({{% relref "dev/calaos_mobile" %}}) | Applications iOS et Android | Qt / QML | [calaos_mobile](https://github.com/calaos/calaos_mobile) |
| [Remote UI]({{% relref "dev/calaos_remoteui" %}}) | Écrans muraux autonomes | ESP32-P4 | [calaos_remote_ui](https://github.com/calaos/calaos_remote_ui) |
| [Calaos WebApp]({{% relref "dev/calaos_webapp" %}}) | Interface dans le navigateur | JavaScript | [calaos-web-app](https://github.com/calaos/calaos-web-app) |
| [Calaos DDNS]({{% relref "dev/calaos_ddns" %}}) | Accès distant et certificats | Go | [calaos_ddns](https://github.com/calaos/calaos_ddns) |
| [Fichiers de configuration]({{% relref "dev/calaos_config" %}}) | Format et emplacement de la configuration | XML | [calaos_base](https://github.com/calaos/calaos_base) |

À cela s'ajoutent les composants qui font tourner le système lui-même : **calaos-container**, le daemon qui gère les services en containers, et **calaos-os**, l'outil d'administration. Ils sont décrits dans [Sous le capot]({{% relref "calaos_os/advanced/btrfs" %}}) et [L'outil calaos-os]({{% relref "calaos_os/advanced/cli" %}}).

## Ports réseau

| Port | Protocole | Usage |
|---|---|---|
| 5454 | TCP | API JSON, en HTTP et en WebSocket |
| 4545 | UDP | Découverte du serveur sur le réseau local |
| 4646 | UDP | Écoute des automates Wago |
| 8000 | TCP | API d'administration de `calaos-container`, **sur la boucle locale uniquement** |

Le port de l'API est modifiable par le réglage `port_api`.

L'API du port 8000 est celle qu'utilise Calaos Home pour installer le système, configurer le réseau et confirmer un retour en arrière — voir [Calaos Home]({{% relref "dev/calaos_home" %}}).

## Licence

Calaos est un logiciel libre, publié sous **GPL v3** pour l'essentiel de ses composants. Les contributions sont les bienvenues : chaque dépôt accepte les *pull requests*, et les discussions se tiennent sur le forum et les *issues* GitHub.

{{% children description="true" %}}
