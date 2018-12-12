#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <strings.h>
#include <string.h>
#include "notify.h"


/* ********** STRUCTURES ********** */


/*
LEXEME est la structure qui comprend un numero (correspondant au type du lexeme lu)
et val la chaine de caractere correspondant a la valeur du lexeme
*/

typedef struct lexeme {
	int nom_type ;
	int numero_lexeme;
	int numero_ligne;
	char valeur[512] ;
	int reloc ;} LEXEME ;


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

L_LEXEME signe (L_LEXEME liste ) ;
/* ********** FONCTIONS LISTES DE LEXEMES ********** */

L_LEXEME creer_liste (void) ;
L_LEXEME ajout_tete (LEXEME lex, L_LEXEME L) ;
L_LEXEME supprimer_tete (L_LEXEME L) ;
void afficher_liste (L_LEXEME L) ;
void liberer_liste (L_LEXEME L) ;
int est_vide( L_LEXEME L) ;
L_LEXEME renversement_liste(L_LEXEME L1);

/* ********** STRUCTURES PSEUDO INSTRUCTION POUR DICTIONNAIRE********** */

typedef struct {
    char nom_inst[512];
    int nb_op;
    char type_instruction[2];  
    char type_op[3][10] ; } PSEUDO_INSTRUCTION;

typedef struct maillond {
    PSEUDO_INSTRUCTION val;
    struct maillond * suiv ; }* L_PSEUDO_INSTRUCTION;

/* ********** FONCTION DICTIONNAIRE********** */

void initialisation_tab_char_dictionnaire_pseudo(char tab[], int taille);
L_PSEUDO_INSTRUCTION* creer_dictionnaire_pseudo(int l);
L_PSEUDO_INSTRUCTION creer_liste_dictionnaire_pseudo(void);
int liste_est_vide_dictionnaire_pseudo(L_PSEUDO_INSTRUCTION L );
L_PSEUDO_INSTRUCTION ajout_tete_dictionnaire_pseudo(PSEUDO_INSTRUCTION c, L_PSEUDO_INSTRUCTION L) ;
L_PSEUDO_INSTRUCTION supprimer_tete_dictionnaire_pseudo(L_PSEUDO_INSTRUCTION L);
void liberer_liste_dictionnaire_pseudo(L_PSEUDO_INSTRUCTION l);
int hash_pseudo(char nom[], int l);
L_PSEUDO_INSTRUCTION*  lecture_dictionnaire_pseudo(int longueur_table) ;
PSEUDO_INSTRUCTION* recherche_element_pseudo(char mot[], L_PSEUDO_INSTRUCTION* dicti, int longueur_table) ;


/* ********** FONCTION POUR LA GESTION DES PSEUDOS INSTRUCTIONS********** */
L_LEXEME suppression_maillon(L_LEXEME l, int num_lex);
L_LEXEME ajouter_maillon(L_LEXEME l, int num_lex, LEXEME lex);
L_LEXEME ajuster_numero_lexeme(L_LEXEME l);
L_LEXEME fonction_chainage(L_LEXEME l, int num_lex, L_LEXEME l2, int nb_operandes);
L_LEXEME remplacement_pseudo_instruction(L_LEXEME l, int num_lex, char nom_pseudo[512]);
L_LEXEME verification_appartenance_pseudo_instruction(L_LEXEME liste_lexemes, L_PSEUDO_INSTRUCTION* dico, int longueur_dico);










