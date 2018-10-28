#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "analyse_grammaticale.h"



SYMB* creer_tab_symb(int l)
{
    SYMB* tableau=NULL;
    tableau=calloc(l, sizeof(*tableau));
    return tableau;
}


SYMB* conversion_liste_symb_vers_tableau(L_SYMB L){
	int longueur_tab=0;
	L_SYMB p;
	p=L;
	while(!liste_est_vide_L_SYMB(p)){
		longueur_tab++;
		p=p->suiv;
		
	}

	printf("la longueur est : %d\n", longueur_tab);
	SYMB* tab;
	tab=creer_tab_symb(longueur_tab);
	
	p=L;
	
	int i=0;
	while(!liste_est_vide_L_SYMB(p)){	
		tab[longueur_tab-i-1]=p->val;
		p=p->suiv;
		i++;
	}
	
	return tab;
}


/*
int main(){
	printf("-----------------------\n");
	SYMB a;
	strcpy(a.symbole,"aaaaaaaaa");
	a.ligne=1;
	a.section=3;
	a.decalage=3;
	
	SYMB b;
	strcpy(b.symbole,"bbbbbbbb");
    b.ligne=4;
	b.section=5;
	b.decalage=6;
	
	SYMB c;
	strcpy(c.symbole,"ccccccccccccc");
	c.ligne=7;
	c.section=8;
	c.decalage=9;
	
	L_SYMB L1;
	L1=creer_liste_L_SYMB();
	L1=ajout_tete_L_SYMB(a, L1);
	L1=ajout_tete_L_SYMB(b, L1);
	L1=ajout_tete_L_SYMB(c, L1);
	L1=ajout_tete_L_SYMB(c, L1);
	L1=supprimer_tete_L_SYMB(L1);
	liberer_liste_L_SYMB(L1);
	printf("-----------------------\n");
	
	SYMB* tab1;
	tab1=conversion_liste_symb_vers_tableau(L1);
	printf("/////////////////////////////\n");
	
	int j=0;
	for(j=0;j<3;j++){
		printf("on a la valeur suivante : %d\n",tab1[j].ligne);
	}
	return 1;
}
*/
