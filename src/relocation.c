#include "relocation.h"

SYMB* p_symbole ( char etiq[512], SYMB* tab_etiq, int n ) {
	int i ;
	for (i=0 ; i<n ; i++ ){
		if ( strcmp(etiq,tab_etiq[i].symbole)==0){
			return (tab_etiq+i) ;
		}
	}
	return NULL ;
} 


int existence_symbole (char etiq[512] ,L_SYMB l_symb){
	int s = FALSE ;
	L_SYMB p = l_symb ;
	while (p!=NULL) {
		if (strcmp(p->val.symbole,etiq)==0){
			s = TRUE ;
		}
		p=p->suiv ;
	}
	return s ;
}
			
			

RELOC * relocation(SYMB* tab_symb, int size, L_TEXT * pl_text, L_DATA * pl_data, int** dec, int ad_data, int ad_text ){
	
	/* *** Déclaration et initialisation des variables *** */
	RELOC* rel_text ;
	RELOC* rel_data ;
	RELOC reloc ;
	SYMB* p_symb = NULL ;
	L_TEXT l_text = *pl_text ;
	L_DATA l_data = *pl_data ;
	int i ;
	int j=0 ;
	if ( (rel_text=calloc(size+2, sizeof(rel_text))) == NULL ) {
		return NULL ;
	}
	if ( (rel_data=calloc(size+2, sizeof(rel_data))) == NULL ) {
		return NULL ;
	}
	
	/* *** Relocation des sections .text et .data *** */
	strcpy(reloc.nom,".text") ;
		reloc.ad_rel=ad_text ;
		reloc.type = R_MIPS_32 ;
		reloc.p_symb = p_symb ;
		rel_text[j]=reloc ;
	strcpy(reloc.nom,".data") ;
		reloc.ad_rel=ad_data ; 
		rel_data[j]=reloc ;	
	j++ ;
	/* *** Relocation des etiquettes *** */
	
	/* *** Gestion des etiquettes dans .text *** */
	
	l_text = *pl_text ;
	while ( l_text != NULL ){
		for (i=0 ; i < l_text->val.nb_op ; i++){
			if ( (l_text->val).t_operande[i].type == 4 ){
				if ( (p_symb = p_symbole (l_text->val.t_operande[i].val.etiq.nom,tab_symb, size)) != NULL ){
					reloc.p_symb=p_symb ;
					if ( strcmp((l_text->val).type_instruction,"J") ==0 ){
						reloc.type = R_MIPS_26 ;
					}
					else if ( strcmp(l_text->val.type_instruction,"I") ==0 ){
						reloc.type = R_MIPS_LO16 ; /* ??? */
					}
					else if ( (strcmp(l_text->val.t_operande[i].val.etiq.attendu,".word") ==0) || (strcmp(l_text->val.type_instruction,"R") ==0) ){
						reloc.type = R_MIPS_32 ;
					}
				}
				else {
				/* Cas ou symbole défini ailleurs ou non défini */
				}
			}
			rel_text[j]=reloc ;
			j++ ;
		}
		l_text = l_text->suiv ;
	}
	j=1 ;	
			
	/* *** Gestion des etiquettes dans .data *** */	
	l_data = *pl_data ;
	while ( l_data != NULL ){
		if ( (l_data->val).operande.type == 4 ){
			if ( (p_symb = p_symbole (l_data->val.operande.val.etiq.nom,tab_symb, size)) != NULL ){
				reloc.p_symb=p_symb ;
				reloc.type = R_MIPS_32 ;
			}	
			else {
				/* Cas ou symbole défini ailleurs ou non défini */
			}
		rel_data[j]=reloc ;
		j++ ;
		}
		l_data = l_data->suiv ;
	}
	return rel_text ;	
}	



















	
