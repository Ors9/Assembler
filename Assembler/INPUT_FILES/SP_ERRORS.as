MAIN: addr3 , LIST         
 prn #48          ; hiiii  
macr m_macr           
cmp r3, #-6
bne END
endmacr
lea STR , r6;
inc r6
mov *r6 , K1
sub OR , r4
m_macr
dec K
jmp BLABLA
END : stop
STR: . string "abcd"
LIST: . data 6 , -9
.data -100
K: .data 31

