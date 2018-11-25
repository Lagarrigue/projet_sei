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
	RELOC* tab_reloc ;
	if ( (tab_reloc=calloc(size, sizeof(tab_reloc))) == NULL ) {
		return NULL ;
	}
	L_TEXT l_text = *pl_text ;
	L_DATA l_data = *pl_data ;
	while ( l_text != NULL ){
		l_text->val.decalage += ad_text ;
		l_text = l_text->suiv ;	
	}
	while ( l_data != NULL ){
		l_data->val.decalage += ad_data ;
		l_data = l_data->suiv ;	
	}
	return tab_reloc ;	
}	

	
