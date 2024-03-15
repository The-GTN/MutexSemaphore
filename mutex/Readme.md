Mutex
===================

# Binôme

* [Suliac Lavenant](mailto:suliac.lavenant.etu@univ-lille.fr)
* [Antoine Nollet](mailto:antoine.nollet.etu@univ-lille.fr)

# Contenu

* [Readme.md](Readme.md) : le fichier que vous lisez
* [makefile](makefile) : le fichier facilitant l'utilisation du projet
* [/header](./header/) : dossier contenant les headers du projet
  * [hardware.h](header/hardware.h) : entête pour utiliser le timer
  * [hwconfig.h](header/hwconfig.h) : entête pour utiliser le timer
  * [switch.h](header/switch.h) : entête de notre programme librairie
* [/lib](./lib/) : dossier contenant les codes libraires
  * [hardware.ini](lib/hardware.ini) : fichier initialisation du timer
  * [switch.c](lib/switch.c) : fichier source réutilisé dans les sources
* [/src](./src/) : dossier contenant les codes sources du projet
  * [philo](src/philo.c) : le code du problème des philosophes

# Commande

* **make** : fait un **make EXE** où **EXE** est l'ensemble des executable possible
* **make <param>** : utilise le fichier <param>.c (il faut donc le fichier correspondant) et compile le <param>.o pour ensuite créer l'executable <param> (et ensuite supprime le .o)
* **make clean** : supprime les .o du dossier et les executables

# Utilisation

Effectuer la commande suivant : *make philo* pour créer l'executable *philo* du projet

Ensuite, vous pourrez lancer la commande *./philo* afin de constater les philosophes qui mangent.

> Libre à vous de faire un *make* afin de créer les autres executables du projet
