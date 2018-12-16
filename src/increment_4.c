#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "masquage.h"






/* fonction des listes */
int est_vide_code(L_CODE_32 L){
	return !L;
	}

L_CODE_32 creer_liste_code(void){
	return NULL ;
	}

L_CODE_32 ajout_tete_code(CODE_32 code, L_CODE_32 L){
	L_CODE_32 p=calloc(1,sizeof(*p));
	if (p==NULL){
		return NULL;
		}
	else{
		p->val=code;
		p->suiv=L;
		return p;
	}
}

L_CODE_32 supprimer_tete_code(L_CODE_32 L){
	if(!est_vide_code(L)){
		L_CODE_32 p;
		p=L->suiv;
		free(L);
		return p;
	}

	else return NULL;
}

void liberer_liste_code(L_CODE_32 L){
	while(est_vide_code(L) != 0){
		L=supprimer_tete_code(L);
	}
}

/* fonctions de codage en binaire */

L_CODE_32 retournement_de_liste(L_CODE_32 L1){
	L_CODE_32 p;
	p=L1;
	L_CODE_32 L2;
	L2=creer_liste_code();
	
	while(p!=NULL){
		L2=ajout_tete_code(p->val,L2);
		p=p->suiv;
	}
	return L2;

}


unsigned int conversion_LittleEndian_vers_Big_Indian(unsigned int code)
{

   /* code = ((code >> 24)& 0x000000FF) | ((code << 24)& 0xFF000000) | ((code >> 8)& 0x0000FF00) | ((code << 8)& 0x00FF0000);*/

    return code;
}

/*fonction qui s'occupe de remplir chaque mot de 32 bit de la mémoire*/
/* attention dans l'ajout des listes il faudra peut être faire un renversement (little indian/ big indian)*/
L_CODE_32 operation_de_masquage_section_text(L_INSTRUCTION* dicti, int longueur_table, L_CODE_32 liste, TEXT instruction_TEXT)
{

    INSTRUCTION* model_instruction;
    char nom_instruction[10];
    strcpy(nom_instruction, instruction_TEXT.instruction);
    model_instruction=recherche_element(nom_instruction, dicti, longueur_table);
    char type_encodage[10];
    strcpy(type_encodage, (*model_instruction).type_instruction);
    int nombre_operandes;
    nombre_operandes=(*model_instruction).nb_op;
    char type_instruction[10];
    unsigned int opcode=0;
    unsigned int code=0;
    unsigned int instruction=0;
    int i=0;
    CODE_32 mon_code;

    if(strcmp(type_encodage, "R")==0)
    {
        code=0;
        opcode=(*model_instruction).opcode;
        code = code | (opcode & 0x0000003F);
        
        for(i=0; i<nombre_operandes; i++)
        {
            strcpy(type_instruction, (*model_instruction).ordre_op[i]);
            instruction=valeur_operande(instruction_TEXT.t_operande, i, 0);
     
            if(strcmp(type_instruction, "rt")==0)
            {
                code = code | ((instruction<<16)& 0x001F0000);
            }

            else if(strcmp(type_instruction, "rs")==0)
            {
                code = code | ((instruction<<21)& 0x03E00000);
            }

            else if(strcmp(type_instruction, "rd")==0)
            {
                code = code | ((instruction<<11)& 0x0000F800);
            }

            else if(strcmp(type_instruction, "sa")==0)
            {
                code = code | ((instruction<<6)& 0x000007C0);
            }

        }
        code=conversion_LittleEndian_vers_Big_Indian(code);
        mon_code.donnee=code;
        liste=ajout_tete_code(mon_code, liste);
    }

    else if(strcmp(type_encodage, "I")==0)
    {
        code=0;
        opcode=(*model_instruction).opcode;
        code = code | ((opcode <<26) & 0xFC000000);
           
        for(i=0; i<nombre_operandes; i++)
        {
		
            strcpy(type_instruction, (*model_instruction).ordre_op[i]);
            
            if (instruction_TEXT.t_operande[i].type==4){
            
            	instruction=0;
            	/*remplacer adresse etiquette*/
            }
            
            else {
            
            	instruction=valeur_operande(instruction_TEXT.t_operande, i, 0);
            	}
			
            if(strcmp(type_instruction, "rs")==0)
            {
                code = code | ((instruction<<21)& 0x03E00000);
            }

            else if(strcmp(type_instruction, "rt")==0)
            {
                code = code | ((instruction<<16)& 0x001F0000);
            }

            else if(strcmp(type_instruction, "imm")==0)
            {
                code = code | (instruction & 0x0000FFFF);
            }
            
            else if(strcmp(type_instruction, "offset")==0)
            {
                code = code | (instruction & 0x0000FFFF);
            }
            
             else if(strcmp(type_instruction, "boff")==0)
            {
                
                instruction=valeur_operande(instruction_TEXT.t_operande, i, 1);; /* on met l'offset dans le champ immediate*/
                code = code | (instruction & 0x0000FFFF);
                instruction=valeur_operande(instruction_TEXT.t_operande, i, 2);; /* on met le registre dans rs*/
                code = code | ((instruction<<21)& 0x03E00000);
            }
        }
        code=conversion_LittleEndian_vers_Big_Indian(code);
        mon_code.donnee=code;
        liste=ajout_tete_code(mon_code, liste);

    }

    else if(strcmp(type_encodage, "J")==0)
    {
        code=0;
        opcode=(*model_instruction).opcode;
        code = code | ((opcode <<26) & 0xFC000000);        
        strcpy(type_instruction, (*model_instruction).ordre_op[0]);
        instruction=valeur_operande(instruction_TEXT.t_operande, 0, 0);
        
        if(strcmp(type_instruction, "target")==0) {

        	code = code | (instruction & 0x0000FFFF);
        }

        code=conversion_LittleEndian_vers_Big_Indian(code);
        mon_code.donnee=code;
        liste=ajout_tete_code(mon_code, liste);
    }

    return liste;

}

/*fonction qui se charge d'aller chercher la valeur des opérandes dans les union*/
int valeur_operande(OPERANDE op[3], int num, int cas_boff){
	
	/*proobleme dans inst j operande imm doit etre multiple de 4*/
	
		if(cas_boff==1){
			return op[num].val.base_offset.offset;
		}
		
		else if(cas_boff==2){
			return op[num].val.base_offset.reg;
		}
		
		else if(op[num].type==1 && cas_boff==0){
			return op[num].val.reg;
		}
		
		else if(op[num].type==2 && cas_boff==0){
			return op[num].val.imm;
		}
		
		else if(op[num].type==3 && cas_boff==0){
			return op[num].val.sa;
		}
	
		else if(op[num].type==4 && cas_boff==0){
			return 0;
		}
		
		else if(op[num].type==5 && cas_boff==0){
			return op[num].val.nb;
		}
		
		else if(op[num].type==7 && cas_boff==0){
			return op[num].val.imm;
		}
		
		else if(op[num].type==9 && cas_boff==0){
			return op[num].val.tar;
		}
		else {
			return 0 ;
		}
}

/*parcours la section tect et appel la fonction de remplissage de la mémoire à chaque instruction*/
L_CODE_32 parcours_section_text(L_TEXT section, L_INSTRUCTION* dicti, int longueur_table){
	L_TEXT p;
	p=section;
	L_CODE_32 liste_binaire;
    liste_binaire=creer_liste_code();
	
	while(p != NULL){
		liste_binaire=operation_de_masquage_section_text(dicti, longueur_table, liste_binaire,p->val );
		p=p->suiv;
	}
	return liste_binaire;
}


/*fonction qui se charge de coder en binaire l'ensemble des instructions de la section data*/
L_CODE_32 operation_de_masquage_section_data(L_DATA section)
{
	section=retournement_de_liste_L_DATA(section);
	L_DATA p;
	p=section;
	L_CODE_32 liste_binaire;
	liste_binaire=creer_liste_code();
	unsigned int code=0;
	unsigned int i=0;
	unsigned int valeur=0;
	CODE_32 mon_code;
	int j=0;
	int k=0;
	
	while(p != NULL){
		
		if(strcmp(p->val.directive, ".byte")==0){
			
			if(p->val.operande.type==4){
			
				valeur=0;
				/*charger valeur de l'etiquette*/
			}
			else {
				valeur=p->val.operande.val.nb;
				}
			
			
			
			if(i%4==0){
				code=0;
				code=code|(valeur & 0x000000FF);
			}
			else if((i+3)%4==0){
				code=code | ((valeur<<8) & 0x0000FF00);
			}
			else if((i+2)%4==0){
				code=code | ((valeur<<16) & 0x00FF0000);
			}
			else if((i+1)%4==0){
				code=code | ((valeur<<24) & 0xFF000000);
				
				code=conversion_LittleEndian_vers_Big_Indian(code);
				mon_code.donnee=code;
				liste_binaire=ajout_tete_code(mon_code, liste_binaire);
				
			}
			i++;
		}
		
		else if(strcmp(p->val.directive,".word")==0){
		
			if(i%4!=0){
				code=conversion_LittleEndian_vers_Big_Indian(code);
				mon_code.donnee=code;
				liste_binaire=ajout_tete_code(mon_code, liste_binaire);
			}
			
			code=0;
			if(p->val.operande.type==4){
				code=0;
				/*charger valeur de l'etiquette*/
			}
			else{
				code=p->val.operande.val.nb;
			}
			code=conversion_LittleEndian_vers_Big_Indian(code);
			mon_code.donnee=code;
			liste_binaire=ajout_tete_code(mon_code, liste_binaire);
			i=0;
		}
		
		else if(strcmp(p->val.directive, ".space")==0){
		
			for(j=0; j<p->val.operande.val.nb; j++){
			
				if(i%4==0){
					code=0;
				}
				else if((i+3)%4==0){
				
				}
				else if((i+2)%4==0){
				
				}
				else if((i+1)%4==0){
					code=conversion_LittleEndian_vers_Big_Indian(code);
					mon_code.donnee=code;
					liste_binaire=ajout_tete_code(mon_code, liste_binaire);
				}
				i++;
			}
			
		}
		
		else if(strcmp(p->val.directive, ".asciiz")==0){
		
			while(p->val.operande.val.etiq.nom[k] != '\0'){
					valeur=p->val.operande.val.etiq.nom[k];
					if(i%4==0){
						code=0;
						code=code|(valeur & 0x000000FF);
						
					}
					else if((i+3)%4==0){
						code=code | ((valeur<<8) & 0x0000FF00);
						
					}
					else if((i+2)%4==0){
						code=code | ((valeur<<16) & 0x00FF0000);
						
					}
					else if((i+1)%4==0){
						
						code=code | ((valeur<<24) & 0xFF000000);
						code=conversion_LittleEndian_vers_Big_Indian(code);
						mon_code.donnee=code;
						liste_binaire=ajout_tete_code(mon_code, liste_binaire);
				
					}
					
					i++;
					k++;
					
			}
			
			/* on s'occupe du \0 qui vaut NULL, on laisse donc un octet à 0*/
			if(i%4==0){
				code=0;
			}
			else if((i+3)%4==0){
				
			}
			else if((i+2)%4==0){
				
			}
			else if((i+1)%4==0){
				code=conversion_LittleEndian_vers_Big_Indian(code);
				mon_code.donnee=code;
				liste_binaire=ajout_tete_code(mon_code, liste_binaire);
			}
			i++;
	
		}
		p=p->suiv;
	}
	
	if(i%4 !=0){
		code=conversion_LittleEndian_vers_Big_Indian(code);
		mon_code.donnee=code;
		liste_binaire=ajout_tete_code(mon_code, liste_binaire);
	}
	/*gestion des etiquettes*/


	liste_binaire=retournement_de_liste(liste_binaire);
	return liste_binaire; 
}

/* faire un tableau qui regroupe les donnees binaire*/
void affichage_liste_binaire(L_CODE_32 liste){
	L_CODE_32 p;
	p=liste;
	printf("*********************************\n");
	while(p != NULL){
		printf("le code est %u\n", p->val.donnee);
		p=p->suiv;
	}
	printf("********************************\n");

}

int taille_liste_binaire(L_CODE_32 liste){
	L_CODE_32 p;
	p=liste;
	int i=0;
	while(p != NULL){
		i++;
		p=p->suiv;
	}
	
	return i;

}


int* mise_en_memoire_listes_binaire(L_CODE_32 liste_binaire,int* taille){
	*taille=taille_liste_binaire(liste_binaire);
	
	int* t=NULL;
	t=calloc(*taille, sizeof(*t));
	int i=0;
	
	L_CODE_32 p;
	p=liste_binaire;
	
	for(i=0;i<*taille;i++){
		
		t[i]=p->val.donnee;
		p=p->suiv;

	
	}
	return t ;
}
