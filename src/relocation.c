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

int existence_HI16(SYMB symb, RELOC* tab_reloc, int size){
	int i ;
	int s=FALSE ;
	for (i=0 ; i<size ; i++ ){
		if ( (tab_reloc[i].type != UNDIFINED) && ( tab_reloc[i].ad_rel == symb.decalage ) ) {
			s=TRUE ;
		}
	}
	return s ;
} 
	
	
			
char* charge_nom_rel (SYMB symb, char nom[512]){	
	if ( symb.section == 1 ) {
		strcpy(nom,".text");
	}
	else if ( symb.section == 2) {
		strcpy(nom,".data");
	}
	else if ( symb.section == 3) {
		strcpy(nom,".bss");
	}
	return nom ;			
}

SYMB* ajout_tab_symb (SYMB* tab_symb, TEXT text, int* size ,int j){
	SYMB* nouv_tab;
	int i ;
	(*size)+=1 ;
	SYMB symb ;
	strcpy(symb.symbole, text.t_operande[j].val.etiq.nom) ;
	symb.ligne=text.ligne ;
	symb.defined=FALSE ;
	symb.decalage=text.decalage ;
	if ( (nouv_tab=calloc(*size,sizeof(*nouv_tab))) == NULL ) {
		puts("Erreur d'allocation");
		return NULL ;
	}
	for (i=0 ; i<((*size)-1) ; i++ ) {
		nouv_tab[i]=tab_symb[i] ;
		free(tab_symb+i);
	}
	nouv_tab[(*size)-1] = symb ;
	return nouv_tab ;
}

RELOC** relocation(SYMB* tab_symb, int size, L_TEXT * pl_text, L_DATA * pl_data){
	
	/* *** Déclaration et initialisation des variables *** */
	RELOC* rel_text ;
	RELOC* rel_data ;
	RELOC* rel_bss ;
	RELOC** rel; 
	RELOC reloc ;
	SYMB* p_symb = NULL ;
	L_TEXT l_text = *pl_text ;
	L_DATA l_data = *pl_data ;
	char nom[512];
	int i ;
	int compt_t=0 ;
	int compt_d=0 ;
	if ( (rel_text=calloc(size, sizeof(rel_text))) == NULL ) {
		return NULL ;
	}
	if ( (rel_data=calloc(size, sizeof(rel_data))) == NULL ) {
		return NULL ;
	}
	if ( (rel_bss=calloc(size, sizeof(rel_bss))) == NULL ) {
		return NULL ;
	}
	if ( (rel=calloc(3, sizeof(RELOC*))) == NULL ) {
		return NULL ;
	}	
	rel[0]=rel_text ;rel[1]=rel_data ; rel[2]=rel_bss ;	
	/* *** Gestion des etiquettes dans .text *** */
	l_text = *pl_text ;
	while ( l_text != NULL ){
		reloc.ad_rel = (l_text->val).decalage ;
		for (i=0 ; i < l_text->val.nb_op ; i++){
			if ( (l_text->val).t_operande[i].type == 4 ){
			
				/* Si le symbole est deja défini */
				if ( (p_symb = p_symbole (l_text->val.t_operande[i].val.etiq.nom,tab_symb, size)) != NULL ){ 
					reloc.p_symb=p_symb ;
					
					/* Cas R_MIPS_26 */
					if ( strcmp((l_text->val).type_instruction,"J") ==0 ){
						reloc.ad_rel = (*p_symb).decalage ;
						strcpy(reloc.nom,charge_nom_rel(*p_symb,nom));
						reloc.type = R_MIPS_26 ;
						rel_text[compt_t]=reloc ;
						compt_t ++ ;
					}
					
					/* Cas R_MIPS_HI16/R_MIPS_LO16 */
					else if ( strcmp(l_text->val.type_instruction,"I") ==0 ){
						strcpy(reloc.nom,charge_nom_rel(*p_symb,nom));
						/* HI16 : On vérifie que le R_MIPS_HI16 n'existe pas déjà */
						if ( existence_HI16(*p_symb,rel_text,size) == FALSE ) {
							reloc.type = R_MIPS_HI16 ; 
							reloc.ad_rel = (*p_symb).decalage ;
							rel_text[compt_t]=reloc ;
							compt_t ++ ;
						}
						/* LO16 */
						reloc.type = R_MIPS_LO16 ; 
						reloc.ad_rel = l_text->val.decalage ;
						rel_text[compt_t]=reloc ;
						compt_t ++ ;
					
					}
					
					/* Cas R_MIPS_32 */
					else if ( (strcmp(l_text->val.t_operande[i].val.etiq.attendu,".word") ==0) || (strcmp(l_text->val.type_instruction,"R") ==0) ){
						strcpy(reloc.nom,charge_nom_rel(*p_symb,nom));
						reloc.type = R_MIPS_32 ;
						rel_text[compt_t]=reloc ;
						compt_t ++ ;
					}
				}
				/* Cas ou symbole défini ailleurs ou non défini */
				else {
					/* On l'ajoute dans la table de symboles */
					tab_symb = ajout_tab_symb (tab_symb, l_text->val, &size,i) ;
					strcpy(reloc.nom,l_text->val.t_operande[i].val.etiq.nom);
					reloc.type = R_MIPS_26 ;
					reloc.ad_rel = l_text->val.decalage ;
					rel_text[compt_t]=reloc ;
					compt_t ++ ;	
				}
			}
		}
		l_text = l_text->suiv ;
	}
			
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
		
		}
		l_data = l_data->suiv ;
	}
	return rel ;	
}	



















	
