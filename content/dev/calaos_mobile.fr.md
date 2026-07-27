+++
title = 'Calaos Mobile'
date = 2024-03-06T20:18:04Z
weight = 30
summary = "Les applications iOS et Android, et leur socle commun avec Calaos Home."
+++

## Rôle

Calaos Mobile est l'application qui pilote votre installation depuis un téléphone ou une tablette, à la maison comme à l'extérieur. Elle est disponible sur **iOS** et **Android**.

Dépôt : [calaos_mobile](https://github.com/calaos/calaos_mobile), écrit en **Qt / QML**.

## Un dépôt, deux produits

Le même dépôt produit l'application mobile et [Calaos Home]({{% relref "dev/calaos_home" %}}), l'interface de bureau pour écran tactile. Les deux partagent l'essentiel de leur logique :

```text
qml/
├── mobile/            interface des applications mobiles
├── desktop/           interface de Calaos Home
├── SharedComponents/  composants communs
└── quickflux/         gestion d'état

src/                   logique C++ commune, exposée au QML
```

Ce partage a une conséquence utile : **une correction dans les modèles ou dans le client de l'API profite aux deux produits**. Ce qui diverge se limite à la présentation et aux fonctions propres à la plateforme.

## Le client de l'API

`CalaosConnection` est la classe centrale : elle ouvre la connexion, authentifie la session, émet les requêtes et distribue les événements reçus.

Elle accepte plusieurs formes d'adresse et choisit le transport en conséquence :

| Adresse fournie | Connexion établie |
|---|---|
| `ws://` ou `wss://` | WebSocket, tel quel |
| `http://` ou `https://` | Converti vers le WebSocket équivalent |
| Adresse simple, version mobile | `wss://adresse/api` |
| Adresse simple, version de bureau | `ws://adresse:5454/api` |

La version mobile privilégie donc **une connexion chiffrée**, ce qui correspond à son usage : on s'y connecte souvent depuis l'extérieur, à travers Internet. C'est là que le [DNS dynamique]({{% relref "dev/calaos_ddns" %}}) et son certificat prennent tout leur sens.

Sur le réseau local, l'application peut aussi découvrir le serveur automatiquement, par le même mécanisme de diffusion UDP que les autres clients.

## Les modèles exposés au QML

L'interface ne manipule pas directement le JSON de l'API : la couche C++ le traduit en modèles Qt, que le QML consomme.

| Modèle | Contenu |
|---|---|
| `HomeModel` | Structure complète de la maison |
| `RoomModel`, `RoomFilterModel` | Pièces, et filtrage par type |
| `AudioModel` | Lecteurs, playlists, radios |
| `CameraModel` | Caméras |
| `EventLogModel` | Historique des événements |
| `FavoritesModel`, `BookmarkModel` | Raccourcis de l'utilisateur |
| `CalaosWidgetModel` | Widgets configurables |
| `UserInfoModel` | Session |
| `WeatherInfo` | Météo |

## Notifications push

L'application enregistre son jeton auprès du serveur avec la méthode `register_push` de l'API, en précisant la plateforme. Le serveur peut alors la prévenir : alarme déclenchée, équipement déconnecté, pile faible.

Les notifications automatiques liées aux équipements se règlent côté serveur — voir les clés `notif/*` dans [Email]({{% relref "calaos_os/configuration/email" %}}).

## Compiler

Le dépôt fournit des workflows GitHub Actions couvrant la compilation et la publication pour chaque plateforme, y compris les images Docker de build. Ils sont la référence à suivre pour reproduire un environnement de compilation, les chaînes iOS et Android ayant chacune leurs contraintes.

Le dépôt contient également les dossiers `android/` et `ios/`, qui portent les fichiers spécifiques à chaque plateforme, ainsi qu'une politique de confidentialité (`PRIVACY_POLICY.md`) exigée par les magasins d'applications.
