MAIN: addr3 , LIST         
 prn #48          ;noteeee
macr m_macr           
cmp r3, #-6
bne END
endmacr
lea STR , r6;
inc r6
jmp UNDEFINDLABEL          ;label does not exist
mov *r6 , K1                ;label does not exist
sub XDLABEL , r4           ;label does not exist
m_macr
dec K
jmp LIST
END : stop
STR: . string "abcd"
LIST: . data 6 , -9
.data -100
K: .data 31
