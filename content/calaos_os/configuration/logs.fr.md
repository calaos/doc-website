+++
title = 'Journaux'
date = 2024-03-06T20:15:20Z
weight = 30
summary = "Consulter les journaux du système pour comprendre un problème."
+++

## À quoi servent les journaux

Le système note en permanence ce qu'il fait dans des **journaux**. Quand quelque chose ne fonctionne pas, c'est là que se trouve l'explication : quel service a échoué, à quel moment, et pourquoi.

Vous n'avez pas besoin de tout comprendre. Repérer le moment du problème et copier les quelques lignes correspondantes suffit largement pour demander de l'aide.

Cette page se consulte en SSH (voir [Accès SSH]({{% relref "calaos_os/advanced/shell" %}})).

## Voir ce qui se passe en direct

```sh
journalctl -f
```

Les lignes s'affichent au fur et à mesure que le système les écrit. C'est très pratique pour reproduire un problème : lancez cette commande, provoquez le dysfonctionnement, et regardez ce qui apparaît.

Appuyez sur `Ctrl` + `C` pour arrêter l'affichage.

## Voir les journaux d'un service

```sh
journalctl -u NOM_DU_SERVICE
```

Pour retrouver le nom d'un service, voyez la page [Services]({{% relref "calaos_os/configuration/services" %}}).

Ajoutez `-f` pour suivre ce service en direct :

```sh
journalctl -u NOM_DU_SERVICE -f
```

## Se limiter à une période

Les journaux sont volumineux. Pour ne voir que ce qui s'est passé récemment :

```sh
journalctl --since "1 hour ago"
journalctl --since today
journalctl --since "2026-07-25 14:00"
```

Et pour ne garder que les erreurs :

```sh
journalctl -p err --since today
```

## Les journaux du démarrage

Pour voir ce qui s'est passé au dernier démarrage :

```sh
journalctl -b
```

Et au démarrage précédent, utile quand la machine a redémarré toute seule :

```sh
journalctl -b -1
```

## Demander de l'aide

Pour transmettre des journaux au forum ou à un rapport d'anomalie, enregistrez-les dans un fichier :

```sh
journalctl -b > /tmp/journal.txt
```

Puis récupérez ce fichier sur votre ordinateur :

```sh
scp root@ADRESSE_DE_VOTRE_SERVEUR:/tmp/journal.txt .
```

{{% notice tip %}}
Précisez toujours **l'heure approximative** à laquelle le problème s'est produit : c'est ce qui permet de retrouver les bonnes lignes dans un journal qui en contient des milliers.
{{% /notice %}}

{{% notice warning %}}
Relisez ce que vous publiez : les journaux peuvent contenir des adresses, des noms d'équipements, parfois des identifiants. Retirez ce que vous ne souhaitez pas rendre public.
{{% /notice %}}

## Les journaux des services complémentaires

Les services qui tournent en containers ont leurs propres journaux :

```sh
podman logs NOM_DU_CONTAINER
```

La liste des containers s'obtient avec `podman ps`. Voir [Services et modules]({{% relref "calaos_os/containers" %}}).

## Pour aller plus loin

Les journaux sont stockés dans `/var/log`, qui est **exclu des points de restauration**. Concrètement : après un retour en arrière, les journaux de l'incident sont toujours là, et vous pouvez encore comprendre ce qui s'est passé. Voir [Sous le capot]({{% relref "calaos_os/advanced/btrfs" %}}).

Pour connaître la place occupée par les journaux, et la réduire si nécessaire :

```sh
journalctl --disk-usage
journalctl --vacuum-time=30d
```
