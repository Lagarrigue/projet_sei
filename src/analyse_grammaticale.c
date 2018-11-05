#include "analyse_grammaticale.h"
#include <string.h>


 
/* 
section : 
0 -> aucune ( si utilisateur oublie au debut)
1 -> .text
2 -> .bss
3 -> .data	
*/ 

void init (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data, L_SYMB* pl_symb, L_SYMB* pl_attente) {
	puts ("Debut init") ;
	int S = 0 ;
	int nb_op=0 ; /* utile pour les instructions */
	puts("1") ;
	while ( l != NULL ) {
		S=(l->val).nom_type ;
		puts("2") ;
		switch ( S ) {
			case 6 :
				puts("3");
				if (strcmp(l->val.valeur,"space") == 0) {
					l=charge_space (l, section , dec , pl_text , pl_bss , pl_data) ;
				}
				else if (strcmp(l->val.valeur,"set") == 0) {
					if ( section != 0 ) {
						/* Erreur car .set ne peut etre que au debut */
					}
				}
				else if (strcmp(l->val.valeur,"word") == 0) {
					puts("4") ;
					l = charge_word (l, section , dec , pl_text , pl_bss , pl_data) ;
				}
				else if (strcmp(l->val.valeur,"byte") == 0) {
					l = charge_byte (l, section , dec , pl_text , pl_bss , pl_data) ;
				}
				else if (strcmp(l->val.valeur,"asciiz") == 0) {
					l = charge_asciiz (l, section , dec , pl_text , pl_bss , pl_data) ;
				}
				else if (strcmp(l->val.valeur,"text") == 0) {
					section = 1 ;
				}
				else if (strcmp(l->val.valeur,"bss") == 0) {
					section = 2 ;
				}
				else if (strcmp(l->val.valeur,"data") == 0) {
					puts("4") ;
					section = 3 ;
				}
				else { /* Message d'erreur */
				}
				pl_attente = maj_symbole(dec,section, pl_attente, pl_symb ) ;
				break ;
	
			case 7 :
				if (section != 1 ) { 
				/* Erreur car les instructions doivent etre dans .text */
				}
				else {
					/*					
					** Fonction recherche_dico : doit retourner 0 si l'intruction n'existe pas, 
					** sinon le nombre d'opérandes et le type d'intruction (R, I, J ou P)
					** 
					recherche_element(l->val.valeur, L_INSTRUCTION* dicti, int longueur_table) */	
					l=charge_instruction(l,dec,pl_text,nb_op) ;
				}
				pl_attente = maj_symbole(dec,section, pl_attente, pl_symb ) ;
				break ;
	
			case 3 :
				if (section==0) { /* Erreur */
				}
				else {
					l=charge_symbole( l, section, dec , pl_attente) ;
				}	
				break ;
	
			case 1 :
				break ;
			
			case 4 :
				break ;
		}
	puts("lexeme ok") ;
	l=l->suiv ;
	}	
}


/* CAS SYMB_ALPHA */

L_LEXEME charge_instruction (L_LEXEME l, int** dec, L_TEXT* pl_text, int nb_op ){
	LEXEME lexeme = l->val ;
	l=l->suiv ;
	if (l==NULL) {
		return l ;
	}
	OPERANDE operande ;
	TEXT donnee ;
	int i ;
	strcpy(donnee.instruction, lexeme.valeur) ;
	donnee.ligne = lexeme.numero_ligne ;
	donnee.decalage = **dec ;
	**dec += 4 ;
	donnee.nb_op=nb_op ;
	for (i=0 ; i<nb_op ; i++ ) {
		if ( (l->val).nom_type != 8 && (l->val).nom_type !=9 && (l->val).nom_type !=5 ) { /* Si pas registre, val dec ou hexa */
			/* Message d'erreur */
		}
		else {
			strcpy(operande.val, (l->val).valeur) ;
			operande.type = (l->val).nom_type ;
			donnee.t_operande[i] = operande ;
		}
		if ( ((l=l->suiv)->val).nom_type != 2 ) {
			/* Message d'erreur car virgule attendue => EXIT ? */
		}
		l=l->suiv ;
	}
	*pl_text = ajout_tete_L_TEXT (donnee,*pl_text) ;
	return l ;		
}

/* CAS SYMBOLE */

L_LEXEME charge_symbole (L_LEXEME l, int section, int** dec, L_SYMB* pl_attente) {
	LEXEME lexeme = l->val ;
	SYMB symb ;
	strcpy(symb.symbole, lexeme.valeur) ;
	symb.ligne=lexeme.numero_ligne ;
	symb.section=section ;
	symb.decalage=0 ; /* on met à 0 pour l'instant car on ne le connait pas encore */
	*pl_attente = ajout_tete_L_SYMB (symb,*pl_attente) ; /* on ajoute dans la liste d'attente en attendant de connaitre le decalage */ 
	return(l) ;
}
 
L_SYMB* maj_symbole(int** dec, int section, L_SYMB* pl_attente, L_SYMB* pl_symb ) {
	if ( *pl_attente == NULL ) {
		return NULL ; 
	}
	L_SYMB p = *pl_attente ;
	L_SYMB previous = p ;
	SYMB symb ;
	if ( !liste_est_vide_L_SYMB(p) && (p->val).section==section  ) { /* Cas de la tete */
		symb=p->val ;
		symb.decalage = **(dec+section-1) ;
		*pl_symb = ajout_tete_L_SYMB (symb,*pl_symb) ;
		*pl_attente = supprimer_tete_L_SYMB (*pl_attente) ;
		return maj_symbole( dec , section , pl_attente, pl_symb) ; 
	}
	p=p->suiv ;
	while ( !liste_est_vide_L_SYMB(p) ) { /* Cas d'un maillon qui n'est pas en tete */
		if ( (p->val).section==section ) {
			symb=p->val ;
			symb.decalage = **(dec+section-1) ;
			*pl_symb = ajout_tete_L_SYMB (symb,*pl_symb) ;
			(previous->suiv) = (p->suiv) ;	
			free(p) ;
		}
		previous = p ;
		p = p->suiv ;	
	}
	return pl_attente ;		
}

/* CAS DIRECTIVE */
 

L_LEXEME charge_space (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data ) {
	LEXEME lexeme = l->val ;
	l=l->suiv ;
	if (l==NULL) {
		return l ;
	}
	OPERANDE operande ;
	BSS donnee2 ;
	DATA donnee3 ;	
	if (l->val.nom_type == 8 || l->val.nom_type == 9 ) { /* si valeur décimale ou hexadécimale*/
		strcpy(operande.val, (l->val).valeur) ;
		operande.type = (l->val).nom_type ;
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
	puts("5") ;
	do {	
		l=l->suiv ;
		if (l==NULL) {
			return l ;
		}
		if (l->val.nom_type == 8 || l->val.nom_type == 9 || l->val.nom_type == 7) { 
		/* si valeur décimale, hexadécimale ou symbole alpha*/
			puts("6") ;
			strcpy(operande.val, (l->val).valeur) ;
			operande.type = (l->val).nom_type ;
		}
		else { /* sinon message d'erreur A FAIRE */
		}
		switch (section) {
			
			case 1 :
			donnee1.ligne=lexeme.numero_ligne ;
			if ( (**dec)%4 != 0 ) **dec += (4-(**dec)%4) ; /* on aligne sur 4 octets si besoin */
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
			puts("7") ;
			donnee3.ligne=lexeme.numero_ligne ;
			printf("%d \n");
			if ( (**(dec+2))%4 != 0 ) {
				**(dec+2) += (4-(**(dec+2))%4) ; /* on aligne sur 4 octets si besoin */
			} 
			donnee3.decalage=**(dec+2) ;
			strcpy(donnee1.instruction,".word") ;
			donnee3.operande=operande  ;
			*pl_data = ajout_tete_L_DATA (donnee3,*pl_data) ;
			**(dec+2) += 4 ;
			puts("8") ;
				break ;

		}
	} while ( (l->suiv != NULL) && ( l=(l->suiv) )->val.nom_type == 2 ) ; /* Si on a une virgule donc d'autre opérandes, on recommence */
	return l ;
}


L_LEXEME charge_byte (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data ) {
	LEXEME lexeme = l->val ;
	OPERANDE operande ;
	TEXT donnee1 ;
	DATA donnee3 ;
	
	do {	
		l=l->suiv ;
		if (l==NULL) {
			return l ;
		}
		if (l->val.nom_type == 8 || l->val.nom_type == 9 || l->val.nom_type == 7) { 
		/* si valeur décimale, hexadécimale ou symbole alpha*/
			strcpy(operande.val, (l->val).valeur) ;
			operande.type = (l->val).nom_type ;
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
	} while ( (l->suiv==NULL) && ( l=(l->suiv) )->val.nom_type == 2 ) ; /* Si on a une virgule donc une autre opérande ensuite, on recommence */
	
	return l ;
}

L_LEXEME charge_asciiz (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data) {
	LEXEME lexeme = l->val ;
	OPERANDE operande ;
	TEXT donnee1 ;
	DATA donnee3 ;
	do {	
		l=l->suiv ;
		if (l==NULL) {
			return l ;
		}
		if ( l->val.nom_type == 12 ) { 
		/* si expression qui est entre des guillemets */
			strcpy(operande.val, (l->val).valeur) ;
			operande.type = (l->val).nom_type ;
		}
		else { /* sinon message d'erreur A FAIRE */
		}
		switch (section) {
	
			case 1 :
			donnee1.ligne=lexeme.numero_ligne ;
			donnee1.decalage=**dec ;
			donnee1.nb_op=1 ;
			strcpy(donnee1.instruction,".asciiz") ;
			donnee1.t_operande[0]=operande  ;
			*pl_text = ajout_tete_L_TEXT (donnee1,*pl_text) ;
			**dec += strlen(operande.val)+1 ;
				break ;
	
			case 2 :
			/* Erreur car impossible dans .bss*/
				break ;
	
			case 3 :
			donnee3.ligne=lexeme.numero_ligne ;
			donnee3.decalage=**(dec+2) ;
			strcpy(donnee1.instruction,".asciiz") ;
			donnee3.operande=operande  ;
			*pl_data = ajout_tete_L_DATA (donnee3,*pl_data) ;
			**(dec+2) += strlen(operande.val)+1 ;
				break ;

		}
	} while ( (l->suiv==NULL) && ( l=(l->suiv) )->val.nom_type == 2 ) ; /* Si on a une virgule donc une autre opérande ensuite, on recommence */
	
	return l ;
}



 





