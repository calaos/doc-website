+++
title = 'Email'
date = 2024-03-06T20:16:01Z
weight = 90
summary = "Configurer l'envoi d'emails pour les notifications de vos règles."
+++

## À quoi ça sert

Votre serveur peut vous envoyer des emails depuis vos règles : prévenir qu'une alarme s'est déclenchée, qu'une porte est restée ouverte, qu'une température est anormale, ou envoyer une photo prise par une caméra.

Pour cela, il faut lui indiquer par quel service d'envoi passer : c'est ce qu'on appelle un **serveur SMTP**. En pratique, celui de votre fournisseur d'accès ou de votre messagerie.

Ces réglages ne sont pas disponibles depuis l'écran tactile : connectez-vous en SSH (voir [Accès SSH]({{% relref "calaos_os/advanced/shell" %}})).

## Ce qu'il vous faut

Avant de commencer, munissez-vous des informations fournies par votre service de messagerie :

- l'**adresse du serveur d'envoi**, du type `smtp.exemple.fr` ;
- le **port** à utiliser, souvent 587 ou 465 ;
- votre **identifiant** et votre **mot de passe**, si le service demande une authentification ;
- l'information de savoir si la connexion doit être **chiffrée** (TLS), ce qui est le cas presque partout aujourd'hui.

Ces informations figurent dans l'aide de votre fournisseur, souvent sous l'intitulé « configurer un logiciel de messagerie ».

## Configurer

Les réglages se définissent avec l'outil [calaos_config]({{% relref "calaos_os/configuration/calaos_config" %}}) :

```sh
calaos_config set smtp_server smtp.exemple.fr
calaos_config set smtp_port 587
calaos_config set smtp_auth true
calaos_config set smtp_tls true
calaos_config set smtp_username mon_identifiant
calaos_config set smtp_password mon_mot_de_passe
```

Voici le rôle de chaque réglage :

| Réglage | Rôle |
|---|---|
| `smtp_server` | Adresse du serveur d'envoi |
| `smtp_port` | Port du serveur d'envoi |
| `smtp_auth` | `true` si le serveur demande un identifiant et un mot de passe |
| `smtp_tls` | `true` pour une connexion chiffrée |
| `smtp_username` | Votre identifiant |
| `smtp_password` | Votre mot de passe |

Pour vérifier ce qui est enregistré :

```sh
calaos_config list
```

{{% notice tip %}}
Beaucoup de services de messagerie refusent désormais votre mot de passe habituel pour ce type d'usage et réclament un **mot de passe d'application**, à générer depuis votre compte. Si l'envoi échoue alors que vos identifiants sont bons, c'est très probablement la cause.
{{% /notice %}}

## Tester

L'outil `calaos_mail` permet d'envoyer un message d'essai. Préparez d'abord un fichier contenant le texte du message, puis lancez l'envoi :

```sh
echo "Ceci est un test depuis mon serveur Calaos." > /tmp/test.txt
calaos_mail --from moi@exemple.fr --to moi@exemple.fr --subject "Test Calaos" --body /tmp/test.txt
```

Si le message arrive, la configuration est bonne.

En cas d'échec, relancez la commande en ajoutant `--verbose` : le détail des échanges avec le serveur d'envoi s'affiche, ce qui indique généralement quel réglage est en cause.

```sh
calaos_mail --from moi@exemple.fr --to moi@exemple.fr --subject "Test Calaos" --body /tmp/test.txt --verbose
```

{{% notice note %}}
Pensez à vérifier votre dossier de courrier indésirable : les messages envoyés par un serveur domestique y atterrissent souvent la première fois.
{{% /notice %}}

## Joindre des fichiers

L'option `--attach` permet de joindre un fichier, et peut être répétée pour en joindre plusieurs :

```sh
calaos_mail --from moi@exemple.fr --to moi@exemple.fr --subject "Photo" --body /tmp/test.txt --attach /tmp/photo.jpg
```

C'est ce mécanisme qui permet de recevoir par email une image prise par une caméra.

## Utiliser l'email dans vos règles

Une fois l'envoi configuré, vous pouvez déclencher des emails depuis vos règles domotiques. La création des règles est décrite dans [Règles]({{% relref "calaos_installer/rules" %}}), au chapitre Calaos Installer.
