#include <stdio.h>
#include <stdlib.h>



/* ********** STRUCTURES ********** */


/*
LEXEME est la structure qui comprend un numero (correspondant au type du lexeme lu)
et val la chaine de caractere correspondant a la valeur du lexeme
*/

typedef struct lexeme {
	int nom_type ;
	int numero_ligne;
	char valeur[512] ;} LEXEME ;


/*
L_LEXEME est une liste de struct LEXEME wesh alors
*/

typedef struct maillon {
    LEXEME val;
    struct maillon *suiv ; } * L_LEXEME ;





/* ********** TABLEAU DE LEXEME ********** */


/*	
	1 -> NL
	2 -> VIRGULE
	3 -> DEUX_PTS
	4 -> COMMENTAIRE
	5 -> REGISTRE
	6 -> SYMBOLE_ALPHA
	7 -> DIRECTIVE
	8 -> VALEUR_DECIMALE
	9 -> VALEUR_HEXADECIMALE
	10 -> SYMBOLE_ALPHA_NUM
	11 -> SIGNE
	12 -> GUILLEMETS
	13 -> PARENTHESES
*/





/* ********** FONCTION ANALYSE LEXICAL ********** */


/* 
Fonction qui ouvre un fichier texte, l'analyse ligne par ligne pour former une liste de LEXEME
ENTREES : pl_lexem : pointeur sur la liste de lexemes
    	  nom_fichier : nom du fichier texte à analyser
SORTIES : void
*/

L_LEXEME analyse_lexicale(char* nom_fichier) ;

void copie_tab_char(char a[], char b[], int taille) ;

void initialisation_lexeme(LEXEME* l) ;

void initialisation_tab_char(char tab[], int taille) ;

void lecture_liste_lexeme(L_LEXEME L) ;


/* ********** FONCTIONS LISTES DE LEXEMES ********** */

L_LEXEME creer_liste (void) ;
L_LEXEME ajout_tete (LEXEME lex, L_LEXEME L) ;
L_LEXEME supprimer_tete (L_LEXEME L) ;
void afficher_liste (L_LEXEME L) ;
void liberer_liste (L_LEXEME L) ;
int est_vide( L_LEXEME L) ;
L_LEXEME renversement_liste(L_LEXEME L1);











