/**
 * @file section.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Wed Sep 20 11:15:28 2017
 * @author François Portet
 * @date Nov 20, 2018
 * @brief Example file to generate ELF object file.
 *
 */

#include <stdlib.h>
#include <string.h>

#include <pelf/pelf.h>



/**
* Looks for a specific name in strtab and symtab and return the index of the symbol corresponding to the name
*
* @param symtab the symbole table
* @param strtab the string table
* @param shstrtab the section string table
* @param sym_name the name for which a symbol is searched for
*
* @return the index of the symbol in symtab or -1 if not found
*/
int elf_get_sym_index_from_name(SECTION symtab, SECTION shstrtab, SECTION strtab, char* sym_name) {
    int i=0;
    Elf32_Sym* symboles = (Elf32_Sym*) symtab->start;
    int section_offset = elf_get_string_index( shstrtab->start, shstrtab->sz, sym_name );
    int offset = elf_get_string_offset( strtab->start, strtab->sz, sym_name );

    if (offset > 0 || section_offset > 0 ) {
        for (i=0; i < (symtab->sz)/sizeof(Elf32_Sym); i++) {
            if (ELF32_ST_TYPE(symboles[i].st_info) == STT_SECTION) {
                if (symboles[i].st_shndx == section_offset) {
                    return i;
                }
            }
            else if (symboles[i].st_name == offset)
                return i;
        }
    }
    return -1;

}


#define add_string_to_table( section, symbol ) /* Explicitly add terminal '\0' */ \
  write_section( section, (unsigned char *)symbol, 1+strlen(symbol), section->sz)
/**
* @brief Generates a predefined shstrtab section containing the basic sections names
* @return the completed shstrtab section
*/
SECTION make_shstrtab_section ( void ) {
    SECTION shstrtab = new_section( ".shstrtab", SECTION_CHUNK_SZ );
    add_string_to_table( shstrtab, "" ); /* ELF string tables start with a '0' */
    add_string_to_table( shstrtab, ".text" );
    add_string_to_table( shstrtab, ".rel.text" );
    add_string_to_table( shstrtab, ".data" );
    add_string_to_table( shstrtab, ".rel.data" );
    add_string_to_table( shstrtab, ".bss" );
    add_string_to_table( shstrtab, ".symtab" );
    add_string_to_table( shstrtab, ".strtab" );
    add_string_to_table( shstrtab, ".shstrtab" );

    return shstrtab;
}


/**
 * @brief Generates the strtab section from an array of char *
 * @param symbols the a sorted array or char *
 * @param nb_sym the number of symbol to include in the section
 *
 * @return the completed strtab section
 */

SECTION make_strtab_section( char* symbols[], int nb_sym) {
    SECTION strtab = new_section( ".strtab", SECTION_CHUNK_SZ );

    add_string_to_table( strtab, "" ); /* ELF string tables start with a '0' */
    int i= 0;
    for (i= 0; i< nb_sym; i++) {
        add_string_to_table( strtab,symbols[i] );
    }

    return strtab;
}


/**
 * @brief Generates the data section from an array of int already in big endian
 *
 * @param  data_prog array of binary value
 * @param  nb_data size of the array
 *
 * @return the completed section
 *
 * the function writes the whole data in one shot to the section.
 */
SECTION make_data_section( int data_prog[], int nb_data ) {
    SECTION        data = new_section( ".data", SECTION_CHUNK_SZ );

    if (nb_data>0) {
        write_section( data, (unsigned char *)(data_prog), sizeof(*data_prog)*nb_data, 0);
    }
    return data;
}

/**
 * @brief Generates the text section from an array of int already in big endian
 *
 * @param  instructions array of binary value
 * @param  nb_inst size of the array
 *
 * @return the completed section
 *
 * the function writes the whole data in one shot to the section.
 */
SECTION make_text_section( int instructions[], int nb_inst) {
    SECTION        text = new_section( ".text", SECTION_CHUNK_SZ );

    if (nb_inst>0) {
        write_section( text, (unsigned char *)(instructions), sizeof(*instructions)*nb_inst, 0);
    }
    return text;
}

/**
 * @brief Generates the bss section from the size of the section
 *
 * @param  bss_prog the size of the bss section in number of bytes
 *
 * @return the completed section
 *
 * Since bss is only composed of zero, only the size of the section is necessary in the elf file
 */
SECTION make_bss_section( int bss_prog) {
    SECTION bss = new_section( ".bss", SECTION_CHUNK_SZ );
    bss -> sz =bss_prog;
    return bss;
}


/**
 * @brief Generates the symtab section from an array of symbols
 *
 * @param  shstrtab the section of the string table of sections
 * @param  strtab the string table section
 * @param  symbols array of Elf32_Sym. The symbols are supposed to be already SORTED.
 * @param  nb_syms size of the array
 *
 *
 * @return the completed section
 *
 * shstrtab and strtab are necessary to bind the symbol names to the symbol entries.
 * Every symtab starts with an empty entry followed by the 3 standard sections :
 *     - text of type SECTION
 *     - data of type SECTION
 *     - bss of type SECTION
 *
 * These entries are present even if they are not in the original text code
 *
 */
SECTION make_symtab_section(SECTION shstrtab, SECTION strtab,  Elf32_Sym symbols[], int nb_syms  ) {

    SECTION    symtab = new_section( ".symtab", SECTION_CHUNK_SZ );
    int i;
    unsigned int offset = 0;

    Elf32_Sym   entry= {0};
    entry.st_name  = 0;
    entry.st_value = 0;
    entry.st_size  = 0;
    entry.st_info  = 0;
    entry.st_other = 0;
    entry.st_shndx = SHN_UNDEF;

    write_section( symtab, (unsigned char *)&entry, sizeof(entry), offset);


    entry.st_name  = 0;
    entry.st_value = 0;
    entry.st_size  = 0;
    entry.st_info  = ELF32_ST_INFO( STB_LOCAL, STT_SECTION );
    entry.st_other = 0;
    entry.st_shndx = elf_get_string_index( shstrtab->start, shstrtab->sz, ".text" );

    write_section( symtab, (unsigned char *)&entry, sizeof(entry), symtab->sz);

    entry.st_name  = 0;
    entry.st_value = 0;
    entry.st_size  = 0;
    entry.st_info  = ELF32_ST_INFO( STB_LOCAL, STT_SECTION );
    entry.st_other = 0;
    entry.st_shndx = elf_get_string_index( shstrtab->start, shstrtab->sz, ".data" );

    write_section( symtab, (unsigned char *)&entry, sizeof(entry), symtab->sz);

    entry.st_name  = 0;
    entry.st_value = 0;
    entry.st_size  = 0;
    entry.st_info  = ELF32_ST_INFO( STB_LOCAL, STT_SECTION );
    entry.st_other = 0;
    entry.st_shndx = elf_get_string_index( shstrtab->start, shstrtab->sz, ".bss" );

    write_section( symtab, (unsigned char *)&entry, sizeof(entry), symtab->sz);

    for (i =0; i< nb_syms ; i++) {
        write_section( symtab, (unsigned char *)&symbols[i], sizeof(symbols[i]), symtab->sz);
    }
    return symtab;
}


/**
 * @brief Generates a relocation section from an array of Elf32_Rel
 *
 * @param relname the name of the relocation section (e.g., rel.text, rel.data)
 * @param relocations relocation entries already ready to be writen and SORTED
 * @param nb_reloc size of the array
 *
 * @return the completed section
 *
 */
SECTION make_rel32_section(char *relname, Elf32_Rel relocations[], int nb_reloc) {

    SECTION reltab = new_section( relname, SECTION_CHUNK_SZ );
    int i ;

    for (i=0; i<nb_reloc; i++) {
        Elf32_Rel rel = relocations[i];
        write_section( reltab, (unsigned char *)&rel, sizeof( rel ), reltab->sz);

    }
    return reltab;
}


/* ************ FONCTION MAIN DE LA GENERATION ELF ************* */


void make_elf ( int * text_prog , int text_size , int *  data_prog , int data_size,char * sym_char[], int sym_size,char* name,L_BSS* pl_bss, SYMB* tab_symb, int symb_size,RELOC* rel_text,int rel_text_size, RELOC* rel_data, int rel_data_size  ) {

    /*
    ***** prepare sections ******
    */
    
    SECTION     text = NULL;
    SECTION     data = NULL;
    SECTION      bss = NULL;
    SECTION shstrtab = NULL;
    SECTION   strtab = NULL;
    SECTION   symtab = NULL;
    SECTION  reltext = NULL;
    SECTION  reldata = NULL;

    /* 
    ***** make predefined section table *****
    */

    shstrtab = make_shstrtab_section();

    /*
    ***** make hard coded program data already in big endian *****
    */   

    int bss_prog = bss_size( pl_bss) ; 
    char* machine = "mips";        

    
    /* 
    ***** pelf options *****
    */
    
    int noreorder =1;


    /* 
    ***** Create text, data and bss sections ***** 
    */

    text = make_text_section(text_prog, text_size);

    if ( !text ) {
        fprintf( stderr, "Unable to write .text section (missing information).\n" );
        exit(EXIT_FAILURE) ;
    }

    data = make_data_section(data_prog, data_size);
    if ( !data ) {
        fprintf( stderr, "Unable to write .data section (missing information).\n" );
       exit(EXIT_FAILURE) ;
    }

    bss = make_bss_section(bss_prog);
    if ( !bss ) {
        fprintf( stderr, "Unable to write .bss section (missing information).\n" );
        exit(EXIT_FAILURE) ;
    }

    /* 
    ***** Write all string linked to symbols *****
    */

    strtab   = make_strtab_section( sym_char, sym_size);


    /* 
    ***** make hard coded symbols data (needs shstrtab and strtab)
    ***** "boucle" is in strtab, has value (relative @) 4 and is defined in the text section
    ***** "tab" is in strtab, has value (relative @) 16 and is defined in the bss section
    *****
    ***** => Remplir le tableau de symbole avec nom, ad_rel, size=0 (car inutile), 
    *****    st_info ,st_other=0 (inutile), st_shndx=indice section du symb
    */

    Elf32_Sym* syms = charge_elf32_sym (tab_symb, symb_size, strtab, shstrtab) ; 
    symtab   = make_symtab_section( shstrtab, strtab, syms,symb_size);


    /* 
    ***** make hard coded text and data relocation entries (needs symtab and strtab)
    ***** first relocation "ADDI $2, $3, boucle" at adress 4 of the text section is a R_MIPS_LO16 with respect to the symbole "boucle" so 
    ***** the relocation is made with respect to the .text section symbol (since the value of boucle, 4, will be the addend)
    ***** second relocation ".word tab" at adress 0 of the data section is a R_MIPS_32 with respect to the symbole "tab" so the relocation is made with 
    ***** respect to the .bss section symbol (since the value of tab, 16, will be the addend)
    *****
    ***** => Remplir les tabs de reloc
    */

    Elf32_Rel* text_reloc = charge_elf32_rel(rel_text,rel_text_size, ".text", symtab, shstrtab, strtab) ;
    Elf32_Rel* data_reloc = charge_elf32_rel(rel_data,rel_data_size, ".data", symtab, shstrtab, strtab) ;
   
    reltext  = make_rel32_section( ".rel.text", text_reloc,rel_text_size);
    reldata  = make_rel32_section( ".rel.data", data_reloc,rel_data_size);


    /*
    ***** write these sections in file
    */
    
    elf_write_relocatable( name, machine, noreorder,
                           text->start, text->sz,
                           data->start, data->sz,
                           bss->start, bss->sz,
                           shstrtab->start, shstrtab->sz,
                           strtab->start, strtab->sz,
                           symtab->start, symtab->sz,
                           reltext->start, reltext->sz,
                           reldata->start, reldata->sz);

    print_section( text );
    print_section( data );
    print_section( bss );
    print_section( strtab );
    print_section( symtab );

    /*clean up */
    del_section(     text );
    del_section(     data );
    del_section(      bss );
    del_section( shstrtab );
    del_section(   strtab );
    del_section(   symtab );
    del_section(  reltext );
    del_section(  reldata );

}
