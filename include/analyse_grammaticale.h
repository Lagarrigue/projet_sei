#include <stdio.h>
#include <stdlib.h>
#include "analyse_lexicale.h"
#include <string.h>



/* ********** STRUCTURES ********** */




/* operandes */

typedef union operande_bis {
	char val1 ;		/* pour .byte */
	int val2 ; 		/* pour .word */
	char val3[512] ; 	/* pour asciiz ou symbole*/
	unsigned int val4 ; 	/* pour .space */
	/* autre ? */
			} OPERANDE_BIS ;

typedef struct operande {
	char val[512] ; } OPERANDE ;
	
typedef struct l_operande {
    	OPERANDE val ;
    	struct l_operande *suiv ; } * L_OPERANDE ;




/*  .text  */

typedef struct text {
	char instruction[512] ;
	int ligne ;
	int decalage ;
	int nb_op ;
	OPERANDE t_operande[3] ; } TEXT ;

typedef struct l_text {
    	TEXT val ;
    	struct l_text *suiv ; } * L_TEXT ;	




/*  .data  */

typedef struct data {
	char directive[512] ;
	int ligne ;
	int decalage ;
	OPERANDE operande ; } DATA ;

typedef struct l_data {
    	DATA val ;
    	struct l_data *suiv ; } * L_DATA ;




/*  .bss  */

typedef struct bss {
	char directive[512] ;
	int ligne ;
	int decalage ;
	OPERANDE operande ; } BSS ;

typedef struct l_bss {
    	BSS val ;
    	struct l_bss *suiv ; } * L_BSS ;




/*  symboles  */

typedef struct symb {
	char symbole[512] ;
	int ligne ; /* ligne de definition dans le code et non pas d'utilisation ! */
	int section ; /* 1=text 2=DATA 3=BSS */
	int decalage ; } SYMB ;

typedef struct l_symb {
    	SYMB val ;
    	struct l_symb *suiv ; } * L_SYMB ;


/* ********** FONCTION ANALYSE GRAMMATICALE ********** */

L_LEXEME charge_space (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data ) ;
L_LEXEME charge_word (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data ) ;
L_LEXEME charge_byte (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data ) ;
L_LEXEME charge_asciiz (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data) ;


/* ********** FONCTION DE LISTES ********** */

 
/*  OPERANDE  */

int liste_est_vide_L_OPERANDE(L_OPERANDE L ) ;
L_OPERANDE creer_liste_L_OPERANDE(void) ;
L_OPERANDE ajout_tete_L_OPERANDE(OPERANDE c, L_OPERANDE L) ;
L_OPERANDE supprimer_tete_L_OPERANDE(L_OPERANDE L) ;
void liberer_liste_L_OPERANDE(L_OPERANDE l) ;


/*  TEXT  */

int liste_est_vide_L_TEXT(L_TEXT L ) ;
L_TEXT creer_liste_L_TEXT(void);
L_TEXT ajout_tete_L_TEXT(TEXT c, L_TEXT L) ;
L_TEXT supprimer_tete_L_TEXT(L_TEXT L) ;
void liberer_liste_L_TEXT(L_TEXT l) ;


/*  DATA  */

int liste_est_vide_L_DATA(L_DATA L ) ;
L_DATA creer_liste_L_DATA(void) ;
L_DATA ajout_tete_L_DATA(DATA c, L_DATA L) ;
L_DATA supprimer_tete_L_DATA(L_DATA L) ;
void liberer_liste_L_DATA(L_DATA l) ;


/*  BSS  */

int liste_est_vide_L_BSS(L_BSS L ) ;
L_BSS creer_liste_L_BSS(void) ;
L_BSS ajout_tete_L_BSS(BSS c, L_BSS L) ;
L_BSS supprimer_tete_L_BSS(L_BSS L) ;
void liberer_liste_L_BSS(L_BSS l) ;


/*  SYMB  */

int liste_est_vide_L_SYMB(L_SYMB L ) ;
L_SYMB creer_liste_L_SYMB(void) ;
L_SYMB ajout_tete_L_SYMB(SYMB c, L_SYMB L) ;
L_SYMB supprimer_tete_L_SYMB(L_SYMB L) ;
void liberer_liste_L_SYMB(L_SYMB l) ;


/* ********** FONCTION DE TABLEAUX  ********** */

SYMB* creer_tab_symb(int l) ;
SYMB* conversion_liste_symb_vers_tableau(L_SYMB L) ;




