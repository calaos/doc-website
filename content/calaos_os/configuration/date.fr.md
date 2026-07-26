+++
title = 'Date & Heure'
date = 2024-03-06T20:15:54Z
weight = 60
summary = "Fuseau horaire et mise à l'heure automatique du serveur."
+++

## Pourquoi c'est important

L'heure du serveur n'est pas un détail de confort. Elle conditionne le bon fonctionnement de votre installation :

- **vos règles horaires** — allumer à 19 h, fermer les volets au coucher du soleil — se déclenchent selon l'heure du serveur ;
- **le lever et le coucher du soleil** sont calculés à partir de la date ;
- **les écrans Remote UI** refusent de se connecter si l'heure est fausse, leur sécurité reposant dessus ;
- **l'historique de vos mesures** est horodaté avec cette heure.

Une heure décalée produit des symptômes déroutants : des règles qui se déclenchent au mauvais moment, ou des équipements qui refusent de se connecter sans raison apparente.

Ces réglages ne sont pas disponibles depuis l'écran tactile : connectez-vous en SSH (voir [Accès SSH]({{% relref "calaos_os/advanced/shell" %}})).

## Vérifier l'heure actuelle

```sh
timedatectl
```

Cette commande affiche l'heure locale, le fuseau horaire configuré, et indique si la mise à l'heure automatique est active.

Vérifiez trois choses : que l'heure affichée est la bonne, que le fuseau correspond à votre pays, et que la synchronisation automatique est activée.

## Changer le fuseau horaire

Pour voir la liste des fuseaux disponibles :

```sh
timedatectl list-timezones
```

La liste est longue ; pour ne garder que ceux d'Europe :

```sh
timedatectl list-timezones | grep Europe
```

Puis appliquez le vôtre :

```sh
timedatectl set-timezone Europe/Paris
```

## La mise à l'heure automatique

Votre serveur ajuste son horloge en interrogeant des serveurs de temps sur Internet. C'est ce qui garantit qu'il reste à l'heure sur la durée, y compris lors des changements d'heure.

Pour vérifier ou activer :

```sh
timedatectl set-ntp true
```

{{% notice tip %}}
Si l'heure est systématiquement fausse après chaque coupure de courant, c'est souvent la **pile de la carte mère** qui est morte. C'est une pile bouton, peu coûteuse et facile à remplacer. La mise à l'heure automatique corrige le problème dès que le réseau est disponible, mais quelques minutes peuvent s'écouler avant.
{{% /notice %}}

## Après un changement

Redémarrez le serveur Calaos pour que le nouveau réglage soit pris en compte partout, ou redémarrez simplement la machine :

```sh
reboot
```

## Voir aussi

La gestion du temps dans vos règles — plages horaires, calendriers, lever et coucher du soleil — est décrite dans [Gestion du temps]({{% relref "calaos_installer/time" %}}), au chapitre Calaos Installer.

Pour les écrans muraux et leur exigence d'une heure correcte, voyez [Remote UI]({{% relref "hardware/remote_ui" %}}).
