+++
title = "Règles de chauffage"
date = 2026-10-15T09:30:00Z
weight = 50
summary = "Programmer la consigne, gérer l'absence, la fenêtre ouverte, le hors-gel, et allumer un circulateur commun, pas à pas."
+++

Un thermostat ou une clim ne contient aucune politique : pas de programme horaire, pas de mode absence. C'est le rôle des [règles]({{% relref "calaos_installer/rules" %}}), qui **écrivent la consigne ou le mode du device**, comme elles écriraient n'importe quel IO. Cette page donne des recettes complètes, avec la maison de démonstration.

## La commande dans une action de règle

Dans une règle, glissez le device (pas ses IO possédés) dans la zone des actions. Calaos Installer affiche un **formulaire** à la place du texte libre : un champ par commande que le device accepte (mode, consigne, consigne basse et haute, ventilation, oscillation). Seuls les champs dont le lien existe sur ce device sont proposés ; les bornes et les listes de valeurs viennent du device.

![Formulaire de commande d'un device dans une action](/en/climate/images/inst-action-form.png?width=36pc&classes=shadow)
*Une action sur « Radiateur bureau » : mode Chauffage, consigne 21 °C.*

La valeur enregistrée est un texte :

| Vous voulez... | Valeur de l'action |
|---|---|
| éteindre | `off` |
| chauffer | `heat` |
| changer seulement la consigne | `{"target_temp":17}` |
| mode et consigne ensemble | `{"mode":"heat","target_temp":21}` |
| régler une plage en mode auto | `{"mode":"auto","target_temp_low":20,"target_temp_high":25}` |
| ventilation | `{"fan_mode":"low"}` |

Seuls les champs présents sont appliqués, et un champ invalide est refusé et journalisé sans empêcher les autres. Un bouton permet de repasser en saisie de texte si vous préférez écrire le JSON vous-même. La valeur ne peut pas être « celle d'un autre IO » : pour recopier une valeur dans une consigne, la règle écrit directement l'IO de consigne.

{{% notice warning %}}
Ne commandez jamais le **relais** d'un thermostat dans une règle : écrivez son mode (`off`, `heat`). Le thermostat remettrait le relais dans l'état voulu, et le journal se remplirait d'avertissements.
{{% /notice %}}

## 1. Programmer la consigne

*Le bureau est chauffé à 21 °C de 7 h 30 à 18 h en semaine, à 17 °C le reste du temps.*

1. Créez une **plage horaire** « Bureau occupé » (lundi à vendredi, 7 h 30 à 18 h) : voir [Programmation horaire]({{% relref "calaos_installer/time" %}}).
2. Règle « Bureau : confort en journée » : condition, la plage « Bureau occupé » est **vraie** ; action, « Radiateur bureau » reçoit `{"mode":"heat","target_temp":21}`.
3. Règle « Bureau : réduit hors journée » : condition, la plage est **fausse** ; action, `{"target_temp":17}`.

![Une règle complète](/en/climate/images/inst-rule-complete.png?width=40pc&classes=shadow)
*La règle « Bureau : confort en journée » : une condition, une action.*

Pensez à une règle de **remise en état au démarrage** (condition de démarrage) si vous voulez que la consigne soit juste après un redémarrage du serveur.

## 2. Absence et retour

*Quand personne n'est là, tout le chauffage descend à 16 °C ; au retour, on remonte.*

1. Créez une **variable interne** booléenne « Absence ».
2. Règle « Absence : abaisser le chauffage » : condition, « Absence » == vrai ; actions :
   - Radiateur bureau : `{"mode":"heat","target_temp":16}`
   - PAC cuisine : `{"mode":"auto","target_temp_low":16,"target_temp_high":28}`
   - Plancher séjour : `{"mode":"heat","target_temp":16}`
3. Règle « Retour : rétablir » : condition, « Absence » == faux ; actions : `{"target_temp":21}` sur le bureau et le plancher, `{"target_temp_low":20,"target_temp_high":25}` sur la PAC.

Les actions d'une règle s'exécutent dans l'ordre. Pour réutiliser ces actions depuis un bouton ou une programmation, regroupez-les dans un [scénario]({{% relref "calaos_installer/scenario" %}}) « Absence » et mettez le scénario en action.

## 3. Fenêtre ouverte

*Si la fenêtre du bureau est ouverte, le radiateur s'arrête.*

- Règle « Fenêtre bureau ouverte » : condition, « Fenêtre bureau » == vrai ; action, « Radiateur bureau » reçoit `off`.
- Règle « Fenêtre bureau fermée » : condition, « Fenêtre bureau » == faux ; action, `heat`.

La consigne n'est pas touchée : à la fermeture, le thermostat reprend là où il en était.

## 4. Hors-gel

*La nuit, si le salon descend sous 10 °C, la clim chauffe à 17 °C.*

Règle « Hors-gel : nuit d'hiver » : conditions, la plage « Nuit » est vraie **et** « Température salon » < 10 ; action, « Clim salon » reçoit `{"mode":"heat","target_temp":17}`. Les conditions d'une règle doivent toutes être vraies.

Pour un hors-gel permanent (maison fermée pour l'hiver), mettez la consigne à 7 °C en mode `heat` : c'est la borne basse par défaut d'un thermostat.

## 5. Un circulateur ou une chaudière commun

Chaque thermostat a **sa propre sortie** (une sortie ne peut appartenir qu'à un thermostat). Pour que le circulateur tourne dès qu'un thermostat chauffe, une règle allume la sortie commune.

- Une règle **par thermostat** « Circulateur : le bureau chauffe » : condition, l'IO **action** du thermostat == `heating` ; action, « Circulateur chauffage » allumé. Faites de même pour la PAC et le plancher.
- Une règle « Circulateur : plus aucun thermostat ne chauffe » : trois conditions (les trois IO action différents de `heating`) ; action, « Circulateur chauffage » éteint.

![Règle du circulateur](/en/climate/images/inst-rule-circulator.png?width=40pc&classes=shadow)
*Le circulateur s'arrête quand plus aucun thermostat ne chauffe.*

Les IO « action » (`Radiateur bureau action`...) sont des IO possédés des devices : on les trouve sous le device dans l'arborescence et on les glisse en condition.

## Pièges

- **Boucles** : une règle qui écrit une consigne et qui a cette même consigne en condition peut se rappeler indéfiniment. Utilisez une variable interne pour mémoriser que l'action a eu lieu.
- **Règles contradictoires** : une programmation horaire et une règle d'absence écrivent la même consigne ; la dernière exécutée l'emporte. Ajoutez la condition « Absence == faux » aux règles de la programmation.
- **Après un redémarrage du serveur**, rien ne garantit que la consigne correspond à l'heure : ajoutez une règle à condition de démarrage qui la rétablit.
