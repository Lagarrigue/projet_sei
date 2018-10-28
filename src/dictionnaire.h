#include <stdio.h>
#include <stdlib.h>

/* ********** STRUCTURES ********** */

typedef struct
{
    char nom_inst[10];
    int nb_op;
    int num_decallage;
} INSTRUCTION;

typedef struct maillon
{
    INSTRUCTION val;
    struct maillon* suiv ;
}* L_INSTRUCTION;


/* ********** FONCTION DICTIONNAIRE********** */


void initialisation_tab_char_dictionnaire(char tab[], int taille);
L_INSTRUCTION* creer_dictionnaire(int l);
L_INSTRUCTION creer_liste_dictionnaire(void);
int liste_est_vide_dictionnaire(L_INSTRUCTION L );
L_INSTRUCTION ajout_tete_dictionnaire(INSTRUCTION c, L_INSTRUCTION L) ;
L_INSTRUCTION supprimer_tete_dictionnaire(L_INSTRUCTION L);
void liberer_liste_dictionnaire(L_INSTRUCTION l);
int hash(char nom[], int l);
L_INSTRUCTION* lecture_dictionnaire(int longueur_table);

/*
int recherche_element(char mot[], L_INSTRUCTION L){
    int val_hash=0;
    L_INSTRUCTIONp;
    while(p->val!=NULL){
        if(p->val.nom_inst,mot){//comparaison chaines à terminer
            return 1;
        }
        p=p->suiv;
        }
        return 0;
    }



//corriger les petits trucs sur début programme
//ajouter en queue de liste chaque élément
void analyse_grammaticale(L_LEXEMME LLEX){
    L_LEXEMME p;
    p=LLEX
    if (p->val.valeur=="text" && p-val.nom_type==6){//il faut fonction de oomparaison chaines
        //on va utiliser un automate et lire chaque lexeme, et en utilisant le dictionnaire on vérifie la syntaxe
        // à quoi serve les décallages ?
        //c'est quoi les primitives ??
    }
}

*/


