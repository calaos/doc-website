+++
title = 'Réseau'
date = 2024-03-06T20:15:06Z
weight = 10
summary = "Adresse IP, DNS, Wi-Fi : configurer la connexion réseau du serveur."
+++

## Adresse automatique ou adresse fixe

Par défaut, votre serveur demande une adresse à votre box : c'est le mode **automatique**, ou DHCP. Cela fonctionne immédiatement, sans rien configurer.

L'inconvénient est que cette adresse peut changer, notamment après une coupure de courant ou un redémarrage de la box. Vos applications, qui cherchent le serveur à son ancienne adresse, ne le trouvent alors plus.

{{% notice tip %}}
**Donnez une adresse fixe à votre serveur.** Vous le retrouverez toujours au même endroit, et vos applications continueront de fonctionner après une coupure. C'est le seul réglage réseau vraiment recommandé.

Deux façons d'y arriver : configurer une adresse fixe sur le serveur, comme décrit ci-dessous, ou demander à votre box de toujours attribuer la même adresse à cette machine — cherchez « bail statique » ou « réservation DHCP » dans son interface.
{{% /notice %}}

## Depuis l'écran tactile

Si un écran tactile est branché sur le serveur, l'interface **Calaos Home** permet de configurer entièrement le réseau depuis son écran de configuration : choix entre adresse automatique et adresse fixe, saisie de l'adresse, du masque, de la passerelle et des serveurs DNS.

C'est la méthode la plus simple, et elle couvre tous les cas.

## Sans écran tactile

Connectez-vous au serveur en SSH (voir [Accès SSH]({{% relref "calaos_os/advanced/shell" %}})).

### Voir les interfaces réseau

Commencez par regarder de quelles connexions dispose la machine :

```sh
calaos-os network list
```

La liste affiche le nom de chaque interface. Repérez celle qui vous intéresse : les noms commençant par `en` correspondent en général aux connexions par câble, ceux commençant par `wl` au Wi-Fi.

### Passer en adresse automatique

```sh
calaos-os network configure dhcp INTERFACE
```

en remplaçant `INTERFACE` par le nom relevé à l'étape précédente.

### Configurer une adresse fixe

```sh
calaos-os network configure static INTERFACE ADRESSE MASQUE PASSERELLE DNS
```

Par exemple, pour donner l'adresse `192.168.1.50` à une machine dont la box est en `192.168.1.1` :

```sh
calaos-os network configure static eth0 192.168.1.50 255.255.255.0 192.168.1.1 192.168.1.1
```

Dans cet exemple :

- `192.168.1.50` est l'adresse que vous choisissez pour le serveur ;
- `255.255.255.0` est le masque de réseau, cette valeur convient à la quasi-totalité des installations domestiques ;
- `192.168.1.1` est l'adresse de votre box, qui sert à la fois de passerelle et de serveur DNS.

{{% notice warning %}}
Choisissez une adresse **en dehors de la plage distribuée automatiquement par votre box**, sinon deux appareils pourraient se retrouver avec la même adresse. Cette plage est indiquée dans l'interface de votre box, souvent dans la section DHCP.
{{% /notice %}}

Vous pouvez indiquer plusieurs serveurs DNS à la suite, séparés par des espaces.

## Vérifier que ça marche

Après le changement, vérifiez que le serveur voit toujours Internet :

```sh
ping -c 3 calaos.fr
```

Trois réponses signifient que tout va bien.

{{% notice info %}}
Si vous étiez connecté en SSH et que vous avez changé l'adresse du serveur, **votre connexion est coupée** : c'est normal, le serveur n'est plus à la même adresse. Reconnectez-vous à la nouvelle.
{{% /notice %}}

## Le nom de la machine

Sur votre réseau, le serveur porte aussi un nom, qui permet parfois de l'atteindre sans connaître son adresse. Pour le consulter :

```sh
hostname
```

## Si vous ne trouvez plus votre serveur

Regardez la page [En cas de problème]({{% relref "calaos_os/troubleshooting" %}}), qui traite ce cas en détail.
