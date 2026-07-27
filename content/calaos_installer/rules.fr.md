+++
title = 'Créer des règles'
date = 2024-03-06T20:13:01Z
weight = 60
summary = "Le cœur de la domotique : conditions, actions, et comment les combiner."
+++

## Le principe

Une règle, c'est une phrase : **si telles conditions sont vraies, alors fais telles actions**.

C'est le seul mécanisme qui fait réagir votre installation. Sans règle, un interrupteur n'allume rien : les deux équipements existent, mais rien ne les relie.

```text
┌──────────────┐        ┌──────────────┐
│  CONDITIONS  │  ───▶  │   ACTIONS    │
│  si vrai...  │        │  ...alors    │
└──────────────┘        └──────────────┘
```

## Créer une règle

Cliquez sur le bouton **+** au-dessus de la liste des règles, puis donnez un nom et un type — le type sert uniquement à classer vos règles, il n'a aucun effet sur leur fonctionnement.

![Ajouter une règle](/en/getting_started/images/add_rule.png?width=20pc&classes=shadow)

Ensuite, **glissez-déposez** les équipements depuis l'arborescence : l'interrupteur dans la zone des conditions, la lampe dans celle des actions.

![Règle simple](/en/getting_started/images/simple_rule.png?width=20pc&classes=shadow)

Vous venez de créer un télérupteur : l'appui sur l'interrupteur bascule la lampe.

{{% notice tip %}}
Nommez vos règles par ce qu'elles font, pas par les équipements concernés. « Éteindre le garage après 5 minutes » se relit sans effort dans deux ans ; « Règle 12 » ou « Garage » ne dit rien.
{{% /notice %}}

## Les conditions

Une condition porte sur l'état d'une IO. Quand plusieurs conditions sont présentes, **toutes doivent être vraies** pour que les actions s'exécutent.

Pour comparer une valeur, six opérateurs sont disponibles :

| Opérateur | Signification |
|---|---|
| `==` | est égal à |
| `!=` | est différent de |
| `>` | est supérieur à |
| `>=` | est supérieur ou égal à |
| `<` | est inférieur à |
| `<=` | est inférieur ou égal à |

Une condition sur une température s'écrit ainsi « Sonde salon `<` 19 ».

Deux conditions particulières complètent les comparaisons :

**La condition de démarrage** est vraie une fois, au lancement du serveur. Elle sert à remettre l'installation dans un état connu après un redémarrage : replacer un mode, éteindre ce qui doit l'être.

**La condition script** évalue un bout de code Lua, pour les cas qu'une comparaison n'exprime pas — voir [Scripts en Lua]({{% relref "calaos_installer/scripts" %}}).

## Les actions

Une action agit sur une IO ou déclenche autre chose. Plusieurs types coexistent :

| Type | Effet |
|---|---|
| **Standard** | Change l'état d'une IO : allumer, éteindre, ouvrir, écrire une valeur |
| **Email** | Envoie un message |
| **Notification** | Envoie une notification sur les téléphones |
| **Script** | Exécute du code Lua |
| **Écran tactile** | Agit sur l'affichage local |

Les actions d'une règle s'exécutent **dans l'ordre où elles apparaissent**.

L'envoi d'emails suppose d'avoir configuré le serveur d'envoi au préalable : voir [Email]({{% relref "calaos_os/configuration/email" %}}).

## Combiner avec le reste

Les règles prennent toute leur puissance en s'appuyant sur les autres objets du chapitre.

**Avec une plage horaire** — ajoutez-la en condition, et l'automatisme ne s'applique que la nuit, ou qu'en semaine. Voir [Programmation horaire]({{% relref "calaos_installer/time" %}}).

**Avec un scénario** — mettez-le en action, et votre règle horaire tient en une ligne au lieu de quinze. Voir [Scenario]({{% relref "calaos_installer/scenario" %}}).

**Avec une variable interne** — mémorisez un état entre deux déclenchements, pour éviter qu'une règle se répète. Voir [Variable interne]({{% relref "calaos_installer/internal_var" %}}).

## Quelques montages classiques

**Éteindre après un délai** — une règle démarre un minuteur quand la lumière s'allume ; une autre éteint la lumière à l'échéance du minuteur.

**Détecteur de mouvement, la nuit seulement** — conditions : le détecteur passe à vrai **et** la plage horaire « nuit » est active. Action : allumer.

**Remise en état au démarrage** — condition de démarrage, actions : rétablir les modes et éteindre ce qui doit l'être.

**Alerte** — condition : la sonde de congélateur dépasse un seuil. Actions : notification et email.

## Les pièges courants

**La règle qui se déclenche en boucle.** Si une règle modifie une IO qui figure dans ses propres conditions, elle peut se rappeler indéfiniment. Utilisez une variable interne pour mémoriser que l'action a déjà eu lieu.

**Les conditions trop larges.** Plusieurs règles qui agissent sur la même lampe finissent par se contredire. Regardez d'abord si une règle existante ne devrait pas être complétée plutôt qu'une nouvelle ajoutée.

**Confondre « allumer » et « basculer ».** Un interrupteur en condition d'une règle qui *allume* laissera la lumière allumée au second appui. Pour un télérupteur, c'est le basculement qu'il faut choisir.

## Tester

Les règles ne s'appliquent qu'une fois la configuration **envoyée au serveur** — voir [Envoyer le projet]({{% relref "getting_started/upload" %}}).

Si une règle ne se comporte pas comme prévu, le journal d'événements du serveur montre les changements d'état dans l'ordre : voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}).
