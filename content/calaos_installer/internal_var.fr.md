+++
title = 'Variable interne'
date = 2024-03-06T20:12:37Z
weight = 30
summary = "Mémoriser un état qui ne correspond à aucun équipement physique."
+++

## À quoi ça sert

Une **variable interne** est une IO sans matériel derrière : elle ne pilote rien et ne mesure rien, elle se contente de retenir une valeur.

C'est ce qui manque dès que vos automatismes deviennent un peu réfléchis. Quelques exemples :

- **un mode** — « Vacances », « Nuit », « Personne à la maison » — que plusieurs règles consultent ;
- **un compteur** — nombre de passages devant un détecteur, nombre de déclenchements d'une alarme ;
- **un drapeau** — « l'arrosage a déjà tourné aujourd'hui », pour éviter qu'une règle se déclenche deux fois ;
- **un réglage** que vous voulez modifier depuis une interface sans toucher à la configuration, comme une température de consigne.

Une variable interne s'utilise dans les règles exactement comme n'importe quelle IO : en condition comme en action.

## Les trois types

| Type | Contient | Usage typique |
|---|---|---|
| **Booléen** | vrai ou faux | Un mode actif ou non, un drapeau |
| **Entier** | un nombre | Un compteur, une consigne, un niveau |
| **Chaîne** | du texte | Un message, un état nommé |

Choisissez le plus simple qui convienne. Un mode « Vacances » est un booléen ; s'il vous faut trois modes exclusifs, un entier ou une chaîne sera plus lisible qu'une collection de booléens qu'il faut penser à remettre à zéro.

## Créer une variable

Dans le menu **Ajouter**, choisissez **Variable interne**, puis renseignez :

- **le nom**, comme pour toute IO ;
- **le type** parmi les trois ci-dessus ;
- **la valeur initiale**.

Trois options complètent la création :

| Option | Effet |
|---|---|
| **Visible** | Affiche la variable dans les interfaces, pour la consulter ou la modifier à la main |
| **Lecture / écriture** | Autorise sa modification depuis les interfaces, ou la réserve aux règles |
| **Sauvegarder** | Conserve la valeur au redémarrage du serveur |

## L'option « Sauvegarder »

C'est la plus importante, et celle dont l'oubli se remarque au pire moment.

Sans elle, la variable **repart à sa valeur initiale à chaque redémarrage** du serveur. Après une mise à jour ou une coupure de courant, votre mode « Vacances » se désactive tout seul, et l'arrosage repart alors que vous êtes absent.

{{% notice tip %}}
Demandez-vous : « si le serveur redémarre cette nuit, cette valeur doit-elle être retrouvée telle quelle ? » Si oui, activez **Sauvegarder**.

À l'inverse, une variable de travail — un compteur remis à zéro chaque matin — gagne à repartir de zéro.
{{% /notice %}}

## Un exemple complet

Pour empêcher l'arrosage automatique de tourner deux fois le même jour :

1. Créez une variable booléenne **« Arrosage effectué »**, valeur initiale *faux*, sans sauvegarde.
2. La règle d'arrosage ajoute une condition : ne se déclencher que si cette variable est *fausse*.
3. Dans ses actions, la règle passe la variable à *vrai*.
4. Une règle horaire remet la variable à *faux* chaque nuit.

Le même principe — une variable qui mémorise ce qui a déjà eu lieu — résout une grande partie des automatismes qui « se déclenchent en boucle ».

## Rendre une variable modifiable

Une variable **visible** et en **lecture/écriture** apparaît dans les applications comme un élément pilotable. C'est le moyen le plus simple d'offrir un réglage à l'utilisateur : une consigne de température, un mode, un nombre de minutes.

Vos règles lisent alors cette variable au lieu d'une valeur figée dans la configuration — et l'utilisateur peut ajuster son installation sans ouvrir Calaos Installer.

## Voir aussi

- [Créer des règles]({{% relref "calaos_installer/rules" %}}) — pour lire et modifier vos variables
- [Scripts en Lua]({{% relref "calaos_installer/scripts" %}}) — pour les manipuler dans du code
