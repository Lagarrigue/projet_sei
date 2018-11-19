#include "dictionnaire_registre.h"

void initialisation_tab_char_dictionnaire_reg(char tab[], int taille)
{
    int i=0;
    for(i=0; i<taille; i++)
    {
        tab[i]='\0';
    }
}

L_REGISTRE_reg* creer_dictionnaire_reg(int l) /*on crée une table de hashage par allocation dynamique*/
{
    L_REGISTRE_reg* tableau=NULL;
    tableau=calloc(l, sizeof(*tableau));
    return tableau;
}

L_REGISTRE_reg creer_liste_dictionnaire_reg(void)
{
    return NULL;
}

int liste_est_vide_dictionnaire_reg(L_REGISTRE_reg L )
{
    return !L;
}

L_REGISTRE_reg ajout_tete_dictionnaire_reg(REGISTRE_reg c, L_REGISTRE_reg L)
{
    L_REGISTRE_reg p=calloc(1,sizeof(*p));

    if (p==NULL)
    {
        return NULL;
    }
    p->val=c;
    p->suiv=L;
    return p;
}


L_REGISTRE_reg supprimer_tete_dictionnaire_reg(L_REGISTRE_reg L)
{
	L_REGISTRE_reg p= calloc(1,sizeof(*p));;
    if (!liste_est_vide_dictionnaire_reg(L))
    {
        p=L->suiv;
        free(L);
        return p;
    }
    else
        return NULL;
}

void liberer_liste_dictionnaire_reg(L_REGISTRE_reg l)
{
    while(liste_est_vide_dictionnaire_reg(l) != 0)
    {
        l=supprimer_tete_dictionnaire_reg(l);
    }
}


int hash_reg(char* nom, int l) /* fonction qui calcul l'indice d'un élément dans la table de hashage (basé sur le nom) */
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
L_REGISTRE_reg*  lecture_dictionnaire_reg(int longueur_table)
{
    L_REGISTRE_reg* T;
    FILE* fichier;
    fichier=fopen("src/dico_registre.txt","r");
    if (fichier==NULL)
    {
        perror("Erreur ouverture fichier");
    }

    else
    {
        puts("ouverture en lecture reussie du fichier");

        T=creer_dictionnaire_reg(longueur_table);
        char lecture_chiffre[512];
        initialisation_tab_char_dictionnaire_reg(lecture_chiffre, 512);
        char lecture_lettres[512];
	initialisation_tab_char_dictionnaire_reg(lecture_lettres, 512);

        int nb_instruction=0;
        int i;
        int indice_tableau=0;
        REGISTRE_reg registre;
        fscanf(fichier, "%d ", &(nb_instruction));

        for(i=0; i<nb_instruction; i++) /*on lit ligne par ligne le fichier des instructions*/
        {
            fscanf(fichier, "%s %s ", lecture_lettres, lecture_chiffre);/*espace après le dernier %d pour lire un caractère en plus (ici le \n)*/
            strcpy(registre.nom_chiffre,lecture_chiffre);
            strcpy(registre.nom_lettre, lecture_lettres) ;

            indice_tableau=hash_reg(lecture_lettres,longueur_table);/*changer taille tableau*/

            T[indice_tableau]=ajout_tete_dictionnaire_reg(registre, T[indice_tableau]);
            initialisation_tab_char_dictionnaire_reg(lecture_chiffre,512);
            initialisation_tab_char_dictionnaire_reg(lecture_lettres,512);
        }
    }
	printf("dictionnaire cree avec succes\n");
	fclose(fichier);
	return T;
}

/* cette fonction renvoie le type INSTRUCTION* (adresse) qui contient le nom de l'instruction, son nb d'opérandes et son type d'instruction (R, IJP),
le format des données est définit plus haut dans le struct*/

REGISTRE_reg* recherche_element_reg(char mot[], L_REGISTRE_reg* dicti, int longueur_table){
    int val_hash=0;
    val_hash=hash_reg(mot, longueur_table);
    L_REGISTRE_reg p;
    p=dicti[val_hash];
    while (p !=NULL){ /* parcours de la liste si les clefs de plusieurs éléments sont identiques */
        if( strcmp(p->val.nom_lettre,mot)==0 ){/*comparaison des chaines non sensible à la case*/
            return &(p->val);
        }
        p=p->suiv;
    }
        return NULL;
    }

