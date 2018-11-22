#include "analyse_grammaticale.h"


 
/* 
section : 
0 -> aucune ( si utilisateur oublie au debut)
1 -> .text
2 -> .bss
3 -> .data	
*/ 

void init (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data, L_SYMB* pl_symb, L_SYMB* pl_attente, L_INSTRUCTION* dicti) {
	int S = 0 ;
	/*int nb_op=0 ; */
	INSTRUCTION* p_instruction ;
	while ( l != NULL ) {
		S=(l->val).nom_type ;
		switch ( S ) {
			case 6 : /* CAS DIRECTIVE */
				if (strcmp(l->val.valeur,"space") == 0) {
					pl_attente = maj_symbole(dec,section, pl_attente, pl_symb, 0 ) ;
					l=charge_space (l, section , dec , pl_text , pl_bss , pl_data) ;
				}
				else if (strcmp(l->val.valeur,"set") == 0) {
					if ( section != 0 ) {
						/* Erreur car .set ne peut etre que au debut */
						WARNING_MSG("(ligne %d) .set doit être au début du programme",l->val.numero_ligne); 
					/* A CORRIGER : si noredor suit le .set, il va etre lu dans case instruction */
					}
					else { 
						pl_attente = maj_symbole(dec,section, pl_attente, pl_symb, 0 ) ;
						l=charge_set(l) ;
						}
				}
				else if (strcmp(l->val.valeur,"word") == 0) {
					l = charge_word (l, section , dec , pl_text , pl_bss , pl_data) ;
					pl_attente = maj_symbole(dec,section, pl_attente, pl_symb, 1 ) ;
				}
				else if (strcmp(l->val.valeur,"byte") == 0) {
					pl_attente = maj_symbole(dec,section, pl_attente, pl_symb, 0 ) ;
					l = charge_byte (l, section , dec , pl_text , pl_bss , pl_data) ;
				}
				else if (strcmp(l->val.valeur,"asciiz") == 0) {
					pl_attente = maj_symbole(dec,section, pl_attente, pl_symb, 0 ) ;
					l = charge_asciiz (l, section , dec , pl_text , pl_bss , pl_data) ;
				}
				else if (strcmp(l->val.valeur,"text") == 0) {
					section = 1 ;
				}
				else if (strcmp(l->val.valeur,"bss") == 0) {
					section = 2 ;
				}
				else if (strcmp(l->val.valeur,"data") == 0) {
					section = 3 ;
				}
				else { 
					WARNING_MSG("(ligne %d) Directive non reconnue",l->val.numero_ligne);
				}
				break ;
	
			case 7 : /* CAS INSTRUCTION */
				if (section != 1 ) { 
				/* Erreur car les instructions doivent etre dans .text */
					WARNING_MSG("(ligne %d) Les instructions doivent être dans la section .text",l->val.numero_ligne);
				}
				else {
					if ( (p_instruction = recherche_element(l->val.valeur, dicti, 15)) == NULL ) {
						WARNING_MSG("(ligne %d)  Instruction non reconnue",l->val.numero_ligne);
					}
					else {	
						pl_attente = maj_symbole(dec,section, pl_attente, pl_symb, 0 ) ;
						l=charge_instruction (l , dec, pl_text, *p_instruction) ;
					}
				}
				break ;
	
			case 3 : /* CAS ETIQUETTE (ou DEUX_PTS d'apres notre analyse lexicale */
				if (section==0) { 
					WARNING_MSG("(ligne %d) Les étiquettes doivent être dans une section ",l->val.numero_ligne);
				}
				else {
					l=charge_symbole( l, section, dec , pl_attente) ;
				}	
				break ;
	
			case 1 : /* CAS NL */
				break ;
			
			case 4 : /* CAS COMMENTAIRE */
				break ;
		}
		if (l != NULL) {
			l=l->suiv ;
		}
	}	
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
 
L_SYMB* maj_symbole(int** dec, int section, L_SYMB* pl_attente, L_SYMB* pl_symb,int word ) {
	if ( *pl_attente == NULL ) {
		return pl_attente ; 
	}
	L_SYMB p = *pl_attente ;
	L_SYMB previous = p ;
	SYMB symb ;
	if ( (p->val).section==section  ) { /* Cas de la tete */
		symb=p->val ;
		symb.decalage = **(dec+section-1) ;
		if (word ==1) { /* si on a un .word , maj_symbole est lancé apres avoir chargé charge_word donc on fais dec-4 */
			symb.decalage-=4 ;
		}
		*pl_symb = ajout_tete_L_SYMB (symb,*pl_symb) ;
		*pl_attente = supprimer_tete_L_SYMB (*pl_attente) ;
		return maj_symbole( dec , section , pl_attente, pl_symb, word) ; 
	}
	p=p->suiv ;
	while ( !liste_est_vide_L_SYMB(p) ) { /* Cas d'un maillon qui n'est pas en tete */
		if ( (p->val).section==section ) {
			symb=p->val ;
			symb.decalage = **(dec+section-1) ;
			*pl_symb = ajout_tete_L_SYMB (symb,*pl_symb) ;
			if ( (p->suiv) == NULL ) {return NULL ; }
			(previous->suiv) = (p->suiv) ;	
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
	if (l==NULL) { return l ; }
	OPERANDE operande ; 
	BSS donnee2 ;
	DATA donnee3 ;	
	if (l->val.nom_type == 8 ) { /* valeur decimale */
		(operande.val).nb = strtol( (l->val).valeur ,NULL, 10) ;
	}
	else if (l->val.nom_type == 9 ) { /* valeur hexadécimale*/
		(operande.val).nb = strtol( (l->val).valeur ,NULL, 16) ;
	}
	else { /* sinon message d'erreur */
		WARNING_MSG("(ligne %d) Valeur décimale ou hexadicimale attendue ",l->val.numero_ligne);
	}
	switch (section) {

		case 1 :
		/* Erreur car impossible dans .text */
			WARNING_MSG("(ligne %d) .space impossible dans .text",l->val.numero_ligne);
			break ;

		case 2 :
		donnee2.ligne=lexeme.numero_ligne ;
		donnee2.decalage=**(dec+1) ;
		donnee2.operande= operande ;
		strcpy(donnee2.directive,".space") ;
		*pl_bss = ajout_tete_L_BSS(donnee2,*pl_bss) ;
		**(dec+1) += (operande.val).nb ;
			break ;

		case 3 :
		donnee3.ligne=lexeme.numero_ligne ;
		donnee3.decalage=**(dec+2) ;
		strcpy(donnee3.directive,".space") ;
		donnee3.operande=operande  ;
		*pl_data = ajout_tete_L_DATA (donnee3,*pl_data) ;
		**(dec+2) += (operande.val).nb ;
			break ;
	}
	return l ;
	}
		
L_LEXEME charge_set (L_LEXEME l) {
	if (l->suiv == NULL ) {
		return l ;
	}
	l=l->suiv ;
	if ( strcmp(l->val.valeur, "noreorder") != 0) {
		WARNING_MSG("(ligne %d) .set ne prend que en compte noreorder",l->val.numero_ligne);
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
		if (l==NULL) { return l ;}
		if ( l->val.nom_type == 2 ){ /* SI virgule on passe au suivant */
			l=l->suiv ;
			if (l==NULL) { return l ;}
		}
		if ( (l->val).nom_type == 11 ) { /* on gère le cas où on a un "-" d'un nombre négatif */
			l=signe(l) ;
		}
		if (l->val.nom_type == 8 ) {/* si decimal */
			(operande.val).nb = strtol( (l->val).valeur ,NULL, 10);
		} 
		else if ( l->val.nom_type == 9 ) {/* si hexa */
			(operande.val).nb = strtol( (l->val).valeur ,NULL, 16) ;
		} 
		else if ( l->val.nom_type == 7 ) { /* si symbole alpha*/
			strcpy((operande.val).etiq, (l->val).valeur) ;
			WARNING_MSG("(ligne %d)  Etiquette non géré pour le moment",l->val.numero_ligne);
		}
		else { 
			WARNING_MSG("(ligne %d) Valeur décimale, hexadécimale ou symboles alpha attendue",l->val.numero_ligne);
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
				WARNING_MSG("(ligne %d) .word impossible dans .bss",l->val.numero_ligne);
				break ;
	
			case 3 :
			donnee3.ligne=lexeme.numero_ligne ;
			if ( (**(dec+2))%4 != 0 ) {
				**(dec+2) += (4-(**(dec+2))%4) ; /* on aligne sur 4 octets si besoin */
			} 
			donnee3.decalage=**(dec+2) ;
			strcpy(donnee3.directive,".word") ;
			donnee3.operande=operande  ;
			*pl_data = ajout_tete_L_DATA (donnee3,*pl_data) ;
			**(dec+2) += 4 ;
				break ;
			
		}
	} while ( (l->suiv != NULL) && (l->suiv)->val.nom_type == 2 ) ; /* Si on a une virgule donc d'autre opérandes, on recommence */
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
		if ( l->val.nom_type == 2 ){ /* SI virgule on passe au suivant */
			l=l->suiv ;
			if (l==NULL) { return l ;}
		}
		if ( (l->val).nom_type == 11 ) { /* on gère le cas où on a un "-" d'un nombre négatif */
			l=signe(l) ;
		}
		if (l->val.nom_type == 8 ) {/* si decimal */
			(operande.val).nb = strtol( (l->val).valeur ,NULL, 10);
		} 
		else if ( l->val.nom_type == 9 ) {/* si hexa */
			(operande.val).nb = strtol( (l->val).valeur ,NULL, 16) ;
		} 
		else if ( l->val.nom_type == 7 ) { /* si symbole alpha*/
			strcpy((operande.val).etiq, (l->val).valeur) ;
			WARNING_MSG("(ligne %d)  Etiquette non géré pour le moment",l->val.numero_ligne);
		}
		else { /* sinon message d'erreur */
			WARNING_MSG("(ligne %d)  Valeur décimale, hexadécimale ou symboles alpha attendue",l->val.numero_ligne);
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
			WARNING_MSG("(ligne %d) .byte impossible dans .bss",l->val.numero_ligne);
				break ;
	
			case 3 :
			donnee3.ligne=lexeme.numero_ligne ;
			donnee3.decalage=**(dec+2) ;
			strcpy(donnee3.directive,".byte") ;
			donnee3.operande=operande  ;
			*pl_data = ajout_tete_L_DATA (donnee3,*pl_data) ;
			**(dec+2) += 1 ;
				break ;

		}
	} while ( (l->suiv==NULL) && (l->suiv)->val.nom_type == 2 ) ; /* Si on a une virgule donc une autre opérande ensuite, on recommence */
	
	return l ;
}

L_LEXEME charge_asciiz (L_LEXEME l, int section, int** dec, L_TEXT* pl_text, L_BSS* pl_bss, L_DATA* pl_data) {
	LEXEME lexeme = l->val ;
	OPERANDE operande ;
	operande.type=10 ;
	TEXT donnee1 ;
	DATA donnee3 ;
	do {	
		l=l->suiv ;
		if (l==NULL) {
			return l ;
		}
		if ( l->val.nom_type == 2 ){ /* SI virgule on passe au suivant */
			l=l->suiv ;
			if (l==NULL) { return l ;}
		}
		if ( l->val.nom_type == 12 ) { 
		/* si expression qui est entre des guillemets */
			strcpy((operande.val).etiq, (l->val).valeur) ;
		}
		else { /* sinon message d'erreur A FAIRE */
			WARNING_MSG("(ligne %d) Expression entre guillemets attendue",l->val.numero_ligne);
		}
		switch (section) {
	
			case 1 :
			donnee1.ligne=lexeme.numero_ligne ;
			donnee1.decalage=**dec ;
			donnee1.nb_op=1 ;
			strcpy(donnee1.instruction,".asciiz") ;
			donnee1.t_operande[0]=operande  ;
			*pl_text = ajout_tete_L_TEXT (donnee1,*pl_text) ;
			**dec += strlen((operande.val).etiq)+1 ;
				break ;
	
			case 2 :
			/* Erreur car impossible dans .bss*/
				WARNING_MSG("(ligne %d) .asciiz impossible dans .bss",l->val.numero_ligne);
				break ;
	
			case 3 :
			donnee3.ligne=lexeme.numero_ligne ;
			donnee3.decalage=**(dec+2) ;
			strcpy(donnee3.directive,".asciiz") ;
			donnee3.operande=operande  ;
			*pl_data = ajout_tete_L_DATA (donnee3,*pl_data) ;
			**(dec+2) += strlen((operande.val).etiq)+1 ;
				break ;

		}
	} while ( (l->suiv==NULL) && (l->suiv)->val.nom_type == 2 ) ; /* Si on a une virgule donc une autre opérande ensuite, on recommence */
	return l ;
}



 





