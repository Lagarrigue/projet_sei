#include "analyse_lexicale.h"
#include <stdlib.h>

int main() {
	L_LEXEME l_lexeme ;
	l_lexeme = creer_liste() ;
	char nom_fichier[512] ;
	puts("Entrer le nom du fichier :") ;
	scanf("%s", nom_fichier) ;
	puts("Analyse lexicale en cours ...") ;
	l_lexeme=analyse_lexicale(nom_fichier) ;
	puts("Analyse lexicale teerminée.") ;
	int a=0 ;
	while (a != 1 && a != 2 ) {
		puts("Afficher la liste des lexemes ?  OUI (1)  NON (2)") ;
		scanf("%d",&a) ;
		if (a == 1) lecture_liste_lexeme(l_lexeme)  ; 
	}
	return 0 ;
}

