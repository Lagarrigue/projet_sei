#include <stdio.h>
#include <stdlib.h>

/* ********** STRUCTURES ********** */

typedef struct {
    char nom_inst[512];
    int nb_op;
    char type_instruction[2];  
    char type_op[3][10] ; 
    unsigned int opcode;
    char ordre_op[3][10] ;} INSTRUCTION;

typedef struct maillonb {
    INSTRUCTION val;
    struct maillonb* suiv ; }* L_INSTRUCTION;

/* ********** FONCTION DICTIONNAIRE********** */


void initialisation_tab_char_dictionnaire(char tab[], int taille);
L_INSTRUCTION* creer_dictionnaire(int l);
L_INSTRUCTION creer_liste_dictionnaire(void);
int liste_est_vide_dictionnaire(L_INSTRUCTION L );
L_INSTRUCTION ajout_tete_dictionnaire(INSTRUCTION c, L_INSTRUCTION L) ;
L_INSTRUCTION supprimer_tete_dictionnaire(L_INSTRUCTION L);
void liberer_liste_dictionnaire(L_INSTRUCTION l);
int hash(char nom[], int l);
L_INSTRUCTION*  lecture_dictionnaire(int longueur_table) ;
INSTRUCTION* recherche_element(char mot[], L_INSTRUCTION* dicti, int longueur_table) ;

