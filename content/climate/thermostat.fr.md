+++
title = "Thermostat interne"
date = 2026-10-15T09:10:00Z
weight = 30
summary = "Calaos régule : hystérésis, durées minimales, sûretés (sonde muette, point de rosée, sonde hors limites), une sortie par thermostat."
+++

## Quand l'utiliser

Le thermostat interne (`thermostat`) est fait pour les installations où **rien ne régule à votre place** : des radiateurs électriques derrière un relais, une vanne de plancher chauffant, une chaudière avec un contact, une pompe à chaleur pilotée sur contacts secs. Calaos lit une sonde de température, compare à la consigne, et commute un ou deux relais.

Si l'appareil possède sa propre régulation (clim split, pompe à chaleur ESPHome), préférez le mode relayé : voir [Clim ESPHome]({{% relref "climate/esphome" %}}).

## Ce que le thermostat fait, et ce qui reste à vos règles

La **régulation** est dans le thermostat et ne se modifie pas : elle réagit à chaque changement de la température, de la consigne, du mode et de l'humidité, ainsi qu'à ses minuteurs.

La **politique** reste à vos règles : abaissement de nuit, absence, fenêtre ouverte, programmation horaire. Elles écrivent la consigne ou le mode du thermostat, comme elles le feraient pour n'importe quel IO. Le thermostat n'a pas de planificateur interne ; voir [Règles de chauffage]({{% relref "climate/rules" %}}).

## La régulation

Le thermostat a un mode (`off`, `heat`, et `cool` et `auto` s'il possède une sortie de refroidissement) et une ou deux consignes :

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
| Plancher chaud / froid | 0,5 °C | 900 s | 900 s | 2 °C |

Un plancher a une grande inertie : une hystérésis large et de longues durées évitent les à-coups. Pour tous les modèles, `sensor_timeout` vaut 9000 s et `put_back_delay` 1 s (maximum 60 s). Toutes ces valeurs se changent ensuite dans l'éditeur de device.

## Les sûretés

Un thermostat qui commande des relais ne doit jamais rester allumé sur une information fausse. Dans tous les cas ci-dessous, **les sorties sont coupées**, le mode et la consigne restent réglables, et la régulation reprend d'elle-même quand la situation redevient normale.

| Situation | Ce qui est affiché | Détail |
|---|---|---|
| Au démarrage, la sonde n'a encore rien lu | « En attente de la sonde » | Pas de régulation tant que la sonde n'a pas livré une valeur après le démarrage. Une valeur gardée en mémoire ou retenue par MQTT ne compte pas. |
| La sonde n'a rien envoyé depuis `sensor_timeout` secondes | « Sonde muette » | Même une valeur identique répétée compte comme une lecture. 9000 s par défaut, au-dessus d'un rapport horaire Zigbee manqué. |
| La sonde se déclare déconnectée | « Sonde déconnectée » | Lien de connexion de la sonde (MQTT, Wago...). |
| Le lien `current_temp` n'existe pas | « Sonde absente » | Device mal configuré. |
| Plancher : humidité trop haute, muette ou jamais lue | « Humidité trop haute » ou « Sonde d'humidité muette » | Voir le point de rosée ci-dessous. |
| La lecture sort des bornes de vraisemblance | « Sonde hors limites » | Voir plus bas (fonction en cours de développement). |

### Le point de rosée (plancher rafraîchissant)

Refroidir un plancher sous le point de rosée fait **condenser** l'humidité de l'air sur le sol. Avec l'émetteur « Plancher » et une sortie de refroidissement, Calaos exige donc un lien `humidity` et coupe le refroidissement tant que l'humidité atteint `humidity_max` (60 % par défaut dans le modèle « Plancher chaud / froid », 65 % si le paramètre est absent), ou tant que la sonde d'humidité est muette ou n'a jamais lu.

**Sans sonde d'humidité, un plancher n'a ni mode `cool` ni mode `auto`** : ils sont refusés au chargement (le refus est journalisé) et ne sont pas proposés dans les interfaces.

### Sonde hors limites

{{% notice warning %}}
Cette fonction (ticket CLIM-9) est **en cours de développement** : les noms ci-dessous peuvent encore évoluer avant la publication.
{{% /notice %}}

Une sonde peut mentir sans se taire : une sonde Wago dont le câble est coupé lit parfois une valeur absurde (-50 °C, 0 °C) au lieu de se déclarer déconnectée. Deux paramètres du thermostat, `valid_min` et `valid_max`, bornent les lectures acceptées. Sans ces paramètres, aucun contrôle n'est fait.

Une lecture hors bornes compte comme **une absence de lecture valide** : les sorties sont coupées, le thermostat affiche **« Sonde hors limites »**, le mode et la consigne restent réglables, et tout redevient normal dès la première lecture dans les bornes. Les bornes d'une pièce, d'un plancher ou d'une eau chaude ne sont pas les mêmes ; les modèles en proposent. Exemple pour une pièce : `valid_min` = 5 et `valid_max` = 40.

## Une sortie, un thermostat

Un relais lié comme sortie de chauffage ou de refroidissement appartient à **un seul** thermostat. Deux thermostats qui écrivent le même relais se battraient : l'un l'allume, l'autre l'éteint. Calaos refuse donc le second lien au chargement (un message dans le journal), et l'assistant d'installation ne propose pas une sortie déjà prise.

![Sortie déjà prise, refusée par l'éditeur](/en/climate/images/inst-output-refused.png?width=30pc&classes=shadow)
*Un relais déjà utilisé par un autre thermostat est refusé.*

Pour un chauffage à plusieurs zones qui partagent une chaudière ou un circulateur, on crée **une sortie par zone** (une vanne ou un relais par thermostat), et **une règle** qui allume la sortie commune dès qu'une zone chauffe. Voir l'exemple dans [Règles de chauffage]({{% relref "climate/rules" %}}).

## Les sorties sont protégées

Les relais pilotés par un thermostat ne doivent pas être écrits par ailleurs. Si une règle, un script Lua ou une commande écrit directement l'un d'eux, la valeur est appliquée **mais un avertissement est journalisé** : Calaos ne bloque jamais le moteur de règles. Le thermostat remet ensuite la sortie dans l'état voulu, après `put_back_delay` secondes (le délai double tant que l'autre écrivain recommence, jusqu'à `put_back_delay_max`), et journalise chaque remise en erreur. L'assistant MCP, lui, ne voit pas ces sorties et ne peut pas les écrire.

Pour couper un thermostat, écrivez son **mode** (`off`), jamais son relais.

## Le thermostat dans les interfaces

Le statut affiché combine l'action (chauffe, refroidit, en veille), le verrou de durée minimale et les défauts ci-dessus : voir [Afficher et régler]({{% relref "climate/clients" %}}).
