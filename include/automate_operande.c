#include "analyse_grammaticale.h"

typedef union {
	unsigned char reg;
	short imm ;
	unsigned char sa ; 
	char etiq[512] ;
	short ad_rel;
	unsigned int ad_abs ;
	struct {
		unsigned char reg ;
		short offset ;} base_offset ;
	} VAL_OPERANDE ;

 /* typedef enum { REG , IMM , SA , ETIQ , AD_REL , AD_ABS , OFFSET , BASE_OFF , TARGAET } TYPE_VAL_OPERANDE ; 
		   1      2     3    4      5        6         7        8          9
*/



typedef struct {
	int type ;
	VAL_OPERANDE val ; } OPERANDE
	
	
 

	/* recherche_instruction (l->val) ;
	 * si existe => instruction= charge_instruction(l->val) ;
	 */
L_LEXEME charge_instruction (L_LEXEME l/*correspond à celui juste apres l'instr*/, int** dec, L_TEXT* pl_text, INSTRUCTION instruction){
	int nb_op = instruction.nb_op ;
	int i ;
	char type_op_attendu[3][10] = instruction.type_op ;
	enum { DEBUT, REG , IMM , SA , ETIQ , AD_REL , AD_ABS,BASE_OFF, TARGET, OFFSET ,VIRGULE} ;
	int S ;
	OPERANDE operande ;
	LEXEME lexeme ;
	while ( i<nb_op ) {

		switch (S) {

			case DEBUT :
				type_op_attendu = (instruction.type_op)[i] ;
				if ( l->val.nom_type == 4) { S=ETIQ ;}
				else if ( l->val.nom_type == 2 ) {S=SIGNE ;}
				else if ( strcmp(type_op_attendu[i],"reg")==0 )    {S=REG ;}
				else if ( strcmp(type_op_attendu[i],"imm")==0 )    {S=IMM ;}
				else if ( strcmp(type_op_attendu[i],"sa")==0 ) 	   {S=SA ;}
				else if ( strcmp(type_op_attendu[i],"boff")==0 )   {S=BASE_OFF ;}
				else if ( strcmp(type_op_attendu[i],"offset")==0 ) {S=OFFSET ;}
				else if ( strcmp(type_op_attendu[i],"target")==0 ) {S=TARGET ;}
				}
		
			case REG :
				operande.type = 1 ;
				operande.val = valeur_reg(l->val) ; /* Transforme '$3' en 3*/
				break ;

			case IMM :
				operande.type = 2 ;
				operande.val = valeur_imm(l->val) ; /* Transforme en short, vérifie la taile, etc ...*/
				break ;

			case SA :
				operande.type = 3 ;
				operande.val = valeur_sa(l->val) ;
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
				lexeme = signe(l)
				S=DEBUT ;
				break ;
		
			case AD_REL :
				break ;

			case AD_ABS :
				break ;

			case TARGET :
				operande.val = charge_target() ;
				break ;

			case OFFSET :
				operande.val = charge_offset() ;
				break ;
		}

		(*l_text->val).t_operande[i] = operande ;

		if ( i<(nb_op-1)) { /* Si on est pas au dernier opérande : on doit gérer les virgules */
			l=l->suiv ;
			if (l==NULL) { return NULL } ;
			if (l->val).nom_type != 2 ) {
				WARNING_MSG("(ligne %d) virgule attendue",l->val.numero_ligne);
				return l ; }
			else {
				l=l->suiv ;
				i++ ;
			}
		}
	return l ;		
}




