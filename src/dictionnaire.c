#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionnaire.h"
/* à changer : ajouter num ligne au lexeme */
/* ajouter les \n */
/* ajouter les , */


void initialisation_tab_char_dictionnaire(char tab[], int taille) {
    	int i=0 ;
    	for(i=0 ; i<taille ; i++) {
        	tab[i]='\0';
    	}
}

L_INSTRUCTION* creer_dictionnaire(int l) {
    L_INSTRUCTION* tableau=NULL;
    tableau=calloc(l, sizeof(*tableau));
    return tableau;
}



int hash(char* nom, int l) {
    	int i=0;
	int h=0;
   	while(nom[i]!='\0') {
        	h=h+nom[i];
        	i++;
	}
    	return h%l;
}

/*format des fichiers : on met l'ensemble de champs à la suite que l'on sépare par une virgule ex ADD, 3, 3, 0*/
L_INSTRUCTION*  lecture_dictionnaire (int longueur_table)  {
    	L_INSTRUCTION* T;
   	FILE* fichier;
   	fichier=fopen("dictionnaire.txt","r");
   	if (fichier==NULL) {
        	perror("Erreur ouverture fichier");
    	}

    	else {
		puts("ouverture en lecture reussie du fichier");
		T=creer_dictionnaire(longueur_table);
        	char nom[512];
        	initialisation_tab_char_dictionnaire(nom, 512);
        	/*à initialiser \0*/
        	int operandes=0;
        	int decallage=0;
        	int nb_instruction=0;
        	int i;
        	int indice_tableau=0;
        	INSTRUCTION instruction;
        	fscanf(fichier, "%d ", &(nb_instruction));
			
        	for(i=0; i<nb_instruction; i++) {
        		fscanf(fichier, "%s %d %d ", nom, &(operandes), &(decallage)); /*espace après le dernier %d pour lire un caractère en plus (ici le \n)*/
			instruction.nb_op=operandes;
        		instruction.num_decallage=decallage;
        		strcpy(instruction.nom_inst,nom);
        		indice_tableau=hash(nom,longueur_table); /*changer taille tableau*/
			T[indice_tableau]=ajout_tete_dictionnaire(instruction, T[indice_tableau]);
			initialisation_tab_char_dictionnaire(nom,512);
        	}
        	puts("------------------");
    	}
	puts("Dictionnaire cree avec succes");
	return T;
		/* Fermer le fichier */
}


int recherche_element(char mot[], L_INSTRUCTION* dicti, int longueur_table){
	int val_hash=0;
    	val_hash=hash(mot, longueur_table);
    	L_INSTRUCTION p;
    	p=dicti[val_hash];
    	while (p !=NULL){
    		if(strcmp(p->val.nom_inst,mot)==0){/*comparaison chaines à terminer*/
    		        return 1;
    	    	}
        	p=p->suiv;
        }
        return 0;
}


/*
corriger les petits trucs sur début programme
ajouter en queue de liste chaque élément
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


/* ********** FONCTIONS DE LISTES ********** */


L_INSTRUCTION creer_liste_dictionnaire(void) {
    	return NULL ;
}

int liste_est_vide_dictionnaire(L_INSTRUCTION L ){
    	return !L;
}

L_INSTRUCTION ajout_tete_dictionnaire(INSTRUCTION c, L_INSTRUCTION L){
    	L_INSTRUCTION p=calloc(1,sizeof(*p));
    	if (p==NULL) {
        	return NULL;
    	}
    	p->val=c ;
    	p->suiv=L ;
    	return p ;
}


L_INSTRUCTION supprimer_tete_dictionnaire(L_INSTRUCTION L) {
    	if (!liste_est_vide_dictionnaire(L)) {
        	L_INSTRUCTION p;
        	p=L->suiv;
        	free(L);
        	return p ;
    	}
    	else
        	return NULL ;
}

void liberer_liste_dictionnaire(L_INSTRUCTION l) {
    	while(liste_est_vide_dictionnaire(l) != 0) {
        	l=supprimer_tete_dictionnaire(l) ;
    	}
}

