Dépôt ASE
===================

# Binôme

* [Suliac Lavenant](mailto:suliac.lavenant.etu@univ-lille.fr)
* [Antoine Nollet](mailto:antoine.nollet.etu@univ-lille.fr)

# Contenu

* [Readme.md](Readme.md) : le fichier que vous lisez
* [/TryThrow](/TryThrow) : le TP1 sur le retour à un contexte (try throw)
* [/SwitchTo](/SwitchTo) : Le TP2 sur la création et changement de contexte (switchTo)
* [/yield](/yield) : Le TP3 sur l'ordonnancement sur interruptions (yield et timer)
* [/sem](/sem) : Le TP4 sur la synchronisation entre contextes (semaphores)
* [/mutex](/mutex) : Le TP final sur la prévention des interblocages (mutex).


# Explications Gestion Interbloquage

## Problème des Philosophes

L'académique et classique problème des philosophes est le suivant : cinq philosophes attablés en cercle autour d'un plat de spaghettis mangent et pensent alternativement sans fin. Une fourchette est disposée entre chaque couple de philosophes voisins. Un philosophe doit préalablement s'emparer des deux fourchettes qui sont autour de lui pour manger.    

## Solution Triviale

Une première résolution est disponible [ici](/sem/src/philo.c).
Nous utilisons des sémaphores avec des compteurs à 1 pour représenter les différentes fourchettes, et chaque philosophe est représenté par un contexte.

En fonction du délai de pensée, un bloquage peut survenir. Tout les philosophe ont pris leur fourchette gauche mais maintenant ils ne peuvent pas prendre la droite car elle est déja utilisé. Ce problème mène à un SegmentFault, à un interbloquage.


## Solution Prévention Interbloquage

Notre implémentation des mutex est visible [ici](/mutex/lib/switch.c).

Un mutex comprend 4 chose :
* Un contexte propriétaire
* Un booléen indiquant s'il est verrouillé ou non
* Un pointeur contexte indiquant le premier contexte en attente pour ce mutex (un pointeur a aussi été rajouté dans la structure du contexte pour faire une liste chainé)
* Un pointeur sur un mutex qui sert a faire la liste des mutex possédé par un contexte (le depart de cette liste est un pointeur dans la structure du contexte)

Il a fallut ajouté en attribut de nos contextes :
* Un pointeur indiquant quel est le prochain contexte bloqué par le mutex qui bloque ce contexte
* Un pointeur sur le premier mutex possédé par ce contexte, qui donnera accès aux autres mutex possédés

Pour l'implémentation de nos mutex, nous avons défini 3 opérations de base :
* void mutex_init(struct mutex_s *mutex)
* void mutex_lock(struct mutex_s *mutex)
* void mutex_unlock(struct mutex_s *mutex)

mutex_init initialise notre mutex avec ses valeurs par défaut. (voir [ici](/mutex/lib/switch.c) pour plus de détails)

mutex_lock suit les étapes suivantes :
* désactive les interruptions par timer
* ajout du contexte courant a la fin de la liste des contextes bloqués par le mutex
* état du contexte courant passe à bloqué par un mutex
* tente la détection d'interblocages des mutex (plus de détails par la suite)
* réactive les interruptions par timer
* attend que le mutex se libère et que le contexte courant soit le premier dans la liste des bloqués par le mutex
* désactive les interruptions par timer
* le mutex se verouille et le contexte courant devient son propriétaire  
* le contexte n'est plus bloqué par le mutex, il peut s'executer
* enleve le contexte courant de la liste des contextes bloqués par le mutex
* ajoute a la liste des mutexs possédé par le contexte courant le mutex actuel
* réactive les interruptions par timer

mutex_unLock suit les étapes suivantes :
* désactive les interruptions par timer
* déverrouille, seulement si le contexte courant est le propriétaire du mutex
* enleve la mutex de la liste des mutex possédés par le contexte courant
* réactive les interruptions par timer


Notre détection de l'interblocage, utilisé dans mutex_lock, consistera à parcourir les contextes de l'anneau des contextes existants et à compter le nombre de contextes qui sont non-bloqués par un mutex (via le statut ). Si le nombre de contextes non-bloqués par un mutex est égal à 0, alors tout les contextes sont bloqués, il y a interbloquage, on devra alors stopper le programme et retourner une erreur d'interblocage.

Si on décommente la ligne 46 du code de [philo.c](/mutex/src/philo.c) , cela rajoutera un contexte et ainsi, même si les philosophes se casseront la tête à manger leurs pâtes, un autre contexte pourra continuer d'exister.
