#include "analyse_grammaticale.h"
#include "pelf/elf.h"

#ifndef _RELOCATION_H_
#define _RELOCATION_H_
 

typedef struct reloc {
	char nom[512] ;
	unsigned int ad_rel ;
	enum type {UNDIFINED, R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16} type ;
	SYMB * p_symb ; } RELOC ;
	
typedef struct l_reloc {
    	RELOC val ;
    	struct l_reloc *suiv ; } * L_RELOC ;
 
 
   	
typedef struct reloc_elf {
	char nom_etiq[256];} RELOC_ELF;


typedef struct maillon_reloc {
    RELOC_ELF val;
    struct maillon *suiv ; } * L_RELOC_ELF ;
	
/* attendu : { REG , IMM , SA , ETIQ , AD_REL , AD_ABS , OFFSET , BASE_OFF , TARGAET, ASCIIZ } SINON = 0
               1      2     3    4      5        6         7        8          9        10
*/	
	
SYMB* p_symbole ( char etiq[512], SYMB* tab_etiq, int n ) ;
SYMB* relocation(SYMB* tab_symb, int * size, L_TEXT * pl_text, L_DATA * pl_data, L_RELOC* pl_rel_text, L_RELOC* pl_rel_data) ;
int existence_symbole (char etiq[512] ,L_SYMB l_symb) ;
SYMB* ajout_tab_symb (SYMB* tab_symb,int section, DATA data, BSS bss,  TEXT text, int* size ,int j);
char* charge_nom_rel (SYMB symb, char nom[512]) ;
int existence_HI16(SYMB symb, L_RELOC l_reloc) ;
OPERANDE replacement_operande( char type_attendu[10] , int dec, VAL_OPERANDE etiq ,SYMB symb) ;

void lecture_tab_reloc(RELOC* tab, int size) ;

char** range_symb (L_LEXEME liste, SYMB* tab_symb, int size) ;


int liste_est_vide_L_RELOC(L_RELOC L ) ;
L_RELOC creer_liste_L_RELOC(void);
L_RELOC ajout_tete_L_RELOC(RELOC c, L_RELOC L) ;
L_RELOC supprimer_tete_L_RELOC(L_RELOC L) ;
void liberer_liste_L_RELOC(L_RELOC l) ;
void lecture_liste_L_RELOC(L_RELOC* L) ;
RELOC* liste_to_tab( L_RELOC l_reloc, int* size) ;

 
 
#endif
