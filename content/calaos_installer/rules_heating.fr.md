+++
title = "Règles de chauffage"
date = 2026-10-15T09:30:00Z
weight = 50
summary = "Programmer la consigne, gérer l'absence, la fenêtre ouverte, le hors-gel, un circulateur commun, un plancher réversible sur pompe à chaleur et l'eau chaude sanitaire, pas à pas."
+++

Un thermostat ou une clim ne contient aucune politique : pas de programme horaire, pas de mode absence. C'est le rôle des [règles]({{% relref "calaos_installer/rules" %}}), qui **écrivent la consigne ou le mode du device**, comme elles écriraient n'importe quel IO. Cette page donne des recettes complètes.

## La commande dans une action de règle

Dans une règle, glissez le device (pas ses IO possédés) dans la zone des actions. Calaos Installer affiche un **formulaire** à la place du texte libre : un champ par commande que le device accepte (mode, consigne, consigne basse et haute, ventilation, oscillation). Seuls les champs dont le lien existe sur ce device sont proposés ; les bornes et les listes de valeurs viennent du device.

![Formulaire de commande d'un device dans une action](/en/calaos_installer/devices/images/inst-action-form.png?width=36pc&classes=shadow)
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

![Une règle complète](/en/calaos_installer/devices/images/inst-rule-complete.png?width=40pc&classes=shadow)
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

## 5. Un circulateur ou une chaudière commun (plancher chauffant)

*Le circulateur tourne dès qu'une zone demande de la chaleur (ou du froid), et s'arrête quand plus aucune zone ne demande rien.*

Pour un plancher chauffant seul, chaque zone est un « Thermostat de chauffage » (émetteur « Plancher chauffant ») dont la sortie est sa vanne. Chaque thermostat a **sa propre sortie** (une vanne ou un relais par zone ; une sortie ne peut appartenir qu'à un thermostat). Plutôt que de deviner ce que fait le thermostat, les règles lisent **ces sorties elles-mêmes** : ce sont exactement les relais que la régulation commande.

- Une règle **par sortie de zone**, par exemple « Circulateur : Relais radiateur bureau allumé » : condition, la sortie « Relais radiateur bureau » == `true` ; action, « Circulateur chauffage » allumé. Faites de même pour chaque sortie de chauffage **et** de refroidissement des autres zones (« PAC cuisine chaud », « PAC cuisine froid », « Plancher chaud », « Plancher froid »).
- Une règle « Circulateur : toutes les zones éteintes » : une condition par sortie de zone, toutes à == `false` ; action, « Circulateur chauffage » éteint.

Les conditions sont du type **Condition sur évènement de sortie** (menu **+** des conditions, après avoir sélectionné la sortie dans l'arborescence) : c'est le type qui sait lire l'état d'un relais. Le type « Condition normale » ne s'applique qu'aux entrées.

![Règle du circulateur](/en/calaos_installer/devices/images/inst-rule-circulator.png?width=40pc&classes=shadow)
*La règle « toutes les zones éteintes » : une condition par sortie (la cinquième, « Plancher froid », est sous la liste visible) et l'extinction du circulateur.*

### Pourquoi ça marche dans tous les modes

Une sortie est allumée **si et seulement si** le thermostat demande de chauffer ou de refroidir à cet instant. Le mode (`heat`, `cool`, `auto`), la consigne, les durées minimales de marche et d'arrêt, les sûretés (sonde muette, sonde hors limites, point de rosée) et les verrouillages du thermostat décident tous de l'état de la sortie, et la règle ne voit que le résultat. Elle n'a donc pas à connaître le mode : un thermostat en `cool` ou en `auto`, ou coupé par une sûreté, se comporte correctement sans règle supplémentaire. À l'inverse, une règle écrite sur l'IO « action » du thermostat (`heating`) ignore le refroidissement et décale la demande dès qu'une durée minimale retarde la commutation du relais.

Les sorties ne sont jamais écrites par ces règles, seulement lues : le circulateur est la seule sortie qu'elles commandent.

## 6. Plancher réversible : neuf zones et une pompe à chaleur

*Une maison de neuf zones de plancher hydraulique. Une pompe à chaleur fait l'eau chaude ou l'eau froide selon deux contacts secs, « demande de chaud » et « demande de froid », qui ne doivent jamais être allumés ensemble. Chaque zone a une seule vanne, la même pour le chaud et le froid. Six zones peuvent rafraîchir ; la salle de bain, l'entrée et la buanderie ne font que chauffer.*

Calaos n'a pas de device « pompe à chaleur » : les thermostats de zone commandent leurs vannes, et des règles en tirent la demande à la pompe. L'ensemble tient en quatre pièces :

1. un thermostat par zone, avec le modèle **Zone de plancher réversible** ;
2. une variable **« Mode plancher »** (0 arrêt, 1 chaud, 2 froid) qui donne le mode à toutes les zones ;
3. des règles qui allument le bon contact de la pompe selon les vannes ouvertes et le mode ;
4. des règles qui remettent dans le mode de la maison une zone réglée à la main dans l'autre sens.

### Étape 1 : les neuf zones

Pour chaque zone, ajoutez un device avec le modèle **Zone de plancher réversible** (voir [Créer un device]({{% relref "calaos_installer/devices/templates" %}})) : la sonde de la zone, **sa** vanne (un relais par zone : une sortie n'a qu'un thermostat) et les modes. Les six zones qui rafraîchissent gardent **Chauffage** et **Froid** ; les trois autres ne gardent que **Chauffage**, donc le paramètre `modes` vaut `off,heat`.

![Zone qui chauffe et rafraîchit](/en/calaos_installer/devices/images/inst-wizard-floor-zone.png?width=36pc&classes=shadow)
*« Plancher séjour » : chaud et froid, avec une sonde d'humidité.*

![Zone en chauffage seul](/en/calaos_installer/devices/images/inst-wizard-floor-zone-heat.png?width=36pc&classes=shadow)
*« Plancher salle de bain » : chaud seulement.*

Créez aussi les deux relais de la pompe à chaleur, « PAC demande chaud » et « PAC demande froid » : ce sont de simples sorties, **qu'aucun thermostat ne possède**, et que seules les règles ci-dessous commandent.

![Les zones dans l'arborescence](/en/calaos_installer/devices/images/inst-tree-floor.png?width=30pc&classes=shadow)
*Neuf zones, leurs vannes, les deux contacts de la pompe et la variable « Mode plancher ».*

### Étape 2 : le mode de la maison

Créez une [variable interne]({{% relref "calaos_installer/internal_var" %}}) de type **entier**, « Mode plancher », valeur initiale 0, avec **Sauvegarder** coché pour qu'elle survive à un redémarrage. Ses valeurs :

| Valeur | Mode de la maison |
|---|---|
| 0 | arrêt |
| 1 | chaud |
| 2 | froid |

Elle se change comme tout IO : depuis un scénario, une règle (selon la température extérieure ou le calendrier), ou à la main si vous la rendez visible dans les interfaces. Trois règles la répercutent sur les zones :

| Règle | Condition | Actions |
|---|---|---|
| Plancher : arrêt général | « Mode plancher » == 0 | chaque zone reçoit `off` |
| Plancher : mode chaud | « Mode plancher » == 1 | chaque zone reçoit `heat` |
| Plancher : mode froid | « Mode plancher » == 2 | les six zones qui rafraîchissent reçoivent `cool`, les trois autres `off` |

![Règle du mode chaud](/en/calaos_installer/devices/images/inst-rule-floor-mode.png?width=40pc&classes=shadow)
*La règle « mode chaud » : une condition sur la variable, neuf actions.*

Une zone sans refroidissement refuse `cool` (elle n'a pas ce mode) : en mode froid, elle reçoit `off` et s'éteint.

### Étape 3 : les contacts de la pompe à chaleur

Une vanne est allumée **si et seulement si** son thermostat demande de la chaleur ou du froid : comme pour le circulateur, les règles lisent les vannes. La vanne ne dit pas dans quel sens, c'est le mode de la maison qui le dit.

| Règle | Conditions | Actions, dans cet ordre |
|---|---|---|
| PAC chaud : vanne *zone* ouverte (neuf règles) | vanne *zone* == `true` **et** « Mode plancher » == 1 | « PAC demande froid » éteint, puis « PAC demande chaud » allumé |
| PAC froid : vanne *zone* ouverte (six règles, les zones qui rafraîchissent) | vanne *zone* == `true` **et** « Mode plancher » == 2 | « PAC demande chaud » éteint, puis « PAC demande froid » allumé |
| PAC : toutes les vannes fermées | les neuf vannes == `false` | « PAC demande chaud » et « PAC demande froid » éteints |

Chaque règle éteint d'abord le contact opposé, puis allume le sien : les deux contacts ne sont jamais allumés ensemble. Les conditions sur les vannes sont du type **Condition sur évènement de sortie** (comme pour le circulateur) et celle sur la variable une **Condition normale**.

![Règle du contact chaud](/en/calaos_installer/devices/images/inst-rule-floor-heat-contact.png?width=40pc&classes=shadow)
*Une vanne s'ouvre en mode chaud : le contact « froid » est éteint, le contact « chaud » allumé.*

![Règle du contact froid](/en/calaos_installer/devices/images/inst-rule-floor-cool-contact.png?width=40pc&classes=shadow)
*La même zone en mode froid.*

![Règle des vannes fermées](/en/calaos_installer/devices/images/inst-rule-floor-all-off.png?width=40pc&classes=shadow)
*Les neuf vannes fermées : la pompe n'a plus de demande.*

Au changement de mode de la maison, les vannes ouvertes dans l'ancien sens se ferment après `min_on_time` (15 minutes), puis se rouvrent dans le nouveau sens après `min_off_time` : la règle « toutes les vannes fermées » éteint les contacts quand la dernière se ferme, et le contact du nouveau sens s'allume à la première réouverture.

### Étape 4 : une zone réglée à la main dans l'autre sens

Quelqu'un met une zone en `cool` pendant la saison de chauffe : sa vanne s'ouvrirait, la pompe fournirait de l'eau chaude, et la zone se réchaufferait en voulant se rafraîchir. Une règle par zone et par sens remet la zone dans le mode de la maison, dès que son mode change :

| Règle | Conditions | Action |
|---|---|---|
| Plancher *zone* : retour en chaud (six zones) | *zone* == `cool` **et** « Mode plancher » == 1 | *zone* reçoit `heat` |
| Plancher *zone* : retour en froid (neuf zones) | *zone* == `heat` **et** « Mode plancher » == 2 | *zone* reçoit `cool` (`off` pour une zone sans refroidissement) |

La condition sur *zone* est une **Condition sur évènement de sortie** sur le device lui-même (sélectionnez le device dans l'arborescence, pas un de ses IO) : la valeur comparée est son mode. L'arrêt à la main (`off`) n'est pas contrarié.

![Règle de retour en mode chaud](/en/calaos_installer/devices/images/inst-rule-floor-putback.png?width=40pc&classes=shadow)
*Une zone du séjour passée en froid pendant la saison de chauffe revient en chaud.*

### Le point de rosée

Cette installation n'a pas de sonde d'humidité dans chaque zone : la pompe à chaleur garde l'eau froide au-dessus du point de rosée. Le thermostat de zone ne coupe donc pas le froid de lui-même ; le serveur l'indique par un avertissement au chargement. Une zone **avec** sonde d'humidité (le séjour, ci-dessus) coupe son refroidissement dès que l'humidité atteint `humidity_max` : sa vanne se ferme et les contacts de la pompe suivent la vanne, sans règle de plus. Voir [Thermostat interne]({{% relref "calaos_installer/devices/thermostat" %}}).

## 7. Eau chaude sanitaire sur la même pompe à chaleur

*Le ballon d'eau chaude sanitaire est chauffé par la même pompe à chaleur, mais il ne dépend ni des zones ni du mode de la maison.*

L'eau chaude est un **thermostat indépendant**, avec **sa** sonde et **son** relais, qui n'a aucun lien avec les neuf zones et qu'aucune des règles précédentes ne lit. C'est la demande d'eau chaude de la pompe à chaleur, que le thermostat commute tout seul.

1. Créez la sonde du ballon et le relais de demande d'eau chaude, puis un **Thermostat de chauffage**. L'émetteur n'a pas d'équivalent pour un ballon : choisissez « Radiateurs à eau » (300 s de marche et d'arrêt minimaux), puis adaptez.

   ![Assistant eau chaude sanitaire](/en/calaos_installer/devices/images/inst-wizard-dhw.png?width=36pc&classes=shadow)
   *« Eau chaude sanitaire » : la sonde du ballon et le relais de demande.*

2. Dans l'**éditeur de device**, relevez ce que les modèles règlent pour une pièce :

   | Paramètre | Modèle | Pour un ballon | Pourquoi |
   |---|---|---|---|
   | `valid_max` | 60 | 95 | à 60 °C, une sonde de ballon correctement chaude serait jugée « hors limites » et couperait la production |
   | `valid_min` | -30 | 5 | une sonde débranchée ou en court-circuit sort de ces bornes |
   | `hysteresis` | 0,5 | 3 | un ballon a de l'inertie : évitez les redémarrages courts |

   ![Éditeur du thermostat d'eau chaude](/en/calaos_installer/devices/images/inst-dhw-editor.png?width=36pc&classes=shadow)
   *Les paramètres de « Eau chaude sanitaire ».*

3. Relevez la **consigne maximale** : la consigne d'un thermostat est une variable interne limitée à 30 °C par défaut. Ouvrez ses propriétés dans l'arborescence et réglez `value_max` à 65 (et la valeur à 55 par exemple) : une consigne de 55 °C serait sinon ramenée à 30 °C.

   ![Propriétés de la consigne](/en/calaos_installer/devices/images/inst-dhw-setpoint.png?width=30pc&classes=shadow)
   *La consigne de l'eau chaude : bornes relevées.*

Le relais de demande appartient au thermostat : n'écrivez que son **mode** dans les règles (par exemple `off` pour n'autoriser l'eau chaude qu'à certaines heures, comme dans la recette 1). Il n'a pas de rapport avec « PAC demande chaud » et « PAC demande froid », que seules les zones commandent.

## Pièges

- **Boucles** : une règle qui écrit une consigne et qui a cette même consigne en condition peut se rappeler indéfiniment. Utilisez une variable interne pour mémoriser que l'action a eu lieu.
- **Règles contradictoires** : une programmation horaire et une règle d'absence écrivent la même consigne ; la dernière exécutée l'emporte. Ajoutez la condition « Absence == faux » aux règles de la programmation.
- **Après un redémarrage du serveur**, rien ne garantit que la consigne correspond à l'heure : ajoutez une règle à condition de démarrage qui la rétablit.
- **Un contact de pompe à chaleur ne se commande que par des règles** : aucun thermostat n'en est propriétaire, et la règle « toutes les vannes fermées » doit exister, sinon un contact reste allumé après la dernière demande.
