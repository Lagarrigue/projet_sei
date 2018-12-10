#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "masquage.h"



/* definition des types */


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

    code = ((code >> 24)& 0x000000FF) | ((code << 24)& 0xFF000000) | ((code >> 8)& 0x0000FF00) | ((code << 8)& 0x00FF0000);

    return code;
}


/* attention dans l'ajout des listes il faudra peut être faire un renversement (little indian/ big indian)*/
L_CODE_32 operation_de_masquage_section_text(L_INSTRUCTION* dicti, int longueur_table, L_CODE_32 liste, TEXT instruction_TEXT)
{

    INSTRUCTION* model_instruction;
    char nom_instruction[10];
    strcpy(nom_instruction, instruction_TEXT.instruction);
	
    model_instruction=recherche_element(nom_instruction, dicti, longueur_table);
    char type_encodage[10];
	
    strcpy(type_encodage, (*model_instruction).type_instruction);
    /*strcpy(type_encodage,"R");*/
	
	
    int nombre_operandes;
    nombre_operandes=(*model_instruction).nb_op;
	
    char type_instruction[10];
    
	

    unsigned int opcode=0;
    unsigned int code=0;
    unsigned int instruction=3;
    
    int i=0;
	
    CODE_32 mon_code;
   

    if(strcmp(type_encodage, "R")==0)
    {

        code=0;
        
		/*opcode à changer*/
		/*à trouver et à attribuerà l'aide du dico d'instructions (ajouter l'opcode ?) puis attribuer la valeurs*/
        opcode=(*model_instruction).opcode;
        code = code | (opcode & 0x0000003F);
        
        for(i=0; i<nombre_operandes; i++)
        {
            
            /*type_instruction à changer en utilisant le dico d'ins*/
            strcpy(type_instruction, (*model_instruction).ordre_op[i]);
            /*printf("%s \n", (*model_instruction).ordre_op[i]);*/

            /*valeur de l'instruction à changer (trouver dans la liste) .data ou les autres*/
            instruction=valeur_operande(instruction_TEXT.t_operande, i);
     
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
        /*code=conversion_LittleEndian_vers_Big_Indian(code);*/
        mon_code.donnee=code;
        liste=ajout_tete_code(mon_code, liste);
    }

    else if(strcmp(type_encodage, "I")==0)
    {
        code=0;
   
		/*opcode à changer*/
		/*à trouver et à attribuerà l'aide du dico d'instructions (ajouter l'opcode ?) puis attribuer la valeurs*/
        opcode=(*model_instruction).opcode;
        
        code = code | ((opcode <<26) & 0xFC000000);
        
        
        
        for(i=0; i<nombre_operandes; i++)
        {
			/*type_instruction à changer en utilisant le dico d'ins*/
            strcpy(type_instruction, (*model_instruction).ordre_op[i]);

            /*valeur de l'instruction à changer (trouver dans la liste) .data ou les autres*/
            instruction=valeur_operande(instruction_TEXT.t_operande, i);
			
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
                
                instruction=3; /* on met l'offset dans le champ immediate*/
                code = code | (instruction & 0x0000FFFF);
                
                instruction=7; /* on met le registre dans rs*/
                
                code = code | ((instruction<<21)& 0x03E00000);
            }
        }
        /*code=conversion_LittleEndian_vers_Big_Indian(code);*/
        mon_code.donnee=code;
        liste=ajout_tete_code(mon_code, liste);

    }

    else if(strcmp(type_encodage, "J")==0)
    {
        code=0;
   
		/*opcode à changer*/
		/*à trouver et à attribuerà l'aide du dico d'instructions (ajouter l'opcode ?) puis attribuer la valeurs*/
        opcode=(*model_instruction).opcode;
        code = code | ((opcode <<26) & 0xFC000000);
        
        /*type_instruction à changer en utilisant le dico d'ins*/
        strcpy(type_instruction, (*model_instruction).ordre_op[0]);
        
        /*valeur de l'instruction à changer (trouver dans la liste) .data ou les autres*/
        instruction=valeur_operande(instruction_TEXT.t_operande, 0);
        
        if(strcmp(type_instruction, "target")==0) {
        	code = code | (instruction & 0x0000FFFF);
        }

        code=conversion_LittleEndian_vers_Big_Indian(code);
        mon_code.donnee=code;
        liste=ajout_tete_code(mon_code, liste);
    }

    return liste;/* il faut renverser la liste !! avant de la retourner avec la foonction*/

}



/* normalement la liste .data à été décallée */
int operation_de_masquage_section_data(L_INSTRUCTION* dicti, int longueur_table)
{
	/*char type_primitive[10];*/ /* .byte . asciiz .word .space*/
	unsigned donnee=0;
	unsigned code;
	int i=0;
	code=0;
	CODE_32 mon_code;
    	L_CODE_32 liste;
    	liste=creer_liste_code();
	unsigned int nb_mots_32=0;
	
	
	for(i=0; i<nb_mots_32; i++){
		donnee=10;/* on va chercher la donne du mot n°i dans .data*/
		code=0;/* pour remettre tous les bits à 0*/
		code = code | (donnee & 0xFFFFFFFF);/* mascage par sécuriré*/
		code=conversion_LittleEndian_vers_Big_Indian(code);
        mon_code.donnee=code;
        liste=ajout_tete_code(mon_code, liste);
	}
	
	/*retourne la liste renversée*/

	
}

int valeur_operande(OPERANDE op[3], int num){
	
	/*proobleme dans inst j operande imm au lieu de tar dans op 3 au lieu de 1)*/
	
		
		
		if(op[num].type==1){
			printf("operande %d\n", op[num].val.reg);
			return op[num].val.reg;
		}
		
		else if(op[num].type==2){
		
			printf("operande %d\n", op[num].val.imm);
			return op[num].val.imm;
		}
		
		else if(op[num].type==3){
			printf("operande %d\n", op[num].val.sa);
			return op[num].val.sa;
		}
		
		else if(op[num].type==5){
			printf("operande %d\n", op[num].val.nb);
			return op[num].val.nb;
		}
		
		else if(op[num].type==6){
		
			printf("operande %d\n", op[num].val.tar);
			return op[num].val.tar;
		}
		
}


int parcours_section_text(L_TEXT section, L_INSTRUCTION* dicti, int longueur_table){
	L_TEXT p;
	p=section;
	L_CODE_32 liste_binaire;
    	liste_binaire=creer_liste_code();
	
	/*proobleme dans inst j operande imm au lieu de tar dans op 3 au lieu de 1)*/
	while(p != NULL){
		printf("instruction : %s\n", p->val.instruction);
		liste_binaire=operation_de_masquage_section_text(dicti, longueur_table, liste_binaire,p->val );
		/*valeur_operande(p->val.t_operande, 2);*/
		printf("binaire : %d\n", liste_binaire->val.donnee);
		
		p=p->suiv;
	}
	
	/*attention renversement de la liste binaire*/

}

/*il manque le fait d'aller chercher les donnees dans les deux section, il faut retourner les listes également*/
/*teste le cas des base offset */
/*probleme avec deux instructions pour les opcodes*/

/*
int main()
{
    printf("Hello world!\n");


    printf("la valeur sera %d\n",operation_de_masquage());
    return 0;
}

*/
