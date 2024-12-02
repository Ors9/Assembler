macr m_macr       ;macro declaration        
cmp r3, #-6
bne END
endmacr            ;endmacr 
macr m_macr         ;name already in use    
cmp r3, #-6
bne END
endmacr             ;endmacr
MAIN: addr3 , LIST         
LOOP: prn #48  
macr macr          ;reserved macro name
endmacr            ;endmacr
macr endmacr       ;reserved macro name
endmacr            ;endmacr
macr add           ;reserved macro name
dasd
endmacr            ;endmacr               
cmp r3, #-6
bne END
macr 5m_macr       ;invalid macro name       
endmacr blabla     ;externus text after endmacr
macr m_m$a#cr      ;invalid macro name       
endmacr            ;endmacr 
macr               ;missing macro name       
endmacr
macr    NAAAAMEEEEEEEEEEEEEEE_TOOOOOOOOOOO_LOOOOOOOONGGGGGGGGGGGGGGGGGGG ;exceed max label size
endmacr            ;endmacr
lea STR , r6 
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
