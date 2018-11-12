#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "analyse_grammaticale.h"

void lecture_liste_L_TEXT(L_TEXT L){
    L_TEXT p;
    int j=0;
    int i=0;
    p=L;
    if(liste_est_vide_L_TEXT(L)==0)
    {
	while(!liste_est_vide_L_TEXT(p)){
        	printf("------------------------\nLa donnee TEXT n°%d est :\n",j);
        	j++ ;
        	printf("* instruction : ");
        	while(p->val.instruction[i] !='\0'){
            	printf("%c",p->val.instruction[i]);
            	i++;
        	}
        	printf("\n");
        	printf("* ligne : %d\n",p->val.ligne);
        	printf("* decalage : %d\n",p->val.decalage);
        	printf("* nb_op : %d\n",p->val.nb_op);
        	p=p->suiv;
        	i=0;
		puts("") ;
	}
     }

     else 
     {
     printf("La liste TEXT est vide\n");
     }
}

void lecture_liste_L_DATA(L_DATA L){
    L_DATA p;
    int j=0;
    int i=0;
    p=L;
    if(liste_est_vide_L_DATA(L)==0)
    {
	while(!liste_est_vide_L_DATA(p)){
        	printf("-------------------------\nLa donnee DATA n°%d est :\n",j);
        	j++ ;
        	printf("* directive : ");
        	while(p->val.directive[i] !='\0'){
            		printf("%c",p->val.directive[i]);
            		i++;
        	}
        	printf("\n");
        	printf("* ligne : %d\n",p->val.ligne);
        	printf("* decalage : %d\n",p->val.decalage);
        	printf("* operande %s\n", p->val.operande.val);
        	p=p->suiv;
        	i=0;
		puts("") ;
	}
     }

     else 
     {
     printf("La liste  DATA est vide\n");
     }
}

void lecture_liste_L_BSS(L_BSS L){
    L_BSS p;
    int j=0;
    int i=0;
    p=L;
    if(liste_est_vide_L_BSS(L)==0)
    {
	while(!liste_est_vide_L_BSS(p)){
        	printf("-------------------------\nLa donnee BSS n°%d est :\n",j);
        	j++ ;
        	printf("* directive : ");
        	while(p->val.directive[i] !='\0'){
            	printf("%c",p->val.directive[i]);
            	i++;
        	}
        	printf("\n");
        	printf("* ligne : %d\n",p->val.ligne);
        	printf("* decalage : %d\n",p->val.decalage);
        	printf("* operande %s\n", p->val.operande.val);
        	p=p->suiv;
        	i=0;
		puts("") ;
	}
     }

     else 
     {
     printf("La liste BSS est vide\n");
     }
}

void lecture_liste_L_SYMB(L_SYMB L){
    L_SYMB p;
    int j=0;
    int i=0;
    p=L;
    if(liste_est_vide_L_SYMB(L)==0)
    {
	while(!liste_est_vide_L_SYMB(p)){
        	printf("-------------------------\nLa donnee SYMB n°%d est : \n",j);
        	j++ ;
        	printf("* symbole : ");
        	while(p->val.symbole[i] !='\0'){
            		printf("%c",p->val.symbole[i]);
            		i++;
        	}
        	printf("\n");
        	printf("* ligne : %d\n",p->val.ligne);
        	printf("* section : %d\n",p->val.section);
        	printf("* decalage : %d\n",p->val.decalage);
        	p=p->suiv;
        	i=0;
		puts("") ;
	}
     }

     else 
     {
     printf("La liste SYMB vide\n");
     }
}

void lecture_tab_symb( SYMB* tab, int size) {
	int i ;
	SYMB symb ;
	for (i=0 ; i<size ; i++) {
		symb=tab[i] ;
		printf("-------------------------\n") ;
        	printf("%s : ", symb.symbole) ;
        	printf("\n");
        	printf("* ligne : %d\n",symb.ligne);
        	printf("* section : %d\n", symb.section);
        	printf("* decalage : %d\n",symb.decalage);
		puts("") ;
	}
}
