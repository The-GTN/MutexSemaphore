Yield
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
  * [pingpong.c](pingpong.c) : le code de `pingpong` (utilisant notre `yield`) (pingpong qui s'arrête)
  * [pingpongtimer.c](pingpongtimer.c) : le code de `pingpong` mais qui utilise également le timer (s'arrête)
  * [pingponginfinite.c](pingponginfinite.c) : le code de `pingpong` mais en infini
  * [pingponginfinitetimer.c](pingponginfinitetimer.c) : le code de `pingpong` mais qui utilise également le timer en infini
  * [pingpongpang.c](pingpongpang.c) : le code de `pingpongpang` (utilisant notre `yield`) (pingpongpang qui s'arrête)
  * [pingpongpangtimer.c](pingpongpangtimer.c) : le code de `pingpongpang` mais qui utilise également le timer (s'arrête)
  * [pingpongpanginfinite.c](pingpongpanginfinite.c) : le code de `pingpongpang` mais en infini
  * [pingpongpanginfinitetimer.c](pingpongpanginfinitetimer.c) : le code de `pingpongpang` mais qui utilise également le timer en infini

# Commande

* **make** : fait un **make EXE** où **EXE** est l'ensemble des executable possible
* **make <param>** : utilise le fichier <param>.c (il faut donc le fichier correspondant) et compile le <param>.o pour ensuite créer l'executable <param> (et ensuite supprime le .o)
* **make clean** : supprime les .o du dossier et les executables

# Utilisation

Effectuer la commande suivant : *make pingpong* pour créer l'executable *pingpong* du projet

Ensuite, vous pourrez lancer la commande *./pingpong* afin de constater le ping pong.

> Libre à vous de faire un *make* afin de créer les autres executables du projet
