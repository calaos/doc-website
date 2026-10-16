+++
title = "Thermostat interne"
date = 2026-10-15T09:10:00Z
weight = 30
summary = "Calaos régule : hystérésis, durées minimales, sûretés (sonde muette, point de rosée, sonde hors limites), une sortie par thermostat."
+++

## Quand l'utiliser

Le thermostat interne (`thermostat`) est fait pour les installations où **rien ne régule à votre place** : des radiateurs électriques derrière un relais, une vanne de plancher chauffant ou rafraîchissant, une chaudière avec un contact, une pompe à chaleur pilotée sur contacts secs. Calaos lit une sonde de température, compare à la consigne, et commute un ou deux relais.

Si l'appareil possède sa propre régulation (clim split, pompe à chaleur ESPHome), préférez le mode relayé : voir [Clim ESPHome]({{% relref "hardware/esphome_climate" %}}).

## Ce que le thermostat fait, et ce qui reste à vos règles

La **régulation** est dans le thermostat et ne se modifie pas : elle réagit à chaque changement de la température, de la consigne, du mode et de l'humidité, ainsi qu'à ses minuteurs.

La **politique** reste à vos règles : abaissement de nuit, absence, fenêtre ouverte, programmation horaire. Elles écrivent la consigne ou le mode du thermostat, comme elles le feraient pour n'importe quel IO. Le thermostat n'a pas de planificateur interne ; voir [Règles de chauffage]({{% relref "calaos_installer/rules_heating" %}}).

## La régulation

Le thermostat a un mode (`off`, `heat`, et `cool` et `auto` s'il peut refroidir) et une ou deux consignes. Les modes proposés dépendent des sorties liées, ou du paramètre `modes` pour une [vanne de zone](#la-vanne-de-zone-sortie-unique) :

| Mode | Comportement |
|---|---|
| `off` | les deux sorties sont coupées |
| `heat` | chauffage **allumé** sous `consigne − hystérésis`, **coupé** dès que la consigne est atteinte |
| `cool` | refroidissement **allumé** au-dessus de `consigne + hystérésis`, **coupé** dès que la consigne est atteinte |
| `auto` | le chauffage suit la consigne **basse**, le refroidissement la consigne **haute** ; entre les deux, rien ne marche |

Avec une consigne de 21 °C et une hystérésis de 0,5 °C, le chauffage s'allume à 20,5 °C et se coupe à 21 °C. L'hystérésis évite qu'un relais claque à chaque dixième de degré.

### Les durées minimales

`min_on_time` et `min_off_time` fixent, pour chaque sortie, la durée minimale de marche et d'arrêt. Une commutation demandée pendant ce délai est **reportée** jusqu'à son échéance, puis la régulation est réévaluée. Les interfaces affichent alors « Démarre dans N min ». Les durées protègent un compresseur ou une vanne thermique d'enchaînements trop rapides.

En mode `auto`, les deux sorties ne sont **jamais** allumées ensemble, et le passage du chaud au froid attend aussi la durée minimale d'arrêt de l'autre sortie. Le paramètre `min_range` impose un écart minimal entre la consigne basse et la consigne haute ; une plage plus serrée est refusée.

### Les valeurs par défaut des modèles

Le modèle propose des valeurs adaptées à ce que commandent les sorties (le choix **Émetteur** de l'assistant) :

| Modèle, émetteur | Hystérésis | Marche min | Arrêt min | Écart min |
|---|---|---|---|---|
| Thermostat chauffage, radiateurs électriques | 0,3 °C | 120 s | 120 s | - |
| Thermostat chauffage, radiateurs à eau | 0,5 °C | 300 s | 300 s | - |
| Thermostat chauffage, plancher chauffant | 0,5 °C | 900 s | 900 s | - |
| Chaud / froid, pompe à chaleur réversible sur contacts | 0,5 °C | 600 s | 300 s | 2 °C |
| Chaud / froid, ventilo-convecteurs | 0,5 °C | 120 s | 120 s | 1 °C |
| Thermostat de plancher réversible (deux sorties) | 0,5 °C | 900 s | 900 s | 2 °C |
| Zone de plancher réversible (une vanne) | 0,5 °C | 900 s | 900 s | - |

Un plancher a une grande inertie : une hystérésis large et de longues durées évitent les à-coups. Pour tous les modèles de thermostat, `sensor_timeout` vaut 9000 s, `put_back_delay` 1 s (maximum 60 s), et les lectures acceptées vont de -30 à 60 °C (voir [Sonde hors limites](#sonde-hors-limites)). Toutes ces valeurs se changent ensuite dans l'éditeur de device.

## Les sûretés

Un thermostat qui commande des relais ne doit jamais rester allumé sur une information fausse. Dans tous les cas ci-dessous, **les sorties sont coupées**, le mode et la consigne restent réglables, et la régulation reprend d'elle-même quand la situation redevient normale.

| Situation | Ce qui est affiché | Détail |
|---|---|---|
| Au démarrage, la sonde n'a encore rien lu | « En attente de la sonde » | Pas de régulation tant que la sonde n'a pas livré une valeur après le démarrage. Une valeur gardée en mémoire ou retenue par MQTT ne compte pas. |
| La sonde n'a rien envoyé depuis `sensor_timeout` secondes | « Sonde muette » | Même une valeur identique répétée compte comme une lecture. 9000 s par défaut, au-dessus d'un rapport horaire Zigbee manqué. |
| La sonde se déclare déconnectée | « Sonde déconnectée » | Lien de connexion de la sonde (MQTT, Wago...). |
| Le lien `current_temp` n'existe pas | « Sonde absente » | Device mal configuré. |
| Plancher avec sonde d'humidité : humidité trop haute, muette ou jamais lue | « Humidité trop haute » ou « Sonde d'humidité muette » | Voir le point de rosée ci-dessous. |
| La lecture sort des bornes de vraisemblance, ou n'est pas un nombre | « Sonde hors limites » | Voir [Sonde hors limites](#sonde-hors-limites). |

Quand plusieurs situations se cumulent, l'interface n'en affiche qu'une, par ordre de priorité : sonde absente, en attente, muette, déconnectée, hors limites, puis les défauts d'humidité.

### Le point de rosée (plancher rafraîchissant)

Refroidir un plancher sous le point de rosée fait **condenser** l'humidité de l'air sur le sol. Avec l'émetteur « Plancher » (`emitter` = `floor`) et un mode de refroidissement, le thermostat peut donc surveiller une **sonde d'humidité** (lien `humidity`) : il coupe le refroidissement tant que l'humidité atteint `humidity_max` (60 % dans les modèles, 65 % si le paramètre est absent), ou tant que la sonde est muette, déconnectée ou n'a jamais lu.

La sonde d'humidité est **facultative** :

- **avec** une sonde liée, la coupure s'applique, en mode `cool` comme en mode `auto` ;
- **sans** sonde, le refroidissement est permis et le thermostat n'a aucune protection contre la condensation : c'est à l'installation de garder l'eau au-dessus du point de rosée (la pompe à chaleur, par exemple). Le serveur le signale par un avertissement dans le journal au chargement ;
- un lien `humidity` vers un IO introuvable compte comme une sonde liée : le refroidissement reste coupé plutôt que d'être autorisé par une faute de frappe.

Seul l'émetteur « Plancher » est concerné : avec un autre émetteur, l'humidité n'est pas lue. Le modèle « Thermostat de plancher réversible » (deux sorties) demande toujours une sonde d'humidité ; le modèle « Zone de plancher réversible » (une vanne) la laisse facultative.

### Sonde hors limites

Une sonde peut mentir sans se taire : une DS18B20 lit 85 °C à la mise sous tension et -127 °C débranchée, une sonde Wago dont le câble est coupé ou en court-circuit lit une valeur absurde au lieu de se déclarer déconnectée. Deux paramètres du thermostat, `valid_min` et `valid_max`, bornent les lectures acceptées (bornes incluses) ; une borne absente n'est pas contrôlée.

Une lecture hors bornes compte comme **une absence de lecture valide** :

- les sorties sont coupées **aussitôt**, sans attendre `min_on_time` ; `min_off_time` retarde en revanche le redémarrage ;
- le thermostat affiche **« Sonde hors limites »**, et son mode et ses consignes restent réglables ;
- la régulation reprend dès la première lecture dans les bornes ;
- une lecture qui n'est pas un nombre (NaN, infini) est toujours hors limites, même sans bornes ;
- il n'y a **pas d'hystérésis** sur les bornes : une sonde qui oscille de part et d'autre d'une borne fait cycler les sorties au rythme de `min_off_time` ;
- si `valid_max` est inférieur à `valid_min`, les deux bornes sont ignorées (message dans le journal).

Les modèles de thermostat écrivent `valid_min` = -30 et `valid_max` = 60 : cela couvre les valeurs absurdes ci-dessus sans gêner un garage hors gel. Les modèles de clim ESPHome n'ont pas de bornes. Modifiez-les dans l'éditeur de device selon ce que mesure la sonde : `valid_min` = 5 et `valid_max` = 40 pour une pièce de vie, mais une sonde de ballon d'eau chaude sanitaire doit monter plus haut que 60 °C (voir [Règles de chauffage]({{% relref "calaos_installer/rules_heating" %}})).

## La vanne de zone (sortie unique)

### La situation

Un plancher réversible chauffe l'hiver et rafraîchit l'été. Chaque zone (une pièce ou un groupe de pièces) a **une électrovanne** sur son circuit :

- vanne ouverte, l'eau circule dans le plancher de la zone ;
- vanne fermée, elle ne circule pas.

La vanne ne sait pas si l'eau est chaude ou froide. C'est la **pompe à chaleur** qui le décide, pour toute la maison.

Le thermostat de la zone n'a donc pas deux sorties, « chauffage » et « refroidissement », mais **une seule** : la **Vanne de zone** (lien `output`). Le modèle **Zone de plancher réversible** de l'assistant crée le thermostat ainsi.

### Ce que fait le thermostat de zone

| Mode de la zone | La vanne s'ouvre quand... |
|---|---|
| `heat` (chaud) | la pièce est **sous** la consigne |
| `cool` (froid) | la pièce est **au-dessus** de la consigne |
| `off` (arrêt) | jamais : la vanne reste fermée |

La vanne se referme quand la consigne est atteinte.

Le thermostat a donc besoin de **son propre mode** pour savoir dans quel cas ouvrir. Il ne connaît pas le mode de la pompe à chaleur : rien ne lui dit si elle fait de l'eau chaude ou de l'eau froide. Si la pompe chauffe et qu'une zone est en froid, la zone ouvre sa vanne quand la pièce est trop chaude... et reçoit de l'eau chaude.

C'est pourquoi une installation réversible a **un mode pour toute la maison**, et des règles qui le donnent à chaque zone et à la pompe à chaleur. La recette complète est dans [Règles de chauffage]({{% relref "calaos_installer/rules_heating" %}}), recette 6, et un [exemple prêt à ouvrir]({{% relref "calaos_installer/devices/examples" %}}) la contient toute faite.

### Les zones qui ne rafraîchissent pas

Le paramètre `modes` liste les modes que la zone accepte, en plus de `off` :

- une zone qui chauffe et rafraîchit porte `modes` = `off,heat,cool` ;
- une salle de bain qu'on ne rafraîchit jamais porte `modes` = `off,heat`. Elle refuse `cool` : la demande est ignorée et notée dans le journal ;
- sans ce paramètre, la zone ne propose que le chauffage, par prudence (le journal le signale) ;
- `auto` (chaud ou froid selon une plage de consignes basse et haute) n'est possible que si `heat` et `cool` sont dans la liste.

### Les détails

- Une vanne de zone remplace `output_heat` et `output_cool` : un thermostat qui lie les deux est refusé au chargement (message dans le journal), et la vanne est ignorée.
- Comme tout relais de thermostat, une vanne n'appartient qu'à une zone.
- Quand le mode de la zone change de sens (du chaud au froid, ou l'inverse), une vanne ouverte ne se ferme qu'à la fin de sa durée minimale de marche (`min_on_time`, 15 minutes dans le modèle). Elle ne se rouvre dans le nouveau sens qu'après sa durée minimale d'arrêt (`min_off_time`). Les interfaces affichent « Démarre dans N min ». Le mode `off` ferme la vanne tout de suite.
- L'action affichée suit le mode : « chauffe » ou « refroidit ».
- Avec une sonde d'humidité liée, le froid est coupé dès que l'humidité dépasse `humidity_max`, y compris pour une vanne qui attend son changement de sens (voir [Le point de rosée](#le-point-de-rosée-plancher-rafraîchissant)).

## Une sortie, un thermostat

Un relais lié comme sortie de chauffage ou de refroidissement appartient à **un seul** thermostat. Deux thermostats qui écrivent le même relais se battraient : l'un l'allume, l'autre l'éteint. Calaos refuse donc le second lien au chargement (un message dans le journal), et l'assistant d'installation ne propose pas une sortie déjà prise.

![Sortie déjà prise, refusée par l'éditeur](/en/calaos_installer/devices/images/inst-output-refused.png?width=30pc&classes=shadow)
*Un relais déjà utilisé par un autre thermostat est refusé.*

Pour un chauffage à plusieurs zones qui partagent une chaudière, un circulateur ou une pompe à chaleur, on crée **une sortie par zone** (une vanne ou un relais par thermostat), et des **règles** qui allument la sortie commune dès qu'une sortie de zone est allumée. Voir les exemples dans [Règles de chauffage]({{% relref "calaos_installer/rules_heating" %}}).

## Les sorties sont protégées

Les relais pilotés par un thermostat ne doivent pas être écrits par ailleurs. Si une règle, un script Lua ou une commande écrit directement l'un d'eux, la valeur est appliquée **mais un avertissement est journalisé** : Calaos ne bloque jamais le moteur de règles. Le thermostat remet ensuite la sortie dans l'état voulu, après `put_back_delay` secondes (le délai double tant que l'autre écrivain recommence, jusqu'à `put_back_delay_max`), et journalise chaque remise en erreur. L'assistant MCP, lui, ne voit pas ces sorties et ne peut pas les écrire.

Pour couper un thermostat, écrivez son **mode** (`off`), jamais son relais.

## Le thermostat dans les interfaces

Le statut affiché combine l'action (chauffe, refroidit, en veille), le verrou de durée minimale et les défauts ci-dessus : voir [Afficher et régler]({{% relref "calaos_installer/devices/clients" %}}).
