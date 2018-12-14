
/**
 * @file main.c
 * @author François Portet <francois.portet@imag.fr> from François Cayre
 * @brief Main entry point for MIPS assembler.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#include <global.h>
#include <notify.h>
#include <lex.h>


#include "masquage.h"


/**
 * @param exec Name of executable.
 * @return Nothing.
 * @brief Print usage.
 *
 */
void print_usage( char *exec ) {
    fprintf(stderr, "Usage: %s file.s\n",
            exec);
}



/**
 * @param argc Number of arguments on the command line.
 * @param argv Value of arguments on the command line.
 * @return Whether this was a success or not.
 * @brief Main entry point for MIPS assembler.
 *
 */
int main ( int argc, char *argv[] ) {
	system("clear");

    char         	 *file 	= NULL;
    
    /* Initialisation des variables pour l'analyse lexicale */
	L_LEXEME l_lexeme ;
	l_lexeme = creer_liste() ;
    /* Initialisation des variables pour l'analyse grammaticale */
	int section=0 ;
	int** dec ; 
	int i ;
	int size ;
	SYMB* tab ;
	
	if ( (dec = calloc(3,sizeof(int*)) ) == NULL ) {
		puts("Erreur d'allocation") ;
		exit( EXIT_FAILURE );
		}
	else {
		for (i=0 ; i<3 ; i++ ) {
			dec[i]=calloc(1,sizeof(int));
			if (dec[i] == NULL) {
				puts("Erreur d'allocation") ;
				exit( EXIT_FAILURE );
			}
		}
	}
	int a=0 ;
	int num=0 ;
	char nom[512] ;
	char* p_nom = nom ;
	L_TEXT l_text = creer_liste_L_TEXT() ;
	L_BSS l_bss = creer_liste_L_BSS() ;
	L_DATA l_data = creer_liste_L_DATA() ;
	L_SYMB l_symb = creer_liste_L_SYMB() ;
	L_SYMB l_attente = creer_liste_L_SYMB() ;
	L_INSTRUCTION* dictionnaire = lecture_dictionnaire(15) ;
	L_RELOC l_rel_text = creer_liste_L_RELOC() ; 
	L_RELOC l_rel_data = creer_liste_L_RELOC() ;
	RELOC* rel_text ;
	RELOC* rel_data ;
	int size_rel_text ;
	int size_rel_data ;
	
    if ( argc <2 ) {
        print_usage(argv[0]);
        exit( EXIT_FAILURE );
    }


    file  	= argv[argc-1];


    if ( NULL == file ) {
        fprintf( stderr, "Missing ASM source file, aborting.\n" );
        exit( EXIT_FAILURE );
    }



    /* ---------------- do the lexical analysis -------------------*/
	
	l_lexeme=analyse_lexicale(file) ;
	l_lexeme=ajuster_numero_lexeme(l_lexeme);
	
	L_PSEUDO_INSTRUCTION* dico_pseudo;
    	int longueur_dico_pseudo=7;
    	dico_pseudo=lecture_dictionnaire_pseudo(longueur_dico_pseudo);
        l_lexeme=verification_appartenance_pseudo_instruction(l_lexeme, dico_pseudo, longueur_dico_pseudo);
	
    
    /* ---------------- do the gramatical analysis ------------------*/
    	
    	a=0 ;
    	num=1 ;
    	while ( num != 0 ) {
    		num=0 ;
    		init (&l_lexeme, section, dec, &l_text, &l_bss, &l_data, &l_symb, &l_attente,dictionnaire,&num,p_nom) ;
    		if (num != 0) {
			liberer_liste_L_TEXT(l_text) ;
			liberer_liste_L_DATA(l_data) ;
			liberer_liste_L_BSS(l_bss)  ;
			liberer_liste_L_SYMB(l_symb) ;
			liberer_liste_L_SYMB(l_attente) ; 
			l_text = creer_liste_L_TEXT() ;
			l_bss = creer_liste_L_BSS() ;
			l_data = creer_liste_L_DATA() ;
			l_symb = creer_liste_L_SYMB() ;
			l_attente = creer_liste_L_SYMB() ;
			
		}
    		l_lexeme = remplacement_pseudo_instruction(l_lexeme, num, p_nom) ; 
    	}
    	size=longueur_l_symb(l_symb);
    	tab=creer_tab_symb(size);
    	conversion_liste_symb_vers_tableau(tab,l_symb,size) ;
    	int size_reloc=size*size ;
    	if ( (rel_text=calloc(size_reloc, sizeof(rel_text))) == NULL ) {
    		printf("Erreur d'allocation");
		exit( EXIT_FAILURE );
	}
	if ( (rel_data=calloc(size_reloc, sizeof(rel_data))) == NULL ) {
		printf("Erreur d'allocation");
		exit( EXIT_FAILURE );
	}
	
    	relocation(tab, size, &l_text, &l_data, &l_rel_text, &l_rel_data) ;
    	rel_text = liste_to_tab(l_rel_text, &size_rel_text);
    	rel_data = liste_to_tab(l_rel_data, &size_rel_data);
    	while (a == 0 ) {
		puts("Afficher la liste des lexemes ?  OUI (1)  NON (2)") ;
		scanf("%d",&a) ;
		if (a == 1) { 
			puts("");
			lecture_liste_lexeme(l_lexeme)  ; } 
	}
	a=0 ;
	puts("");
    	while (a == 0 ) {
		puts("Afficher les sections ?  OUI (1)  NON (2)") ;
		scanf("%d",&a) ;
		if (a == 1) { 
    			lecture_liste_L_BSS(l_bss) ;
    			lecture_liste_L_DATA(l_data) ;
    			lecture_liste_L_TEXT(l_text) ;
    			lecture_tab_symb(tab, size) ; 
    		}
    	}
    	a=0;
    	while (a == 0 ) {
		puts("Afficher les reloc ?  OUI (1)  NON (2)") ;
		scanf("%d",&a) ;
		if (a == 1) { 
			puts(" RELOCATION DANS .TEXT");
			lecture_tab_reloc(rel_text,size_rel_text) ;
    			puts("\n RELOCATION DANS .DATA");
    			lecture_tab_reloc(rel_data,size_rel_data) ;
    		}
   	 }
   	 
	puts("Generation du binaire");
	L_CODE_32 l_bin_text;
	l_bin_text=parcours_section_text(l_text, dictionnaire, 15);
	
	L_CODE_32 l_bin_data;
	l_bin_data=operation_de_masquage_section_data(l_data);
	
	mise_en_memoire_listes_binaire(l_bin_text);
	mise_en_memoire_listes_binaire(l_bin_data);
	
	/*int elf = elf ( int text_prog[] , int text_size , int data_prog[] , int data_size,char * sym_char[], int sym_size,char* name,L_BSS* pl_bss, SYMB* tab_symb, int symb_size,RELOC* rel_text,int rel_text_size, RELOC* rel_data, int rel_data_size  )*/
	
	puts("generation binaire reussie");

    /* ---------------- Free memory and terminate -------------------*/

    /* TODO free everything properly*/
    
    
    
    
    exit( EXIT_SUCCESS );
}
