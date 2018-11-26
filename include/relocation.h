
#include "analyse_grammaticale.h"


typedef struct reloc {
	char nom[512] ;
	unsigned int ad_rel ;
	enum type { R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16} type ;
	SYMB * p_symb ; } RELOC ;
	
/* attendu : { REG , IMM , SA , ETIQ , AD_REL , AD_ABS , OFFSET , BASE_OFF , TARGAET, ASCIIZ } SINON = 0
               1      2     3    4      5        6         7        8          9        10
*/	
	
SYMB* p_symbole ( char etiq[512], SYMB* tab_etiq, int n ) ;
