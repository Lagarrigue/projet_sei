#include "analyse_lexicale.h"



L_LEXEME suppression_maillon(L_LEXEME l, int num_lex){
	
	L_LEXEME p1;
	L_LEXEME p2;
	L_LEXEME p;
	int m=0;
	int num ;
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
		num=l->val.numero_lexeme;
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
	initialisation_lexeme(&lex);
	int nb=0;
	L_LEXEME l2;
	l2=creer_liste();
	L_LEXEME p1;
	int k=0;
	k=num_lex;
	p1=l;
		
	while(p1->val.numero_lexeme != k && p1->suiv !=NULL){
		p1=p1->suiv;
	}

	/* gerer les else en cas de probleme sur target ou immediate utiliser fonction déjà utilisées*/
	/* on ne fait pas ce sera fait tout seul par la partie de theo la verification des operandes doit etre faite juste après l'entree dans ma condition else if*/
	/*revoir les histoires de signe avec le mail : pas clair du tout que faire si on a un - dans le code assemble ??????*/
	/*verification dans analyse lexicale des registres parenthèse*/
	/* comment controler le registre des base offsets utiliser : utiliser le dico de analyse lex*/
	/*à faire dans analyse lexicalz*/
	/* ajoute rles numeros de lexeme*/
	/* dans l'analyse lexicale donner le num aux lexeme et modition fonction affichage*/
	/* verifier les parentheses pour les base offf*/
	
	/* faire absolument verification oeprandes des pseudo*/




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
		lex.nom_type=p1->suiv->val.nom_type;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		nb=0;
		if(p1->suiv->suiv->suiv->val.nom_type==9) {
			nb = strtol(p1->suiv->suiv->suiv->val.valeur,NULL, 16);
			lex.nom_type=9;
		}
		else if (p1->suiv->suiv->suiv->val.nom_type==8){
			nb = strtol(p1->suiv->suiv->suiv->val.valeur,NULL, 10);
			lex.nom_type=8;
		}
		else {
		}
		nb=nb & 0xFFFF;
		sprintf(lex.valeur, "%d", nb);
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

		strcpy(lex.valeur,"LW");
		lex.nom_type=7;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);


		/* chargement du LUI*/
		nb=0;
		if(p1->suiv->suiv->suiv->val.nom_type==9) {
			nb = strtol(p1->suiv->suiv->suiv->val.valeur,NULL, 16);
			lex.nom_type=9;
		}
		else if (p1->suiv->suiv->suiv->val.nom_type==8){
			nb = strtol(p1->suiv->suiv->suiv->val.valeur,NULL, 10);
			lex.nom_type=8;
		}
		else {
		}
		nb=nb & 0x3FF0000;
		sprintf(lex.valeur, "%d", nb);
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

		/* chargement du LW*/
		strcpy(lex.valeur,"1");
		lex.nom_type=13;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		strcpy(lex.valeur,",");
		lex.nom_type=2;
		lex.numero_ligne=p1->val.numero_ligne;
		l2=ajout_tete(lex,l2);

		nb=0;
		if(p1->suiv->suiv->suiv->val.nom_type==9) {
			nb = strtol(p1->suiv->suiv->suiv->val.valeur,NULL, 16);
			lex.nom_type=9;
		}
		else if (p1->suiv->suiv->suiv->val.nom_type==8){
			nb = strtol(p1->suiv->suiv->suiv->val.valeur,NULL, 10);
			lex.nom_type=8;
		}
		else {
		}
		nb=nb & 0xFFFF;
		sprintf(lex.valeur, "%d", nb);
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


		/* chargement du LUI*/
		nb=0;
		if(p1->suiv->suiv->suiv->val.nom_type==9) {
			nb = strtol(p1->suiv->suiv->suiv->val.valeur,NULL, 16);
			lex.nom_type=9;
		}
		else if (p1->suiv->suiv->suiv->val.nom_type==8){
			nb = strtol(p1->suiv->suiv->suiv->val.valeur,NULL, 10);
			lex.nom_type=8;
		}
		else {
		}
		nb=nb & 0x3FF0000;
		sprintf(lex.valeur, "%d", nb);
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
	
	/*int longueur_dico=7;*/
	/*L_PSEUDO_INSTRUCTION* dico*/
	/*dico=lecture_dictionnaire_pseudo(longueur_dico);*/
	
	while(p !=NULL){
		if(p->val.nom_type==7){
			pseudo=recherche_element_pseudo(p->val.valeur , dico, longueur_dico);
			if(pseudo != NULL && (strcmp(p->val.valeur, "LW")==0 || strcmp(p->val.valeur, "SW")==0) ){ /* trouve rmeilleure solution avec les deux dico*/
				liste_lexemes=remplacement_pseudo_instruction(liste_lexemes, p->val.numero_lexeme , p->val.valeur);
			}
			else if(pseudo != NULL){
				liste_lexemes=remplacement_pseudo_instruction(liste_lexemes, p->val.numero_lexeme , p->val.valeur);
			}
		}
		
		p=p->suiv;
	}
	
	return liste_lexemes;
}

/*
int main(){
int a=0;

	printf("///////// %d\n",a |0X0F);

	L_LEXEME l;


	l=creer_liste();


	LEXEME lex7;
	lex7.nom_type=7;
	strcpy(lex7.valeur, "HDJDJD");
	lex7.numero_ligne=45;
	lex7.numero_lexeme=1;


	LEXEME lex0;
	lex0.nom_type=2;
	strcpy(lex0.valeur, "BLT");
	lex0.numero_ligne=45;
	lex0.numero_lexeme=2;

	LEXEME lex1;
	lex1.nom_type=5;
	strcpy(lex1.valeur, "VAL1");
	lex1.numero_ligne=45;
	lex1.numero_lexeme=3;

	LEXEME lex2;
	lex2.nom_type=2;
	strcpy(lex2.valeur, ",");
	lex2.numero_ligne=45;
	lex2.numero_lexeme=4;

	LEXEME lex3;
	lex3.nom_type=8;
	strcpy(lex3.valeur, "500");
	lex3.numero_ligne=45;
	lex3.numero_lexeme=5;

	LEXEME lex4;
	lex4.nom_type=2;
	strcpy(lex4.valeur, ",");
	lex4.numero_ligne=45;
	lex4.numero_lexeme=6;

	LEXEME lex5;
	lex5.nom_type=8;
	strcpy(lex5.valeur, "VAL3");
	lex5.numero_ligne=45;
	lex5.numero_lexeme=7;

	LEXEME lex6;
	lex6.nom_type=8;
	strcpy(lex6.valeur, "FIN");
	lex6.numero_ligne=45;
	lex6.numero_lexeme=8;

	l=ajout_tete(lex6, l);
	l=ajout_tete(lex5, l);
	l=ajout_tete(lex4, l);
	l=ajout_tete(lex3, l);
	l=ajout_tete(lex2, l);
	l=ajout_tete(lex1, l);
	l=ajout_tete(lex0, l);
	l=ajout_tete(lex7, l);


	

	
	printf("------------------------------------------------\n");


	printf("------------------------------------------------\n");
	
	l=remplacement_pseudo_instruction(l, 2 , "BLT");
	
	
	lecture_liste_lexeme(l);
	


	return 1;
}

*/
