#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyse_lexicale.h"

int hash_pseudo(char* nom, int l) /* fonction qui calcul l'indice d'un élément dans la table de hashage (basé sur le nom) */
{
    int i=0;
    int h=0;
    while(nom[i]!='\0')
    {
        h=h+nom[i];
        i++;
    }
    return h%l;
}

/*format des fichiers : on met l'ensemble de champs à la suite que l'on sépare par une virgule ex ADD, 3, 3, 0*/
L_PSEUDO_INSTRUCTION*  lecture_dictionnaire_pseudo(int longueur_table) 
{
    L_PSEUDO_INSTRUCTION* T;
    FILE* fichier;
    fichier=fopen("src/pseudo_instructions.txt","r"); /* à changer pour le definitif */
    if (fichier==NULL)
    {
        perror("Erreur ouverture fichier");
    }

    else
    {
        puts("ouverture en lecture reussie du fichier");

        T=creer_dictionnaire_pseudo(longueur_table);
        char lecture_nom[512];
        initialisation_tab_char_dictionnaire_pseudo(lecture_nom, 512);
        int lecture_nb_operandes=0;
 
        int nb_instruction=0;
        int i;
        int indice_tableau=0;
        
        char lecture_type_operande_1[15];
        initialisation_tab_char_dictionnaire_pseudo(lecture_type_operande_1, 15);
        char lecture_type_operande_2[15];
        initialisation_tab_char_dictionnaire_pseudo(lecture_type_operande_2, 15);
        char lecture_type_operande_3[15];
        initialisation_tab_char_dictionnaire_pseudo(lecture_type_operande_2, 15);
        
        PSEUDO_INSTRUCTION instruction;
        fscanf(fichier, "%d ", &(nb_instruction));

		
        for(i=0; i<nb_instruction; i++) /*on lit ligne par ligne le fichier des instructions*/
        {
            fscanf(fichier, "%s %d %s %s %s", lecture_nom, &(lecture_nb_operandes), lecture_type_operande_1, lecture_type_operande_2, lecture_type_operande_3);/*espace après le dernier %d pour lire un caractère en plus (ici le \n)*/
            
            instruction.nb_op=lecture_nb_operandes;
     
            strcpy(instruction.nom_inst,lecture_nom);
            strcpy(instruction.type_op[0],lecture_type_operande_1);
            strcpy(instruction.type_op[1],lecture_type_operande_2);
            strcpy(instruction.type_op[2],lecture_type_operande_3);
            
            indice_tableau=hash_pseudo(lecture_nom,longueur_table);/*changer taille tableau*/

            T[indice_tableau]=ajout_tete_dictionnaire_pseudo(instruction, T[indice_tableau]);
            initialisation_tab_char_dictionnaire_pseudo(lecture_nom,512);
   
            initialisation_tab_char_dictionnaire_pseudo(lecture_type_operande_1, 15);
            initialisation_tab_char_dictionnaire_pseudo(lecture_type_operande_2, 15);
            initialisation_tab_char_dictionnaire_pseudo(lecture_type_operande_3, 15);
        }
    }
	printf("dictionnaire cree avec succes\n");
	fclose(fichier);
	return T;
}

/* cette fonction renvoie le type INSTRUCTION* (adresse) qui contient le nom de l'instruction, son nb d'opérandes et son type d'instruction (R, IJP),
le format des données est définit plus haut dans le struct*/

PSEUDO_INSTRUCTION* recherche_element_pseudo(char mot[], L_PSEUDO_INSTRUCTION* dicti, int longueur_table){
    int val_hash=0;
    val_hash=hash_pseudo(mot, longueur_table);
    L_PSEUDO_INSTRUCTION p;
    p=dicti[val_hash];
    while (p !=NULL){ /* parcours de la liste si les clefs de plusieurs éléments sont identiques */
        if( strcmp(p->val.nom_inst,mot)==0 ){/*comparaison des chaines non sensible à la case*/
            return &(p->val);
        }
        p=p->suiv;
    }
        return NULL;
    }
    

void initialisation_tab_char_dictionnaire_pseudo(char tab[], int taille)
{
    int i=0;
    for(i=0; i<taille; i++)
    {
        tab[i]='\0';
    }
}

L_PSEUDO_INSTRUCTION* creer_dictionnaire_pseudo(int l) /*on crée une table de hashage par allocation dynamique*/
{
    L_PSEUDO_INSTRUCTION* tableau=NULL;
    tableau=calloc(l, sizeof(*tableau));
    return tableau;
}

L_PSEUDO_INSTRUCTION creer_liste_dictionnaire_pseudo(void)
{
    return NULL;
}

int liste_est_vide_dictionnaire_pseudo(L_PSEUDO_INSTRUCTION L )
{
    return !L;
}

L_PSEUDO_INSTRUCTION ajout_tete_dictionnaire_pseudo(PSEUDO_INSTRUCTION c, L_PSEUDO_INSTRUCTION L)
{
    L_PSEUDO_INSTRUCTION p=calloc(1,sizeof(*p));

    if (p==NULL)
    {
        return NULL;
    }
    p->val=c;
    p->suiv=L;
    return p;
}


L_PSEUDO_INSTRUCTION supprimer_tete_dictionnaire_pseudo(L_PSEUDO_INSTRUCTION L)
{
    if (!liste_est_vide_dictionnaire_pseudo(L))
    {
        L_PSEUDO_INSTRUCTION p;
        p=L->suiv;
        free(L);
        return p;
    }
    else
        return NULL;
}

void liberer_liste_dictionnaire_pseudo(L_PSEUDO_INSTRUCTION l)
{
    while(liste_est_vide_dictionnaire_pseudo(l) != 0)
    {
        l=supprimer_tete_dictionnaire_pseudo(l);
    }
}


