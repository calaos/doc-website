+++
title = 'Scenario'
date = 2024-03-06T20:12:45Z
weight = 40
summary = "Enchaîner plusieurs actions et les déclencher d'un seul geste."
+++

## À quoi ça sert

Un **scénario** regroupe une suite d'actions sous un seul nom. Au lieu d'éteindre huit lumières, de fermer les volets et de couper la musique une par une, vous déclenchez « Départ ».

Les usages classiques :

- **Départ** — tout éteindre, fermer les volets, activer la surveillance ;
- **Nuit** — éteindre le rez-de-chaussée, tamiser le couloir ;
- **Cinéma** — baisser l'éclairage, fermer les volets, allumer le vidéoprojecteur ;
- **Réveil** — ouvrir les volets, allumer le chauffage de la salle de bain.

Un scénario est une IO comme une autre : il apparaît dans les interfaces, et vos règles peuvent le déclencher.

## Scénario ou règle ?

Les deux se ressemblent, et la confusion est fréquente.

| | Rôle | Se déclenche |
|---|---|---|
| **Règle** | Réagir à quelque chose | Automatiquement, quand ses conditions sont vraies |
| **Scénario** | Faire une suite d'actions | Quand on le demande — bouton, règle, application |

Autrement dit : la **règle décide quand**, le **scénario décrit quoi faire**. Les deux se combinent très bien — une règle horaire qui déclenche le scénario « Nuit » est plus lisible que la même règle contenant quinze actions.

{{% notice tip %}}
Dès qu'une suite d'actions doit pouvoir être lancée **depuis plusieurs endroits** — un bouton mural, l'application, une règle horaire — faites-en un scénario. Vous ne la décrirez qu'une fois, et une modification profitera à tous les déclencheurs.
{{% /notice %}}

## Créer un scénario

Dans le menu **Ajouter**, choisissez **Scénario**, donnez-lui un nom, puis composez la liste des actions : pour chaque équipement concerné, l'état à appliquer.

L'option **Visible** détermine s'il apparaît dans les interfaces. Un scénario destiné à n'être appelé que par des règles peut rester masqué.

## Déclencher un scénario

Trois façons, toutes équivalentes du point de vue du serveur :

- **depuis une interface** — l'utilisateur appuie sur le scénario dans l'application ou sur l'écran mural ;
- **depuis une règle** — le scénario devient une action de cette règle ;
- **depuis un bouton physique** — une règle relie l'interrupteur au scénario.

Un même scénario peut évidemment être déclenché par les trois.

## Bien concevoir ses scénarios

**Restez sur une intention claire.** Un scénario nommé « Départ » doit faire ce que son nom annonce. Un scénario fourre-tout devient impossible à modifier sereinement.

**Attention aux scénarios qui s'appellent entre eux.** C'est possible, mais un enchaînement circulaire — « Nuit » appelle « Rez-de-chaussée » qui rappelle « Nuit » — produit des comportements difficiles à diagnostiquer.

**Pensez au retour.** Si vous créez « Cinéma », prévoyez comment revenir à un état normal : un second scénario, ou une règle qui rétablit l'éclairage quand le vidéoprojecteur s'éteint.

## Voir aussi

- [Créer des règles]({{% relref "calaos_installer/rules" %}}) — pour déclencher vos scénarios automatiquement
- [Programmation horaire]({{% relref "calaos_installer/time" %}}) — pour les déclencher à heure fixe
- [Variable interne]({{% relref "calaos_installer/internal_var" %}}) — pour mémoriser quel scénario est actif
