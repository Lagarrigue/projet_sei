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

int existence_HI16(SYMB symb, L_RELOC l_reloc){
	L_RELOC p=l_reloc ;
	int s=FALSE ;
	while( p != NULL ) {
		if ( (p->val.type != UNDIFINED) && ( p->val.ad_rel == symb.decalage ) ) {
			s=TRUE ;
		}
		p=p->suiv ;
	}
	return s ;
} 
	
	
			
char* charge_nom_rel (SYMB symb, char nom[512]){	
	if ( symb.section == 1 ) {
		strcpy(nom,".text");
	}
	else if ( symb.section == 3) {
		strcpy(nom,".data");
	}
	else if ( symb.section == 2) {
		strcpy(nom,".bss");
	}
	return nom ;			
}

SYMB* ajout_tab_symb (SYMB* tab_symb,int section, DATA data, BSS bss,  TEXT text, int* size ,int j){
	(*size)+=1 ;
	SYMB* nouv_tab ;
	int i ;
	SYMB symb ;
	symb.defined=FALSE ;
	if ( section == 1 ){
		strcpy(symb.symbole, text.t_operande[j].val.etiq.nom) ; 
		symb.ligne=text.ligne ;
		symb.decalage=text.decalage ;
	}
	else if ( section == 2 ){
		strcpy(symb.symbole, bss.operande.val.etiq.nom) ; 
		symb.ligne=bss.ligne ;
		symb.decalage=bss.decalage ;
	}
	else if ( section == 3 ){
		strcpy(symb.symbole, data.operande.val.etiq.nom) ; 
		symb.ligne=data.ligne ;
		symb.decalage=data.decalage ;
	}
	
	else {
		WARNING_MSG("Problème de section dans la relocation");
	}
	if ( (nouv_tab=calloc(*size,sizeof(SYMB))) == NULL ) {
		puts("Erreur d'allocation");
		return NULL ;
	}
	for (i=0 ; i<((*size)-1) ; i++ ) {
		nouv_tab[i]=tab_symb[i] ;
		
	}
	nouv_tab[(*size)-1] = symb ;
	return nouv_tab ;
}

void relocation(SYMB* tab_symb, int size, L_TEXT * pl_text, L_DATA * pl_data, L_RELOC* pl_rel_text, L_RELOC* pl_rel_data){
	/* *** Déclaration et initialisation des variables *** */
	RELOC reloc ;
	SYMB* p_symb = NULL ;
	L_TEXT l_text = *pl_text ;
	L_DATA l_data = *pl_data ;
	char nom[512];
	int i ;
	DATA data ;
	TEXT text ;
	BSS bss ;	
	
	/* *** Gestion des etiquettes dans .text *** */
	while ( l_text != NULL ){
		reloc.ad_rel = ((l_text)->val).decalage ;
		for (i=0 ; i <(l_text)->val.nb_op ; i++){
			if ( ((l_text)->val).t_operande[i].type == 4 ){
				/* Si le symbole est deja défini */
				
				if ( (p_symb = p_symbole ((l_text)->val.t_operande[i].val.etiq.nom,tab_symb, size)) != NULL ){ 
					reloc.p_symb=p_symb ;
					
					/* Cas R_MIPS_26 */
					if ( strcmp(((l_text)->val).type_instruction,"J") ==0 ){
						reloc.ad_rel = (*p_symb).decalage ;
						strcpy(reloc.nom,charge_nom_rel(*p_symb,nom));
						reloc.type = R_MIPS_26 ;
						*pl_rel_text = ajout_tete_L_RELOC(reloc, *pl_rel_text) ;
					}
					
					/* Cas R_MIPS_HI16/R_MIPS_LO16 */
					else if ( strcmp((l_text)->val.type_instruction,"I") ==0 ){
						strcpy(reloc.nom,charge_nom_rel(*p_symb,nom));
						/* HI16 : On vérifie que le R_MIPS_HI16 n'existe pas déjà */
						if ( existence_HI16(*p_symb,*pl_rel_text) == FALSE ) {
							reloc.type = R_MIPS_HI16 ; 
							reloc.ad_rel = (*p_symb).decalage ;
							*pl_rel_text = ajout_tete_L_RELOC(reloc, *pl_rel_text) ;
						}
						/* LO16 */
						reloc.type = R_MIPS_LO16 ; 
						reloc.ad_rel = (l_text)->val.decalage ;
						*pl_rel_text = ajout_tete_L_RELOC(reloc, *pl_rel_text) ;
					
					}
					
					/* Cas R_MIPS_32 */
					else if ( (strcmp((l_text)->val.t_operande[i].val.etiq.attendu,".word") ==0) || (strcmp((l_text)->val.type_instruction,"R") ==0) ){
						strcpy(reloc.nom,charge_nom_rel(*p_symb,nom));
						reloc.type = R_MIPS_32 ;
						*pl_rel_text = ajout_tete_L_RELOC(reloc, *pl_rel_text) ;
					}
				}
				/* Cas ou symbole défini ailleurs ou non défini */
				else {
					/* On l'ajoute dans la table de symboles */
				
					text = (l_text)->val ;
				
					tab_symb = ajout_tab_symb (tab_symb,1,data,bss,text, &size,i) ;
					strcpy(reloc.nom,(l_text)->val.t_operande[i].val.etiq.nom);
					reloc.type = R_MIPS_26 ;
					reloc.ad_rel = (l_text)->val.decalage ;
					*pl_rel_text = ajout_tete_L_RELOC(reloc, *pl_rel_text) ;
						
				}
			}
		}
		(l_text) =(l_text)->suiv ;
	}
	/* *** Gestion des etiquettes dans .data *** */	
	while ( l_data != NULL ){
		reloc.ad_rel = (l_data->val).decalage ;
		if ( (l_data->val).operande.type == 4 ){
			
			/* Si le symbole est deja défini */
			if ( (p_symb = p_symbole (l_data->val.operande.val.etiq.nom,tab_symb, size)) != NULL ){ 
				reloc.p_symb=p_symb ;
				strcpy(reloc.nom,charge_nom_rel(*p_symb,nom));
				reloc.type = R_MIPS_32 ;
				*pl_rel_data = ajout_tete_L_RELOC(reloc, *pl_rel_data) ;
				
			}
			/* Cas ou symbole défini ailleurs ou non défini */
			else {
				data = l_data->val ;
				tab_symb = ajout_tab_symb (tab_symb,3,data,bss,text, &size,i) ;
				strcpy(reloc.nom,l_data->val.operande.val.etiq.nom);
				reloc.type = R_MIPS_26 ;
				reloc.ad_rel = l_data->val.decalage ;
				*pl_rel_data = ajout_tete_L_RELOC(reloc, *pl_rel_data) ;	
			}
		}
		l_data = l_data->suiv ;
	}
	
	/* *** Gestion des etiquettes dans .bss *** */	
	/* Rien à relloc dans cette section */

}	



int liste_est_vide_L_RELOC(L_RELOC L )
{
    return !L;
}

L_RELOC creer_liste_L_RELOC(void)
{
    return NULL;
}

L_RELOC ajout_tete_L_RELOC(RELOC c, L_RELOC L)
{ 
    L_RELOC p=calloc(1,sizeof(c));
    if (p==NULL) {puts("Erreur d'alloc"); return NULL ;}
    p->val=c;
    p->suiv=L;
    return p;
}

L_RELOC supprimer_tete_L_RELOC(L_RELOC L)
{
    if (!liste_est_vide_L_RELOC(L))
    {
        L_RELOC p;
        p=L->suiv;
        free(L);
        return p;
    }
    else
        return NULL;
}

void liberer_liste_L_RELOC(L_RELOC l)
{
    while(l != NULL)
    {
        l=supprimer_tete_L_RELOC(l);
    }
}


RELOC* liste_to_tab( L_RELOC l_reloc, int* size){
	int i=0 ;
	int j;
	RELOC* tab ;
	L_RELOC l=l_reloc ;
	while (l!=NULL){
		i++ ;
		l=l->suiv ;
	}
	if ( (tab=calloc(i,sizeof(RELOC))) == NULL) {
		puts("Error de calloc") ;
	}
	for (j=0;j<i;j++){
		tab[j]= (l_reloc->val) ;
		l_reloc = l_reloc->suiv ;
	}
	*size = i ;
	return tab ;
}














	
