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

## Le service Envoy

Calaos OS embarque un service dédié, qui interroge la passerelle Enphase et met les valeurs à disposition sur le serveur.

Sa configuration vit dans `/mnt/calaos/envoy/envoy.toml`, créé automatiquement au premier démarrage à partir d'un modèle. Le service expose une interface web locale sur le **port 8100**.

Après toute modification du fichier, redémarrez le service :

```sh
systemctl restart envoy
```

Et pour vérifier qu'il fonctionne :

```sh
systemctl status envoy
journalctl -u envoy
```

Le niveau de détail des journaux se règle dans la section `[log]` du fichier de configuration.

Comme tous les services complémentaires, il tourne dans un container et ses données vivent sous `/mnt/calaos` — voir [Services et modules]({{% relref "calaos_os/containers" %}}).

## Prérequis

**La passerelle Envoy doit être joignable** depuis le serveur Calaos, et l'installation photovoltaïque doit déjà fonctionner et remonter ses données dans l'interface Enphase.

{{% notice tip %}}
Donnez une **adresse IP fixe** à l'Envoy, ou réservez-la sur votre box. Une passerelle qui change d'adresse fait disparaître toutes vos mesures d'un coup, et l'historique s'interrompt sans explication apparente.
{{% /notice %}}

## Récupérer les valeurs dans Calaos

Les mesures exposées par le service se déclarent ensuite comme des IO d'entrée analogique dans Calaos Installer, en pointant vers le service local.

Le principe est celui des [Web IO]({{% relref "hardware/webio" %}}) : Calaos interroge une adresse à intervalles réguliers et extrait la valeur qui l'intéresse. Commencez toujours par regarder ce que renvoie réellement le service avant de configurer :

```sh
curl http://127.0.0.1:8100/
```

Vous saurez alors quelles valeurs sont disponibles et sous quelle forme.

## Historiser la production

Activez l'**enregistrement des valeurs** sur ces IO pour construire des courbes de production et de consommation dans le temps. Les mesures sont alors envoyées à la base d'historique, et exploitables sous forme de graphiques.

Voir [Créer des IO]({{% relref "calaos_installer/io" %}}) pour cette option, et [Services et modules]({{% relref "calaos_os/containers" %}}) pour la base d'historique.

{{% notice note %}}
Une donnée de production évolue de façon continue : inutile de l'interroger toutes les secondes. Un relevé toutes les quelques minutes suffit largement pour des courbes lisibles, et allège la charge sur la passerelle.
{{% /notice %}}

## Piloter selon la production

Une fois la production disponible comme entrée analogique, elle s'utilise en condition de règle avec les opérateurs de comparaison :

> Si production `>` 2000 **et** chauffe-eau éteint, alors allumer le chauffe-eau.

Pensez à éviter les basculements incessants autour du seuil : une [variable interne]({{% relref "calaos_installer/internal_var" %}}) mémorisant que l'appareil a déjà démarré, ou un [minuteur]({{% relref "calaos_installer/time" %}}) imposant une durée minimale de marche, évitent qu'un nuage passager ne fasse claquer un relais toutes les minutes.

Voir [Créer des règles]({{% relref "calaos_installer/rules" %}}).

## Diagnostic

1. **le service tourne-t-il** ? `systemctl status envoy`
2. **répond-il** sur `http://127.0.0.1:8100/` ?
3. **la passerelle Envoy est-elle joignable** depuis le serveur ?
4. **les identifiants Enphase** sont-ils toujours valides ?

Voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}).
