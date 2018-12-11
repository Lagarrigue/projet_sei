#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "analyse_grammaticale.h"
#include <string.h>



/* ********** liste de TEXT ********* */

int liste_est_vide_L_TEXT(L_TEXT L )
{
    return !L;
}

L_TEXT creer_liste_L_TEXT(void)
{
    return NULL;
}

L_TEXT ajout_tete_L_TEXT(TEXT c, L_TEXT L)
{
    L_TEXT p=calloc(1,sizeof(*p));

    if (p==NULL)
    {
        return NULL;
    }
    p->val=c;
    p->suiv=L;
    return p;
}

L_TEXT supprimer_tete_L_TEXT(L_TEXT L)
{
    if (!liste_est_vide_L_TEXT(L))
    {
        L_TEXT p;
        p=L->suiv;
        free(L);
        return p;
    }
    else
        return NULL;
}

void liberer_liste_L_TEXT(L_TEXT l)
{
    while(l != NULL)
    {
        l=supprimer_tete_L_TEXT(l);
    }
}



/* ********** liste de OPERANDE ********** */

int liste_est_vide_L_OPERANDE(L_OPERANDE L )
{
    return !L;
}

L_OPERANDE creer_liste_L_OPERANDE(void)
{
    return NULL;
}

L_OPERANDE ajout_tete_L_OPERANDE(OPERANDE c, L_OPERANDE L)
{
    L_OPERANDE p=calloc(1,sizeof(*p));

    if (p==NULL)
    {
        return NULL;
    }
    p->val=c;
    p->suiv=L;
    return p;
}

L_OPERANDE supprimer_tete_L_OPERANDE(L_OPERANDE L)
{
    if (!liste_est_vide_L_OPERANDE(L))
    {
        L_OPERANDE p;
        p=L->suiv;
        free(L);
        return p;
    }
    else
        return NULL;
}

void liberer_liste_L_OPERANDE(L_OPERANDE l)
{
    while(l != NULL)
    {
        l=supprimer_tete_L_OPERANDE(l);
    }
}



/* ********** liste de DATA ********** */

int liste_est_vide_L_DATA(L_DATA L )
{
    return !L;
}

L_DATA creer_liste_L_DATA(void)
{
    return NULL;
}

L_DATA ajout_tete_L_DATA(DATA c, L_DATA L)
{
    L_DATA p=calloc(1,sizeof(*p));

    if (p==NULL)
    {
        return NULL;
    }
    p->val=c;
    p->suiv=L;
    return p;
}

L_DATA supprimer_tete_L_DATA(L_DATA L) { 
	L_DATA p ;
	if (L==NULL){
		return NULL ;}
    	else
    	{
       	 	p = L->suiv;
        	free(L);
       		 return p;
    	}
}

void liberer_liste_L_DATA(L_DATA l)
{
    while(l != NULL)
    {
        l=supprimer_tete_L_DATA(l);
    }

}

L_DATA retournement_de_liste_L_DATA(L_DATA L1){
	L_DATA p;
	p=L1;
	L_DATA L2;
	L2=creer_liste_L_DATA();
	
	while(p!=NULL){
		L2=ajout_tete_L_DATA(p->val,L2);
		p=p->suiv;
	}
	return L2;

}

/* ********** liste de BSS ********** */

int liste_est_vide_L_BSS(L_BSS L )
{
    return !L;
}

L_BSS creer_liste_L_BSS(void)
{
    return NULL;
}

L_BSS ajout_tete_L_BSS(BSS c, L_BSS L)
{
    L_BSS p=calloc(1,sizeof(*p));

    if (p==NULL)
    {
        return NULL;
    }
    p->val=c;
    p->suiv=L;
    return p;
}

L_BSS supprimer_tete_L_BSS(L_BSS L)
{
    if (!liste_est_vide_L_BSS(L))
    {
        L_BSS p;
        p=L->suiv;
        free(L);
        return p;
    }
    else
        return NULL;
}

void liberer_liste_L_BSS(L_BSS l)
{
    while(l != NULL)
    {
        l=supprimer_tete_L_BSS(l);
    }
}



/* ********** liste de SYMB ********** */


int liste_est_vide_L_SYMB(L_SYMB L )
{
    return !L;
}

L_SYMB creer_liste_L_SYMB(void)
{
    return NULL;
}

L_SYMB ajout_tete_L_SYMB(SYMB c, L_SYMB L)
{
    L_SYMB p=calloc(1,sizeof(*p));

    if (p==NULL)
    {
        return NULL;
    }
    p->val=c;
    p->suiv=L;
    return p;
}

L_SYMB supprimer_tete_L_SYMB(L_SYMB L)
{
    if (!liste_est_vide_L_SYMB(L))
    {
        L_SYMB p;
        p=L->suiv;
        free(L);
        return p;
    }
    else
        return NULL;
}

void liberer_liste_L_SYMB(L_SYMB l)
{
    while(l != NULL)
    {
        l=supprimer_tete_L_SYMB(l);
    }
}



/* ********** TABLEAU DES SYMBOLES ********** */

int longueur_l_symb(L_SYMB l) {
	int n ;
	L_SYMB p;
	p=l;
	while(!liste_est_vide_L_SYMB(p)){
		n++;
		p=p->suiv;
		
	}
	return n ;
}


SYMB* creer_tab_symb(int l)
{
    SYMB* tableau=NULL;
    if ( (tableau=calloc(l, sizeof(*tableau))) == NULL ) { 
    	puts("Erreur allocation tableau");
    	return NULL ; }
    return tableau;
}


void conversion_liste_symb_vers_tableau(SYMB* tab, L_SYMB L, int size){
	L_SYMB p=L ;	
	int i=0;
	while(!liste_est_vide_L_SYMB(p)){	
		tab[size-i-1]=p->val;
		p=p->suiv;
		i++;
	}
}


