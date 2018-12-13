

#include "analyse_lexicale.h"



L_LEXEME suppression_maillon(L_LEXEME l, int num_lex){
	
	L_LEXEME p1;
	L_LEXEME p2;
	L_LEXEME p;
	int m=0;
	if(l==NULL){
		return NULL;
	}

	else if(l->suiv==NULL){

		return NULL;
	}

	else if(num_lex==l->val.numero_lexeme){
		l=supprimer_tete(l);
		return l;
	}

	else{
		p1=l;
		m=0;
		while( p1->suiv != NULL){
			if(m==0 && p1->suiv !=NULL){
			
				if(p1->suiv->val.numero_lexeme==num_lex){
					m=1;
					p2=p1;
				}
				p1=p1->suiv;
			}
			else{
				p1=p1->suiv;
				
				}
		}

		if(p2->suiv !=NULL){
			p=p2->suiv->suiv;
			p2->suiv=p;

		return l;
		}

		else{
			return l;
		}
	}
}


/* le maillon est ajouté après le lexeme portant le numero*/
L_LEXEME ajouter_maillon(L_LEXEME l, int num_lex, LEXEME lex){
	L_LEXEME p1;
	L_LEXEME p;
	p=calloc(1,sizeof(*p));


	if(l==NULL){
		return NULL;
	}


	else{
		p1=l;
		int num=0;
		num=l->val.numero_lexeme;
		

		while( p1->suiv != NULL && num != num_lex){
			p1=p1->suiv;
			num=p1->val.numero_lexeme;
		}

			p->val=lex;
			p->suiv=p1->suiv;
			p1->suiv=p;

		return l;

	}
}

L_LEXEME ajuster_numero_lexeme(L_LEXEME l){
	int i=1;
	L_LEXEME p;
	p=l;
	while(p->suiv !=NULL){
		p->val.numero_lexeme=i;
		i++;
		p=p->suiv;
	}
	return l;
}

L_LEXEME fonction_chainage(L_LEXEME l, int num_lex, L_LEXEME l2, int nb_operandes){
		
		
		L_LEXEME p;
		int i=0;
		i=num_lex;
		int j=0;
		
		/* partie supression operandes de la liste lexeme*/
		
		
		if (nb_operandes==0){
			l=suppression_maillon(l,i);
		}
		else if (nb_operandes !=0){
			for(j=i; j<i+nb_operandes*2; j++){
				l=suppression_maillon(l,j);
			}
		}
		
		/* partie chainage de la nouvelle fonction et operandes*/
		if(i==l->val.numero_lexeme){
            l2=renversement_liste(l2);
            p=l2;
            while(p->suiv !=NULL)
            {
                l=ajout_tete(p->val, l);
                p=p->suiv;

            }
            l=ajout_tete(p->val,l);
		}
		
		else{
            l2=renversement_liste(l2);
            p=l2;
            do
            {
                l=ajouter_maillon(l,i-1,p->val);
                p=p->suiv;
            }
            while(p->suiv !=NULL);
            l=ajouter_maillon(l,i-1,p->val);
		}
		
		l=ajuster_numero_lexeme(l);
        liberer_liste(l2);
        return l;

}


L_LEXEME remplacement_pseudo_instruction(L_LEXEME l, int num_lex, char* nom_pseudo){
	LEXEME lex;
	L_LEXEME l2;
	l2=creer_liste();
	L_LEXEME p1;
	int k=0;
	k=num_lex;
	p1=l;
	if (num_lex == 0){
		return l ;}
	while(p1->val.numero_lexeme != k && p1->suiv !=NULL){
		p1=p1->suiv;
	}



	if(strcasecmp(nom_pseudo, "NOP")==0 ){

		strcpy(lex.valeur,"0");
		lex.nom_type=8;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);


		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,"0");
		lex.nom_type=5;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=l->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,"0");
		lex.nom_type=5;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,"SLL");
		lex.nom_type=7;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);      
            
	return fonction_chainage(l, num_lex, l2, 0);
	}

	else if (strcasecmp(nom_pseudo, "LW")==0 ){
		
	    /* chargement du LW*/
		strcpy(lex.valeur,p1->suiv->val.valeur);
		lex.nom_type=13;
		lex.numero_ligne=p1->val.numero_ligne;
		lex.reloc=2;
		l2=ajout_tete(lex,l2);
		
		strcpy(lex.valeur, p1->suiv->suiv->suiv->val.valeur);
		lex.nom_type=7;
		lex.reloc=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);
		

		strcpy(lex.valeur,p1->suiv->val.valeur);
		lex.nom_type=p1->suiv->val.nom_type;
		lex.numero_ligne=p1->val.numero_ligne;
		lex.reloc=2;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,"LW");
		lex.nom_type=7;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);
		
		strcpy(lex.valeur, p1->suiv->suiv->suiv->val.valeur);
		lex.nom_type=7;
		lex.reloc=1;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,p1->suiv->val.valeur);
		lex.nom_type=p1->suiv->val.nom_type;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		
		strcpy(lex.valeur,"LUI");
		lex.nom_type=7;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		return fonction_chainage(l, num_lex, l2, 2);

	}
	else if (strcasecmp(nom_pseudo, "SW")==0){

		/* chargement du SW*/
		strcpy(lex.valeur,"1");
		lex.nom_type=13;
		lex.reloc=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);
		
		strcpy(lex.valeur, p1->suiv->suiv->suiv->val.valeur);
		lex.nom_type=7;
		lex.reloc=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);
		
		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,p1->suiv->val.valeur);
		lex.nom_type=p1->suiv->val.nom_type;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,"SW");
		lex.nom_type=7;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		
		strcpy(lex.valeur, p1->suiv->suiv->suiv->val.valeur);
		lex.nom_type=7;
		lex.reloc=1;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);
		
		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,"1");
		lex.nom_type=5;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		
		strcpy(lex.valeur,"LUI");
		lex.nom_type=7;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);
		
		return fonction_chainage(l, num_lex, l2, 2);

	}
	else if (strcasecmp(nom_pseudo, "MOVE")==0){

		strcpy(lex.valeur,"0");
		lex.nom_type=5;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=l->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,p1->suiv->suiv->suiv->val.valeur);
		lex.nom_type=p1->suiv->suiv->suiv->val.nom_type;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,p1->suiv->val.valeur);
		lex.nom_type=p1->suiv->val.nom_type;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		
		strcpy(lex.valeur,"ADD");
		lex.nom_type=7;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);
		
		
		return fonction_chainage(l, num_lex, l2, 2);
	} 
	
	else if (strcasecmp(nom_pseudo, "NEG")==0){

		strcpy(lex.valeur,p1->suiv->suiv->suiv->val.valeur);
		lex.nom_type=p1->suiv->suiv->suiv->val.nom_type;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);
		
		strcpy(lex.valeur,"0");
		lex.nom_type=5;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,p1->suiv->val.valeur);
		lex.nom_type=p1->suiv->val.nom_type;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);
		
		strcpy(lex.valeur,"SUB");
		lex.nom_type=7;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);
		
	
		return fonction_chainage(l, num_lex, l2, 2);
	}
	else if (strcasecmp(nom_pseudo, "LI")==0){

		strcpy(lex.valeur,p1->suiv->suiv->suiv->val.valeur);
		lex.nom_type=p1->suiv->suiv->suiv->val.nom_type;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,"0");
		lex.nom_type=5;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);
	
		strcpy(lex.valeur,p1->suiv->val.valeur);
		lex.nom_type=p1->suiv->val.nom_type;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);
		
		strcpy(lex.valeur,"ADDI");
		lex.nom_type=7;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);
		
	
		return fonction_chainage(l, num_lex, l2, 2);
	}

	else if (strcasecmp(nom_pseudo, "BLT")==0){


		/* chargement de BNE*/
		strcpy(lex.valeur,p1->suiv->suiv->suiv->suiv->suiv->val.valeur);
		lex.nom_type=p1->suiv->suiv->suiv->suiv->suiv->val.nom_type;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,"0");
		lex.nom_type=5;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,"1");
		lex.nom_type=5;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,"BNE");
		lex.nom_type=7;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);


		/* chargement de SLT*/
		strcpy(lex.valeur,p1->suiv->suiv->suiv->val.valeur);
		lex.nom_type=p1->suiv->suiv->suiv->val.nom_type;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,p1->suiv->val.valeur);
		lex.nom_type=p1->suiv->val.nom_type;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,"1");
		lex.nom_type=5;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);
		
		strcpy(lex.valeur,"SLT");	
		lex.nom_type=7;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);
		
		return fonction_chainage(l, num_lex, l2, 3);
	}

	else{
		return l;
	}



}

/* les types des opérandes annoncés par le dictionnaire d'instruction n'ont pas aboutis et ont été considérés comme une erreur*/
/* on va vérifier si cette instruction n'est pas en fait une pseudo instruction*/
/* dans ce cas on renvoie la liste de lexeme à partir du point du */
L_LEXEME verification_appartenance_pseudo_instruction(L_LEXEME liste_lexemes, L_PSEUDO_INSTRUCTION* dico, int longueur_dico){
	L_LEXEME p;
	p=liste_lexemes;
	
	
	PSEUDO_INSTRUCTION* pseudo;
	
	
	while(p !=NULL){
		if(p->val.nom_type==7){
			pseudo=recherche_element_pseudo(p->val.valeur , dico, longueur_dico);
			if(pseudo != NULL && strcasecmp(p->val.valeur, "LW") !=0 && strcasecmp(p->val.valeur, "SW")!=0 ){ 
				liste_lexemes=remplacement_pseudo_instruction(liste_lexemes, p->val.numero_lexeme , p->val.valeur);
			}
		}
		
		p=p->suiv;
	}
	
	return liste_lexemes;
}


