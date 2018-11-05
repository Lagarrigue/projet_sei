#include "analyse_grammaticale.h"
#include <stdlib.h>

int main() {
	L_LEXEME l_lexeme ;
	l_lexeme = creer_liste() ;
	char nom_fichier[512] = "tests/test1.txt" ;
	int section=0 ;
	int* dec[3] ; 
	if (dec = calloc(3,sizeof(int*)) == NULL ) {
		puts("Erreur d'allocation") ;
		return 0 ;
		}
	int a=0 ;
	L_TEXT l_text = creer_liste_L_TEXT() ;
	L_BSS l_bss = creer_liste_L_BSS() ;
	L_DATA l_data = creer_liste_L_DATA() ;
	L_SYMB l_symb = creer_liste_L_SYMB() ;;
	L_SYMB l_attente = creer_liste_L_SYMB() ;
	/*puts("Entrer le nom du fichier :") ;
	scanf("%s", nom_fichier) ;*/
	puts("Analyse lexicale en cours ...") ;
	l_lexeme=analyse_lexicale(nom_fichier) ;
	puts("Analyse lexicale teerminée.") ;
	l_lexeme = supprimer_tete (l_lexeme) ; /* car 1er lexeme est une erreur */ 
	while (a != 1 && a != 2 ) {
		puts("Afficher la liste des lexemes ?  OUI (1)  NON (2)") ;
		scanf("%d",&a) ;
		if (a == 1) { lecture_liste_lexeme(l_lexeme)  ; } 
	}
	for (a=0 ; a<3 ; a++ ) {
		dec[a] = 0 ;
	}
	init (l_lexeme, section, dec, &l_text, &l_bss, &l_data, &l_symb, &l_attente) ;
	return 0 ;
}

