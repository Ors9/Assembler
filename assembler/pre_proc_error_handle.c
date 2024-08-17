#include "assembler.h"
#include "pre_processor.h"



/**
 * @brief Examines a macro name to ensure it meets all the required criteria.
 *
 * This function checks the macro name length, ensures it does not start with a digit,
 * and that it consists only of alphanumeric characters and underscores. It also checks
 * if the name conflicts with existing commands, registers, or macro names.
 *
 * @param macro_name_line A string containing the macro name to be checked.
 * @param line_count The current line number in the source file.
 * @param head A pointer to the head of the macro list.
 * @return int Returns SUCCESS (0) if the macro name is valid, otherwise FAIL (1).
 */
int macro_examine(char * macro_name_line , int line_count , Macro_list * head){
    int i = 0;
    int result = SUCCESS;

    /* Check if the macro name exceeds the maximum label size */
    if( strlen(macro_name_line) >= MAX_LABEL_SIZE){
        print_error_message(EXCEEDED_MACRO_NAME, line_count);
        result = FAIL; /* Macro name too long */
        return FAIL;
    }
    
    /* Check if the macro name is missing */
    if( macro_name_line[0] == '\n'){
        print_error_message(MISSING_MACRO_NAME, line_count);
        result = FAIL;/* Macro name is missing */
        return FAIL;
    }
    
    /* Check if the macro name starts with a digit */
    if( isdigit(macro_name_line[0])  ){
        print_error_message(INVALID_MACRO_NAME, line_count);
        result = FAIL;/* Macro name starts with a digit */
    }

    /* Check if the macro name contains only valid characters */
    while(macro_name_line[i] != '\n'){
        if( (isdigit(macro_name_line[i]) == 0) && (islower(macro_name_line[i]) == 0) && (isupper(macro_name_line[i]) == 0) && macro_name_line[i] != '_')   
        {
           print_error_message(INVALID_MACRO_NAME, line_count);
           result = FAIL; /* Invalid character in macro name */
        }
        i++;  
    }

    /*Check if the macro name doesnt have invalid name*/
    result += cmp_with_reserved_words(macro_name_line ,INVALID_MACRO_NAME , line_count);

    /* Check if the macro name already exists */
    result += check_if_macro_name_already_exit(head, macro_name_line, line_count);
    
    
    
    return result; /* Return the result of the macro name examination */
}



/**
 * @brief Checks for extraneous text after the end of a macro.
 *
 * This function checks if there is any text after the "endmacr" line in the source file.
 * If such text is found, it prints an error message.
 *
 * @param endmacr_line A string containing the line after "endmacr".
 * @param line_count The current line number in the source file.
 * @return int Returns SUCCESS (0) if there is no extraneous text, otherwise FAIL (1).
 */
int end_macro_examine(char * endmacr_line , int line_count){
    int result = SUCCESS;
    
    if(endmacr_line[0] != '\n'){  
        print_error_message(EXTRANOUS_TEXT_AFTER_END_OF_MACRO, line_count);
        result = FAIL; /* Extraneous text after end of macro */
    }
    return result;/* Return the result of the examination */
}


/**
 * @brief Checks if a macro name already exists in the list.
 *
 * This function iterates through the list of macros and checks if the provided
 * macro name already exists. If it does, it prints an error message.
 *
 * @param head A pointer to the head of the macro list.
 * @param macro_name A string containing the macro name to be checked.
 * @param line_count The current line number in the source file.
 * @return int Returns SUCCESS (0) if the macro name is unique, otherwise FAIL (1).
 */
int check_if_macro_name_already_exit(Macro_list * head , char * macro_name , int line_count){
    
    int result = SUCCESS;

    /* Iterate through the macro list to check for existing macro name */
    while( head != NULL){
        if( strcmp(head->macro_name, macro_name) == 0){
            print_error_message(MACRO_NAME_ALREADY_DEFINED, line_count);
            result = FAIL;/* Macro name already defined */
        }

        head = head->next_macro;
    }

    return result; /* Return the result of the check */
}
