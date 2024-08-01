; file ps.as 
.entry LIST 
.extern fn1 
MAIN: add r3, LIST 
MAIN: add r3, endmacr ;invalid name and label already define
.data 5, 6, 7 ;data in command section
.string "abc";data in command section
: ;invalid label name
add r3, 1LIST@
add r3, macr
add r3, cmp
add @r3, LIST 
add @r3, 
add ,@r3, ,LIST,
add @r3, LIST csacas   
add
jsr fn1 
jsr @fn1
jsr
LOOP: prn #48
prn #4899999
prn #R48
prn 
prn #hi

 lea STR, r6 cdscds
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
 cmp
 add
 1
 XASCASMC
 stop XASCASMC
 rtsx
END: stop 

STR: .string "abcd" 
LIST: .data 6, -9 
 .data -100 
K: .data 31 
.extern L3
STR: .string "abcd" ; label already define
cmp
 add
 1
 XASCASMC
