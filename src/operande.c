#include "analyse_grammaticale.h"


L_LEXEME charge_instruction (L_LEXEME l , int** dec, L_TEXT* pl_text, INSTRUCTION instruction, L_SYMB* pl_symb, int* num, char* p_nom){

	/* *** Déclaration des variables + initaisation *** */
	int nb_op = instruction.nb_op ;
	int i=0 ;
	int j ;
	int s=0 ;
	int reloc=0 ; 
	int num_lexeme=l->val.numero_lexeme ;
	printf("numlex=%d\n",num_lexeme);
	strcpy(p_nom,instruction.nom_inst);
	char** type_op_attendu ;
	TEXT donnee ;
	OPERANDE operande ;
	/*SMB symb ; */
	strcpy(donnee.instruction, instruction.nom_inst) ;
	donnee.ligne = (l->val).numero_ligne ;
	donnee.decalage = **dec ;
	**dec += 4 ;
	donnee.nb_op = nb_op ;
	if ( (type_op_attendu = calloc(3,sizeof(*type_op_attendu)) ) == NULL ) {
		puts("Erreur d'allocation") ; return NULL ;}
	else {  for (j=0 ; j<3 ; j++ ) {
			type_op_attendu[j]=calloc(10,sizeof(**type_op_attendu));
			if (type_op_attendu[j] == NULL) {
				puts("Erreur d'allocation") ;  return NULL ; }
			}}
			
	/* *** Début de "l'automate" *** */
	if ( nb_op == 0 ) {
		*pl_text = ajout_tete_L_TEXT (donnee,*pl_text) ;
		return l ;
	}
	l=l->suiv ;
	if (l==NULL) { return NULL ; } ;
	while ( i<nb_op ) {
		type_op_attendu[i] = (instruction.type_op)[i] ;
		
		/* Etiquette */
		if ( (l->val).nom_type == 7) { 
			strcpy(operande.val.etiq.nom, (l->val).valeur) ;
			strcpy(operande.val.etiq.attendu, (instruction.type_op)[i]);
			operande.type = 4 ;
		}
		
		/* Signe - */
		else if ( (l->val).nom_type == 11 ) {
			l = signe(l) ;
			i-- ;
			s = 1 ;
		}
		
		/* Registre */
		else if ( strcmp(type_op_attendu[i],"reg")==0 ) {
			if ( l->val.nom_type != 5 ) {
				WARNING_MSG("(ligne %d) [operande n°%d] Registre attendu (EXIT OPERANDE)",l->val.numero_ligne,i+1);
				return l ;
			}
			operande.type = 1 ;
			(operande.val).reg = valeur_reg(l->val) ; 
		}
		
		/* Immediat */
		else if ( strcmp(type_op_attendu[i],"imm")==0 )    {
			if ( l->val.nom_type != 8 && l->val.nom_type != 9) {
				WARNING_MSG("(ligne %d) [operande n°%d] Immédiat attendu (EXIT OPERANDE)",l->val.numero_ligne,i+1);
				return l ;
			}
			operande.type = 2 ;
			(operande.val).imm = valeur_imm(l->val) ; 
		}
		
		/* Shift amout */
		else if ( strcmp(type_op_attendu[i],"sa")==0 ) 	   {
			if ( l->val.nom_type != 8 && l->val.nom_type != 9) {
				WARNING_MSG("(ligne %d) [operande n°%d] Shift amount attendu (EXIT OPERANDE)",l->val.numero_ligne,i+1);
				return l ;
			}
			operande.type = 3 ;
			(operande.val).sa = valeur_sa(l->val) ; 
		}
		
		/* Base offset */
		else if ( strcmp(type_op_attendu[i],"boff")==0 )   {
			if ( l->val.nom_type != 8 && l->val.nom_type != 9/*&& l->suiv->val.nom_type != 13*/) {
				WARNING_MSG("(ligne %d) [operande n°%d] Base offset attendu (EXIT OPERANDE)",l->val.numero_ligne,i+1);
				return l ;
			}
			operande.type = 8 ;
			(operande.val).base_offset = valeur_base_off(l, instruction,&reloc) ;
			puts("On est sortit de boff");
			if (reloc==1) {
				printf("num lex a changer : %d",num_lexeme);
				*num = num_lexeme ;
				puts("On a rentré le num du lex a chager");
				return NULL;}
			if (l->suiv == NULL) {return NULL;}
			l=l->suiv ;
			
		}
		
		/* Offset */
		else if ( strcmp(type_op_attendu[i],"offset")==0 ) {
			if ( l->val.nom_type != 8 && l->val.nom_type != 9) {
				WARNING_MSG("(ligne %d) [operande n°%d] Offset attendu (EXIT OPERANDE)",l->val.numero_ligne,i+1);
				return l ;
			}
			operande.type = 7 ;
			(operande.val).imm = valeur_offset(l->val) ; 
		}
		
		/* Target */
		else if ( strcmp(type_op_attendu[i],"target")==0 ) {
			if ( l->val.nom_type != 8 && l->val.nom_type != 9) {
				WARNING_MSG("(ligne %d) [operande n°%d] Target attendu (EXIT OPERANDE)",l->val.numero_ligne,i+1);
				return l ;
			}		
			operande.type = 9 ;
			(operande.val).tar = valeur_target(l->val) ; 
		}
		
		/* Erreur type */
		else {
			WARNING_MSG("(ligne %d)Problème d'operande (EXIT OPERANDE)",l->val.numero_ligne);
			return l ;
		}
		
		/* Ajout de l'operande */
		donnee.t_operande[i] = operande ;
		
		/* Gestion des virgules */
		if ( i<(nb_op-1) && s == 0 ) { 
			l=l->suiv ;
			if (l==NULL) { return NULL ; } ;
			if ( (l->val).nom_type != 2 ) {
				WARNING_MSG("(ligne %d) Virgule attendue apres operande n°%d (EXIT OPERANDE) ",l->val.numero_ligne,i+1);
				return l ; 
			}
			else {
				l=l->suiv ;
				if (l==NULL) { return NULL ; } ;
			}
		}
		i++ ;
		s=0 ;
	}
	puts("on ajoute l'instrction à L_TEXT");
	*pl_text = ajout_tete_L_TEXT (donnee,*pl_text) ;
	puts("instrction ajoutée à L_TEXT");
	return l ;		
}




/* *** Fonctions utiles à l'automate *** */

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
	if (l.nom_type != 5 && l.nom_type != 3) {
			WARNING_MSG("(ligne %d) Registre (ou etiquette) attendu",l.numero_ligne);
	}
	unsigned char s=0 ;
	long int val= strtol(l.valeur , NULL, 10) ;
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
		if(valeur < ((short) pow(2,15))-1 && valeur >= (-(short) pow(2,15))){
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
		if(valeur < ((short) pow(2,15))-1 && valeur >= (-(short) pow(2,15))){
			valeur_finale=valeur;
			return valeur_finale;
		}
		else{
			WARNING_MSG("valeur instruction ligne n°%d trop grande, c'est un short",lex.numero_ligne);
			return 0;
		}
	}
	
	else {
		WARNING_MSG("(ligne %d) Valeur hexa, décimale ou etiquette attendue",lex.numero_ligne);
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
	return 69 ;
}

/* codage sur 28 bits non signes et divisible par 4 */
long valeur_target(LEXEME lex){
	long valeur=0;
	if(lex.nom_type==8){
		valeur=strtol(lex.valeur,NULL, 10);
		if(valeur < ((long) pow(2,26))-1 && valeur >= 0 && valeur%4==0){
			return valeur;
		}
			
		else{
			WARNING_MSG("valeur instruction ligne n°%d trop grande qui doit etre positive, divisible par 4 , c'est codé sur 28 bits",lex.numero_ligne);
			return 0;
		}
	}
	else if(lex.nom_type==9){
		valeur=strtol(lex.valeur,NULL , 16);
		if(valeur < ((long) pow(2,26))-1 && valeur >= 0 && valeur%4==0){
			return valeur;
		}
		else{
			WARNING_MSG("valeur instruction ligne n°%d trop grande qui doit etre positive, divisible par 4, c'est codé sur 28 bits",lex.numero_ligne);
			return 0;
		}
	}
	else {
		WARNING_MSG("(ligne %d) Valeur hexa, décimale ou etiquette attendue",lex.numero_ligne);
		return 0 ;
	}
}


/* codage sur 18 bits signés */
long valeur_offset(LEXEME lex){
	long valeur=0;
	if(lex.nom_type==8){
		valeur=strtol(lex.valeur,NULL, 10);
		if(valeur <= ((long) pow(2,17))-1 && valeur >= (-(long) pow(2,17))){
			return valeur;
		}
			
		else{
			WARNING_MSG("valeur instruction ligne n°%d trop grande, c'est codé sur 18 bits signés",lex.numero_ligne);
			return 0 ;
		}
	}
	else if(lex.nom_type==9){
		valeur=strtol(lex.valeur,NULL , 16);
		if(valeur < ((short) pow(2,17))-1 && valeur >= (-(long) pow(2,17))){
			return valeur;
		}
		else{
			WARNING_MSG("valeur instruction ligne n°%d trop grande c'est codé sur 18 bits signés",lex.numero_ligne);
			return 0;
		}
	}
	else {
		WARNING_MSG("(ligne %d) Valeur hexa, décimale ou etiquette attendue",lex.numero_ligne);
		return 0 ;
	}
}

/* l'offset est codé ici sur 16 bits signés*/
BASE_OFFSET valeur_base_off(L_LEXEME l_lexeme, INSTRUCTION instruction,int* reloc ){
	BASE_OFFSET b_o ;
	LEXEME l = l_lexeme->val ;
	LEXEME suiv ;
	unsigned char reg;
	short offset ;
	puts("ENTREE BOF");
	puts(instruction.nom_inst);
	if((l.nom_type==8 || l.nom_type==9) && ( (l_lexeme->suiv) != NULL ) && (l_lexeme->suiv->val).nom_type==13){
		suiv= l_lexeme->suiv->val ;
		if(l.nom_type==8){
			offset = strtol(l.valeur, NULL, 10) ;
			if( ( offset < ((short) pow(2,15))-1) && (offset >= (-(short) pow(2,15)))){
				b_o.offset=offset ;
			}
			else{
				WARNING_MSG("valeur instruction ligne n°%d trop grande, c'est codé sur 16 bits signés en complément à 2",l.numero_ligne);
				b_o.offset=0 ;
				return b_o;
			}
		}
		else if (l.nom_type==9){
			offset=strtol(l.valeur, NULL , 16) ;
			if( (offset < ((short) pow(2,15))-1) && (offset >= (-(short) pow(2,15)))){
				b_o.offset=offset ;
			}
			else{
				WARNING_MSG("valeur instruction ligne n°%d trop grande, c'est codé sur 16 bits signés en complément à 2",l.numero_ligne);
				b_o.offset=0 ;
				return b_o;
			}
		}
		else {
			WARNING_MSG("(ligne %d) Valeur décimale ou hexadécimale attendue ",l.numero_ligne) ;
		}
		reg=valeur_reg(suiv);
		b_o.reg = reg ;
		b_o.offset = offset ;
		return (b_o) ;
	}
	else if ( (l.nom_type==8 || l.nom_type==9) && (( (l_lexeme->suiv)==NULL ))) {
		if ( (strcmp(instruction.nom_inst,"SW") ==0) || (strcmp(instruction.nom_inst,"LW") ==0)){
			*reloc = 1 ;
			puts("ON PASSE RELOC A 1");
		}
		else {
			WARNING_MSG("(ligne %d) Base offset attendu ",l.numero_ligne) ;
		}
		return b_o ;
	}
	else if ( (l.nom_type==8 || l.nom_type==9) && ((l_lexeme->suiv->val).nom_type != 13)) {
		puts("Detection reloc SW ou LW");
		if ( strcmp(instruction.nom_inst,"SW") == 0 ){
			(*reloc) = 1 ;
			puts("ON PASSE RELOC A 1 pour SW");
		}
		else if ( strcmp(instruction.nom_inst,"LW") ==0 ){
			(*reloc) = 1 ;
			puts("ON PASSE RELOC A 1pour LW");
		}
		puts("On a passé reloc a un et n quitte boff");
		return b_o ;
		}
	else{
		WARNING_MSG("(ligne %d) Base offset attendu ",l.numero_ligne) ;
		return (b_o) ;
	}
}


