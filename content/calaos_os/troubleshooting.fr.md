+++
title = 'En cas de problème'
date = 2026-07-25T10:00:00Z
weight = 70
summary = "Les pannes courantes, et quoi faire dans chaque cas."
+++

## Comment utiliser cette page

Cherchez le symptôme qui ressemble le plus à ce que vous constatez, et suivez les vérifications dans l'ordre indiqué. Elles vont du plus simple au plus compliqué : ne sautez pas les premières, ce sont souvent les bonnes.

---

## L'ordinateur ne démarre pas sur la clé USB

**Vérifiez d'abord** que la clé est bien branchée, de préférence sur un port USB à l'arrière de la machine, et que l'ordinateur est complètement éteint avant de le rallumer.

**Appelez le menu de démarrage.** Dès l'allumage, appuyez plusieurs fois sur la touche du menu de démarrage : le plus souvent **F12**, parfois **F11**, **F9**, **F8** ou **Échap**. Si vous ne savez pas laquelle, essayez-les l'une après l'autre en redémarrant à chaque fois.

**Désactivez le Secure Boot.** Certains ordinateurs refusent de démarrer sur autre chose que leur système d'origine. Entrez dans les réglages de la machine — touche **Suppr** ou **F2** à l'allumage — et cherchez une option nommée *Secure Boot*, à désactiver.

**Refaites la clé.** Une clé mal copiée ne démarre pas. Recommencez la préparation décrite dans [Installation]({{% relref "calaos_os/installation" %}}), et essayez si possible une autre clé.

---

## Le premier démarrage semble bloqué

Vous voyez l'écran d'accueil Calaos OS et le message *« Calaos-OS is currently initializing. Please wait... »*, et plus rien ne se passe depuis longtemps.

**C'est normal, et il ne faut surtout pas éteindre la machine.** Au tout premier démarrage, Calaos OS décompresse et installe tous ses services. Sur une clé USB lente, cette étape peut durer **de 30 à 60 minutes**.

Laissez faire. L'opération n'a lieu qu'une fois : les démarrages suivants sont rapides.

Si vous avez éteint la machine en cours de route, rallumez-la simplement — l'installation des services reprendra.

{{% notice tip %}}
Pour la prochaine fois : la durée dépend presque entièrement de la vitesse de votre clé USB. Une clé USB 3.0 de marque ramène cette attente à quelques minutes. Voir [De quoi ai-je besoin ?]({{% relref "calaos_os/requirements" %}}).
{{% /notice %}}

---

## Le serveur ne démarre plus depuis une mise à jour

C'est exactement la situation pour laquelle les points de restauration existent : votre système d'avant est toujours là.

Rendez-vous sur [Revenir en arrière]({{% relref "calaos_os/rollback" %}}) et suivez la procédure. Elle prend quelques minutes et ne demande aucune compétence particulière.

---

## Je ne trouve plus mon serveur sur le réseau

**Le serveur est-il allumé ?** Vérifiez les voyants de la machine, et le voyant du port réseau sur la box ou le switch.

**Son adresse a-t-elle changé ?** C'est la cause la plus fréquente, surtout après une coupure de courant. Regardez la liste des appareils connectés dans l'interface de votre box : votre serveur devrait y figurer, avec sa nouvelle adresse.

**Pour éviter que cela recommence**, donnez-lui une adresse fixe : voir [Réseau]({{% relref "calaos_os/configuration/network" %}}).

**Si vous avez accès à la machine**, un écran et un clavier branchés dessus vous permettent de vérifier sa configuration réseau avec :

```sh
calaos-os network list
```

---

## Mes applications ne se connectent plus au serveur

**Avez-vous changé les identifiants récemment ?** Après un changement, toutes les applications doivent être reconfigurées avec les nouveaux : Calaos Installer, les applications mobiles, la web app. Voir [Changer les mots de passe]({{% relref "calaos_os/security" %}}).

**Le serveur répond-il ?** Testez depuis un ordinateur du même réseau. Si le serveur ne répond pas du tout, voyez le symptôme précédent.

**Depuis l'extérieur uniquement ?** Le problème vient probablement de votre accès distant : voir [DNS dynamique]({{% relref "calaos_os/configuration/calaos_ddns" %}}).

---

## L'écran tactile reste noir ou ne répond pas

Voyez la page [Écran tactile]({{% relref "calaos_os/configuration/touchscreen" %}}), qui détaille les vérifications à faire.

Rappelez-vous que **le serveur fonctionne même sans écran** : si votre domotique répond depuis les applications mobiles, le problème est purement lié à l'affichage, et votre installation n'est pas en danger.

---

## Une partie de mon installation ne répond plus

Si seuls certains équipements ne répondent plus — vos ampoules Zigbee, vos mesures de température — c'est probablement le service correspondant qui est en cause, pas Calaos dans son ensemble.

Connectez-vous en SSH et regardez ce qui a échoué :

```sh
systemctl --failed
```

Cette commande couvre **tous** les services, y compris ceux des équipements Zigbee, MQTT ou des historiques.

Puis consultez les journaux du service concerné, comme décrit dans [Journaux]({{% relref "calaos_os/configuration/logs" %}}).

Un redémarrage du service suffit souvent : voir [Services]({{% relref "calaos_os/configuration/services" %}}).

---

## Le disque est plein

Trois choses occupent de la place et grandissent avec le temps : les points de restauration, l'historique de vos mesures et les journaux.

**Voir ce qui occupe le disque :**

```sh
df -h /
```

**Réduire les journaux**, si ce sont eux :

```sh
journalctl --disk-usage
journalctl --vacuum-time=30d
```

**Les points de restauration** sont normalement nettoyés automatiquement : Calaos OS n'en conserve qu'un nombre limité et supprime les plus anciens tout seul. Si le disque se remplit malgré tout, voyez [Sous le capot]({{% relref "calaos_os/advanced/btrfs" %}}).

{{% notice warning %}}
Ne supprimez pas de fichiers au hasard pour faire de la place, en particulier dans `/mnt/calaos` : c'est là que vit toute votre configuration.
{{% /notice %}}

---

## L'heure est fausse, mes règles se déclenchent au mauvais moment

Voir [Date & Heure]({{% relref "calaos_os/configuration/date" %}}). Une heure décalée provoque aussi des refus de connexion des écrans Remote UI.

---

## Demander de l'aide

Si rien de ce qui précède ne règle votre problème, le forum Calaos et le [dépôt GitHub](https://github.com/calaos/) sont là pour ça.

Pour obtenir une réponse utile du premier coup, précisez :

- **ce que vous constatez**, et depuis quand ;
- **ce qui a changé juste avant** : une mise à jour, un nouvel équipement, une coupure de courant ;
- **l'heure approximative** du problème ;
- **les journaux** correspondants, récupérés comme expliqué dans [Journaux]({{% relref "calaos_os/configuration/logs" %}}).

{{% notice tip %}}
Avant de tout réinstaller, demandez : une réinstallation fait perdre du temps et efface souvent les traces qui auraient permis de comprendre la panne. Dans la plupart des cas, un retour en arrière ou le redémarrage d'un service suffit.
{{% /notice %}}
