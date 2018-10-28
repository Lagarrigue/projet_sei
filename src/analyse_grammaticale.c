#include "analyse_grammaticale.h"
#include <string.h>


 
/* 
section : 
0 -> aucune ( si utilisateur oublie au debut)
1 -> .text
2 -> .bss
3 -> .data	
*/ 

void init (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data) {
	enum {NL, VIRGULE, DEUX_PTS, COMMENTAIRE, REGISTRE, DIRECTIVE, SYMBOL_ALPHA, VALEUR_DEC, VALEUR_HEX, SYMBOL_ALPHA_NUM, SIGNE, GUILLEMETS, PARENTHESE} ;
	while ( l != NULL ) {
		switch (l->val.nom_type) {
	
			case DIRECTIVE :
				if (strcmp(l->val.valeur,"space") == 0) {
					l=charge_space (l, section , dec , pl_text , pl_bss , pl_data) ;
					l=l->suiv ;				
				}
				else if (strcmp(l->val.valeur,"set") == 0) {
				}
				else if (strcmp(l->val.valeur,"word") == 0) {
					l = charge_word (l, section , dec , pl_text , pl_bss , pl_data) ;
					l=l->suiv ;
				}
				else if (strcmp(l->val.valeur,"byte") == 0) {
				}
				else if (strcmp(l->val.valeur,"asciiz") == 0) {
				}
				else if (strcmp(l->val.valeur,"text") == 0) {
					section = 1 ;
					l=l->suiv ;
				}
				else if (strcmp(l->val.valeur,"bss") == 0) {
					section = 2 ;
					l=l->suiv ;
				}
				else if (strcmp(l->val.valeur,"data") == 0) {
					section = 3 ;
					l=l->suiv ;
				}
				else { /* Message d'erreur */
				}
				break ;
	
			case SYMBOL_ALPHA :
				break ;
	
			case DEUX_PTS :
				break ;
	
			case NL :
				init(l->suiv, section, dec, pl_text, pl_bss, pl_data );
				break ;
			
			case COMMENTAIRE :
				init(l->suiv, section, dec, pl_text, pl_bss, pl_data );
				break ;
		}
	}	
}

/* FONCTIONS ANNEXES */
 

/* CAS DIRECTIVE */
 

L_LEXEME charge_space (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data ) {
	LEXEME lexeme = l->val ;
	l=l->suiv ;
	OPERANDE operande ;
	BSS donnee2 ;
	DATA donnee3 ;	
	if (l->val.nom_type == 8 || l->val.nom_type == 9 ) { /* si valeur décimale ou hexadécimale*/
		strcpy(operande.val, l->val.valeur) ;
	}
	else { /* sinon message d'erreur */
	}
	switch (section) {

		case 1 :
		/* Erreur car impossible dans .text */
			break ;

		case 2 :
		donnee2.ligne=lexeme.numero_ligne ;
		donnee2.decalage=**(dec+1) ;
		donnee2.operande= operande ;
		strcpy(donnee2.directive,".space") ;
		*pl_bss = ajout_tete_L_BSS(donnee2,*pl_bss) ;
		**(dec+1) += atoi(operande.val) ;
			break ;

		case 3 :
		donnee3.ligne=lexeme.numero_ligne ;
		donnee3.decalage=**(dec+2) ;
		strcpy(donnee3.directive,".space") ;
		donnee3.operande=operande  ;
		*pl_data = ajout_tete_L_DATA (donnee3,*pl_data) ;
		**(dec+2) += atoi(operande.val) ;
			break ;
	}
	return l ;
	}
		


L_LEXEME charge_word (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data ) {
	LEXEME lexeme = l->val ;
	OPERANDE operande ;
	TEXT donnee1 ;
	DATA donnee3 ;
	do {	
		l=l->suiv ;
		if (l->val.nom_type == 8 || l->val.nom_type == 9 || l->val.nom_type == 7) { 
		/* si valeur décimale, hexadécimale ou symbole alpha*/
			strcpy(operande.val, l->val.valeur) ;
		}
		else { /* sinon message d'erreur A FAIRE */
		}
		switch (section) {
	
			case 1 :
			donnee1.ligne=lexeme.numero_ligne ;
			donnee1.decalage=**dec ;
			donnee1.nb_op=1 ;
			strcpy(donnee1.instruction,".word") ;
			donnee1.t_operande[0]=operande  ;
			*pl_text = ajout_tete_L_TEXT (donnee1,*pl_text) ;
			**dec += 4 ;
				break ;
	
			case 2 :
			/* Erreur car impossible dans .bss*/
				break ;
	
			case 3 :
			donnee3.ligne=lexeme.numero_ligne ;
			donnee3.decalage=**(dec+2) ;
			strcpy(donnee1.instruction,".word") ;
			donnee3.operande=operande  ;
			*pl_data = ajout_tete_L_DATA (donnee3,*pl_data) ;
			**(dec+2) += 4 ;
				break ;

		}
	} while ( ( l=(l->suiv) )->val.nom_type == 2 ) ; /* Si on a une virgule donc d'autre opérandes, on recommence */
	return l ;
}

/* salut */ 

L_LEXEME charge_byte (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data ) {
	LEXEME lexeme = l->val ;
	OPERANDE operande ;
	TEXT donnee1 ;
	DATA donnee3 ;
	do {	
		l=l->suiv ;
		if (l->val.nom_type == 8 || l->val.nom_type == 9 || l->val.nom_type == 7) { 
		/* si valeur décimale, hexadécimale ou symbole alpha*/
			strcpy(operande.val, l->val.valeur) ;
		}
		else { /* sinon message d'erreur A FAIRE */
		}
		switch (section) {
	
			case 1 :
			donnee1.ligne=lexeme.numero_ligne ;
			donnee1.decalage=**dec ;
			donnee1.nb_op=1 ;
			copie_tab_char(donnee1.instruction,".byte",512) ;
			donnee1.t_operande[0]=operande  ;
			*pl_text = ajout_tete_L_TEXT (donnee1,*pl_text) ;
			**dec += 1 ;
				break ;
	
			case 2 :
			/* Erreur car impossible dans .bss*/
				break ;
	
			case 3 :
			donnee3.ligne=lexeme.numero_ligne ;
			donnee3.decalage=**(dec+2) ;
			strcpy(donnee1.instruction,".byte") ;
			donnee3.operande=operande  ;
			*pl_data = ajout_tete_L_DATA (donnee3,*pl_data) ;
			**(dec+2) += 1 ;
				break ;

		}
	} while ( ( l=(l->suiv) )->val.nom_type == 2 ) ; /* Si on a une virgule donc une autre opérande ensuite, on recommence */
	
	return l ;
}

/* L_LEXEME charge_asciiz (LEXEME lexeme, int section) {} */
 
/* CAS SYMB_ALPHA */
