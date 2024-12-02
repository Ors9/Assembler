# Target: assembler
assembler: assembler.o Pre_Proc.o First_Passage_Errors.o First_Passage.o Functions.o Pre_Proc_Error_Handler.o Translation_Unit.o Second_Pass.o
	gcc -Wall -ansi -pedantic assembler.o Pre_Proc.o First_Passage_Errors.o First_Passage.o Functions.o Pre_Proc_Error_Handler.o Translation_Unit.o Second_Pass.o -o assembler

# Compile assembler.c into assembler.o
assembler.o: assembler.c assembler.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o

# Compile Pre_Proc.c into Pre_Proc.o
Pre_Proc.o: Pre_Proc.c assembler.h 
	gcc -c -Wall -ansi -pedantic Pre_Proc.c -o Pre_Proc.o

# Compile First_Passage_Errors.c into First_Passage_Errors.o
First_Passage_Errors.o: First_Passage_Errors.c assembler.h 
	gcc -c -Wall -ansi -pedantic First_Passage_Errors.c -o First_Passage_Errors.o

# Compile First_Passage.c into First_Passage.o
First_Passage.o: First_Passage.c assembler.h 
	gcc -c -Wall -ansi -pedantic First_Passage.c -o First_Passage.o

# Compile Functions.c into Functions.o
Functions.o: Functions.c assembler.h 
	gcc -c -Wall -ansi -pedantic Functions.c -o Functions.o

# Compile Pre_Proc_Error_Handler.c into Pre_Proc_Error_Handler.o
Pre_Proc_Error_Handler.o: Pre_Proc_Error_Handler.c assembler.h 
	gcc -c -Wall -ansi -pedantic Pre_Proc_Error_Handler.c -o Pre_Proc_Error_Handler.o

# Compile Translation_Unit.c into Translation_Unit.o
Translation_Unit.o: Translation_Unit.c assembler.h 
	gcc -c -Wall -ansi -pedantic Translation_Unit.c -o Translation_Unit.o

# Compile Second_Pass.c into Second_Pass.o
Second_Pass.o: Second_Pass.c assembler.h 
	gcc -c -Wall -ansi -pedantic Second_Pass.c -o Second_Pass.o
