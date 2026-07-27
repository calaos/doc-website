+++
title = 'Scripts en LUA'
date = 2024-03-06T20:13:18Z
weight = 70
summary = "Écrire du code quand les règles ne suffisent plus."
+++

## Quand en avoir besoin

Les règles couvrent la grande majorité des besoins, et il vaut mieux s'y tenir tant qu'elles suffisent : elles se relisent d'un coup d'œil, et n'importe qui peut les modifier.

Un script devient utile quand vous butez sur :

- **un calcul** — une moyenne de plusieurs sondes, une consigne qui dépend de la température extérieure ;
- **une boucle** — traiter une liste d'équipements sans écrire quinze actions ;
- **une logique conditionnelle imbriquée** — plusieurs cas de figure qui donneraient une dizaine de règles ;
- **un appel à un service extérieur** — interroger une API météo, prévenir un service tiers.

Le langage utilisé est **Lua**, choisi pour sa simplicité et sa légèreté.

## Où écrire un script

Un script s'utilise à deux endroits dans une règle :

**En condition** — le script renvoie vrai ou faux, et la règle s'exécute en conséquence.

**En action** — le script s'exécute quand la règle se déclenche.

Dans les deux cas, l'éditeur intégré de Calaos Installer offre la coloration syntaxique.

## Accéder à vos équipements

Un objet global nommé `calaos` donne accès à votre installation. Chaque IO est désignée par son **identifiant**, visible dans ses propriétés.

### Lire et écrire une valeur

```lua
-- Lire l'état d'une IO
local temperature = calaos:getIOValue("io_12")

-- Agir sur une sortie
calaos:setIOValue("output_3", true)
```

`getIOValue` renvoie un booléen, un nombre ou du texte selon le type de l'IO. Les noms `getInputValue`, `getOutputValue` et `setOutputValue` existent aussi et font la même chose : ils datent d'anciennes versions.

### Lire et écrire un paramètre

```lua
local nom = calaos:getIOParam("io_12", "name")
calaos:setIOParam("io_12", "visible", "false")
```

### Attendre un changement

```lua
calaos:waitForIO("io_12")
```

Le script se met en pause jusqu'à ce que l'IO change d'état.

### Interroger une adresse web

```lua
local reponse = calaos:requestUrl("https://exemple.fr/api")
```

### Envoyer une notification

```lua
calaos:sendPushNotif("Le congélateur est au-dessus de -15 °C")
```

### Afficher un message de diagnostic

```lua
print("Température relevée : " .. temperature)
```

`print` écrit dans les journaux du serveur — voir [Journaux]({{% relref "calaos_os/configuration/logs" %}}). C'est votre principal outil pour comprendre ce que fait un script.

## Un exemple complet

Allumer un chauffage d'appoint si la moyenne de trois sondes descend sous la consigne :

```lua
local s1 = calaos:getIOValue("temp_salon")
local s2 = calaos:getIOValue("temp_couloir")
local s3 = calaos:getIOValue("temp_chambre")

local moyenne = (s1 + s2 + s3) / 3
local consigne = calaos:getIOValue("var_consigne")

print("Moyenne : " .. moyenne .. " / consigne : " .. consigne)

if moyenne < consigne then
    calaos:setIOValue("chauffage_appoint", true)
else
    calaos:setIOValue("chauffage_appoint", false)
end
```

La consigne est ici une [variable interne]({{% relref "calaos_installer/internal_var" %}}) modifiable depuis les applications : l'utilisateur ajuste son confort sans qu'on touche au script.

## Bonnes pratiques

{{% notice warning %}}
**Un script bloque le serveur pendant son exécution.** Évitez les boucles longues et les attentes inutiles : une requête vers un site injoignable peut figer votre domotique le temps du délai d'attente.
{{% /notice %}}

**Commentez les identifiants.** `io_12` ne dit rien ; une ligne de commentaire rappelant de quel équipement il s'agit vous fera gagner du temps plus tard.

**Vérifiez vos valeurs avant de calculer.** Une sonde débranchée peut renvoyer une valeur inattendue, et une division par zéro arrêtera le script.

**Testez avec `print`.** Affichez les valeurs intermédiaires dans les journaux avant de faire agir le script sur de vrais équipements.

**Préférez une règle quand c'est possible.** Un script qui ne fait qu'allumer une lampe quand une autre s'allume est plus difficile à maintenir que la règle équivalente.

## Voir aussi

- [Créer des règles]({{% relref "calaos_installer/rules" %}}) — où s'insèrent les scripts
- [Variable interne]({{% relref "calaos_installer/internal_var" %}}) — pour échanger des valeurs avec vos règles
