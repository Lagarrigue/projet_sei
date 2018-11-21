
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

#include "analyse_grammaticale.h"
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
		return 1 ;
		}
	else {
		for (i=0 ; i<3 ; i++ ) {
			dec[i]=calloc(1,sizeof(int));
			if (dec[i] == NULL) {
				puts("Erreur d'allocation") ;
				return 1 ;
			}
		}
	}
	int a=0 ;
	L_TEXT l_text = creer_liste_L_TEXT() ;
	L_BSS l_bss = creer_liste_L_BSS() ;
	L_DATA l_data = creer_liste_L_DATA() ;
	L_SYMB l_symb = creer_liste_L_SYMB() ;;
	L_SYMB l_attente = creer_liste_L_SYMB() ;
	L_INSTRUCTION* dictionnaire = lecture_dictionnaire(15) ;


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
        puts("Analyse lexicale en cours ...") ;
	l_lexeme=analyse_lexicale(file) ;
	puts("Analyse lexicale teerminée.") ;
	while (a == 0 ) {
		puts("Afficher la liste des lexemes ?  OUI (1)  NON (2)") ;
		scanf("%d",&a) ;
		if (a == 1) { lecture_liste_lexeme(l_lexeme)  ; } 
	}
    
    /* ---------------- do the gramatical analysis ------------------*/
    
    a=0 ;
    init (l_lexeme, section, dec, &l_text, &l_bss, &l_data, &l_symb, &l_attente,dictionnaire) ;
    lecture_liste_L_BSS(l_bss) ;
    lecture_liste_L_DATA(l_data) ;
    lecture_liste_L_TEXT(l_text) ;
    size=longueur_l_symb(l_symb) ;
    tab=creer_tab_symb(size);
    conversion_liste_symb_vers_tableau(tab,l_symb,size) ;
    lecture_tab_symb(tab, size) ; 
    /* 
    while (a == 0 ) {
		puts("Afficher les listes des text, bss, data et symbole ?  OUI (1)  NON (2)") ;
		scanf("%d",&a) ;
		if (a == 1) { 
			lecture_liste_L_BSS(l_bss) ;
    			lecture_liste_L_DATA(l_data) ;
			lecture_liste_L_SYMB(l_symb) ;
			lecture_liste_L_TEXT(l_text) ;
    			lecture_tab_symb(tab, size) ; 
		} 
    }
    */
    /* ---------------- Free memory and terminate -------------------*/

    /* TODO free everything properly*/
    exit( EXIT_SUCCESS );
}
