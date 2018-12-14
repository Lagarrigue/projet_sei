#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "analyse_lexicale.h"
#include "notify.h"

#ifndef _DICTIONNAIRE_REGISTRE_H
#define _DICTIONNAIRE_REGISTRE_H

/* ********** STRUCTURES ********** */

typedef struct {
    char nom_lettre[512] ;
    char nom_chiffre[512] ; } REGISTRE_reg ;

typedef struct maillonc {
    REGISTRE_reg val;
    struct maillonc* suiv ; } * L_REGISTRE_reg ;

/* ********** FONCTION DICTIONNAIRE********** */


void initialisation_tab_char_dictionnaire_reg(char tab[], int taille);
L_REGISTRE_reg* creer_dictionnaire_reg(int l);
L_REGISTRE_reg creer_liste_dictionnaire_reg(void);
int liste_est_vide_dictionnaire_reg(L_REGISTRE_reg L );
L_REGISTRE_reg supprimer_tete_dictionnaire_reg(L_REGISTRE_reg L);
void liberer_liste_dictionnaire_reg(L_REGISTRE_reg l);
int hash_reg(char* nom, int l);
L_REGISTRE_reg*  lecture_dictionnaire_reg(int longueur_table);
REGISTRE_reg* recherche_element_reg(char mot[], L_REGISTRE_reg* dicti, int longueur_table);
L_REGISTRE_reg ajout_tete_dictionnaire_reg(REGISTRE_reg c, L_REGISTRE_reg L) ;

#endif
