macr m_macr           
cmp r3, #-6
bne END
endmacr


MAIN: addr3 , LIST         
LOOP: prn #48          ;reagvaregv

macr macr ;invalid macro name
endmacr

macr endmacr ;invalid macro name
endmacr

macr add  ;invalid macro name
dasd
endmacr                  
cmp r3, #-6
bne END

macr m_macr  ;NAME ALREADY USE     
cmp r3, #-6
bne END
endmacr


macr 1m_macr    ;invalid macro name       
endmacr X ;externus text after endmacr
macr m_m^acr    ;invalid macro name       
endmacr
macr     ;missing macro name       
endmacr
macr    AFMKFODAMIOASDFNIODAFNIOADFNIODAFNIADOFNOIADNOIFIOESANDFOIE ;too longmacro name       
endmacr

lea STR , r6;fwerfrewg
inc r6
mov *r6 , K
sub r1 , r4

m_macr


dec K
jmp LOOP
END : stop
STR: . string "abcd"
LIST: . data 6 , -9
.data -100
K: .data 31
