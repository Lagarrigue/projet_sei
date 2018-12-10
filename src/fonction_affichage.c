#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "relocation.h"

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
        		if ( (p->val).t_operande[n].type ==  4){
        			printf(" # OP %d : %s  (ETIQ)\n",n+1,((p->val).t_operande[n]).val.etiq.nom);
        		}
        		else if ( (p->val).t_operande[n].type ==  10 ){
        			printf(" # OP %d : %s  (STR)\n",n+1,((p->val).t_operande[n]).val.etiq.nom);
        		}
        		else if ( ((p->val).t_operande[n]).type == 1 ){
        			printf(" # OP %d : %d  (REG)\n",n+1 ,((p->val).t_operande[n]).val.reg);
        		}
        		else if ( ((p->val).t_operande[n]).type == 2 ){
        			printf(" # OP %d : %hd  (IMM)\n",n+1 ,((p->val).t_operande[n]).val.imm);
        		}
        		else if ( ((p->val).t_operande[n]).type == 3 ){
        			printf(" # OP %d : %d  (SA)\n",n+1 ,((p->val).t_operande[n]).val.sa);
        		}
        		else if ( ((p->val).t_operande[n]).type == 7 ){
        			printf(" # OP %d : %hd  (OFFSET)\n",n+1 ,((p->val).t_operande[n]).val.imm);
        		}
        		else if ( ((p->val).t_operande[n]).type == 8 ){
        			printf(" # OP %d : %d (%hd) (BASE_OFF)\n",n +1,((p->val).t_operande[n]).val.base_offset.reg, ((p->val).t_operande[n]).val.base_offset.offset);
        		}
        		else if ( ((p->val).t_operande[n]).type == 9 ){
        			printf(" # OP %d : %ld  (TARGET)\n",n+1 ,((p->val).t_operande[n]).val.tar);
        		}
        		else { 
        			printf("* valeur op : %d\n", ((p->val).t_operande[n]).val.nb) ;
        		}
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
        	printf("* operande : "); 
        	if ( (p->val).operande.type ==  4 || (p->val).operande.type==10 ){
        		printf("%s\n",(p->val).operande.val.etiq.nom);
        	}
        	else { 
        		printf("%d\n", (p->val).operande.val.nb) ;
        	}
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
    p=L;
    if(liste_est_vide_L_BSS(L)==0)
    {
	while(!liste_est_vide_L_BSS(p)){
        	printf("-------------------------\nLa donnee BSS n°%d est :\n",j);
        	j++ ;
        	printf("* directive : ");
            	printf("%s \n",p->val.directive);
        	printf("* ligne : %d\n",p->val.ligne);
        	printf("* decalage : %d\n",p->val.decalage); 
        	printf("* operande : "); 
        	if ( (p->val).operande.type ==  4 ){
        		printf("%s\n",(p->val).operande.val.etiq.nom);
        	}
        	else { 
        		printf("%d\n", (p->val).operande.val.nb) ;
        	} 
        	p=p->suiv;
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

void lecture_tab_reloc(RELOC* tab, int size) {
 	int i ;
 	char type[512];
	for (i=0 ; i<size ; i++) {
		if (tab[i].type == UNDIFINED) { strcpy(type,"UNDIFINED") ;}
		else if (tab[i].type == R_MIPS_32) { strcpy(type,"R_MIPS_32") ;}
		else if (tab[i].type == R_MIPS_LO16) { strcpy(type,"R_MIPS_LO16") ;}
		else if (tab[i].type == R_MIPS_HI16) { strcpy(type,"R_MIPS_HI16") ;}
		else if (tab[i].type == R_MIPS_26) { strcpy(type,"R_MIPS_26") ;}
		printf(" OFFSET     TYPE      VALUE \n") ;
        	printf("   %d       %s         %s ", tab[i].ad_rel, type,(tab[i]).nom )  ;
        	printf("\n");
	}
}
