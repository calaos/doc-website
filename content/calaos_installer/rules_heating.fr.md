+++
title = "Règles de chauffage"
date = 2026-10-15T09:30:00Z
weight = 50
summary = "Programmer la consigne, gérer l'absence, la fenêtre ouverte, le hors-gel, un circulateur commun, un plancher réversible sur pompe à chaleur et l'eau chaude sanitaire, pas à pas."
+++

Un thermostat ou une clim ne contient aucune politique : pas de programme horaire, pas de mode absence. C'est le rôle des [règles]({{% relref "calaos_installer/rules" %}}), qui **écrivent la consigne ou le mode du device**, comme elles écriraient n'importe quel IO. Cette page donne des recettes complètes. Vous retrouverez la plupart dans des [projets d'exemple]({{% relref "calaos_installer/devices/examples" %}}) à télécharger et à ouvrir dans Calaos Installer.

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

*La maison a neuf zones de plancher hydraulique et une pompe à chaleur (PAC) réversible : l'hiver elle fait de l'eau chaude, l'été de l'eau froide. Elle se commande par deux contacts secs, « demande de chaud » et « demande de froid », qui ne doivent jamais être fermés ensemble. Chaque zone a une seule électrovanne, la même pour le chaud et le froid. Six zones peuvent rafraîchir ; la salle de bain, l'entrée et la buanderie ne font que chauffer.*

Ce projet complet est [téléchargeable]({{% relref "calaos_installer/devices/examples" %}}) : vous pouvez l'ouvrir dans Calaos Installer en lisant cette recette.

### Le principe

Une vanne ouverte laisse passer l'eau ; c'est la PAC qui décide si elle est chaude ou froide. Chaque zone a son thermostat, qui ouvre sa vanne quand la pièce a besoin d'eau : en mode chaud quand la pièce est trop froide, en mode froid quand elle est trop chaude. Le thermostat doit donc avoir le bon mode, et il ne connaît pas celui de la PAC (voir [La vanne de zone]({{% relref "calaos_installer/devices/thermostat" %}})).

Un seul réglage décide du sens pour toute la maison : la variable **« Mode plancher »**. Des règles le recopient dans chaque zone, puis allument le bon contact de la PAC dès qu'une vanne est ouverte :

{{< mermaid >}}
graph TD
    M["Mode plancher<br/>0 arrêt, 1 chaud, 2 froid"] -->|"règles « Plancher : mode... »"| Z["9 thermostats de zone<br/>heat, cool ou off"]
    Z -->|"régulation"| V["9 vannes de zone<br/>ouvertes ou fermées"]
    V -->|"règles « PAC chaud / froid »"| P["PAC<br/>contact chaud ou contact froid"]
    M -->|"donne le sens"| P
{{< /mermaid >}}

L'été, on passe « Mode plancher » à 2 : un seul geste, et les règles mettent chaque zone en froid.

### Étape 1 : les neuf zones

Pour chaque zone, ajoutez un device avec le modèle **Zone de plancher réversible** (voir [Créer un device]({{% relref "calaos_installer/devices/templates" %}})) : la sonde de la zone, **sa** vanne (une vanne par zone : une sortie n'a qu'un thermostat) et les modes.

- Les six zones qui rafraîchissent gardent **Chauffage** et **Froid** : leur paramètre `modes` vaut `off,heat,cool`.
- Les trois autres ne gardent que **Chauffage** : `modes` vaut `off,heat`. Elles refusent le mode froid.

![Zone qui chauffe et rafraîchit](/en/calaos_installer/devices/images/inst-wizard-floor-zone.png?width=36pc&classes=shadow)
*« Plancher séjour » : chaud et froid, avec une sonde d'humidité.*

![Zone en chauffage seul](/en/calaos_installer/devices/images/inst-wizard-floor-zone-heat.png?width=36pc&classes=shadow)
*« Plancher salle de bain » : chaud seulement.*

Créez aussi les deux relais de la PAC, « PAC demande chaud » et « PAC demande froid ». Ce sont de simples sorties : **aucun thermostat ne les possède**, seules les règles de l'étape 3 les commandent.

![Les zones dans l'arborescence](/en/calaos_installer/devices/images/inst-tree-floor.png?width=30pc&classes=shadow)
*Neuf zones, leurs vannes, les deux contacts de la PAC et la variable « Mode plancher ».*

### Étape 2 : le mode de toute la maison

Créez une [variable interne]({{% relref "calaos_installer/internal_var" %}}) de type **entier**, « Mode plancher », valeur initiale 0, avec **Sauvegarder** coché pour qu'elle survive à un redémarrage.

| Valeur | Mode de la maison |
|---|---|
| 0 | arrêt |
| 1 | chaud |
| 2 | froid |

Trois règles la recopient dans les zones. Voici celle de l'été, **« Plancher : mode froid »** :

- **Condition** : « Mode plancher » == 2.
- **Actions** :
  - Plancher séjour, cuisine, bureau, chambre parents, chambre 1 et chambre 2 reçoivent `cool` ;
  - Plancher salle de bain, entrée et buanderie reçoivent `off` : elles ne rafraîchissent pas, elles s'éteignent.

Dès que « Mode plancher » passe à 2, cette règle s'exécute et met les neuf zones dans le bon mode : vous ne touchez à aucune pièce. Ses deux sœurs sont construites de la même façon :

| Règle | Condition | Actions |
|---|---|---|
| Plancher : arrêt général | « Mode plancher » == 0 | chaque zone reçoit `off` |
| Plancher : mode chaud | « Mode plancher » == 1 | chaque zone reçoit `heat` |
| Plancher : mode froid | « Mode plancher » == 2 | six zones reçoivent `cool`, les trois autres `off` |

![Règle du mode chaud](/en/calaos_installer/devices/images/inst-rule-floor-mode.png?width=40pc&classes=shadow)
*La règle « Plancher : mode chaud » : une condition sur la variable, neuf actions.*

« Mode plancher » se change comme tout IO : rendez-la visible pour la régler depuis les interfaces, mettez-la dans un [scénario]({{% relref "calaos_installer/scenario" %}}) « Été » ou « Hiver », ou faites-la changer par une règle (calendrier, température extérieure).

### Étape 3 : les contacts de la PAC

La PAC ne doit produire que si une zone a besoin d'eau, et dans le bon sens :

- une vanne ouverte veut dire qu'une zone a besoin d'eau (son thermostat ne l'ouvre que pour cela) ;
- « Mode plancher » donne le sens.

Les règles lisent donc les vannes, comme pour le circulateur de la recette 5 :

| Règle | Conditions | Actions, dans cet ordre |
|---|---|---|
| PAC chaud : vanne *zone* ouverte (neuf règles) | vanne *zone* == `true` **et** « Mode plancher » == 1 | « PAC demande froid » éteint, puis « PAC demande chaud » allumé |
| PAC froid : vanne *zone* ouverte (six règles, les zones qui rafraîchissent) | vanne *zone* == `true` **et** « Mode plancher » == 2 | « PAC demande chaud » éteint, puis « PAC demande froid » allumé |
| PAC : toutes les vannes fermées | les neuf vannes == `false` | « PAC demande chaud » et « PAC demande froid » éteints |

Les conditions sur les vannes sont du type **Condition sur évènement de sortie** (comme pour le circulateur), celle sur la variable une **Condition normale**.

Les deux contacts ne peuvent jamais être allumés ensemble :

- une règle « PAC chaud » demande « Mode plancher » == 1, une règle « PAC froid » == 2 : la variable n'a qu'une valeur à la fois ;
- chaque règle éteint d'abord le contact opposé, puis allume le sien ;
- aucun thermostat ne commande ces contacts, seulement ces règles.

![Règle du contact chaud](/en/calaos_installer/devices/images/inst-rule-floor-heat-contact.png?width=40pc&classes=shadow)
*Une vanne s'ouvre en mode chaud : le contact « froid » est éteint, le contact « chaud » allumé.*

![Règle du contact froid](/en/calaos_installer/devices/images/inst-rule-floor-cool-contact.png?width=40pc&classes=shadow)
*La même zone en mode froid.*

![Règle des vannes fermées](/en/calaos_installer/devices/images/inst-rule-floor-all-off.png?width=40pc&classes=shadow)
*Les neuf vannes fermées : la PAC n'a plus de demande.*

Au passage du chaud au froid, le contact suit tout de suite le nouveau mode si une vanne est encore ouverte. Les vannes ouvertes pour chauffer se ferment à la fin de leur durée minimale de marche (15 minutes), puis se rouvrent pour rafraîchir après leur durée minimale d'arrêt. Pour que tout s'arrête d'abord, passez par 0 : l'arrêt ferme les vannes tout de suite.

### Étape 4 : une zone réglée à la main dans l'autre sens

En plein hiver, quelqu'un met le séjour en froid depuis son téléphone. Sans règle, le séjour ouvrirait sa vanne dès qu'il fait trop chaud, la PAC (en chaud) enverrait de l'eau chaude, et la pièce se réchaufferait en voulant se rafraîchir.

Une règle par zone et par sens remet la zone dans le mode de la maison dès que son mode change :

| Règle | Conditions | Action |
|---|---|---|
| Plancher *zone* : retour en chaud (six zones) | *zone* == `cool` **et** « Mode plancher » == 1 | *zone* reçoit `heat` |
| Plancher *zone* : retour en froid (neuf zones) | *zone* == `heat` **et** « Mode plancher » == 2 | *zone* reçoit `cool` (`off` pour une zone qui ne rafraîchit pas) |

La condition sur *zone* est une **Condition sur évènement de sortie** sur le device lui-même (sélectionnez le device dans l'arborescence, pas un de ses IO) : la valeur comparée est son mode.

Pendant cet aller-retour, rien ne bouge sur l'installation :

- une vanne ouverte ne change pas de sens avant sa durée minimale de marche : elle reste ouverte et continue de chauffer ;
- les contacts de la PAC suivent « Mode plancher », pas le mode de la zone : la PAC ne passe jamais en froid pour une seule zone.

Une zone mise à `off` à la main n'est jamais touchée par ces règles.

![Règle de retour en mode chaud](/en/calaos_installer/devices/images/inst-rule-floor-putback.png?width=40pc&classes=shadow)
*Le séjour passé en froid pendant la saison de chauffe revient en chaud.*

### Questions fréquentes

**Je passe une zone en froid alors que la PAC chauffe ?**
La règle « retour en chaud » de la zone la remet aussitôt en `heat`. La pièce continue de chauffer normalement. Pour rafraîchir, c'est toute la maison qui passe en froid.

**Comment passer en été ?**
Mettez « Mode plancher » à 2, depuis les interfaces, un scénario ou une règle. La règle « Plancher : mode froid » met les six zones en froid et éteint les trois autres. Pour revenir en hiver, mettez-la à 1.

**Comment éteindre une seule pièce ?**
Mettez sa zone en `off`, depuis les interfaces ou une règle : les règles de retour ne la touchent pas. Elle reste éteinte jusqu'au prochain changement de « Mode plancher », qui remet chaque zone dans le mode de la maison.

**Et sans sonde d'humidité ?**
Le froid reste permis. Sans sonde, le thermostat ne peut pas éviter la condensation : c'est la PAC qui doit garder l'eau au-dessus du point de rosée. Le serveur le rappelle au chargement par un avertissement pour chaque zone sans sonde (« cooling has no dew-point cut ») : c'est attendu. Une zone **avec** sonde (le séjour, ici) coupe son froid dès que l'humidité atteint `humidity_max` ; sa vanne se ferme et les contacts de la PAC suivent, sans règle de plus. Voir [Thermostat interne]({{% relref "calaos_installer/devices/thermostat" %}}).

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
