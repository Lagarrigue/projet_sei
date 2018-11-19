#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "analyse_grammaticale.h"

void lecture_liste_L_TEXT(L_TEXT L){
    L_TEXT p;
    int j=0;
    int i=0;
    int n ;
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
        	for (n=0 ; n<p->val.nb_op ; n++ ) {
        		if ( (p->val).t_operande[n].type ==  4 ){
        			printf(" # OP %d : %s  (type :%d)\n",n ,((p->val).t_operande[n]).val.etiq, (p->val).t_operande[n]).type);
        		}
        		else if ( ((p->val).t_operande[n]).type == 1 ){
        			printf(" # OP %d : %lf  (type :%d)\n",n ,((p->val).t_operande[n]).val.reg, (p->val).t_operande[n]).type);
        		}
        		else if ( ((p->val).t_operande[n]).type == 2 ){
        			printf(" # OP %d : %lf  (type :%d)\n",n ,((p->val).t_operande[n]).val.imm, (p->val).t_operande[n]).type);
        		}
        		else if ( ((p->val).t_operande[n]).type == 3 ){
        			printf(" # OP %d : %lf  (type :%d)\n",n ,((p->val).t_operande[n]).val.sa, (p->val).t_operande[n]).type);
        		}
        		else if ( ((p->val).t_operande[n]).type == 5 ){
        			printf(" # OP %d : %lf  (type :%d)\n",n ,((p->val).t_operande[n]).val.ad_rel, (p->val).t_operande[n]).type);
        		}
        		else if ( ((p->val).t_operande[n]).type == 6 ){
        			printf(" # OP %d : %lf  (type :%d)\n",n ,((p->val).t_operande[n]).val.ad_abs, (p->val).t_operande[n]).type);
        		}
        		else if ( ((p->val).t_operande[n]).type == 7 ){
        			printf(" # OP %d : %lf  (type :%d)\n",n ,((p->val).t_operande[n]).val.imm, (p->val).t_operande[n]).type);
        		}
        		else if ( ((p->val).t_operande[n]).type == 8 ){
        			printf(" # OP %d : %lf  (type :%d)\n",n ,((p->val).t_operande[n]).val.base_offset, (p->val).t_operande[n]).type);
        		}
        		else if ( ((p->val).t_operande[n]).type == 9 ){
        			printf(" # OP %d : %lf  (type :%d)\n",n ,((p->val).t_operande[n]).val.tar, (p->val).t_operande[n]).type);
        		}
      			n++ ;
      		}	
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
        	printf("* operande : %d\n", p->val.operande.val.nb); 
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
        	printf("* operande : %d\n", p->val.operande.val.nb); 
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
	for (i=0 ; i<size ; i++) {
		printf("-------------------------\n") ;
        	printf("%s : ", (tab[i]).symbole)  ;
        	printf("\n");
        	printf("* ligne : %d\n",tab[i].ligne);
        	printf("* section : %d\n", tab[i].section);
        	printf("* decalage : %d\n",tab[i].decalage);
		puts("") ;
	}
}
