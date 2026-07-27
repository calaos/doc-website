+++
title = 'Enphase Envoy'
date = 2024-03-11T17:05:41Z
summary = 'Passerelle Envoy de Enphase pour panneaux photovoltaique'
+++

## Introduction

L'**Envoy** est la passerelle des installations photovoltaïques Enphase : elle collecte les données de chaque micro-onduleur et connaît à tout moment la production de l'installation, la consommation de la maison, et le solde entre les deux.

Calaos récupère ces valeurs pour les afficher, les historiser, et surtout **s'en servir dans des règles**.

## À quoi ça sert

C'est le pilotage par la production qui rend l'intégration intéressante : plutôt que de subir votre production, vous déclenchez les consommations au bon moment.

- **lancer le chauffe-eau ou la borne de recharge** quand la production dépasse un seuil ;
- **repousser un lave-linge** aux heures de forte production ;
- **suivre l'autoconsommation** avec des graphiques dans le temps ;
- **être alerté** en cas d'arrêt de production inattendu.

## Pourquoi un service dédié

Depuis une mise à jour du firmware Enphase en 2022, **on ne peut plus interroger directement la passerelle** sur le réseau local. Il faut d'abord s'authentifier auprès du service en ligne Enlighten, en récupérer un jeton valable pour votre passerelle, et seulement ensuite interroger l'Envoy chez vous.

Cette gymnastique est hors de portée d'une simple requête HTTP. Calaos OS embarque donc un service dédié qui s'en charge : il gère l'authentification, renouvelle le jeton tout seul, interroge la passerelle et **met les données à disposition sous une forme directement exploitable**.

Le code est disponible sur [go-envoy](https://github.com/raoulh/go-envoy).

## Configurer l'accès

La configuration se fait en une commande, depuis le serveur en SSH. Munissez-vous de trois informations : votre **compte Enlighten** (celui de l'application Enphase) et le **numéro de série de votre passerelle**, inscrit sur l'appareil et visible dans l'application.

```sh
envoy config set -h 192.168.1.134 -u mon.compte@exemple.fr -s 1234567890 -p mon_mot_de_passe
```

| Option | Signification |
|---|---|
| `-h` | Adresse IP de la passerelle sur votre réseau |
| `-u` | Identifiant du compte Enlighten |
| `-s` | Numéro de série de la passerelle |
| `-p` | Mot de passe du compte Enlighten |

{{% notice tip %}}
L'option `-h` est facultative : sans elle, le service **cherche la passerelle tout seul** sur le réseau local. Renseignez-la si la détection échoue, ou si vous avez plusieurs passerelles.
{{% /notice %}}

Ces informations sont enregistrées avec le jeton d'authentification dans le dossier de données du service, sous `/mnt/calaos/envoy`. Elles font donc partie de vos données et survivent aux mises à jour — voir [Sauvegarder sa configuration]({{% relref "calaos_os/backup" %}}).

## Vérifier que ça fonctionne

La façon la plus rapide de valider la configuration :

```sh
envoy now
```

qui affiche la production et la consommation instantanées :

```text
🔌Production: 59.43W / 2354W    Consumption: 1689.83W   Net import: 1630.40W
```

Si cette commande répond, l'authentification et l'accès à la passerelle sont bons. D'autres commandes permettent d'aller plus loin :

| Commande | Effet |
|---|---|
| `envoy now` | Production et consommation instantanées |
| `envoy today` | Statistiques de la journée |
| `envoy info` | Informations sur la passerelle |
| `envoy production` | Données brutes de production, en JSON |
| `envoy inventory` | Inventaire des équipements |
| `envoy inverters` | État de chaque micro-onduleur |

## Le service

Le daemon interroge la passerelle en continu, met les données en cache et les expose sur le **port 8100**.

```sh
systemctl status envoy
systemctl restart envoy
journalctl -u envoy
```

Sa configuration générale — port, niveau de journalisation — vit dans `/mnt/calaos/envoy/envoy.toml`. Le niveau de détail des journaux se règle dans la section `[log]`.

Comme tous les services complémentaires, il tourne dans un container — voir [Services et modules]({{% relref "calaos_os/containers" %}}).

## Récupérer les valeurs dans Calaos

Le service expose trois points d'entrée en JSON :

| Adresse | Contenu |
|---|---|
| `http://127.0.0.1:8100/api/production` | Production et consommation |
| `http://127.0.0.1:8100/api/inventory` | Inventaire des équipements |
| `http://127.0.0.1:8100/api/inverters` | État de chaque micro-onduleur |

Une interface web est également disponible sur `http://adresse-du-serveur:8100/`.

Ces valeurs se déclarent ensuite dans Calaos Installer comme des **entrées analogiques de type Web IO**, qui interrogent le service et extraient la valeur voulue — voir [Web IO]({{% relref "hardware/webio" %}}).

Regardez d'abord ce que renvoie le point d'entrée pour repérer le chemin à utiliser :

```sh
curl http://127.0.0.1:8100/api/production
```

La réponse contient deux listes, `production` et `consumption`, chacune composée de mesures. Les champs les plus utiles sont :

| Champ | Signification |
|---|---|
| `wNow` | Puissance instantanée, en watts |
| `whToday` | Énergie cumulée depuis le début de la journée, en wattheures |
| `whLifetime` | Énergie cumulée depuis l'installation |

{{% notice note %}}
Une donnée de production évolue de façon continue : inutile de l'interroger toutes les secondes. Un relevé toutes les quelques minutes suffit largement pour des courbes lisibles, et allège la charge sur la passerelle.
{{% /notice %}}

## Historiser la production

Activez l'**enregistrement des valeurs** sur ces IO pour construire des courbes de production et de consommation dans le temps. Voir [Créer des IO]({{% relref "calaos_installer/io" %}}).

## Piloter selon la production

Une fois la production disponible comme entrée analogique, elle s'utilise en condition de règle :

> Si production `>` 2000 **et** chauffe-eau éteint, alors allumer le chauffe-eau.

Pensez à éviter les basculements incessants autour du seuil : une [variable interne]({{% relref "calaos_installer/internal_var" %}}) mémorisant que l'appareil a déjà démarré, ou un [minuteur]({{% relref "calaos_installer/time" %}}) imposant une durée minimale de marche, évitent qu'un nuage passager ne fasse claquer un relais toutes les minutes.

Voir [Créer des règles]({{% relref "calaos_installer/rules" %}}).

## Diagnostic

1. **`envoy now` répond-il ?** C'est le test le plus direct : s'il échoue, le problème est dans la configuration ou l'accès à la passerelle, pas dans Calaos.
2. **le service tourne-t-il ?** `systemctl status envoy`
3. **le point d'entrée répond-il ?** `curl http://127.0.0.1:8100/api/production`
4. **le mot de passe Enlighten a-t-il changé ?** L'authentification passe par le service en ligne d'Enphase : un changement de mot de passe interrompt la collecte.

{{% notice warning %}}
Le service a besoin d'un **accès à Internet** pour renouveler son jeton auprès d'Enphase. Une coupure prolongée de la connexion finit donc par interrompre la remontée des données, même si la passerelle reste joignable sur le réseau local.
{{% /notice %}}

Voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}).
