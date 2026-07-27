+++
title = 'Calaos Server'
date = 2024-03-06T20:17:52Z
weight = 10
summary = "Le cœur du système : pilotes matériels, moteur de règles et API JSON."
+++

## Rôle

`calaos_server` est le seul composant qui parle au matériel. Il charge la configuration, instancie les entrées/sorties, exécute le moteur de règles, journalise l'historique et expose une API que consomment tous les autres composants.

Dépôt : [calaos_base](https://github.com/calaos/calaos_base) — écrit en **C++**, publié sous **GPL v3**.

## Compiler

```sh
./autogen.sh
make
sudo make install
```

Dépendances principales :

| Bibliothèque | Usage |
|---|---|
| libuv (> 1.10) | Boucle d'événements, réseau, timers |
| jansson (> 2.5) | Sérialisation JSON |
| libcurl (> 7.20) | Requêtes HTTP sortantes |
| LuaJIT | Exécution des scripts utilisateur |
| sigc++ (> 2.4.1) | Signaux entre objets |

Et de façon optionnelle, selon le matériel visé : **owfs** pour le 1-Wire, **OLA** pour le DMX, **knxd** pour KNX, **ImageMagick** pour le traitement des images de caméras.

Le dépôt embarque par ailleurs plusieurs bibliothèques dans `src/lib` : `uvw` (surcouche C++ de libuv), `llhttp`, `exprtk` pour l'évaluation d'expressions, `sqlite_modern_cpp`, `libquickmail` pour l'envoi d'emails, et `sunset.c` pour le calcul des heures de lever et de coucher du soleil.

## Organisation du code

Le code du serveur vit dans `src/bin/calaos_server` :

| Emplacement | Contenu |
|---|---|
| `Calaos.cpp` | Point d'entrée de la logique métier |
| `CalaosConfig.cpp` | Chargement et écriture de la configuration |
| `IO/` | Tous les pilotes d'entrées/sorties |
| `IOBase.cpp`, `IOFactory.cpp` | Classe de base des IO et fabrique associée |
| `Rules/`, `Rule.cpp`, `ListeRule.cpp` | Moteur de règles |
| `Scenario/` | Scénarios |
| `LuaScript/` | Intégration LuaJIT |
| `JsonApi.cpp` | Implémentation de l'API |
| `JsonApiHandlerWS.cpp`, `JsonApiHandlerHttp.cpp` | Transports WebSocket et HTTP |
| `HttpServer.cpp`, `WebSocket.cpp`, `UDPServer.cpp` | Serveurs réseau |
| `EventManager.cpp` | Diffusion des événements aux clients |
| `HistLogger.cpp`, `DataLogger.cpp` | Historique et enregistrement des mesures |
| `NotifManager.cpp` | Notifications par email et push |
| `IPCam/` | Caméras IP |
| `McpServerManager.cpp` | Exposition du système à des assistants via MCP |

## Les entrées/sorties

Tout ce que Calaos pilote est une **IO**, dérivée de `IOBase` et créée par `IOFactory` à partir du type déclaré dans la configuration.

Les types génériques ne dépendent d'aucun matériel : `InputSwitch`, `InputSwitchLongPress`, `InputSwitchTriple`, `InputAnalog`, `InputTemp`, `InputTime`, `InputTimer`, `InPlageHoraire`, `IntValue`, `OutputLight`, `OutputLightDimmer`, `OutputLightRGB`, `OutputShutter`, `OutputShutterSmart`, `OutputAnalog`, `OutputString`.

Les pilotes matériels vivent dans leurs sous-dossiers : `Wago`, `KNX`, `Gpio`, `OneWire`, `Mqtt`, `Hue`, `OLA`, `MySensors`, `Reolink`, `LAN`, `Web`, `RemoteUI`. `ExternProc` permet en outre de déléguer une IO à un processus externe.

{{% notice tip %}}
**Ajouter un pilote** consiste à dériver `IOBase`, à décrire ses paramètres via `IODoc`, puis à l'enregistrer dans `IOFactory`. `IODoc` alimente la documentation intégrée des paramètres, ce qui rend le nouveau type immédiatement exploitable depuis Calaos Installer.
{{% /notice %}}

Le fichier `IO/io_types.txt` récapitule les types disponibles.

## Règles, scénarios et scripts

Le moteur de règles associe des **conditions** portant sur l'état des entrées à des **actions** sur les sorties. Les règles sont décrites dans `rules.xml` et rechargées à chaque envoi de configuration.

Pour ce que les règles ne savent pas exprimer, `LuaScript/` embarque **LuaJIT** : un script reçoit l'état du système et agit sur les IO. Voir [Scripts]({{% relref "calaos_installer/scripts" %}}).

Les **plages horaires** et les calculs solaires reposent sur `TimeRange`, `Calendar` et `sunset.c` — d'où l'importance de renseigner [latitude et longitude]({{% relref "calaos_os/configuration/date" %}}).

## L'API JSON

C'est le point d'entrée de tous les clients. Elle est disponible sous deux transports :

| Transport | Adresse |
|---|---|
| WebSocket | `ws://serveur:5454/api` |
| HTTP | `http://serveur:5454/api.php` |

Le WebSocket est à privilégier : il permet au serveur de **pousser les changements d'état** au client, là où HTTP impose d'interroger périodiquement.

### Déroulement d'une session

1. **`login`** — authentification avec les identifiants `cn_user` / `cn_pass`.
2. **`get_home`** — récupération de la structure : pièces, IO, types, paramètres d'affichage.
3. **`get_state`** / **`get_states`** — lecture de l'état courant des IO.
4. **`set_state`** — action sur une sortie.
5. Le serveur émet ensuite des messages **`event`** à chaque changement.

Chaque requête porte un `msg` (le nom de la méthode) et un `msg_id` que la réponse reprend, ce qui permet d'associer réponses et requêtes sur une connexion multiplexée.

### Principales méthodes

| Domaine | Méthodes |
|---|---|
| Session | `login`, `change_cred` |
| État | `get_home`, `get_io`, `get_state`, `get_states`, `set_state`, `query` |
| Réglages | `get_param`, `set_param`, `del_param` |
| Plages horaires | `get_timerange`, `set_timerange` |
| Scénarios | `autoscenario` |
| Historique | `eventlog`, `get_stats` |
| Caméras | `get_camera_pic` |
| Audio | `get_playlists`, `get_playlist`, `get_artists`, `get_album`, `get_radios`, `audio_action` |
| Notifications | `register_push` |

La liste fait autorité dans `JsonApiHandlerWS.cpp` ; c'est la référence à consulter avant d'écrire un client.

## Découverte sur le réseau

Un client qui ignore l'adresse du serveur envoie en diffusion UDP le message `CALAOS_DISCOVER` sur le **port 4545**. Le serveur répond avec ses informations de connexion.

C'est ce mécanisme qui permet à Calaos Home, aux applications mobiles et aux écrans Remote UI de trouver le serveur sans configuration. Voir `UDPServer.cpp`.

## Ports

| Port | Protocole | Usage |
|---|---|---|
| 5454 | TCP | API JSON (HTTP et WebSocket), modifiable par le réglage `port_api` |
| 4545 | UDP | Découverte |
| 4646 | UDP | Écoute des automates Wago |

## Journalisation

Les traces passent par `Logger`, avec des **domaines** permettant de cibler un sous-système. Elles sont réglables via `debug_enabled`, `debug_level` et `debug_domains` — voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}).

Dans le code, on écrit par exemple :

```cpp
cDebugDom("network") << "Got a CALAOS_DISCOVER";
```

## Les outils annexes

Le même dépôt fournit plusieurs utilitaires, dans `src/bin/tools` :

| Outil | Rôle |
|---|---|
| `calaos_config` | Lecture et écriture des réglages — voir [calaos_config]({{% relref "calaos_os/configuration/calaos_config" %}}) |
| `calaos_mail` | Envoi d'emails, utilisé par les notifications — voir [Email]({{% relref "calaos_os/configuration/email" %}}) |
| `calaos_picture` | Traitement des images de caméras |
| `wago_test` | Test et diagnostic des automates Wago |

S'y ajoute `src/bin/calaos_mcp`, qui expose l'installation à des assistants compatibles **MCP**.
