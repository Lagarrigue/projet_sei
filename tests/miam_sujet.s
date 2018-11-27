
# TEST_RETURN_CODE = PASS
# allons au ru
.set noreorder
.text
    ADDI $1,$0,8
boucle:
    BEQ $0 , $1 , byebye
    addi $1 , $1 , 1
    J boucle 
    NOP
byebye:
    JAL viteviteauru

.data
lunchtime: 
    .word 12
    .word menu
    .asciiz "ils disent : \"au ru!\""
.bss 
menu:
    .space 24

