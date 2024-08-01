#include "assembler.h"
#include "pre_processor.h"
#include "first_phase.h"

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




