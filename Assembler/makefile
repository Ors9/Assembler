# Target: assembler
assembler: assembler.o pre_processor.o first_phase.o second_phase.o translation_unit.o pre_proc_error_handle.o fp_error_handle.o sp_error_handle.o general_functions.o
	gcc -Wall -ansi -pedantic assembler.o pre_processor.o first_phase.o second_phase.o translation_unit.o pre_proc_error_handle.o fp_error_handle.o sp_error_handle.o general_functions.o -o assembler 

# Compile assembler.c into assembler.o
assembler.o: assembler.c assembler.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o

# Compile pre_processor.c into pre_processor.o
pre_processor.o: pre_processor.c assembler.h pre_processor.h
	gcc -c -Wall -ansi -pedantic pre_processor.c -o pre_processor.o

# Compile first_phase.c into first_phase.o
first_phase.o: first_phase.c assembler.h first_phase.h
	gcc -c -Wall -ansi -pedantic first_phase.c -o first_phase.o

# Compile second_phase.c into second_phase.o
second_phase.o: second_phase.c assembler.h second_phase.h
	gcc -c -Wall -ansi -pedantic second_phase.c -o second_phase.o

# Compile translation_unit.c into translation_unit.o
translation_unit.o: translation_unit.c assembler.h translation_unit.h
	gcc -c -Wall -ansi -pedantic translation_unit.c -o translation_unit.o

# Compile pre_proc_error_handle.c into pre_proc_error_handle.o
pre_proc_error_handle.o: pre_proc_error_handle.c assembler.h
	gcc -c -Wall -ansi -pedantic pre_proc_error_handle.c -o pre_proc_error_handle.o

# Compile fp_error_handle.c into fp_error_handle.o
fp_error_handle.o: fp_error_handle.c assembler.h
	gcc -c -Wall -ansi -pedantic fp_error_handle.c -o fp_error_handle.o

# Compile sp_error_handle.c into sp_error_handle.o
sp_error_handle.o: sp_error_handle.c assembler.h
	gcc -c -Wall -ansi -pedantic sp_error_handle.c -o sp_error_handle.o

# Compile general_functions.c into general_functions.o
general_functions.o: general_functions.c assembler.h
	gcc -c -Wall -ansi -pedantic general_functions.c -o general_functions.o

# Target: pre_processor
pre_processor: pre_processor.o
	gcc -Wall -ansi -pedantic pre_processor.o -o pre_processor

# Execute assembler
exec: assembler
	./assembler
