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
    while(liste_est_vide_L_TEXT(l) != 0)
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
    while(liste_est_vide_L_OPERANDE(l) != 0)
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

L_DATA supprimer_tete_L_DATA(L_DATA L)
{
    if (!liste_est_vide_L_DATA(L))
    {
        L_DATA p;
        p=L->suiv;
        free(L);
        return p;
    }
    else
        return NULL;
}

void liberer_liste_L_DATA(L_DATA l)
{
    while(liste_est_vide_L_DATA(l) != 0)
    {
        l=supprimer_tete_L_DATA(l);
    }
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
    while(liste_est_vide_L_BSS(l) != 0)
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
    while(liste_est_vide_L_SYMB(l) != 0)
    {
        l=supprimer_tete_L_SYMB(l);
    }
}



/* ********** TABLEAU DES SYMBOLES ********** */

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

