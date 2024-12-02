#include "assembler.h"

/**
 * @brief Main function of the assembler program.
 * 
 * This function initializes the assembler table for each file passed as a command-line argument.
 * It then processes each file through the preprocessor phase. Memory for the assembler table is 
 * dynamically allocated for each file.
 * 
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments, where each element represents a file name.
 * @return 0 on successful completion of the program.
 */
int main(int argc , char ** argv){
   int i;
   ASSEMBLER_TABLE * assembler_table = NULL;

   /* Iterate over each command line argument (file name)*/
   for(i = 1 ; i < argc  ; i++){
        /* Allocate memory for the assembler table */
        assembler_table = generic_malloc(sizeof(ASSEMBLER_TABLE));
         /* Initialize the heads of various assembler lists to NULL */
        assembler_table->macro_head = NULL;
        assembler_table->label_head = NULL;
        assembler_table->entry_head = NULL;
        assembler_table->extern_head = NULL;
        assembler_table->command_head = NULL;
        assembler_table->instruction_head = NULL;
   
        /* Start the preprocessor phase for the current file */
        Pre_Proc( &assembler_table , argv[i]);
        
        /* Free the assembler table memory after processing */
        free_assembler_table(&assembler_table);
        
    }
    return 0;

}/*End of main*/

