Try Throw
===================

# Binôme

* [Suliac Lavenant](mailto:suliac.lavenant.etu@univ-lille.fr)
* [Antoine Nollet](mailto:antoine.nollet.etu@univ-lille.fr)

# Contenu

* [Readme.md](Readme.md) : le fichier que vous lisez
* [makefile](makefile) : le fichier facilitant l'utilisation du projet
* [try.h](try.h) : le header de [try.c](try.c), définissant les signatures `try` et `throw`
* [try.c](try.c) : le code de `try` et de `throw`
* [try_mul.c](try_mul.c) : le code de `mul` utilisant notre `try` et notre `throw`

# Commande

* **make** : fait un **make try_mul**
* **make <param>** : utilise le fichier <param>.c (il faut donc le fichier correspondant) et compile le <param>.o pour ensuite créer l'executable <param> (et ensuite supprime le .o)
* **make clean** : supprime les .o du dossier
* **make totalclean** : supprime les .o et les .exe du dossier

# Utilisation

Effectuer la commande suivant : *make* pour créer l'executable *try_mul* du projet

Ensuite, vous pourrez lancer la commande *./try_mul* afin de pouvoir rentrer une suite d'entier dont on voudra le produit.

> Faire ctrl+d afin de mettre fin au recueil des entiers.
> Lorsque vous rentrer un 0, cela met aussi fin au recueil des entiers
