# TEST_RETURN_CODE = PASS
# allons au ru
.byte
.set noreorder
.text
    LW $t0 , lunchtime
    LW $6, -200($7)
    ADDI $t1,$zero,8
boucle:
    BEQ $t0 , $t1 , byebye
    NOP
    addi $t1 , $t1 , 1
    J boucle 
    NOP
byebye:
    JAL viteviteauru

.text
lunchtime: 
    .word 12
    .word menu
    .asciiz "ils disent : \"au ru!\""
.bss 
menu:
    .space 24

