# TEST_RETURN_CODE = PASS
# allons au ru
.set noreorder

.text
    LW $t0 , lunchtime
    LW $6, 200
    ADDI $1,$0,8
boucle:
    BEQ $0 , $1 , byebye
    NOP
    ADDI $1 , $1 , 1
    J boucle 
    NOP
byebye:
      ADDI $1 , $1 , 1
      NOP
   

.data
lunchtime: 
    .word 12
    .word menu
    .asciiz "ils disent : \"au ru!\""
    
.bss 
menu:
    .space 24


