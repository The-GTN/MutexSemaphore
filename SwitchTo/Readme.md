SwitchTo
===================

# Binôme

* [Suliac Lavenant](mailto:suliac.lavenant.etu@univ-lille.fr)
* [Antoine Nollet](mailto:antoine.nollet.etu@univ-lille.fr)

# Contenu

* [Readme.md](Readme.md) : le fichier que vous lisez
* [makefile](makefile) : le fichier facilitant l'utilisation du projet
* [switch.h](switch.h) : le header de [switch.c](switch.c), définissant les signatures `switch`
* [switch.c](switch.c) : le code de `switchTo`
* [pingpong.c](pingpong.c) : le code de `pingpong` utilisant notre `switchTo`
* [pingponginfinite.c](pingponginfinite.c) : le code de `pingpong` utilisant notre `switchTo` mais version infini
* [pingpongpang.c](pingpongpang.c) : le code de `pingpongpang` utilisant notre `switchTo`
* [pingpongpanginfinite.c](pingpongpanginfinite.c) : le code de `pingpongpang` utilisant notre `switchTo` mais version infini

# Commande

* **make** : fait un **make pingpong pingpongpang pingponginfinite pingpongpanginfinite** (ce qui crée tout les executables du projet)
* **make <param>** : utilise le fichier <param>.c (il faut donc le fichier correspondant) et compile le <param>.o pour ensuite créer l'executable <param> (et ensuite supprime le .o)
* **make clean** : supprime les .o du dossier et les executables

# Utilisation

Effectuer la commande suivant : *make pingpong* pour créer l'executable *pingpong* du projet

Ensuite, vous pourrez lancer la commande *./pingpong* afin de constater le ping pong.

> Libre à vous de faire un *make* afin de créer les autres executables du projet
