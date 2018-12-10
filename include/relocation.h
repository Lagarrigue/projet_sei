
#include "analyse_grammaticale.h"


typedef struct reloc {
	char nom[512] ;
	unsigned int ad_rel ;
	enum type {UNDIFINED, R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16} type ;
	SYMB * p_symb ; } RELOC ;
	
/* attendu : { REG , IMM , SA , ETIQ , AD_REL , AD_ABS , OFFSET , BASE_OFF , TARGAET, ASCIIZ } SINON = 0
               1      2     3    4      5        6         7        8          9        10
*/	
	
SYMB* p_symbole ( char etiq[512], SYMB* tab_etiq, int n ) ;
RELOC** relocation(SYMB* tab_symb, int size, L_TEXT * pl_text, L_DATA * pl_data) ;
int existence_symbole (char etiq[512] ,L_SYMB l_symb) ;
SYMB* ajout_tab_symb (SYMB* tab_symb,int section, DATA data, BSS bss,  TEXT text, int* size ,int j);
char* charge_nom_rel (SYMB symb, char nom[512]) ;
int existence_HI16(SYMB symb, RELOC* tab_reloc, int size) ;

void lecture_tab_reloc(RELOC* tab, int size) ;
