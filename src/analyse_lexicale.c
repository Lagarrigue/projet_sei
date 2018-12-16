#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionnaire_registre.h"


/*
NL-->1
VIRGULE-->2
DEUX_PTS-->3
COMMENTAIRE-->4
REGISTRE-->5
DIRECTIVE-->6
SYMBOLE_ALPHA-->7
VALEUR_DEC-->8
VALEUR_HEX-->9
SYMBOL_ALPHA_NUM-->10
SIGNE-->11
GUILLEMETS-->12
PARENTHESE-->13
*/

void copie_tab_char(char a[], char b[], int taille){
    int i=0;
    for(i=0;i<taille;i++){
        b[i]=a[i];
    }
}

void initialisation_lexeme(LEXEME* l){
    int i=0;
    l->nom_type=0;
    l->reloc=0;
    l->numero_ligne=0;
    for(i=0;i<512;i++){
        l->valeur[i]='\0';
    }
}

void initialisation_tab_char(char tab[], int taille){
    int i=0;
    for(i=0;i<taille;i++){
        tab[i]='\0';
    }
}


void lecture_liste_lexeme(L_LEXEME L){
    L_LEXEME p;
    int i=0;
    int j=0;
    p=L;
    if(est_vide(L)==0)
    {
	while(!est_vide(p)){
        	j++;
       	 	printf("------------------------------\n");
        	printf("lexeme n° %d\n",j);
        	printf("type : %d\n",p->val.nom_type);
        	printf("reloc : %d\n",p->val.reloc);
        	printf("ligne : %d\n",p->val.numero_ligne);


		if(p->val.nom_type==1){printf("NL\n");}
		else if(p->val.nom_type==2){printf("VIRGULE\n");}
		else if(p->val.nom_type==3){printf("DEUX_PTS\n");}
		else if(p->val.nom_type==4){printf("COMMENTAIRE\n");}
		else if(p->val.nom_type==5){printf("REGISTRE\n");}
		else if(p->val.nom_type==6){printf("DIRECTIVE\n");}
		else if(p->val.nom_type==7){printf("SYMBOLE_ALPHA\n");}
		else if(p->val.nom_type==8){printf("SYMBOL_DEC\n");}
		else if(p->val.nom_type==9){printf("SYMBOLE_HEX\n");}
		else if(p->val.nom_type==10){printf("SYMBOL_ALPHA_NUM\n");}
		else if(p->val.nom_type==11){printf("SIGNE\n");}
		else if(p->val.nom_type==12){printf("GUILLEMETS\n");}
		else if(p->val.nom_type==13){printf("PARENTHESE\n");}
		
	
        	while(p->val.valeur[i] !='\0'){
            		printf("%c",p->val.valeur[i]);

            	i++;
        	}
        	p=p->suiv;
        	i=0;
		puts("") ;
	}
     }

     else 
     {
     printf("error liste vide\n");
     }
}


L_LEXEME signe (L_LEXEME liste ) {
	L_LEXEME p = liste ;
	char s[512] = "-" ;
	while (p!=NULL && (p->suiv !=NULL) ) {
		if (p->suiv->val.nom_type == 11) {
			if ( (p->suiv->suiv) != NULL) {
				if ( ((p->suiv->suiv->val).nom_type == 8) || ((p->suiv->suiv->val).nom_type == 9) ) {
					strcpy( (p->suiv->suiv->val).valeur, strcat(s,p->suiv->suiv->val.valeur) ) ;
					p->suiv=p->suiv->suiv ;
				}
				else {
					WARNING_MSG("[ligne %d] Le signe n'est pas suivi d'une valeur hexa ou décimale", p->val.numero_ligne);
					exit( EXIT_FAILURE );
				}
			}
			else {
				WARNING_MSG("[ligne %d] Le signe n'est pas suivi d'une valeur hexa ou décimale", p->val.numero_ligne);
				exit( EXIT_FAILURE );
			}
		}
		p=p->suiv ;
		strcpy(s,"-");	
	}
	return liste ;
}
		

/* ici commence la fonction principale qui permet de lire le fichier assembleur */

L_LEXEME analyse_lexicale(char* nom_fichier){
	L_LEXEME liste_lecture_instructions ;
	liste_lecture_instructions=creer_liste();
	FILE* fichier;
	fichier=fopen(nom_fichier,"r");
	if (fichier==NULL){
		perror("Erreur ouverture du fichier");
		exit( EXIT_FAILURE );
	}

	else
	{

        /* on déclare les variables dont on aura besoin par la suite */
        char ligne[512];
        LEXEME lexeme_ligne;
        lexeme_ligne.nom_type=0;
        char mot[512] ;

        initialisation_lexeme(&lexeme_ligne);
        initialisation_tab_char(mot,512);
        L_REGISTRE_reg * dico_registre;
        dico_registre=lecture_dictionnaire_reg(10);
        REGISTRE_reg* reg;
	/* i,j,k sont des compteur */
        int i=0;
        int j=0;
        int k=0;
        int test_guillemets=0;
        int verif_guillemets=0;
        int numero_ligne_programme=0;
        int nouvelle_ligne=0;
        

        enum {DEBUT, NL, SPACE_TAB, VIRGULE, DEUX_PTS, COMMENTAIRE, REGISTRE, DIRECTIVE, SYMBOL_ALPHA, VALEUR_DEC, VALEUR_HEX, SYMBOL_ALPHA_NUM, SIGNE, GUILLEMETS, PARENTHESE};
        int S=DEBUT;

        while(fgets(ligne,511,fichier) != NULL) /* on lit une nouvelle du fichier texte */
        {

            i=0;
            numero_ligne_programme++;
			nouvelle_ligne=0;
            while(nouvelle_ligne==0) /* on lit la ligne mise en mémoire temporaire caractère par caractère */
            {

                switch(S) /* on déclare l'automate permettant de gérer l'ensemble des lexemes */
                {

                case DEBUT: /* tous les differents caractères spéciaux rencontrés */
                    if ((ligne[i]==' ' )|( ligne[i]=='\t'))
                        S=SPACE_TAB;
                    else if (ligne[i]==',')
                        S=VIRGULE;
                    else if (ligne[i]==':')
                        S=DEUX_PTS;
                    else if (ligne[i]=='#')
                        S=COMMENTAIRE;
                    else if (ligne[i]=='$')
                        S=REGISTRE;/*completer seconde notation */
                    else if (ligne[i]=='.')
                        S=DIRECTIVE;
                    else if (isalpha(ligne[i]))
                        S=SYMBOL_ALPHA_NUM;
                    else if (isdigit(ligne[i]))
                        S=SYMBOL_ALPHA_NUM; /* a faire */
                    else if (isxdigit(ligne[i]))
                        S=SYMBOL_ALPHA_NUM; /*attention confusions dec hex et a faire*/
                    else if ((ligne[i]=='+') | (ligne[i]=='-'))
                        S=SIGNE;
                    else if (ligne[i]=='"')
                        S=GUILLEMETS;
                    else if (ligne[i]=='(')
                        S=PARENTHESE;
                    else if (ligne[i]=='\n')
                   	 	S=NL;
                    break;


                case NL:
                	nouvelle_ligne=1;
                	if (mot[0]!='\0'){
                		j=0;
                    	/*strcpy(lexeme_ligne.valeur,mot);*/
                   	 copie_tab_char(mot, lexeme_ligne.valeur,512);
                    	liste_lecture_instructions=ajout_tete(lexeme_ligne,liste_lecture_instructions);
                    	initialisation_lexeme(&lexeme_ligne);
                    	initialisation_tab_char(mot,512);
                    	}
                    	S=DEBUT;
                	break;

                case SPACE_TAB: /* on saute la tabulation ou l'espace */
                    i++;
                    S=DEBUT;
                    break;

                case VIRGULE:/*on saute la virgule*/
                    lexeme_ligne.nom_type=2;
                    lexeme_ligne.numero_ligne=numero_ligne_programme;
                    /*strcpy(lexeme_ligne.valeur, mot);*/
                    liste_lecture_instructions=ajout_tete(lexeme_ligne,liste_lecture_instructions);
                    /*initialisation_tab_char(liste_lecture_instructions->val.valeur,512);*/
                    S=DEBUT;
                    i++;
                    break;

                case DEUX_PTS: /*on a détecté une étiquete son nom étant déjà stocké dans la liste de lexeme sous le nom_type alpha_num, on a juste à changer le nom_type par celui correspondant à une étiquette*/
                    liste_lecture_instructions->val.nom_type=3;
                    lexeme_ligne.numero_ligne=numero_ligne_programme;
                    if (isdigit(liste_lecture_instructions->val.valeur[0])) /*on gère le cas particulier d'une nouvelle ligne*/
                    {
                        WARNING_MSG("erreur syntaxe nom_etiquette ligne n° %d\n", numero_ligne_programme);
                        exit( EXIT_FAILURE );
                    }
                    S=DEBUT;
                    i++;
                    break;

                case COMMENTAIRE: /*c'est un commentaire on l'ignore et on passe directement à la ligne suivante*/
                    fgets(ligne,511,fichier);
                    numero_ligne_programme++;
                    i=0;
                    S=DEBUT;
                    break;

                case REGISTRE:

                    if (lexeme_ligne.nom_type!=5)/*détection du #*/
                    {
                        lexeme_ligne.nom_type=5;
                        lexeme_ligne.numero_ligne=numero_ligne_programme;
                        i++;
                        S=REGISTRE;
                    }

                    else if (isdigit(ligne[i]) || isalpha(ligne[i])) /*on remplis le lexeme du nom /numero de registre*/
                    {
                        mot[j]=ligne[i];
                        j++;
                        i++;
                        S=REGISTRE;
                    }

                   /*on détecte la fin du nom de registre on stock donc dans la liste de lexemes*/
                      else if (ligne[i]==',' || ligne[i]==' ' || ligne[i]=='\n' || ligne[i]=='"' || ligne[i]=='#')
                    {


                            reg=recherche_element_reg(mot, dico_registre, 10);
                            if(reg != NULL){
                                strcpy(mot,(*reg).nom_chiffre);
                            }
                            else{
                                WARNING_MSG("erreur, registre inexistant a la ligne n° %d\n",numero_ligne_programme);
                                exit( EXIT_FAILURE );
                            }

                        j=0;
                        /*strcpy(lexeme_ligne.valeur, mot);*/
                        copie_tab_char(mot, lexeme_ligne.valeur,512);
                        liste_lecture_instructions=ajout_tete(lexeme_ligne,liste_lecture_instructions);
                        initialisation_lexeme(&lexeme_ligne);
                        initialisation_tab_char(mot,512);
                        S=DEBUT;
                    }
                    break;

                case DIRECTIVE:

                    if (lexeme_ligne.nom_type!=6) /*on détecte un point on est alors dans le cas d'une directive*/
                    {

                        lexeme_ligne.nom_type=6;
                        lexeme_ligne.numero_ligne=numero_ligne_programme;
                        i++;
                        S=DIRECTIVE;
                    }

                    else if (isalpha(ligne[i])) /*on sait que les directive leur nom est composé de caract alpha, on remplis le tableau intermédiaire mot par le nom de la directive*/
                    {
                        mot[j]=ligne[i];
                        j++;
                        i++;
                        S=DIRECTIVE;
                    }

                    /* else if (isalpha(ligne[i])==0) */ /*on détecte autre chose qu'un alpha le nom de la directive est terminé on ajoute le lexeme à la liste de lexemes*/
                   else if (ligne[i]==',' || ligne[i]==' ' || ligne[i]=='"' || ligne[i]=='\n' || ligne[i]=='#')
                    {
                        j=0;
                        /*strcpy(lexeme_ligne.valeur,mot);*/
                        copie_tab_char(mot, lexeme_ligne.valeur,512);
                        liste_lecture_instructions=ajout_tete(lexeme_ligne,liste_lecture_instructions);
                        initialisation_lexeme(&lexeme_ligne);
                        initialisation_tab_char(mot,512);
                        S=DEBUT;
                    }

                    break;

                case SYMBOL_ALPHA_NUM:  /*ici on ajoute dans un lexeme tous les caractère alpha_numériques puis on ajoute dans la liste de lexeme, le type (alpha, num, hexa, ou étiquette) sera redéfinit plus tard*/

                    if (isalpha(ligne[i]) || isdigit(ligne[i]) || ligne[i]=='_')/*le caractère est de type alpha ou digit on j'ajoute au tableau intermédiaire mot*/
                    {
                        mot[j]=ligne[i];
                        j++;
                        i++;
                        S=SYMBOL_ALPHA_NUM;
                    }

                    /*le caractère est différents de alpha ou digit, on ajoute alors un nouveau lexeme à la liste de lexeme*/
                    else if (ligne[i]==',' || ligne[i]==' ' || ligne[i]=='"' || ligne[i]==':' ||ligne[i]=='\n' || ligne[i]=='(' || ligne[i]=='#')
                    {
                        j=0;
                        lexeme_ligne.nom_type=10;
                        lexeme_ligne.numero_ligne=numero_ligne_programme;
                        /*strcpy(lexeme_ligne.valeur,mot);*/
                        copie_tab_char(mot, lexeme_ligne.valeur,512);
                        liste_lecture_instructions=ajout_tete(lexeme_ligne,liste_lecture_instructions);
                        initialisation_lexeme(&lexeme_ligne);
                        initialisation_tab_char(mot,512);
                        S=DEBUT;
                    }

                    break;

                case SIGNE: /*on crée un lexeme pour un éventuel signe qui serai détecté devant un nombre*/
                    lexeme_ligne.nom_type=11;
                    lexeme_ligne.numero_ligne=numero_ligne_programme;
                    mot[j]=ligne[i];
                    /*strcpy(lexeme_ligne.valeur,mot);*/
                    copie_tab_char(mot, lexeme_ligne.valeur,512);
                    liste_lecture_instructions=ajout_tete(lexeme_ligne,liste_lecture_instructions);
                    initialisation_lexeme(&lexeme_ligne);
                    initialisation_tab_char(mot,512);
                    i++;
                    j=0;
                    S=DEBUT;
                    break;

                case GUILLEMETS : /*gestion des guillemets*/
                    if ((ligne[i+1]=='"') & (ligne[i]=='\\')) /*on ajoute le caractere guillemet précédé d'un back slash dans mot, qui appartient à la chaine de caractère, en fait ce n'est pas le guillemet de fin de chaine*/

                    {
                        mot[j]='"';
                        j++;
                        i=i+2;/*on saute le back slash pour ne pas l'afficher dans la liste*/
                        S=GUILLEMETS;
                    }

                    else if (ligne[i] != '"')/*on détecte un caractère qui n'est pas un guillemet on ajoute donc le caractère au tableau intermédiaire mot*/
                    {
                        mot[j]=ligne[i];
                        j++;
                        i++;
                        S=GUILLEMETS;
                    }

                    else if ((ligne[i]=='"' )& (ligne[i-1]!='/'))/*on vérifie que le guilletmet détecté n'est pas précédé d'un anti slash sinon cela signifie que gle uillemet appartient à la chaine de caractère, ici on est donc en fin de chaine*/
                    {
                        verif_guillemets++;
                        if(test_guillemets==1)/*on s'assure qu'on a déjà détecté le guillemet de fermeture (variable test_guillemets) et on ajoute le lexeme dans la liste de lexemes*/
                        {
                            j=0;
                            i++;
                            lexeme_ligne.nom_type=12;
                            lexeme_ligne.numero_ligne=numero_ligne_programme;
                            /*strcpy(lexeme_ligne.valeur,mot);*/
                            copie_tab_char(mot, lexeme_ligne.valeur,512);

                            liste_lecture_instructions=ajout_tete(lexeme_ligne,liste_lecture_instructions);


                            initialisation_lexeme(&lexeme_ligne);
                            initialisation_tab_char(mot,512);
                            test_guillemets=0;
                            S=DEBUT;
                        }

                        else if (test_guillemets==0)/*dans ce cas c'est que l'on a détecté le guillemet d'ouverture on le signale par la variable est_guillemets*/
                        {
                            test_guillemets=1;
                            i++;
                            S=GUILLEMETS;
                        }
                    }

                    if((ligne[i]=='\n') & (verif_guillemets%2 !=0))/*par la fonction modulot on vérifie qu'on a un nombre paire de guillemets sinon on aurait oublié de fermer les guillemets*/
                    {

                        WARNING_MSG("erreur de syntaxe guillemets ligne %d\n", numero_ligne_programme);
                        exit( EXIT_FAILURE );
                        S=DEBUT;
                        verif_guillemets=0;
                    }

                    break;

                case PARENTHESE : /*gestion des parenthèses*/

                    if (lexeme_ligne.nom_type!=13) /*détection de la parenthèse on initialise un lexeme spéciale*/
                    {
                        lexeme_ligne.nom_type=13;
                        lexeme_ligne.numero_ligne=numero_ligne_programme;
                        i++;
                        S=PARENTHESE;
                    }

                    else if ((ligne[i] != '(') & (ligne[i] != ')')) /*on ajoute au lexem un caarctère à l'intérieur de la parenthèse*/
                    {
                        mot[j]=ligne[i];
                        j++;
                        i++;
                        S=PARENTHESE;
                    }

                    else if (ligne[i]==')') /*on détecte la fin de la parenthèse et on ajoute le lexeme à la liste de lexemes*/
                    {
						reg = recherche_element_reg(mot+1,dico_registre,10) ;
						if ( reg !=NULL) {
							strcpy(mot,(*reg).nom_chiffre);
							printf("%s\n",mot);
						}
						else {
							WARNING_MSG("Registre inexistant à la ligne %d",numero_ligne_programme) ;
							exit( EXIT_FAILURE );
						}	
                        j=0;
                        i++;
                        /*strcpy(lexeme_ligne.valeur,mot);*/
                        copie_tab_char(mot, lexeme_ligne.valeur,512);
                        liste_lecture_instructions=ajout_tete(lexeme_ligne,liste_lecture_instructions);
                        initialisation_lexeme(&lexeme_ligne);
                        initialisation_tab_char(mot,512);
                        test_guillemets=0;
                        S=DEBUT;
                    }
                    break;
                }

                if(est_vide(liste_lecture_instructions)==0) /*partie qui permet d'attribuer les type pour les lexemes alpha, digit et hexadécimal, on gère aussi des erreurs au cas ou on mélangerai des digits avec des alphas et inversement*/
                {
                    if ( liste_lecture_instructions->val.nom_type==10)/*lexeme alpha_num non définit*/
                    {
                        if(liste_lecture_instructions->val.valeur[0]=='0' && (liste_lecture_instructions->val.valeur[1]=='x'||liste_lecture_instructions->val.valeur[1]=='X'))/*détection du type hexadécimal*/
                        {
                            int l=2;
                            while(liste_lecture_instructions->val.valeur[l]!='\0'){
                                int hex=liste_lecture_instructions->val.valeur[l];
                                if( (hex<48 || hex>57) && (hex<65|| hex>70) && (hex<97 || hex>102) ){

                                    WARNING_MSG("erreur a la ligne n° %d, ce n'est pas une valeur hexadecimal\n",numero_ligne_programme);
                                    exit( EXIT_FAILURE );
                                }
                                l++;
                            }

                            liste_lecture_instructions->val.nom_type=9;
                            lexeme_ligne.numero_ligne=numero_ligne_programme;
                        }

                        else
                        {

							k=0;

							if(isdigit(liste_lecture_instructions->val.valeur[k]))/*gestion des erreurs et attribution type_lexemme pour caractère décimal*/
							{
								k=0;
								liste_lecture_instructions->val.nom_type=8;
								lexeme_ligne.numero_ligne=numero_ligne_programme;

								while(liste_lecture_instructions->val.valeur[k]!='\0')/*gestion de l'erreur*/
								{
									if(isalpha(liste_lecture_instructions->val.valeur[k]) && liste_lecture_instructions->val.valeur[0] != '0' )
									{
										WARNING_MSG("type 1 erreur de syntaxe ligne n° %d : mélange de symbole digit et alpha\n",numero_ligne_programme);
										exit( EXIT_FAILURE );
									}
									k++;
								}
							}

							else if(isalpha(liste_lecture_instructions->val.valeur[k]) && liste_lecture_instructions->val.valeur[i]!=':')/*gestion des erreurs et attribution type_lexemme pour caractère alpha*/
							{

								k=0;
								liste_lecture_instructions->val.nom_type=7;
								lexeme_ligne.numero_ligne=numero_ligne_programme;

							}

                        }
                    }
                }

            }
        }
    }
    fclose(fichier);
    liste_lecture_instructions=renversement_liste(liste_lecture_instructions);
    liste_lecture_instructions = signe (liste_lecture_instructions );
    return liste_lecture_instructions;
}


