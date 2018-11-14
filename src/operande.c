#include "analyse_grammaticale.h"


	/* recherche_instruction (l->val) ;
	 * si existe => instruction= charge_instruction(l->val) ;
	 */
L_LEXEME charge_instruction (L_LEXEME l/*correspond à celui juste apres l'instr*/, int** dec, L_TEXT* pl_text, INSTRUCTION instruction){
	int nb_op = instruction.nb_op ;
	int i ;
	char** type_op_attendu ;
	if ( (type_op_attendu = calloc(3,sizeof(*type_op_attendu)) ) == NULL ) {
		puts("Erreur d'allocation") ;
		return NULL ;
		}
	else {
		for (i=0 ; i<3 ; i++ ) {
			type_op_attendu[i]=calloc(10,sizeof(**type_op_attendu));
			if (type_op_attendu[i] == NULL) {
				puts("Erreur d'allocation") ;
				return NULL ;
			}
		}
	}
	type_op_attendu = instruction.type_op ;
	enum { DEBUT,SIGNE, REG , IMM , SA , ETIQ , AD_REL , AD_ABS,BASE_OFF, TARGET, OFFSET ,VIRGULE} ;
	int S ;
	OPERANDE operande ;
	while ( i<nb_op ) {

		switch (S) {
			case DEBUT :
				type_op_attendu = (instruction.type_op)[i] ;
				if ( (l->val).nom_type == 4) { S=ETIQ ;}
				else if ( (l->val).nom_type == 2 ) {S=SIGNE ;}
				else if ( strcmp(type_op_attendu[i],"reg")==0 )    {S=REG ;}
				else if ( strcmp(type_op_attendu[i],"imm")==0 )    {S=IMM ;}
				else if ( strcmp(type_op_attendu[i],"sa")==0 ) 	   {S=SA ;}
				else if ( strcmp(type_op_attendu[i],"boff")==0 )   {S=BASE_OFF ;}
				else if ( strcmp(type_op_attendu[i],"offset")==0 ) {S=OFFSET ;}
				else if ( strcmp(type_op_attendu[i],"target")==0 ) {S=TARGET ;}
				break ;
			
			case REG :
				operande.type = 1 ;
				/*operande.val = valeur_reg(l->val) ; */
				break ;

			case IMM :
				operande.type = 2 ;
				/*operande.val = valeur_imm(l->val) ; */
				break ;

			case SA :
				operande.type = 3 ;
				/*operande.val = valeur_sa(l->val) ; */
				break ;

			case ETIQ :
				/* Cherche l'etiquette dans le tableau et renvoie le truc associé en type LEXEME
				 * PB : tableau pas encore créé !
				 */ 
				break ;

			case SIGNE :
				/* Combiner le signe avec le lexeme suivant et renvoyer un LEXEME
				 * 
				 */
				l = signe(l) ;
				S=DEBUT ;
				break ;
		
			case AD_REL :
				break ;

			case AD_ABS :
				break ;

			case TARGET :
				/*operande.val = charge_target() ; */
				break ;

			case OFFSET :
				/*operande.val = charge_offset() ; */
				break ;
		}

		((*pl_text)->val).t_operande[i] = operande ;

		if ( i<(nb_op-1)) { /* Si on est pas au dernier opérande : on doit gérer les virgules */
			l=l->suiv ;
			if (l==NULL) { return NULL ; } ;
			if ( (l->val).nom_type != 2 ) {
				WARNING_MSG("(ligne %d) virgule attendue",l->val.numero_ligne);
				return l ; 
			}
			else {
				l=l->suiv ;
				i++ ;
			}
		}
	}
	return l ;		
}





L_LEXEME signe (L_LEXEME l){ /* Consiste à modifier le lexeme suivant */
	if ( (l=l->suiv)==NULL ) {
		return NULL ;}
	char signe[512] = "-" ;
	if ( (l->val).nom_type == 8 ) {
		strcpy( (l->val).valeur, strcat(signe,(l->val).valeur) ) ;
	}
	else {
		WARNING_MSG("(ligne %d) Valeur décimale attendue apres '-'",l->val.numero_ligne); 
	}
	return l ;
}		
	
	
	







