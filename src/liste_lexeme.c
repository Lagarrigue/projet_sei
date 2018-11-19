#include <stdio.h>
#include <stdlib.h>
#include "analyse_lexicale.h"

L_LEXEME creer_liste(void){
	return NULL ;
	}

L_LEXEME ajout_tete(LEXEME lex, L_LEXEME L){
	L_LEXEME p=calloc(1,sizeof(*p));
	if (p==NULL){
		return NULL;
		}
	else{
		p->val=lex;
		p->suiv=L;
		return p;
	}
}

L_LEXEME supprimer_tete(L_LEXEME L){
	if(!est_vide(L)){
		L_LEXEME p;
		p=L->suiv;
		free(L);
		return p;
	}

	else return NULL;
}

void liberer_liste(L_LEXEME L){
	while(est_vide(L) != 0){
		L=supprimer_tete(L);
	}
}

int est_vide(L_LEXEME L){
	return !L;
	}

L_LEXEME renversement_liste(L_LEXEME L1){
	L_LEXEME p;
	p=L1;
	L_LEXEME L2;
	L2=creer_liste();
	
	while(p!=NULL){
		L2=ajout_tete(p->val,L2);
		p=p->suiv;
	}
	return L2;
}
