#include "assembler.h"
#include "pre_processor.h"
#include "first_phase.h"

/**
 * @brief Main function of the assembler program.
 *
 * This function handles the processing of assembly files provided as command-line arguments.
 * It performs the pre-processing phase and then proceeds to the first phase of assembly if pre-processing succeeds.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments, where each argument is a file name without an extension.
 * @return int Returns 0 on successful completion of all operations.
 */
int main(int argc , char ** argv){
   char * as_file , *am_file;
   int i;
   Macro_list  * macro_head = NULL;

    /* Iterate over each command line argument (file name)*/
   for(i = 1 ; i < argc ; i++){
        as_file = add_file(argv[i], ".as"); /*Add suffix to the name from the argument line*/
        am_file = add_file(argv[i], ".am");

        /* Start the preprocessor phase */
        if( preprocessor_phase(as_file , am_file , &macro_head) == SUCCESS){ 
            /* Proceed to the first phase if preprocessor succeeds */
            first_phase(am_file , argv[i] , macro_head);     
        }
    
        /* Free allocated memory and reset pointers */
        free(as_file);
        free(am_file);
        as_file = NULL;
        am_file = NULL;
        free_macro_list(&macro_head);  
        macro_head = NULL;
    }

    
    return 0;
}/*End of main*/




