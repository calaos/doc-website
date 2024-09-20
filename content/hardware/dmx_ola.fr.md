+++
title = 'DMX avec OLA'
date = 2024-03-06T20:04:48Z
weight = 50
summary = 'Variation de lumières DMX avec Open Light Architecture'
+++

## OLA (Open Lighting Architecture) sur Calaos

### Introduction

**DMX** (Digital Multiplex) est un protocole de communication utilisé principalement dans le contrôle de l'éclairage scénique, des effets lumineux et autres équipements d'automatisation. Il permet de contrôler de nombreux appareils d'éclairage (projecteurs, gradateurs, etc.) via un seul câble, en envoyant des instructions de lumière à chaque appareil connecté.

**OLA (Open Lighting Architecture)** est une plateforme open-source qui permet de gérer et de contrôler des équipements DMX via divers types d'interfaces, y compris des adaptateurs USB DMX. OLA supporte de nombreux protocoles d'éclairage, tels que DMX512, et offre une interface web conviviale pour la gestion des univers DMX et des dispositifs connectés.

---

### Prérequis

- **Adaptateur DMX USB** : Vous aurez besoin d'un adaptateur DMX compatible pour connecter le protocole DMX à votre serveur Calaos. Voici quelques exemples d'adaptateurs supportés :
  - **Enttec USB DMX Pro**
  - **DMXKing UltraDMX Micro**

Pour une liste complète des interfaces DMX supportées, consultez la documentation officielle d'OLA.

---

### Activation et Configuration de OLA sur Calaos

#### Activation de OLA

OLA est déjà inclus dans **Calaos OS**. Pour l'activer, exécutez la commande suivante dans le terminal de votre serveur Calaos :

```bash
sudo systemctl enable olad.service
sudo systemctl start olad.service
```

---

### Configuration de OLA via l'Interface Web

1. **Accéder à l'Interface Web OLA**

   - Ouvrez un navigateur web et accédez à `http://<adresse-ip-de-calaos>:9090`.
   - L'interface web d'OLA vous permet de gérer les univers DMX et de configurer les interfaces connectées.

2. **Configurer l'Universe**

   - Dans la section **"Home"**, clickez sur **"Add Universe"**.
   - Attribuez un numéro d'univers (généralement 1 pour le premier univers) ainsi qu'un nom.
   - Assurez-vous que l'adaptateur USB est assigné à cet univers pour permettre le contrôle DMX à travers OLA. Pour cela il faudra cocher l'adaptateur choisi dans la liste. Si il n'est pas dans la liste, c'est qu'il n'a pas été détecté par OLA.

3. **Tester le Device DMX**

   - Sur la page d'accueil vous pouvez aller sur l'univers crée et cliquer sur l'onglet **"DMX Console"**.
   - Vous pouvez utiliser cette console pour tester vos canaux DMX en envoyant des valeurs DMX aux différents canaux pour vérifier que votre équipement DMX fonctionne correctement.

---

### Ajouter le Dispositif DMX dans Calaos Installer

1. **Ajouter un IO OLA DMX**

    Il est possible de créer un variateur simple ou RGB dans Calaos Installer. Il faut aller dans le menu **"Open Light Architecture"** pour cela.

2. **Configurer l'Entrée/Sortie**

   - **Channel** : Sélectionnez le canal DMX que vous souhaitez contrôler (par exemple, canal 1 pour la première lampe, 2 pour la seconde, etc.).
   - **Universe** : Indiquez l'univers DMX correspondant (généralement 1 si vous n'avez qu'un univers configuré).

---

## Ressources Supplémentaires

- **Documentation OLA** : [Open Lighting Architecture](https://www.openlighting.org/)
