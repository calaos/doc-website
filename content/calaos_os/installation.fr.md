+++
title = 'Installation'
date = 2024-03-06T20:13:48Z
weight = 10
summary = "Préparer la clé USB, essayer, puis installer sur le disque."
+++

## Ce que vous allez faire

L'installation se déroule en trois temps :

1. vous préparez une clé USB contenant Calaos OS ;
2. vous démarrez l'ordinateur sur cette clé, ce qui vous permet d'essayer le système **sans rien installer** ;
3. quand tout vous convient, vous installez sur le disque de l'ordinateur.

Comptez une trentaine de minutes en tout, dont l'essentiel est du temps d'attente.

## 1. Préparer la clé USB

La clé se prépare depuis **Calaos Installer**, le logiciel que vous installez sur votre ordinateur habituel.

Dans le menu **Gestion du projet**, choisissez **Créez une image**. Sélectionnez la dernière version stable de Calaos OS dans la liste, puis cliquez sur **Continuer** : l'image est téléchargée depuis les serveurs de Calaos, ce qui peut prendre quelques minutes selon votre connexion.

Insérez ensuite votre clé USB. Elle apparaît dans la fenêtre : sélectionnez-la et cliquez sur **Continuer** pour que l'image y soit copiée.

{{% notice warning %}}
La clé USB est **entièrement effacée** pendant cette opération. Vérifiez que vous avez bien sélectionné la clé, et pas un disque externe contenant vos documents.
{{% /notice %}}

La procédure détaillée avec les captures d'écran est décrite dans [Installation]({{% relref "getting_started/installation" %}}), au chapitre Démarrage.

## 2. Démarrer l'ordinateur sur la clé

Branchez la clé sur l'ordinateur qui deviendra votre serveur, puis allumez-le.

La plupart des ordinateurs démarrent par défaut sur leur disque interne. Pour leur demander de démarrer sur la clé, il faut appuyer sur une touche dès l'allumage, avant l'apparition du logo du système. Cette touche varie selon la marque : c'est le plus souvent **F12**, parfois **F11**, **F9**, **F8** ou **Échap**. Un menu s'affiche alors, dans lequel vous choisissez la clé USB.

{{% notice tip %}}
Si la clé n'apparaît pas dans la liste, éteignez complètement l'ordinateur, vérifiez que la clé est bien branchée — de préférence sur un port USB à l'arrière de la machine — et recommencez. Si le problème persiste, voyez la page [En cas de problème]({{% relref "calaos_os/troubleshooting" %}}).
{{% /notice %}}

## 3. Choisir entre essayer et installer

Une fois démarré sur la clé, un menu vous propose deux possibilités :

- **démarrer Calaos OS en mode Live**, c'est-à-dire l'exécuter directement depuis la clé USB ;
- **installer Calaos OS** sur le disque de l'ordinateur.

Vous avez cinq secondes pour choisir avec les flèches du clavier, puis la touche Entrée. Sans intervention de votre part, le mode Live démarre.

## 4. Essayer sans rien installer

Le mode **Live** fait tourner un système Calaos OS complet depuis la clé USB.

{{% notice info %}}
**Le disque de l'ordinateur n'est pas touché.** Vous pouvez explorer, configurer, tester votre matériel : rien n'est écrit sur le disque interne. Si le résultat ne vous convient pas, éteignez la machine et retirez la clé, il ne restera aucune trace.
{{% /notice %}}

C'est le moment idéal pour vérifier que votre matériel fonctionne : que la carte réseau est reconnue, que l'écran s'affiche correctement, que vos périphériques USB sont détectés.

Vous pouvez même déjà configurer votre installation domotique — créer vos entrées/sorties, vos règles — car **cette configuration sera reprise lors de l'installation sur le disque** (voir l'étape 6).

## 5. Installer sur le disque

Deux façons de lancer l'installation, selon votre équipement.

### Depuis l'interface Calaos Home

Si un écran tactile est branché sur la machine et que vous avez démarré en mode Live, l'interface **Calaos Home** vous propose d'installer le système sur le disque. C'est la méthode la plus simple : tout se fait à l'écran, sans clavier.

### Depuis le menu de démarrage

Sinon, redémarrez l'ordinateur sur la clé USB et choisissez cette fois l'entrée **Installation** dans le menu du point 3.

Un écran de confirmation vous demande si vous voulez installer Calaos OS. Si vous répondez non, la machine continue simplement de démarrer en mode Live.

Si vous répondez oui, la liste des disques de la machine s'affiche, avec pour chacun son modèle et sa capacité. Sélectionnez celui sur lequel installer, puis validez. La navigation se fait avec les flèches, la barre d'espace pour cocher et la touche Entrée pour valider.

{{% notice warning %}}
**Le disque que vous choisissez est entièrement effacé.** Tout ce qu'il contient est définitivement perdu. Vérifiez bien le modèle et la capacité affichés avant de valider, surtout si la machine contient plusieurs disques.
{{% /notice %}}

## 6. Patienter

L'installation prépare le disque, y copie le système et met en place le démarrage. Elle enregistre aussi le premier point de restauration de votre serveur.

Cette étape dure plusieurs minutes. Ne coupez pas l'alimentation. Quand elle est terminée, l'ordinateur redémarre automatiquement : retirez la clé USB pour qu'il démarre cette fois sur son disque.

{{% notice tip %}}
**Votre configuration du mode Live est conservée.** Tout ce que vous avez configuré pendant l'essai — réglages Calaos, entrées/sorties, règles — se retrouve à l'identique sur le système installé. Vous ne recommencez pas de zéro.
{{% /notice %}}

## 7. Premier démarrage

L'ordinateur démarre désormais sur son disque. Le serveur Calaos se lance automatiquement, ainsi que les services complémentaires.

Si un écran est branché, l'interface Calaos Home s'affiche. Sinon, le serveur est accessible depuis le réseau : c'est depuis Calaos Installer, sur votre ordinateur habituel, que vous allez le retrouver et lui envoyer votre configuration — voir [Envoyer le projet]({{% relref "getting_started/upload" %}}).

{{% notice warning %}}
**Il reste une chose à faire avant d'utiliser votre serveur au quotidien : changer les mots de passe par défaut.** Ils sont identiques sur toutes les installations de Calaos OS et publiés dans cette documentation. Rendez-vous sur [Changer les mots de passe]({{% relref "calaos_os/security" %}}), c'est l'affaire de quelques minutes.
{{% /notice %}}

## Pour aller plus loin

L'installeur ne se contente pas de copier des fichiers. Il :

- partitionne le disque et formate la partition principale en **Btrfs** avec compression ;
- crée une série de sous-volumes qui séparent le système des données variables (journaux, caches) ;
- recopie le système Live tel qu'il tourne, **votre configuration comprise** ;
- exporte les images des containers vers un cache local, ce qui permet au serveur de démarrer une première fois **sans accès à Internet** ;
- installe le chargeur de démarrage et crée le point de restauration numéro 1, daté de l'installation.

Le détail de l'organisation du disque est décrit dans [Sous le capot]({{% relref "calaos_os/advanced/btrfs" %}}).
