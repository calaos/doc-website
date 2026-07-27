+++
title = 'Caméras'
date = 2024-03-06T20:05:19Z
weight = 90
summary = 'Caméra MJPEG'
+++

## Introduction

Calaos affiche les images de vos caméras IP dans ses interfaces : sur l'écran tactile, dans les applications mobiles et dans la web app. Une caméra peut aussi être **jointe à un email** envoyé par une règle, pour recevoir une photo au moment d'un événement.

Le flux utilisé est du **MJPEG**, un format simple que la quasi-totalité des caméras IP sait produire.

## Modèles supportés

Calaos connaît directement plusieurs familles de caméras, et sait comment leur demander une image :

| Famille | Remarque |
|---|---|
| **MJPEG standard** | Fonctionne avec toute caméra exposant un flux MJPEG |
| **Axis** | — |
| **Foscam** | — |
| **Gadspot** | — |
| **Planet** | — |
| **Synology Surveillance Station** | Passe par le serveur de surveillance plutôt que par la caméra |
| **Reolink** | Voir la section dédiée plus bas |

{{% notice tip %}}
Si votre modèle n'est pas dans la liste, essayez le type **MJPEG standard** : il suffit de connaître l'adresse du flux, que le fabricant documente généralement. Beaucoup de caméras non listées fonctionnent ainsi.
{{% /notice %}}

## Ajouter une caméra

Dans Calaos Installer, **Ajouter → Caméra**, puis choisissez la famille correspondant à votre modèle. Renseignez son adresse sur le réseau et, si elle en demande, ses identifiants.

{{% notice warning %}}
**Changez le mot de passe par défaut de vos caméras.** C'est le premier équipement visé sur un réseau domestique, et une caméra restée sur ses identifiants d'usine est accessible à quiconque atteint votre réseau.

Ne l'exposez pas directement sur Internet — voir [DNS dynamique]({{% relref "calaos_os/configuration/calaos_ddns" %}}) pour un accès distant maîtrisé.
{{% /notice %}}

## Ajuster l'image

Deux paramètres permettent d'adapter l'image à l'affichage, sans toucher aux réglages de la caméra :

| Nom | Type | Requis | Description |
|---|---|---|---|
| `rotate` | int | non | Rotation de l'image en degrés. Par exemple `90` pour une rotation horaire, `-90` pour une rotation antihoraire |
| `width` | int | non | Largeur souhaitée en pixels. L'image est redimensionnée en conservant ses proportions. Laisser vide pour garder la taille d'origine |

`width` est utile sur les écrans muraux : réduire une image de caméra 4K à la largeur réellement affichée allège nettement le réseau et l'affichage.

## Détection d'événements Reolink

Les caméras **Reolink** vont plus loin que la simple image : elles savent signaler ce qu'elles détectent. Calaos peut transformer ces événements en entrée, utilisable comme n'importe quel interrupteur dans vos règles.

| Nom | Type | Requis | Description |
|---|---|---|---|
| `hostname` | string | oui | Adresse IP ou nom de la caméra |
| `username` | string | oui | Identifiant de connexion à la caméra |
| `password` | string | oui | Mot de passe |
| `event_type` | liste | oui | Type d'événement à écouter. Par défaut `motion` |

Les types d'événements disponibles :

| Valeur | Détection |
|---|---|
| `motion` | Mouvement |
| `person` | Personne |
| `vehicle` | Véhicule |
| `pet` | Animal |
| `cry` | Pleurs |
| `face` | Visage |
| `package` | Colis |
| `visitor` | Visiteur / sonnette |

{{% notice tip %}}
Ces détections spécialisées valent bien mieux que le simple mouvement : une règle déclenchée sur `person` ignore les branches agitées par le vent et les chats du voisinage, là où `motion` vous enverra des notifications toute la nuit.
{{% /notice %}}

Une entrée Reolink s'utilise ensuite comme un détecteur classique : allumer un éclairage extérieur à l'arrivée d'une personne, envoyer une notification pour la sonnette, enregistrer l'événement dans l'historique. Voir [Créer des règles]({{% relref "calaos_installer/rules" %}}).

## Envoyer une image par email

Une action d'email peut joindre l'image d'une caméra au moment où la règle se déclenche. C'est le moyen le plus simple de recevoir une photo lors d'une détection.

L'envoi d'emails doit être configuré au préalable : voir [Email]({{% relref "calaos_os/configuration/email" %}}).

## Diagnostic

Si l'image ne s'affiche pas :

1. **la caméra est-elle joignable** depuis le serveur Calaos, à l'adresse indiquée ?
2. **le flux s'ouvre-t-il** dans un navigateur, avec l'adresse complète du flux MJPEG ?
3. **les identifiants** sont-ils corrects ? Beaucoup de caméras refusent silencieusement une requête mal authentifiée.
4. **le modèle sélectionné** correspond-il ? En cas de doute, essayez le type MJPEG standard.

Voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}) pour les erreurs côté serveur.
