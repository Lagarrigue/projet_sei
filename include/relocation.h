
#include "analyse_grammaticale.h"


typedef struct reloc {
	int section ;
	unsigned int ad_rel ;
	enum type { R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16} type ;
	SYMB * p_symb ; } RELOC ;
	
	
SYMB* p_symbole ( char etiq[512], SYMB* tab_etiq, int n ) ;
