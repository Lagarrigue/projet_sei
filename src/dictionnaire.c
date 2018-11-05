#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionnaire.h"

/* à changer : ajouter num ligne au lexeme */

/* ajouter les \n */
/* ajouter les , */

/*le type d'instruction sera R, I, J ou P */



int hash(char* nom, int l) /* fonction qui calcul l'indice d'un élément dans la table de hashage (basé sur le nom) */
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
L_INSTRUCTION*  lecture_dictionnaire(int longueur_table) 
{
    L_INSTRUCTION* T;
    FILE* fichier;
    fichier=fopen("src/dictionnaire_instructions.txt","r");
    if (fichier==NULL)
    {
        perror("Erreur ouverture fichier");
    }

    else
    {
        puts("ouverture en lecture reussie du fichier");

        T=creer_dictionnaire(longueur_table);
        char nom[512];
        initialisation_tab_char_dictionnaire(nom, 512);
        int operandes=0;
        char type_instruc;
        int nb_instruction=0;
        int i;
        int indice_tableau=0;
        INSTRUCTION instruction;
        fscanf(fichier, "%d ", &(nb_instruction));

        for(i=0; i<nb_instruction; i++) /*on lit ligne par ligne le fichier des instructions*/
        {
            fscanf(fichier, "%s %d %c ", nom, &(operandes), &type_instruc);/*espace après le dernier %d pour lire un caractère en plus (ici le \n)*/
            instruction.nb_op=operandes;
            instruction.type_instruction = type_instruc ;
            strcpy(instruction.nom_inst,nom);
            indice_tableau=hash(nom,longueur_table);/*changer taille tableau*/

            T[indice_tableau]=ajout_tete_dictionnaire(instruction, T[indice_tableau]);
            initialisation_tab_char_dictionnaire(nom,512);
        }
    }
	printf("dictionnaire cree avec succes\n");
	fclose(fichier);
	return T;
}

/* cette fonction renvoie le type INSTRUCTION* (adresse) qui contient le nom de l'instruction, son nb d'opérandes et son type d'instruction (R, IJP),
le format des données est définit plus haut dans le struct*/

INSTRUCTION* recherche_element(char mot[], L_INSTRUCTION* dicti, int longueur_table){
    int val_hash=0;
    val_hash=hash(mot, longueur_table);
    L_INSTRUCTION p;
    p=dicti[val_hash];
    while (p !=NULL){ /* parcours de la liste si les clefs de plusieurs éléments sont identiques */
        if( strcmp(p->val.nom_inst,mot)==0 ){/*comparaison des chaines non sensible à la case*/
            return &(p->val);
        }
        p=p->suiv;
    }
        return NULL;
    }



void initialisation_tab_char_dictionnaire(char tab[], int taille)
{
    int i=0;
    for(i=0; i<taille; i++)
    {
        tab[i]='\0';
    }
}

L_INSTRUCTION* creer_dictionnaire(int l) /*on crée une table de hashage par allocation dynamique*/
{
    L_INSTRUCTION* tableau=NULL;
    tableau=calloc(l, sizeof(*tableau));
    return tableau;
}

L_INSTRUCTION creer_liste_dictionnaire(void)
{
    return NULL;
}

int liste_est_vide_dictionnaire(L_INSTRUCTION L )
{
    return !L;
}

L_INSTRUCTION ajout_tete_dictionnaire(INSTRUCTION c, L_INSTRUCTION L)
{
    L_INSTRUCTION p=calloc(1,sizeof(*p));

    if (p==NULL)
    {
        return NULL;
    }
    p->val=c;
    p->suiv=L;
    return p;
}


L_INSTRUCTION supprimer_tete_dictionnaire(L_INSTRUCTION L)
{
    if (!liste_est_vide_dictionnaire(L))
    {
        L_INSTRUCTION p;
        p=L->suiv;
        free(L);
        return p;
    }
    else
        return NULL;
}

void liberer_liste_dictionnaire(L_INSTRUCTION l)
{
    while(liste_est_vide_dictionnaire(l) != 0)
    {
        l=supprimer_tete_dictionnaire(l);
    }
}

