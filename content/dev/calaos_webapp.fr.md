+++
title = 'Calaos WebApp'
date = 2024-03-06T20:18:08Z
weight = 40
summary = "L'interface accessible depuis un navigateur."
+++

## Rôle

La WebApp est l'interface de Calaos accessible depuis un **navigateur**, sans rien installer. Elle est utile depuis un ordinateur de passage, ou depuis un appareil pour lequel il n'existe pas d'application native.

Dépôt : [calaos-web-app](https://github.com/calaos/calaos-web-app), écrit en **JavaScript**.

Comme les autres interfaces, elle n'est qu'un **client de l'API** : elle ne parle jamais au matériel directement, et tout ce qu'elle affiche vient de `calaos_server`. Voir [Calaos Server]({{% relref "dev/calaos_server" %}}).

## Construire

La chaîne repose sur Node.js, avec Bower pour les dépendances côté navigateur et Gulp comme outil de build.

```sh
npm install -g bower gulp
npm install
bower install
gulp
```

Pour développer avec rechargement automatique :

```sh
gulp serve
```

Dans ce mode, l'application ne tourne pas sur le serveur Calaos : il faut donc lui indiquer où le joindre, en renseignant l'URL de l'API dans `src/scripts/dev_config.js`, sous la forme `ws://serveur:5454/api`.

{{% notice note %}}
Cette chaîne de construction (Bower, Gulp) date des débuts du projet et n'est plus celle qu'on choisirait aujourd'hui. C'est à connaître avant de monter un environnement de développement : les versions récentes de Node.js peuvent demander quelques ajustements.
{{% /notice %}}

## Déploiement

Sur une installation Calaos OS, la WebApp est servie aux côtés du reste du système ; l'aiguillage des connexions entrantes est assuré par HAProxy, qui gère aussi le certificat mis en place par le [DNS dynamique]({{% relref "dev/calaos_ddns" %}}).

## Écrire son propre client

La WebApp est un bon point de départ pour comprendre comment dialoguer avec le serveur depuis un navigateur : ouverture du WebSocket, `login`, `get_home`, puis traitement des événements reçus.

La séquence complète et la liste des méthodes sont décrites dans [Calaos Server]({{% relref "dev/calaos_server" %}}).
