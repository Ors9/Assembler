; file ps.as 
macr m_macr           
cmp @r3, #-6+
bne END
endmacr
.entry LIST 
.extern fn1 
MAIN: add r3, LIST 
jsr fn1 
LOOP: prn #48 
 lea STR, r6 
 m_macr
 inc r6 
 mov *r6, L3 
 sub r1, r4 
 cmp r3, #-6 
 bne END 
 add r7, *r6 
 clr K 
 sub L3, L3 
.entry MAIN 
 jmp LOOP 
END: stop 
STR: .string "abcd" x
.string 
.string "" 
.string "ab"cd" 
.string abcd";
.string "abcd;
.string abcd
LIST: .data 6, -9 ,XASCASMC:SA ; externus text after data
.data ,-100 ,5
.data 
.data ,-100.0 ,6.5 ,,x,6+,x,-5,+6,99999999,-999999999
.data ,-100.0 ,6.5 ,x,6+,x,-5,+6,99999999,-999999999
.data 99999999,-999999999
K: .data 31, 
.extern L3
.entry fn1 ; already define as entry 
.extern LIST ; already define as entry
.extern
.entry cmp
.entry fpowejfopewjfopewjfpiewfjweripfjewpofjewopfjewpofewjofpewjfow
string
data
add
cmp



m_macr
