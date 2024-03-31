+++
title = 'Mode dégradé'
date = 2024-03-06T20:03:52Z
weight = 20
+++

## Mode dégradé de l'automate Wago

Le mode dégradé dans Calaos est un état de fonctionnement alternatif qui est activé lorsque le serveur Calaos ne peut plus communiquer avec l'automate, par exemple en cas de panne réseau. L'automate passe en mode dégradé après une certaine période (30 secondes) et permet de continuer à piloter certains éléments comme les points lumineux et les volets à l'aide des interrupteurs, bien que toutes les règles du mode serveur ne soient pas gérables. Le mode dégradé ne peut exécuter que quelques règles simples automatiquement générées par le logiciel Calaos Installer lors de la configuration de l'automate. Les entrées peuvent être associées à des actions spécifiques telles que télérupteur, direct, volet, etc., avec des paramètres définis pour chaque action.

## LED

La LED USR sur un automate Wago avec Calaos indique l'état de fonctionnement :

- **Vert** : Tout fonctionne normalement. L'automate communique avec le serveur Calaos, et toutes les fonctions configurées sont actives.
- **Orange** : L'automate est en mode dégradé, souvent dû à une coupure de la communication avec le serveur. Dans ce mode, il y a des limitations : certaines fonctions de base restent gérables manuellement, comme l'éclairage ou les volets, mais sans la totalité des commandes et règles habituelles.

## Regles supportées

Voici la liste des règles supportés dans le mode dégradé:

|Type|Notes|
|-|-|
|Télérupteur|Agit comme un télérupteur et bascule la sortie à 0 ou 1 à chaque appui|
|Direct|Met la sortie à 1 tant que l'entrée est à 1 et à 0 dès qu'on relache|
|Volet|Pilote un volet classique avec 2 sorties montée/descente|
|Volet impulsion|Pilote un volet avec une impulsion au lieu d'un maintien des relais|
|Télérupteur DALI|Pilote une lumière DALI en ON/OFF|
|Télérupteur groupe DALI|Pilote un groupe DALI en ON/OFF|
|Télérupteur KNX|Pilote une sortie KNX (uniquement pour les installations KNX/Wago)|
|Direct KNX|Idem que Direct mais en KNX|

## Programmer le mode dégradé

Afin de programmer la configuration dégradé dans l'automate, le plus simple est d'utiliser [Calaos Installer]({{%relref "calaos_installer" %}}) en utilisant l'outil **Envoyer programme dégradé** dans le menu **Wago**. Il faudra être connecté à l'automate au préalable.

{{% notice note %}}
Calaos Installer va automatiquement envoyer et _convertir_ les règles configurés. Il faut garder à l'esprit que les regles complexes ne sont pas convertis et ne seront donc pas utilisé en mode dégradé. Cela peut laisser des interrupteurs sans fonction en mode dégradé. Une astuce c'est d'utiliser une 2eme configuration uniquement pour le mode dégradé dans Calaos Installer. Cette configuration ne devra pas être envoyé à Calaos Server, mais uniquement à l'automate.
{{% /notice %}}

## Programmer le mode dégradé en console

Il est possible d'utiliser l'outil `wago_test` pour programmer le mode dégradé. L'outil est capable d'envoyer et de recevoir une config complete ou bien de configurer une entrée uniquement.

Pour récupérer la config:

    wago_test host 192.168.0.xxx action save_config failsafe_config.txt

Pour envoyer la config:

    wago_test host 192.168.0.xxx action load_config failsafe_config.txt

Le fichier contient une configuration de cette forme:

    WAGO_SET_OUTTYPE 0 0
    WAGO_SET_OUTADDR 0 0 0 -1
    WAGO_SET_OUTTYPE 1 1
    WAGO_SET_OUTADDR 1 25 0 -1
    WAGO_SET_OUTTYPE 2 3
    WAGO_SET_OUTADDR 2 30 31 -1
    WAGO_SET_OUTTYPE 3 1
    WAGO_SET_OUTADDR 3 0 0 1

Pour changer la config d'une entrée:

    wago_test host 192.168.0.xxx action set_outtype 2 VOLET
    wago_test host 192.168.0.xxx action set_outaddr 2 1 2 -1

En mode dégradé chaque entrée est associée à une action d'un type suivant:

|Type|Notes|
|-|-|
|NONE|Action par défaut, ne fait rien|
|TELERUPTEUR|Agit comme un télérupteur et bascule la sortie à 0 ou 1 à chaque appui|
|DIRECT|Met la sortie à 1 tant que l'entrée est à 1 et à 0 dès qu'on relache|
|VOLET|Pilote un volet classique avec 2 sorties montée/descente|
|VOLET IMPULSE|Pilote un volet avec une impulsion au lieu d'un maintien des relais|
|TELERUPTEUR DALI|Pilote une lumière DALI en ON/OFF|
|TELERUPTEUR DALI GROUP|Pilote un groupe DALI en ON/OFF|
|TELERUPTEUR KNX|Pilote une sortie KNX (uniquement pour les installations KNX/Wago)|
|DIRECT KNX|Idem que Direct mais en KNX|

Ensuite chaque entrée est associée à 3 paramètres:

- **out1**: adresse de sortie. Par exemple l'adresse de la lampe pour le type TELERUPTEUR, l'adresse de la montée du volet pour le type VOLET, l'adresse DALI pour le type DALI, etc.
- **out2**: 2e adresse de sortie si besoin (utilisé que dans le cas des volets pour la descente)
- **SameAs**: fonction spéciale lors de plusieurs actions sur la même sortie

 On peut avoir cette série de règles :

    Input 0 -> NONE
    Input 0 -> out1=0  out2=0  SameAs=-1
    Input 1 -> TELERUPTEUR
    Input 1 -> out1=25 out2=0  SameAs=-1
    Input 2 -> VOLET
    Input 2 -> out1=30 out2=31 SameAs=-1
    Input 3 -> TELERUPTEUR
    Input 3 -> out1=0  out2=0  SameAs=1

Ici on a l'entrée 0 qui ne fait rien. L'entrée 1 pilote une lumière sur la sortie 25. L'entrée 2 pilote un volet avec la montée à l'adresse 30 et la descente à l'adresse 31. L'entrée 3 est particulière. Elle est configurée comme un télérupteur (comme l'entrée 1) mais les adresses sont à 0 et le SameAs est à 1. Cela signifie que l'entrée 3 va piloter la même lumière que l'entrée 1. En utilisant le même bloc fonction (et le même état de la lampe). Ça permet d'avoir plusieurs inter qui pilotent la même lampe/volet. Attention, il faut que le SameAs soit -1 si il n'est pas utilisé.
