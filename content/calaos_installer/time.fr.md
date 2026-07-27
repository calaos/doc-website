+++
title = 'Programmation horaire'
date = 2024-03-06T20:12:57Z
weight = 50
summary = "Déclencher sur l'heure, sur le soleil, ou après un délai."
+++

## Trois outils, trois besoins

Calaos propose trois objets différents pour tout ce qui touche au temps. Choisir le bon dès le départ évite des règles inutilement compliquées.

| Objet | Répond à la question | Exemple |
|---|---|---|
| **Plage horaire** | *Sommes-nous dans une période ?* | « la nuit », « les jours ouvrés de 7 h à 9 h » |
| **Programmation** | *Quelle heure est-il ?* | « tous les jours à 22 h » |
| **Minuteur** | *Combien de temps depuis…* | « éteindre 5 minutes après » |

Tous trois s'ajoutent depuis le menu **Ajouter**, et s'utilisent ensuite dans les règles comme n'importe quelle IO.

## La plage horaire

Une plage horaire est **vraie ou fausse** selon le moment. Elle ne déclenche rien par elle-même : elle sert de **condition** dans vos règles.

C'est l'outil à utiliser dès qu'une automatisation ne doit s'appliquer qu'à certains moments : le détecteur de mouvement n'allume le couloir que la nuit, le chauffage ne monte en température que le matin.

Vous définissez pour chaque jour de la semaine les périodes où la plage est active. Les bornes peuvent être des heures fixes, mais aussi le **lever et le coucher du soleil**, ce qui suit naturellement les saisons.

{{% notice warning %}}
Les heures de lever et de coucher du soleil dépendent de la position de votre maison. Si vous ne l'avez pas renseignée, Calaos utilise **les coordonnées de Paris** par défaut, et vos volets se fermeront à l'heure parisienne.

C'est un réglage à faire une fois : voir [Date & Heure]({{% relref "calaos_os/configuration/date" %}}).
{{% /notice %}}

## La programmation

Une programmation déclenche **à un instant précis**. C'est l'équivalent d'un réveil : à telle date, tel jour, telle heure.

C'est ce qu'on utilise pour les actions ponctuelles et répétitives : arroser à 6 h, lancer le scénario « Nuit » à 23 h, remettre un compteur à zéro à minuit.

## Le minuteur

Un minuteur compte un délai, puis change d'état — ce qui permet à une règle de réagir à son échéance.

Deux options en déterminent le comportement :

| Option | Effet |
|---|---|
| **Démarrage automatique** | Le minuteur se lance dès le démarrage du serveur |
| **Redémarrage automatique** | Il repart au début à chaque échéance |

Avec le redémarrage automatique, vous obtenez un **battement régulier** : une action toutes les dix minutes, une vérification toutes les heures. Sans lui, le minuteur est à usage unique et doit être relancé par une règle.

{{% notice tip %}}
Le minuteur est la réponse à « éteindre automatiquement après un moment » : une règle démarre le minuteur quand la lumière s'allume, une autre éteint la lumière à l'échéance. C'est le montage classique pour un éclairage de garage ou de cave qu'on oublie d'éteindre.
{{% /notice %}}

## Choisir entre les trois

La confusion la plus fréquente oppose la plage horaire et la programmation.

**« À 22 h, ferme les volets »** — c'est une programmation : un événement, à un instant.

**« La nuit, si quelqu'un passe, allume »** — c'est une plage horaire : un état, pendant une période.

Un bon test : si votre phrase contient « à » suivi d'une heure, c'est une programmation. Si elle contient « pendant », « la nuit », « en semaine », c'est une plage horaire.

## L'heure du serveur

Toute cette mécanique repose sur l'horloge du serveur. Une heure fausse produit des symptômes déroutants : règles décalées, volets fermés au mauvais moment, écrans muraux qui refusent de se connecter.

Le serveur se met à l'heure automatiquement par Internet. En cas de doute, voyez [Date & Heure]({{% relref "calaos_os/configuration/date" %}}).

## Voir aussi

- [Créer des règles]({{% relref "calaos_installer/rules" %}}) — pour utiliser ces objets
- [Scenario]({{% relref "calaos_installer/scenario" %}}) — à déclencher à heure fixe
