
#include "analyse_grammaticale.h"


typedef struct reloc {
	char nom[512] ;
	unsigned int ad_rel ;
	enum type {UNDIFINED, R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16} type ;
	SYMB * p_symb ; } RELOC ;
	
typedef struct l_reloc {
    	RELOC val ;
    	struct l_reloc *suiv ; } * L_RELOC ;
	
/* attendu : { REG , IMM , SA , ETIQ , AD_REL , AD_ABS , OFFSET , BASE_OFF , TARGAET, ASCIIZ } SINON = 0
               1      2     3    4      5        6         7        8          9        10
*/	
	
SYMB* p_symbole ( char etiq[512], SYMB* tab_etiq, int n ) ;
void relocation(SYMB* tab_symb, int size, L_TEXT * pl_text, L_DATA * pl_data, L_RELOC* pl_rel_text, L_RELOC* pl_rel_data) ;
int existence_symbole (char etiq[512] ,L_SYMB l_symb) ;
SYMB* ajout_tab_symb (SYMB* tab_symb,int section, DATA data, BSS bss,  TEXT text, int* size ,int j);
char* charge_nom_rel (SYMB symb, char nom[512]) ;
int existence_HI16(SYMB symb, L_RELOC l_reloc) ;

void lecture_tab_reloc(RELOC* tab, int size) ;


int liste_est_vide_L_RELOC(L_RELOC L ) ;
L_RELOC creer_liste_L_RELOC(void);
L_RELOC ajout_tete_L_RELOC(RELOC c, L_RELOC L) ;
L_RELOC supprimer_tete_L_RELOC(L_RELOC L) ;
void liberer_liste_L_RELOC(L_RELOC l) ;
void lecture_liste_L_RELOC(L_RELOC* L) ;
RELOC* liste_to_tab( L_RELOC l_reloc, int* size) ;
