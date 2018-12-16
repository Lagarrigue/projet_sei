/*
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "notify.h"
*/

#include "relocation.h"

#ifndef _MASQUAGE_H
#define _MASQUAGE_H

/* ********** STRUCTURES ********** */

typedef struct code_32 {
	unsigned int donnee ;} CODE_32 ;

typedef struct maillon_code {
    CODE_32 val;
    struct maillon_code *suiv ; } * L_CODE_32 ;
 
/* ********** GENERATION DE EFL ********** */
void make_elf ( int * , int text_size , int *  , int data_size,char * sym_char[], int sym_size,char* name,L_BSS* pl_bss, SYMB* tab_symb, int symb_size,RELOC* rel_text,int rel_text_size, RELOC* rel_data, int rel_data_size  ) ;

/* ********** FONCTION DE GENERATION BINAIRE********** */

int est_vide_code(L_CODE_32 L);
L_CODE_32 creer_liste_code(void);
L_CODE_32 ajout_tete_code(CODE_32 code, L_CODE_32 L);
L_CODE_32 supprimer_tete_code(L_CODE_32 L);
void liberer_liste_code(L_CODE_32 L);
unsigned int conversion_LittleEndian_vers_Big_Indian(unsigned int code);
L_CODE_32 operation_de_masquage_section_text(L_INSTRUCTION* dicti, int longueur_table, L_CODE_32 liste, TEXT instruction_TEXT);
L_CODE_32 retournement_de_liste(L_CODE_32 L1);
L_CODE_32 parcours_section_text(L_TEXT section, L_INSTRUCTION* dicti, int longueur_table);
int valeur_operande(OPERANDE op[3], int num, int cas_boff);
L_CODE_32 operation_de_masquage_section_data(L_DATA section);
void affichage_liste_binaire(L_CODE_32 liste);
int taille_liste_binaire(L_CODE_32 liste);
int* mise_en_memoire_listes_binaire(L_CODE_32 liste_binaire, int*);


/* ********** GENERATION DE EFL ********** */
void make_elf ( int text_prog[] , int text_size , int data_prog[] , int data_size,char * sym_char[], int sym_size,char* name,L_BSS* pl_bss, SYMB* tab_symb, int symb_size,RELOC* rel_text,int rel_text_size, RELOC* rel_data, int rel_data_size  ) ;



#endif
