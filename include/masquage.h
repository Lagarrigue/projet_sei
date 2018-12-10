/*
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "notify.h"
*/
#include "analyse_grammaticale.h"


/* ********** STRUCTURES ********** */

typedef struct code_32 {
	unsigned int donnee ;} CODE_32 ;

typedef struct maillon_code {
    CODE_32 val;
    struct maillon_code *suiv ; } * L_CODE_32 ;

/* ********** FONCTION DICTIONNAIRE********** */

int est_vide_code(L_CODE_32 L);
L_CODE_32 creer_liste_code(void);
L_CODE_32 ajout_tete_code(CODE_32 code, L_CODE_32 L);
L_CODE_32 supprimer_tete_code(L_CODE_32 L);
void liberer_liste_code(L_CODE_32 L);
unsigned int conversion_LittleEndian_vers_Big_Indian(unsigned int code);
L_CODE_32 operation_de_masquage(L_INSTRUCTION* dicti, int longueur_table, L_CODE_32 liste, TEXT instruction_TEXT);
L_CODE_32 retournement_de_liste(L_CODE_32 L1);
int parcours_section_text(L_TEXT section, L_INSTRUCTION* dicti, int longueur_table);
int valeur_operande(OPERANDE op[3], int num);
