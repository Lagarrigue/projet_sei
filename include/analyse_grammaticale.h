#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "analyse_lexicale.h"
#include "instruction.h"
#include "notify.h"
#include <global.h>



/* ********** STRUCTURES ********** */




/* operandes */

typedef struct BO {
	unsigned char reg ;
	short offset ;} BASE_OFFSET ;
	
typedef struct etiq {
	char nom[512] ;
	char attendu[10] ; } ETIQ ;

typedef union {
	unsigned char reg;
	short imm ;
	unsigned char sa ; 
	ETIQ etiq ;
	short ad_rel;
	unsigned int ad_abs ;
	int nb ;
	long tar ;
	BASE_OFFSET base_offset ; } VAL_OPERANDE ;

 /* typedef enum { REG , IMM , SA , ETIQ , AD_REL , AD_ABS , OFFSET , BASE_OFF , TARGAET, ASCIIZ } TYPE_VAL_OPERANDE ; 
		   1      2     3    4      5        6         7        8          9        10
*/



typedef struct {
	int type ;
	VAL_OPERANDE val ; } OPERANDE ;
	
	
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
	int defined ;
	int ligne ; /* ligne de definition dans le code et non pas d'utilisation ! */
	int section ; /* 1=text 2=DATA 3=BSS */
	int decalage ; } SYMB ;

typedef struct l_symb {
    	SYMB val ;
    	struct l_symb *suiv ; } * L_SYMB ;


/* ********** FONCTIONS ANALYSE GRAMMATICALE ********** */

void init (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data, L_SYMB* pl_symb, L_SYMB* pl_attente, L_INSTRUCTION* dictionnaire) ;

L_LEXEME charge_space (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data ) ;
L_LEXEME charge_word (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data ) ;
L_LEXEME charge_byte (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data ) ;
L_LEXEME charge_asciiz (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data) ;
L_LEXEME charge_set (L_LEXEME l) ;

L_LEXEME charge_symbole (L_LEXEME l, int section, int** dec, L_SYMB* pl_attente) ;
L_SYMB* maj_symbole(int** dec, int section, L_SYMB* pl_attente, L_SYMB* pl_symb, int word ) ;


/* ********** FONCTIONS POUR LES OPERANDES ********** */

L_LEXEME charge_instruction (L_LEXEME l , int** dec, L_TEXT* pl_text, INSTRUCTION instruction, L_SYMB*) ;
L_LEXEME signe (L_LEXEME l) ;
short valeur_imm(LEXEME lex) ;
int valeur_sa(LEXEME lex) ;
long valeur_target(LEXEME lex) ;
short valeur_offset(LEXEME lex) ;
unsigned char valeur_reg(LEXEME l) ;
BASE_OFFSET valeur_base_off(LEXEME l, LEXEME suiv ) ;

/* ********** FONCTIONS DE LISTES ********** */

 
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
void lecture_liste_L_TEXT(L_TEXT L) ;

/*  DATA  */

int liste_est_vide_L_DATA(L_DATA L ) ;
L_DATA creer_liste_L_DATA(void) ;
L_DATA ajout_tete_L_DATA(DATA c, L_DATA L) ;
L_DATA supprimer_tete_L_DATA(L_DATA L) ;
void liberer_liste_L_DATA(L_DATA l) ;
void lecture_liste_L_DATA(L_DATA L);

/*  BSS  */

int liste_est_vide_L_BSS(L_BSS L ) ;
L_BSS creer_liste_L_BSS(void) ;
L_BSS ajout_tete_L_BSS(BSS c, L_BSS L) ;
L_BSS supprimer_tete_L_BSS(L_BSS L) ;
void liberer_liste_L_BSS(L_BSS l) ;
void lecture_liste_L_BSS(L_BSS L) ;

/*  SYMB  */

int liste_est_vide_L_SYMB(L_SYMB L ) ;
L_SYMB creer_liste_L_SYMB(void) ;
L_SYMB ajout_tete_L_SYMB(SYMB c, L_SYMB L) ;
L_SYMB supprimer_tete_L_SYMB(L_SYMB L) ;
void liberer_liste_L_SYMB(L_SYMB l) ;
void lecture_liste_L_SYMB(L_SYMB L) ;


/* ********** FONCTIONS DE TABLEAUX  ********** */

SYMB* creer_tab_symb(int l) ;
void conversion_liste_symb_vers_tableau(SYMB* tab, L_SYMB L, int size) ;
void lecture_tab_symb( SYMB* tab, int size) ;
int longueur_l_symb(L_SYMB) ;


