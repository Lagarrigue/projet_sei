
/**
 * @file elf.c
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Sep 21 19:39:37 2017
 * @brief ELF stuff.
 *
 * ELF stuffs.
 */

#include <pelf/pelf.h>

#include <stdio.h>
#include <string.h>

int bss_size(L_BSS* pl_bss){
	L_BSS p  ;
	int i=0 ;
	for (p= *pl_bss ; p!=NULL ; p=p->suiv ) {
		if ( strcmp(".space",p->val.directive)==0 ) {
			i += p->val.operande.val.nb ;
		}
	}
	return i ;
}


Elf32_Sym* charge_elf32_sym (SYMB* tab_symb, int symb_size, SECTION strtab, SECTION shstrtab ) {
	int i ; 
	char section[6] ;
	Elf32_Sym* syms = calloc(symb_size, sizeof(Elf32_Sym)) ;
	if (syms == NULL) { exit(FAILURE) ;}
	
	for (i=0 ; i<symb_size ; i++) {
		syms[i].st_name = elf_get_string_offset( strtab->start, strtab->sz, tab_symb[i].symbole );
    		syms[i].st_size = 0 ; 
    		syms[i].st_other = 0 ;
   		syms[i].st_value = tab_symb[i].decalage ;
   		syms[i].st_info = ELF32_ST_INFO( STB_LOCAL, STT_NOTYPE ); /* JE SAIS PAS !!!!!!!!!!! */
   		if ( tab_symb[i].defined == FALSE ){
    			syms[i].st_shndx = 0 ;
    		}
    		else {
    			if ( tab_symb[i].section == 1 ) {
    				strcpy(section,".text");
    			}
    			else if ( tab_symb[i].section == 2 ) {
    				strcpy(section,".bss");
    			}
    			else if ( tab_symb[i].section == 3 ) {
    				strcpy(section,".data");
    			}
    			syms[i].st_shndx = elf_get_string_index( shstrtab->start, shstrtab->sz, section ) ;
    		}
	}
	return syms ;
	
}


Elf32_Rel* charge_elf32_rel(RELOC* rel, int size, char section[6],SECTION symtab, SECTION shstrtab, SECTION strtab ) {
	int i ;
	Elf32_Rel* reloc = calloc(size, sizeof(Elf32_Rel)) ;
	if (rel == NULL) { exit(FAILURE) ;}
	for (i=0 ; i<size ; i ++) {
		if (rel[i].type == R_MIPS_LO16 ) {
    			reloc[i].r_info = ELF32_R_INFO(elf_get_sym_index_from_name(symtab, shstrtab, strtab,section),R_MIPS_LO16);
    			reloc[i].r_offset = rel[i].ad_rel ;
    		}
    		else if (rel[i].type == R_MIPS_HI16 ) {
    			reloc[i].r_info = ELF32_R_INFO(elf_get_sym_index_from_name(symtab, shstrtab, strtab,section),R_MIPS_HI16);
    			reloc[i].r_offset = rel[i].ad_rel ;
    		}
    		else if (rel[i].type == R_MIPS_32 ) {
    			reloc[i].r_info = ELF32_R_INFO(elf_get_sym_index_from_name(symtab, shstrtab, strtab,section),R_MIPS_32);
    			reloc[i].r_offset = rel[i].ad_rel ;
    		}
    		else if (rel[i].type == R_MIPS_26 ) {
    			reloc[i].r_info = ELF32_R_INFO(elf_get_sym_index_from_name(symtab, shstrtab, strtab,section),R_MIPS_26);
    			reloc[i].r_offset = rel[i].ad_rel ;
    		}
    			
    	}
    	return reloc ;   		
}


int elf_get_last_local_symbol_index(unsigned char* symtab, int size) {
    int i=0;
    int found = -1;
    Elf32_Sym* symboles = (Elf32_Sym*) symtab;
    for (i=0; i < size/sizeof(Elf32_Sym); i++) {
        if (ELF32_ST_BIND(symboles[i].st_info) == STB_LOCAL) {
            found++;
        }
        else if (found) {
            break;
        }
    }
    return found;
}


int elf_get_string_offset( unsigned char *table, size_t size, char *string ) {
    unsigned char *str = 1+table;

restart :

    if ( !strcmp( (char *)str, string ) ) {
        return str-table;
    }
    else {
        unsigned int   len = strlen( (char *)str );

        if ( str > table+size-1 ) return -1;

        str += 1+len;

        goto restart;
    }
}

int elf_get_string_index( unsigned char *table, size_t size, char *string ) {
    unsigned char *str = 1+table;
    unsigned int   idx = 1; /* ELF first section is reserved. */

restart :

    if ( !strcmp( (char *)str, string ) ) {
        return idx;
    }
    else {
        unsigned int   len = strlen( (char *)str );

        if ( str > table+size-1 ) return -1;

        str += 1+len;
        idx++;

        goto restart;
    }
}

char *elf_next_string( unsigned char *table, size_t length, char **str ) {

    if ( !*str ) {
        *str = table+1;
        return *str;
    }

    if ( *str - (char*)table == length ) return NULL;

    *str += 1+strlen( *str );

    return *str;
}

#define catch_arch_name( arch, value )		\
  if ( !strcasecmp( arch, machinename ) ) return value;

int elf_get_machine( char *machinename ) {
    catch_arch_name( "none",       EM_NONE );
    catch_arch_name( "WE3200",     EM_M32 );
    catch_arch_name( "SPARC",      EM_SPARC );
    catch_arch_name( "386",        EM_386 );
    catch_arch_name( "i386",       EM_386 );
    catch_arch_name( "68000",      EM_68K );
    catch_arch_name( "m68k",       EM_68K );
    catch_arch_name( "88000",      EM_88K );
    catch_arch_name( "m88k",       EM_88K );
    catch_arch_name( "iMCU",       EM_IAMCU );
    catch_arch_name( "MCU",        EM_IAMCU );
    catch_arch_name( "80680",      EM_860 );
    catch_arch_name( "i860",       EM_860 );
    catch_arch_name( "mips",       EM_MIPS );
    catch_arch_name( "mips32",     EM_MIPS );
    catch_arch_name( "r3000",      EM_MIPS );
    catch_arch_name( "r3k",        EM_MIPS );
    catch_arch_name( "s370",       EM_S370 );
    catch_arch_name( "S/370",      EM_S370 );
    catch_arch_name( "mipsle",     EM_MIPS_RS3_LE );
    catch_arch_name( "mips32le",   EM_MIPS_RS3_LE );
    catch_arch_name( "r3000le",    EM_MIPS_RS3_LE );
    catch_arch_name( "r3kle",      EM_MIPS_RS3_LE );
    catch_arch_name( "PowerPC",    EM_PPC );
    catch_arch_name( "ppc",        EM_PPC );
    catch_arch_name( "PowerPC64",  EM_PPC64 );
    catch_arch_name( "ppc64",      EM_PPC64 );
    catch_arch_name( "arm",        EM_ARM );
    catch_arch_name( "arm32",      EM_ARM );
    catch_arch_name( "Alpha",      EM_ALPHA );
    catch_arch_name( "SPARCV9",    EM_SPARCV9 );
    catch_arch_name( "IA64",       EM_IA_64 );
    catch_arch_name( "MIPSX",      EM_MIPS_X );
    catch_arch_name( "coldfire",   EM_COLDFIRE );
    catch_arch_name( "68hc12",     EM_68HC12 );
    catch_arch_name( "x86-64",     EM_X86_64 );
    catch_arch_name( "amd64",      EM_X86_64 );
    catch_arch_name( "pdp10",      EM_PDP10 );
    catch_arch_name( "pdp11",      EM_PDP11 );
    catch_arch_name( "68hc11",     EM_68HC11 );
    catch_arch_name( "VAX",        EM_VAX );
    catch_arch_name( "arm64",      EM_AARCH64 );
    catch_arch_name( "microblaze", EM_MICROBLAZE );
    catch_arch_name( "cuda",       EM_CUDA );
    catch_arch_name( "amdgpu",     EM_AMDGPU );


    fprintf( stderr, "ELF :: Machine name %s not handled.\n", machinename );
    return -1;
}

int elf_get_machine_class( unsigned int machine ) {
    if ( EM_MIPS == machine ) return ELFCLASS32;

    fprintf( stderr, "ELF :: Machine type #%u not handled.\n", machine );
    return -1;
}

int elf_get_machine_endianness( unsigned int machine ) {
    if ( EM_MIPS == machine ) return ELFDATA2MSB;

    fprintf( stderr, "ELF :: Machine type #%u not handled.\n", machine );
    return -1;
}

int elf_get_machine_ehsize( unsigned int machine ) {
    int ret = elf_get_machine_class( machine );

    if ( -1 != ret ) {
        return ELFCLASS32 == ret ? sizeof( Elf32_Ehdr ) : sizeof( Elf64_Ehdr );
    }

    return -1;
}

int elf_get_machine_phentsize( unsigned int machine ) {
    int ret = elf_get_machine_class( machine );

    if ( -1 != ret ) {
        return ELFCLASS32 == ret ? sizeof( Elf32_Phdr ) : sizeof( Elf64_Phdr );
    }

    return -1;
}

int elf_get_machine_shentsize( unsigned int machine ) {
    int ret = elf_get_machine_class( machine );

    if ( -1 != ret ) {
        return ELFCLASS32 == ret ? sizeof( Elf32_Shdr ) : sizeof( Elf64_Shdr );
    }

    return -1;
}

int   elf_reloc_num( unsigned int machine, char *rname ) {

    switch( machine ) {
    case EM_MIPS :
        return elf_mips_reloc_num( rname );
    default :
        fprintf( stderr, "Unknown machine.\n" );
    }

    return -1;
}


