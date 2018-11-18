#include "analyse_grammaticale.h"


L_LEXEME charge_instruction (L_LEXEME l , int** dec, L_TEXT* pl_text, INSTRUCTION instruction){
	int nb_op = instruction.nb_op ;
	int i=0 ;
	char** type_op_attendu ;
	TEXT donnee ;
	strcpy(donnee.instruction, instruction.nom_inst) ;
	donnee.ligne = (l->val).numero_ligne ;
	donnee.decalage = **dec ;
	**dec += 4 ;
	donnee.nb_op = nb_op ;
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
	i=0 ;
	enum { DEBUT,SIGNE, REG , IMM , SA , ETIQ , AD_REL , AD_ABS,BASE_OFF, TARGET, OFFSET ,VIRGULE} ;
	int S = DEBUT ;
	OPERANDE operande ;
	l=l->suiv ;
	if (l==NULL) { return NULL ; } ;
	while ( i<nb_op ) {
		switch (S) {
			case DEBUT :
				type_op_attendu[i] = (instruction.type_op)[i] ;
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
				(operande.val).reg = valeur_reg(l->val) ; 
				break ;

			case IMM :
				operande.type = 2 ;
				(operande.val).imm = valeur_imm(l->val) ; 
				break ;

			case SA :
				operande.type = 3 ;
				(operande.val).sa = valeur_sa(l->val) ; 
				break ;

			case ETIQ :
				/* Cherche l'etiquette dans le tableau et renvoie le truc associé en type LEXEME
				 * PB : tableau pas encore créé !
				 */ 
				strcpy(operande.val.etiq, (l->val).valeur) ;
				break ;

			case SIGNE :
				l = signe(l) ;
				S=DEBUT ;
				break ;
		
			case AD_REL :
				break ;

			case AD_ABS :
				break ;

			case TARGET :
				operande.type = 9 ;
				(operande.val).tar = valeur_target(l->val) ; 
				break ;

			case OFFSET :
				operande.type = 7 ;
				(operande.val).imm = valeur_offset(l->val) ; 
				break ;
			
			case BASE_OFF :
				operande.type = 8 ;
				/*operande.val = charge_base_offset() ; */
				break ;
		}

		donnee.t_operande[i] = operande ;
		if ( i<(nb_op-1)) { /* Si on est pas au dernier opérande : on doit gérer les virgules */
			l=l->suiv ;
			if (l==NULL) { return NULL ; } ;
			if ( (l->val).nom_type != 2 ) {
				WARNING_MSG("(ligne %d) Virgule attendue ",l->val.numero_ligne);
				return l ; 
			}
			else {
				l=l->suiv ;
				if (l==NULL) { return NULL ; } ;
			}
		}
		i++ ;
	}
	*pl_text = ajout_tete_L_TEXT (donnee,*pl_text) ;
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
		WARNING_MSG("(ligne %d) Valeur décimale attendue apres '-'",(l->val).numero_ligne);
		return NULL ;
	}
	return l ;
}		
	
unsigned char valeur_reg(LEXEME l) {
	unsigned char s=0 ;
	long int val= strtol( (l.valeur)+1 , NULL, 10) ;
	if (0 <= val && val<32 ) {
		s = (unsigned char)val ;
		return s ;
	}
	else {
		WARNING_MSG("(ligne %d) Registre inconnu ",l.numero_ligne) ;
		return 0 ;
	}
}
		
	
/* immediate : valeur codé sur 16 bits*/
short valeur_imm(LEXEME lex){/* on met en argument le lexeme voulut !*/
	short valeur_finale=0;
	if(lex.nom_type==8){
		short valeur=0;
		valeur=strtol(lex.valeur,NULL, 10);
		if(valeur < ((short) pow(2,16))-1 && valeur >= 0){
			valeur_finale=valeur;
			return valeur_finale;
		}
			
		else{
			WARNING_MSG("valeur instruction ligne n°%d trop grande, c'est un short",lex.numero_ligne);
			return 0;
			}
	}
	else if(lex.nom_type==9){
		short valeur=0;
		valeur=strtol(lex.valeur,NULL , 16);
		if(valeur < ((short) pow(2,16))-1 && valeur >= 0){
			valeur_finale=valeur;
			return valeur_finale;
		}
		else{
			WARNING_MSG("valeur instruction ligne n°%d trop grande, c'est un short",lex.numero_ligne);
			return 0;
		}
	}
	
	else {
		WARNING_MSG("type operande instruction ligne n°%d tnon conforme",lex.numero_ligne);
		return 0 ;
	}
}


/* shift amount valeur comprise entre 0 et 31*/
int valeur_sa(LEXEME lex){
	short valeur_finale=0;
	if(lex.nom_type==8){
		int valeur=0;
		valeur=strtol(lex.valeur, NULL, 10);
		if(valeur < ((int) pow(2,5))-1 && valeur >= 0){
			valeur_finale=valeur;
			return valeur_finale;
		}
			
		else{
			WARNING_MSG("valeur instruction ligne n°%d trop grande comprise entre 0 et 31 inclus ou erreur de signe",lex.numero_ligne);
			return 0;
		}
	}
	else if(lex.nom_type==9){
		int valeur=0;
		valeur=strtol(lex.valeur,NULL , 16);
		if(valeur < ((int) pow(2,5))-1 && valeur >= 0){
			valeur_finale=valeur;
			return valeur_finale;
		}
		else{
			WARNING_MSG("valeur instruction ligne n°%d trop grande comprise entre 0 et 31 inclus ou erreur de signe",lex.numero_ligne);
			return 0;
		}
	}
	else {
		WARNING_MSG("type operande instruction ligne n°%d tnon conforme",lex.numero_ligne);
		return 0 ;
	}
}

/* codage sur 26 bits */
long valeur_target(LEXEME lex){
	long valeur=0;
	if(lex.nom_type==8){
		valeur=strtol(lex.valeur,NULL, 10);
		if(valeur < ((long) pow(2,26))-1 && valeur >= 0){
			return valeur;
		}
			
		else{
			WARNING_MSG("valeur instruction ligne n°%d trop grande, c'est codé sur 26 bits",lex.numero_ligne);
			return 0;
		}
	}
	else if(lex.nom_type==9){
		valeur=strtol(lex.valeur,NULL , 16);
		if(valeur < ((long) pow(2,24))-1 && valeur >= 0){
			return valeur;
		}
		else{
			WARNING_MSG("valeur instruction ligne n°%d trop grande, c'est codé sur 26 bits",lex.numero_ligne);
			return 0;
		}
	}
	else {
		WARNING_MSG("type operande instruction ligne n°%d tnon conforme",lex.numero_ligne);
		return 0 ;
	}
}


/* codage sur 18 bits signés*/
long valeur_offset(LEXEME lex){
	long valeur=0;
	if(lex.nom_type==8){
		valeur=strtol(lex.valeur,NULL, 10);
		if(valeur < ((long) pow(2,18))-1 && valeur >= 0){
			return valeur;
		}
			
		else{
			WARNING_MSG("valeur instruction ligne n°%d trop grande, c'est codé sur 18 bits signés",lex.numero_ligne);
			return 0 ;
		}
	}
	else if(lex.nom_type==9){
		valeur=strtol(lex.valeur,NULL , 16);
		if(valeur < ((long) pow(2,18))-1 && valeur >= 0){
			return valeur;
		}
		else{
			WARNING_MSG("valeur instruction ligne n°%d trop grande c'est codé sur 18 bits signés, c'est un short",lex.numero_ligne);
			return 0;
		}
	}
	else {
		WARNING_MSG("type operande instruction ligne n°%d tnon conforme",lex.numero_ligne);
		return 0 ;
	}
}






