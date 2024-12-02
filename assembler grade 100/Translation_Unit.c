#include "assembler.h"

/**
 * @brief Prints a binary code to a file in octal format.
 * 
 * This function takes a binary code and an address, converts the binary code to octal format,
 * and prints it to a specified file. The sign bit is removed if present before conversion.
 * 
 * @param binary_code The binary code to be converted and printed.
 * @param addr The address associated with the binary code.
 * @param fp_file Pointer to the file where the output will be printed.
 */
void print_dec_to_file_in_octal(unsigned short binary_code , int addr , FILE * fp_file){
    /* Allocate memory for the result string */
    char * result = generic_malloc(MAX_FILE_LINE_LENGTH);
    memset(result , '\0' , MAX_FILE_LINE_LENGTH);

    /* Remove the sign bit if present */
    binary_code = binary_code &  (~(1 << ((sizeof(unsigned short) * 8) - 1)));
    
    /* Format the result as an octal string */
    sprintf(result , "0%d\t%05o\n" , addr , binary_code );

    /* Write the formatted string to the output file */
    fprintf(fp_file , "%s" , result);

    /* Free the allocated memory */
    free(result);
}


/**
 * @brief Processes the object (.ob) file by writing the machine code and instructions in octal format.
 * 
 * This function processes the machine code instructions and commands, and writes them into the 
 * object file (.ob) in octal format. It calculates the instruction count (IC) and data count (DC) 
 * for the file header.
 * 
 * @param instruction_list Pointer to the list of machine code instructions.
 * @param command_list Pointer to the list of machine code commands.
 * @param ob_file Name of the object file to be written.
 * @param IC Instruction count.
 * @param DC Data count.
 */
void process_ob_file(MACHINE_CODE_INSTRUCTION *instruction_list , MACHINE_CODE_COMMAND * command_list ,char * ob_file, int IC , int DC){
    FILE * ob_ptr;

    /* Open the object file for writing */
    ob_ptr = fopen(ob_file , "w");
    /* Check if the file opened successfully */
    if(ob_ptr == NULL){
        print_error(FAILED_TO_OPEN_FILE , NO_LINE);
        exit(1);
    }
    /* Write the instruction count and data count to the file */
    fprintf(ob_ptr,"\n\t%d %d\n" , IC - 100 , DC - 1);
    
    /* Write the commands to the file in octal format */
    while(command_list != NULL){
        print_dec_to_file_in_octal(command_list->binary_code.mila , command_list->addr , ob_ptr);
        command_list = command_list->next;
    }
    /* Write the instructions to the file in octal format */
    while(instruction_list != NULL){
        print_dec_to_file_in_octal(instruction_list->mila.mila , instruction_list->addr + IC - 1  , ob_ptr);
        instruction_list = instruction_list->next;
    }
    /* Close the object file */
    fclose(ob_ptr);
    /* Free the memory for the object file name */
    free(ob_file);
}

/**
 * @brief Processes the entry (.ent) file by writing the list of entries and their addresses.
 * 
 * This function processes the entry list and writes each entry and its corresponding address 
 * into the entry (.ent) file. If the file is empty after processing, it is deleted.
 * 
 * @param ent_list Pointer to the list of entry labels and their addresses.
 * @param ent_file Name of the entry file to be written.
 */
void process_ent_file(ENTRY_LIST *ent_list ,char * ent_file){
    FILE * ent_ptr;
    ADDRESS_LIST * addr_ptr = NULL;
    int is_empty_file = TRUE;
    char ent_line[MAX_FILE_LINE_LENGTH];
    memset(ent_line , '\0',sizeof(ent_line));

    /* Open the entry file for writing */
    ent_ptr = fopen(ent_file , "w");

    /* Check if the file opened successfully */
    if(ent_ptr == NULL){
        print_error(FAILED_TO_OPEN_FILE , NO_LINE);
        exit(1);
    }

    /* Process each entry and its addresses */
    while(ent_list != NULL){
        addr_ptr = ent_list->addr_list;
        while(addr_ptr != NULL){
            /* Format the entry line and write it to the file */
            sprintf(ent_line ,"%s\t0%d\n", ent_list->label , addr_ptr->addr );
            fprintf(ent_ptr, "%s ", ent_line );
            is_empty_file = FALSE;

            memset(ent_line , '\0',sizeof(ent_line));
            
            addr_ptr = addr_ptr->next;
        }
        ent_list = ent_list->next;
    }

    fclose(ent_ptr);

    /* Remove the file if it's empty */
    if(is_empty_file == TRUE){
        if( remove(ent_file) != 0 ){  
            print_error(FAILED_TO_REMOVE_FILE , NO_LINE);
            exit(1);
        }
    }

    free(ent_file);

}

/**
 * @brief Processes the external (.ext) file by writing the list of external labels and their addresses.
 * 
 * This function processes the list of external labels and writes each label and its address into the 
 * external (.ext) file. If the file is empty after processing, it is deleted.
 * 
 * @param ext_list Pointer to the list of external labels and their addresses.
 * @param ext_file Name of the external file to be written.
 */
void process_ext_file(EXTERN_LIST *ext_list ,char * ext_file){
    FILE * ext_ptr;
    ADDRESS_LIST * addr_ptr = NULL;
    int is_empty_file = TRUE;
    char ext_line[MAX_FILE_LINE_LENGTH];
    memset(ext_line , '\0' , sizeof(ext_line));

    /* Open the external file for writing */
    ext_ptr = fopen(ext_file , "w");

    if(ext_ptr == NULL){
        print_error(FAILED_TO_OPEN_FILE , NO_LINE);
        exit(1);
    }
    
    /* Process each external label and its addresses */
    while(ext_list != NULL){
        addr_ptr = ext_list->addr_list;
        while(addr_ptr != NULL){
            /* Format the external line and write it to the file */
            sprintf(ext_line,"%s\t0%d\n", ext_list->label , addr_ptr->addr);
            fprintf(ext_ptr, "%s ", ext_line );
            is_empty_file = FALSE;
            
            memset(ext_line , '\0' , sizeof(ext_line));
            addr_ptr = addr_ptr->next;
        }
        ext_list = ext_list->next;
    }

    fclose(ext_ptr);

    /* Remove the file if it's empty */
    if(is_empty_file == TRUE){
        if( remove(ext_file) != 0 ){  
            print_error(FAILED_TO_REMOVE_FILE , NO_LINE);
            exit(1);
        }
    }

    free(ext_file);
}

/**
 * @brief Handles the translation process by calling functions to generate the object, entry, and external files.
 * 
 * This function calls the respective functions to process the object (.ob), entry (.ent), and external (.ext) files. 
 * It takes the assembler table, file name, instruction count (IC), and data count (DC) as input.
 * 
 * @param assembler Pointer to the assembler table structure.
 * @param file_name Name of the file being processed.
 * @param IC Instruction count.
 * @param DC Data count.
 */
void translationunit(ASSEMBLER_TABLE ** assembler , char *file_name , int IC , int DC){
    /* Process the object file (.ob) */
    process_ob_file( (*assembler)->instruction_head ,  (*assembler)->command_head , add_suffix(file_name , ".ob") , IC , DC);
    /* Process the entry file (.ent) */
    process_ent_file( (*assembler)->entry_head , add_suffix(file_name , ".ent") );
    /* Process the external file (.ext) */
    process_ext_file( (*assembler)->extern_head , add_suffix(file_name , ".ext") );
    
}