+++
title = 'Téléphone approbateur et console web'
date = 2026-10-24T06:00:00Z
weight = 23
summary = "Enrôler votre téléphone comme approbateur, ouvrir la console root depuis l'application web, et réagir à la perte du téléphone."
+++

## Pourquoi un téléphone approbateur

Certaines actions donnent le contrôle total de la box. Pour celles-ci, le mot de passe admin ne suffit pas : la box envoie aussi un **code à 6 chiffres** à un téléphone que vous avez enrôlé à l'avance, le **téléphone approbateur**, et vous saisissez ce code dans l'application web. Quelqu'un qui aurait découvert votre mot de passe admin ne peut donc rien faire sans avoir votre téléphone.

Le code protège :

- l'ouverture de la **console** (un terminal root sur la box, dans votre navigateur) ;
- le changement du **mot de passe admin** depuis l'application web ;
- l'ajout ou le retrait d'un téléphone approbateur.

Les mises à jour et le redémarrage de la box restent protégés par le seul mot de passe admin. Voir [Administrer depuis l'application web]({{% relref "calaos_os/admin" %}}).

Un code est valable 60 secondes et ne sert qu'une fois. Rien reçu ? Vérifiez que les notifications de l'application Calaos sont autorisées, puis redemandez un code au bout de 10 secondes. Après 5 codes incorrects, la session admin se ferme et il faut saisir à nouveau le mot de passe admin.

{{% notice info %}}
Sans téléphone approbateur, la console ne peut pas s'ouvrir et le mot de passe admin ne se change qu'en SSH avec `calaos-os admin set`.
{{% /notice %}}

## Enrôler votre téléphone

Il faut l'application Calaos sur le téléphone, connectée à cette box, avec les notifications autorisées : les codes arrivent par notification push.

1. Dans l'application Calaos, ouvrez les **Options** et renseignez **Nom de cet appareil**, par exemple `iPhone de Marie`. Depuis iOS 16, tous les iPhone s'appellent simplement « iPhone » sinon, et vous ne pourriez pas distinguer deux téléphones. Le nom est envoyé tout de suite à la box. Faites-le **avant** l'enrôlement : la box garde le nom que le téléphone avait à ce moment. Laissez le champ vide pour garder le nom du téléphone.
2. Dans l'application web, ouvrez l'onglet **Admin** (en https, voir [Administrer depuis l'application web]({{% relref "calaos_os/admin" %}})) et déverrouillez-le.
3. Dans la section **Téléphones approbateurs**, cliquez sur **Ajouter un téléphone**. La liste montre les appareils où l'application est connectée à cette box. Si plusieurs portent le même nom, fiez-vous à la date et à la dernière activité.
4. Choisissez votre téléphone et cliquez sur **Envoyer un code à ce téléphone**.
5. Saisissez le code reçu sur le téléphone dans le champ **Code à 6 chiffres** et cliquez sur **Valider**. Cela prouve que vous avez le téléphone en main.

Le téléphone apparaît alors dans la liste. Tous les téléphones connectés à la box reçoivent une notification, et l'application web affiche une alerte de sécurité à la prochaine connexion : **Nouveau téléphone approbateur : « nom »** ; le retrait d'un téléphone affiche **Téléphone approbateur retiré : « nom »**. Si ce n'est pas vous, quelqu'un connaît le mot de passe admin : voir [Téléphone perdu](#téléphone-perdu-ou-réinstallé).

Vous pouvez enrôler **deux** téléphones au plus. Pour ajouter le second, le premier reçoit un code pour l'autoriser. Pour en retirer un, cliquez sur **Retirer** à côté de lui, confirmez avec **Envoyer le code**, puis saisissez le code ; le dernier téléphone ne peut pas être retiré, donc pour changer de téléphone, ajoutez d'abord le nouveau, puis retirez l'ancien.

{{% notice warning %}}
Un téléphone dont l'application ne s'est pas connectée à la box depuis 30 jours ne reçoit plus de codes. La liste vous invite alors à ouvrir l'application Calaos sur ce téléphone : faites-le, et il reçoit de nouveau les codes.
{{% /notice %}}

## Ouvrir la console

La console est un terminal sur la box, avec les droits root, dans votre navigateur. Elle évite le SSH pour une vérification rapide.

1. Ouvrez l'application web en **https://** et déverrouillez l'onglet **Admin**.
2. Dans la section **Box**, ligne **Console**, cliquez sur **Ouvrir**.
3. Un code est envoyé à votre téléphone approbateur. Saisissez-le dans le champ **Code à 6 chiffres** et cliquez sur **Valider**.
4. Le terminal s'ouvre. Un compteur indique quand il se fermera.

Chaque ouverture déclenche une notification **Calaos: console opened** sur tous les téléphones connectés à la box.

Limites :

| Limite | Valeur |
|---|---|
| Inactivité | fermeture après **15 minutes** sans saisie |
| Durée totale | **1 heure**, même si vous continuez à taper |
| En même temps | **2 consoles** sur la box |
| Connexion | **https** uniquement, jamais en http simple |

Rien n'est conservé à la fermeture : ouvrez une nouvelle console avec un nouveau code.

### Quand la console se ferme

| Message | Signification |
|---|---|
| Le code a déjà servi ou date de plus de 30 s. Demandez-en un nouveau. | Le code ne sert qu'une fois et la console doit s'ouvrir dans les 30 secondes qui suivent sa saisie. Recommencez. |
| La console ne s'ouvre qu'en https://. | Ouvrez l'application web avec une adresse qui commence par `https://`. |
| Le shell s'est terminé. | Vous avez tapé `exit` ou terminé le shell. Normal. |
| La console a atteint sa limite de 60 min. | Le maximum d'une heure. Ouvrez-en une nouvelle. |
| La console s'est fermée après 15 min sans saisie. | Inactive pendant 15 minutes. |
| La box a mis fin à la console | Le mot de passe admin a été changé ou réinitialisé, trop de codes faux ont été saisis, la box s'arrête, ou son côté de la console a été perdu. |
| La console s'est fermée : le texte collé était trop long. | Collez par morceaux plus petits. |
| Connexion à la console perdue. | Problème réseau. Ouvrez une nouvelle console. |
| 2 consoles sont déjà ouvertes sur la box, le maximum. Fermez-en une d'abord. | Peut-être depuis un autre navigateur. |
| La box a refusé la connexion à la console. | Rechargez la page et réessayez. |
| La console n'a pas pu être chargée. | Vérifiez la connexion, rechargez la page et réessayez. |
| Vous avez fermé la console. | Vous avez cliqué sur **Fermer la console**. |

## Téléphone perdu ou réinstallé

Un téléphone réinstallé, réinitialisé ou remplacé ne reçoit plus de codes, même si l'application est revenue.

- **Vous avez un second téléphone approbateur** : utilisez-le pour retirer l'ancien (**Retirer**, **Envoyer le code** ; le code part sur l'autre téléphone, ou choisissez **L'envoyer plutôt sur …**, puis saisissez-le), puis ajoutez le nouveau.
- **Vous n'avez pas d'autre téléphone approbateur** : connectez-vous à la box en SSH en `root` (voir [Accès SSH]({{% relref "calaos_os/advanced/shell" %}})) et tapez :

  ```sh
  calaos-os admin reset
  ```

  Cela supprime le mot de passe admin **et tous les téléphones approbateurs**. Changez d'abord le mot de passe de Calaos Server (tant que vous ne l'avez pas fait, quiconque le connaît peut choisir le mot de passe admin avant vous), puis choisissez un nouveau mot de passe admin dans l'application web et enrôlez de nouveau votre téléphone. Voir [Si vous perdez le mot de passe admin]({{% relref "calaos_os/admin#si-vous-perdez-le-mot-de-passe-admin" %}}).

`calaos-os admin set` remplace le mot de passe admin mais garde les téléphones approbateurs.

## Confirmation du certificat dans l'application mobile

L'application Calaos d'un téléphone se connecte à une box de votre réseau avec un certificat que la box a créé elle-même. Au premier contact, l'application affiche **Certificat inconnu** avec l'empreinte de la clé (SHA-256).

- Comparez-la à celle de la box si vous le pouvez, puis touchez **Faire confiance**. L'application retient la clé de cette box : elle ne redemandera plus.
- Touchez **Ne pas se connecter** au moindre doute.
- Si la clé change plus tard, l'application affiche **Certificat modifié !** et demande de toucher deux fois. C'est normal après une réinstallation de la box, mais cela peut aussi signifier que quelqu'un intercepte la connexion : ne faites confiance que si vous savez pourquoi.
- Les box mémorisées sont listées sous **Certificats de confiance** dans les **Options** ; **Oublier** pour que l'application redemande.

Avec un nom dont le certificat Let's Encrypt est valide (voir [calaos_ddns]({{% relref "calaos_os/configuration/calaos_ddns" %}})), l'application vérifie elle-même le certificat et n'affiche aucune confirmation. Si elle affiche **Certificat modifié !** pour un tel nom, c'est légitime seulement si vous venez de passer ce nom du certificat propre à la box à Let's Encrypt et que le message dit que la box présente maintenant un certificat d'une autorité publique. Sinon la connexion est peut-être interceptée : refusez.
