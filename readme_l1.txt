
### DERNIERE MAJ : INCREMENT 3 ###



Auteurs : Théo Garrigou & Augustin Fournier



Rapide description :

Le programme permet de convertir un fichier texte écrit en language MIMPS en découpage de lexemes.
Ce découpage est réalisé en liste de structure contenant le type du lexeme et la valeur de celui-ci.
Ensuite, le programme analyse la grammaire de ce découpage selon les règles du language MIPS. Il charge une pile pour chaque section (.text, .data et .bss) et un tableau des symboles.



Mise en place et utilisation :

Le programme s'utilise à l'aide du terminal.
Il faut se placer dans le repertoire du code
Il faut dans un premier temps compiler les programmes avec la commande make debug.
Pour lancer le programme, il faut rentrer "./test_analyse_lexicale" puis renseigner le nom d'un fichier .txt à analyser.
Un affichage de la liste de lexeme s'affiche dans le terminale une fois le fichier analysé.
Il y a également un affiche de quelques erreur si il y en as avant la liste de lexemes
