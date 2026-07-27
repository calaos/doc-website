+++
title = 'Configuration Wago'
date = 2024-03-06T20:13:28Z
weight = 80
summary = "Déclarer l'automate dans le projet et lui envoyer son programme."
+++

## Ce que fait Calaos Installer

L'automate Wago est le socle de la plupart des installations Calaos : c'est lui qui lit les interrupteurs et commande les circuits électriques.

Calaos Installer intervient à deux niveaux :

- **déclarer l'automate** dans votre projet, pour que le serveur sache le joindre ;
- **programmer l'automate**, c'est-à-dire lui envoyer le programme Calaos qu'il doit exécuter.

{{% notice info %}}
Cette page traite du **logiciel**. Pour le choix des modules, le câblage et le raccordement électrique, voyez [Automate Wago]({{% relref "hardware/wago" %}}) dans le chapitre matériel.
{{% /notice %}}

## Avant de commencer

L'automate doit avoir une **adresse IP fixe** et être joignable sur votre réseau. Ce réglage se fait côté Wago, avant toute chose, et il est décrit dans [Installation Calaos]({{% relref "hardware/wago/codesys" %}}).

Pour vérifier, ouvrez simplement `http://adresse-de-l-automate` dans un navigateur : la page web de l'automate doit s'afficher.

{{% notice tip %}}
Notez cette adresse : elle vous servira aussi bien pour la programmation que pour la déclaration dans le projet. Et donnez au serveur Calaos une adresse fixe également — voir [Réseau]({{% relref "calaos_os/configuration/network" %}}).
{{% /notice %}}

## Programmer l'automate

Un automate sorti du carton ne sait rien faire : il lui faut le programme Calaos.

Deux voies existent, décrites en détail dans [Installation Calaos]({{% relref "hardware/wago/codesys" %}}) :

- **depuis Calaos Installer**, qui téléverse le programme directement ;
- **avec Codesys**, l'outil de programmation Wago, pour les cas particuliers.

La première suffit dans la grande majorité des installations.

{{% notice warning %}}
Le programme doit correspondre au **modèle exact de votre tête d'automate**. Un programme prévu pour une autre référence ne fonctionnera pas correctement.
{{% /notice %}}

## Déclarer les équipements

Une fois l'automate programmé et joignable, vous ajoutez vos équipements comme n'importe quelle IO : **Ajouter → Wago PLC**, puis le type voulu — lumière, volet, interrupteur, entrée analogique.

Chaque IO demande l'**adresse** de l'entrée ou de la sortie correspondante sur l'automate. Cette adresse dépend de l'ordre des modules sur le rail : elle se déduit du plan de votre coffret.

![Nouvelle lumière](/en/getting_started/images/wago_light.png?width=20pc&classes=shadow)

Le détail des adresses selon les modules figure dans les pages matériel : [entrées]({{% relref "hardware/wago/input" %}}), [sorties]({{% relref "hardware/wago/output" %}}) et [entrées analogiques]({{% relref "hardware/wago/analog" %}}).

{{% notice tip %}}
Tenez à jour un tableau associant chaque borne du coffret à son équipement, au fur et à mesure du câblage. C'est fastidieux sur le moment, et c'est ce qui vous sauvera lors de la première panne ou d'un ajout deux ans plus tard.
{{% /notice %}}

## Le mode de secours

L'automate embarque un programme autonome qui prend le relais **si la communication avec le serveur Calaos est interrompue**. Les interrupteurs continuent alors de commander directement les circuits, comme dans une installation électrique traditionnelle.

C'est une sécurité importante : une panne du serveur ne laisse pas la maison sans éclairage. Le fonctionnement de ce mode est décrit dans [Mode failsafe]({{% relref "hardware/wago/failsafe" %}}).

## Mettre à jour le firmware

Calaos Installer permet également de mettre à jour le firmware de l'automate. Cette opération est plus rare et plus délicate que le simple envoi du programme.

{{% notice warning %}}
Ne coupez ni l'alimentation ni le réseau pendant une mise à jour de firmware : un automate interrompu en cours d'opération peut devenir inutilisable.
{{% /notice %}}

## Si l'automate ne répond pas

Vérifiez dans l'ordre :

1. **la page web de l'automate** s'ouvre-t-elle depuis votre ordinateur ?
2. **l'adresse déclarée dans le projet** correspond-elle à l'adresse réelle ?
3. **le serveur Calaos et l'automate** sont-ils bien sur le même réseau ?
4. **le programme Calaos** a-t-il bien été chargé sur l'automate ?

Si l'automate répond mais que les équipements ne réagissent pas, l'erreur vient plus probablement des **adresses d'entrées/sorties** déclarées dans le projet que de la communication elle-même.

Les journaux du serveur indiquent les erreurs de communication : voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}).
