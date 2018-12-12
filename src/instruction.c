#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "instruction.h"

/* à changer : ajouter num ligne au lexeme */

/* ajouter les \n */
/* ajouter les , */

/*le type d'instruction sera R, I, J ou P */



int hash(char* nom, int l) /* fonction qui calcul l'indice d'un élément dans la table de hashage (basé sur le nom) */
{
    int i=0;
    int h=0;
    
    int num=0;
    while(nom[i]!='\0')
    {
        num=nom[i];
        num=conversion_min_maj(num);
        h=h+num;
        i++;
    }
    return h%l;
}

int conversion_min_maj(int lettre){
	if(lettre==97){
		return 65;
	}

	else if(lettre==98){
		return 66;
	}
	
	else if(lettre==99){
		return 67;
	}
	
	else if(lettre==100){
		return 68;
	}
	
	else if(lettre==101){
		return 69;
	}
	
	else if(lettre==102){
		return 70;
	}
	
	else if(lettre==103){
		return 71;
	}
	
	else if(lettre==104){
		return 72;
	}
	
	else if(lettre==105){
		return 73;
	}
	
	else if(lettre==106){
		return 74;
	}
	
	else if(lettre==107){
		return 75;
	}
	
	else if(lettre==108){
		return 76;
	}
	
	else if(lettre==109){
		return 77;
	}
	
	else if(lettre==110){
		return 78;
	}
	
	else if(lettre==111){
		return 79;
	}
	
	else if(lettre==112){
		return 80;
	}
	
	else if(lettre==113){
		return 81;
	}
	
	else if(lettre==114){
		return 82;
	}
	else if(lettre==115){
		return 83;
	}
	else if(lettre==116){
		return 84;
	}
	else if(lettre==117){
		return 85;
	}
	else if(lettre==118){
		return 86;
	}
	else if(lettre==119){
		return 87;
	}
	else if(lettre==120){
		return 88;
	}
	else if(lettre==121){
		return 89;
	}
	else if(lettre==122){
		return 90;
	}
	
	else{
		return lettre;
	}
	

}


/*format des fichiers : on met l'ensemble de champs à la suite que l'on sépare par une virgule ex ADD, 3, 3, 0*/
L_INSTRUCTION*  lecture_dictionnaire(int longueur_table) 
{
    L_INSTRUCTION* T;
    FILE* fichier;
    fichier=fopen("src/dictionnaire_instructions.txt","r"); /* à changer pour le definitif */
    if (fichier==NULL)
    {
        perror("Erreur ouverture fichier");
    }

    else
    {

        T=creer_dictionnaire(longueur_table);
        char lecture_nom[512];
        initialisation_tab_char_dictionnaire(lecture_nom, 512);
        int lecture_nb_operandes=0;
        char lecture_type_instruc[2];
        initialisation_tab_char_dictionnaire(lecture_type_instruc, 2);
        int nb_instruction=0;
        unsigned int lecture_opcode=0;
        int i;
        int indice_tableau=0;
        
        char lecture_type_operande_1[15];
        initialisation_tab_char_dictionnaire(lecture_type_operande_1, 15);
        char lecture_type_operande_2[15];
        initialisation_tab_char_dictionnaire(lecture_type_operande_2, 15);
        char lecture_type_operande_3[15];
        initialisation_tab_char_dictionnaire(lecture_type_operande_3, 15);
        
        char lecture_ordre_operande_1[15];
        initialisation_tab_char_dictionnaire(lecture_ordre_operande_1, 15);
        char lecture_ordre_operande_2[15];
        initialisation_tab_char_dictionnaire(lecture_ordre_operande_2, 15);
        char lecture_ordre_operande_3[15];
        initialisation_tab_char_dictionnaire(lecture_ordre_operande_3, 15);
        
        INSTRUCTION instruction;
        fscanf(fichier, "%d ", &(nb_instruction));

        for(i=0; i<nb_instruction; i++) /*on lit ligne par ligne le fichier des instructions*/
        {
            fscanf(fichier, "%s %d %s %s %s %s %s %s %s %d ", lecture_nom, &(lecture_nb_operandes), lecture_type_instruc, lecture_type_operande_1, lecture_type_operande_2, lecture_type_operande_3, lecture_ordre_operande_1, lecture_ordre_operande_2, lecture_ordre_operande_3, &(lecture_opcode));/*espace après le dernier %d pour lire un caractère en plus (ici le \n)*/
            
            instruction.nb_op=lecture_nb_operandes;
            instruction.opcode=lecture_opcode;
            strcpy(instruction.type_instruction, lecture_type_instruc);
            strcpy(instruction.nom_inst,lecture_nom);
            strcpy(instruction.type_op[0],lecture_type_operande_1);
            strcpy(instruction.type_op[1],lecture_type_operande_2);
            strcpy(instruction.type_op[2],lecture_type_operande_3);
            strcpy(instruction.ordre_op[0],lecture_ordre_operande_1);
            strcpy(instruction.ordre_op[1],lecture_ordre_operande_2);
            strcpy(instruction.ordre_op[2],lecture_ordre_operande_3);
          
            indice_tableau=hash(lecture_nom,longueur_table);/*changer taille tableau*/

            T[indice_tableau]=ajout_tete_dictionnaire(instruction, T[indice_tableau]);
            initialisation_tab_char_dictionnaire(lecture_nom,512);
            initialisation_tab_char_dictionnaire(lecture_type_instruc, 2);
            initialisation_tab_char_dictionnaire(lecture_type_operande_1, 15);
            initialisation_tab_char_dictionnaire(lecture_type_operande_2, 15);
            initialisation_tab_char_dictionnaire(lecture_type_operande_3, 15);
            initialisation_tab_char_dictionnaire(lecture_ordre_operande_1, 15);
            initialisation_tab_char_dictionnaire(lecture_ordre_operande_2, 15);
            initialisation_tab_char_dictionnaire(lecture_ordre_operande_3, 15);
        }
    }
    	
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
        if( strcasecmp(p->val.nom_inst,mot)==0 ){/*comparaison des chaines non sensible à la case*/
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

