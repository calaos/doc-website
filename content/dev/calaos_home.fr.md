+++
title = 'Calaos Home'
date = 2024-03-06T20:17:57Z
weight = 20
summary = "L'interface locale sur écran tactile, et son pilotage du système."
+++

## Rôle

Calaos Home est l'interface affichée sur l'**écran tactile branché au serveur**. Elle présente la maison — pièces, éclairages, volets, températures, caméras, audio — et sert aussi à quelques opérations sur le système lui-même : configuration réseau, installation sur disque, confirmation d'un retour en arrière.

Elle est écrite en **Qt / QML** et vit dans le dépôt [calaos_mobile](https://github.com/calaos/calaos_mobile), qu'elle partage avec les applications mobiles.

{{% notice info %}}
Le dépôt s'appelle `calaos_mobile` mais contient **deux produits** : l'application de bureau pour écran tactile (Calaos Home) et les applications iOS/Android. La partie de bureau se compile avec le drapeau `CALAOS_DESKTOP`.
{{% /notice %}}

## Organisation du code

| Emplacement | Contenu |
|---|---|
| `qml/desktop/` | Interface de Calaos Home |
| `qml/mobile/` | Interface des applications mobiles |
| `qml/SharedComponents/` | Composants communs aux deux |
| `src/` | Logique C++ exposée au QML |

Côté C++, les classes principales sont communes aux deux produits :

| Classe | Rôle |
|---|---|
| `CalaosConnection` | Client de l'API : WebSocket, HTTP, reconnexion |
| `HomeModel`, `RoomModel`, `RoomFilterModel` | Modèles de la maison exposés au QML |
| `AudioModel`, `CameraModel`, `EventLogModel` | Modèles spécialisés |
| `FavoritesModel`, `BookmarkModel`, `CalaosWidgetModel` | Personnalisation de l'interface |
| `HardwareUtils` | Abstraction de la plateforme |

Et certaines n'existent que pour la version de bureau :

| Classe | Rôle |
|---|---|
| `HardwareUtils_desktop` | Découverte du serveur, curseur, options locales |
| `ScreenManager` | Mise en veille de l'écran |
| `XUtils` | Accès à X11, notamment l'extension DPMS |
| `CalaosOsAPI` | Pilotage du système Calaos OS |
| `OSInstaller`, `UsbDisk` | Installation du système sur disque |

L'état de l'interface est géré avec **QuickFlux**, une implémentation du modèle Flux pour QML : les composants émettent des actions, qui sont traitées de façon centralisée.

## Connexion au serveur

Au démarrage, l'application diffuse `CALAOS_DISCOVER` en UDP sur le port 4545 et attend la réponse du serveur. La recherche est relancée toutes les cinq secondes tant qu'aucun serveur n'a répondu.

Si le réglage `calaos_server_host` est renseigné, **la découverte est désactivée** et l'adresse indiquée est utilisée directement. Voir [Écran tactile]({{% relref "calaos_os/configuration/touchscreen" %}}).

`CalaosConnection` construit ensuite l'URL selon ce qui lui est donné :

- une adresse commençant par `ws://` ou `wss://` est utilisée telle quelle ;
- une adresse commençant par `http://` ou `https://` est convertie vers son équivalent WebSocket ;
- une simple adresse IP donne `ws://adresse:5454/api` sur la version de bureau.

## Réglages locaux

L'application lit sa propre configuration dans `local_config.xml`, par les mêmes clés que celles décrites dans [calaos_config]({{% relref "calaos_os/configuration/calaos_config" %}}) : `show_cursor`, `dpms_enable`, `dpms_standby`, `calaos_server_host`, `cn_user`, `cn_pass`, `latitude`, `longitude`.

Au premier lancement, si aucun fichier n'existe, l'application en écrit un avec des valeurs par défaut : identifiants `user` / `pass`, curseur affiché, mise en veille désactivée, et des coordonnées géographiques pointant sur Paris.

`ScreenManager` lit `dpms_standby` en minutes et le convertit en millisecondes ; en l'absence de valeur exploitable, il retombe sur une minute.

## Pilotage du système

C'est la particularité de Calaos Home : au-delà de la domotique, elle administre la machine. Elle passe pour cela par une **API REST locale**, exposée par le daemon `calaos-container` sur `http://127.0.0.1:8000`.

L'authentification se fait par jeton, lu dans `/run/calaos/calaos-ct.token` — un fichier régénéré à chaque démarrage par `init_calaosfs` — et transmis dans un en-tête `Authorization: bearer <jeton>`.

| Point d'entrée | Usage |
|---|---|
| `GET /api/system/info` | Informations sur le système |
| `GET /api/system/fs_status` | État du système de fichiers |
| `POST /api/system/reboot` | Redémarrage de la machine |
| `POST /api/system/restart` | Redémarrage des services |
| `POST /api/system/rollback_snapshot` | Confirmation d'un retour en arrière |
| `GET /api/system/install/list_devices` | Liste des disques disponibles |
| `POST /api/system/install/start` | Lancement de l'installation |
| `GET /api/system/install/status` | Progression de l'installation |
| `GET /api/network/list` | Liste des interfaces réseau |
| `POST /api/network/<interface>` | Configuration d'une interface |

Ces points d'entrée expliquent des comportements décrits ailleurs dans la documentation : le bouton de confirmation qui apparaît quand on a démarré sur un point de restauration — voir [Revenir en arrière]({{% relref "calaos_os/rollback" %}}) — et l'installation sur disque proposée depuis le mode Live, décrite dans [Installation]({{% relref "calaos_os/installation" %}}).

{{% notice warning %}}
Cette API n'écoute que sur la boucle locale. Elle n'est pas accessible depuis le réseau, et n'est pas destinée à être consommée par autre chose que l'interface locale.
{{% /notice %}}

## Compiler

Le projet se construit avec Qt. Les workflows GitHub Actions du dépôt (`.github/workflows/`) décrivent la chaîne complète, y compris les images Docker de compilation, et constituent la référence la plus fiable pour reproduire un build.
