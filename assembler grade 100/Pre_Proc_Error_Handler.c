#include "assembler.h" 

/**
 * @brief Checks if the "endmacr" line contains any extraneous text.
 * 
 * This function verifies if the line after "endmacr" is empty. If there is any 
 * additional text, it reports an error and returns FALSE. Otherwise, it returns TRUE.
 * 
 * @param line Pointer to the line following the "endmacr" keyword.
 * @param line_count The current line number in the source file.
 * @return BOOLEAN indicating whether the check was successful (TRUE) or not (FALSE).
 */
BOOLEAN End_Macr_Check(char * line , int line_count){
    BOOLEAN result = TRUE;
    /* Check if there is any text after "endmacr". If so, report an error */
    if(line[0] != '\n'){
        result = FALSE;   
        print_error(EXTRANOUS_TEXT_AFTER_END_OF_MACRO , line_count);
    }
    return result;
}


/**
 * @brief Checks the validity of a macro name.
 * 
 * This function validates a macro name by checking its length, ensuring it doesn't 
 * start with a digit, and confirming it contains only alphanumeric characters or underscores.
 * It also checks if the macro name is already defined or if it's a reserved word.
 * 
 * @param line Pointer to the macro name.
 * @param head Pointer to the head of the macro list to check for duplicate names.
 * @param line_count The current line number in the source file.
 * @return BOOLEAN indicating whether the macro name is valid (TRUE) or not (FALSE).
 */
BOOLEAN Macro_Name_Check(char * line ,MACRO_LIST * head , int line_count){
    BOOLEAN result = TRUE;
    int i = 0;

     /* Check if the macro name is missing (line starts with newline) */
    if(line[0] == '\n'){
        print_error(MISSING_MACRO_NAME , line_count);
        return FALSE;
    }
    /* Check if the macro name exceeds the maximum allowed length */
    if(strlen(line) >= MAX_LABEL_SIZE){
        print_error(EXCEEDED_MACRO_NAME_LENGTH , line_count);
        return FALSE;
    }
    /* Check if the macro name starts with a digit */
    if(isdigit(line[0])){
        print_error(INVALID_MACRO_NAME , line_count);
        result = FALSE;
    }
     /* Loop through the characters of the macro name to validate them */
    while(line[i] != '\n'){
        if( isupper(line[i]) == 0 && islower(line[i]) == 0 && isdigit(line[i]) == 0 && line[i] != '_'){
            print_error(INVALID_MACRO_NAME , line_count);
            result = FALSE;
        }
        i++;
    }
    
    /* Check if the macro name already exists in the macro list */
    while(head != NULL){
        if(strcmp(head->macro_name , line) == 0){
            print_error(MACRO_NAME_ALREADY_DEFINED ,line_count);
            return FALSE;
        }
        head = head->next;
    }
    /* Check if the macro name is a reserved word */
    result += Reserved_Words_Check(line, CANNOT_BE_RESERVED_WORD ,  line_count);

    return result;
}

/**
* @brief Check if file name is valid.
* @param name Name of the file
* @return TRUE if valid, FALSE otherwise.
 */
BOOLEAN File_Name_Check(char * name){
    if(strlen(name) >= MAX_LABEL_SIZE){
        print_error(FILE_LENGTH_EXCEED,NO_LINE);
        return FALSE;
    }
    return TRUE;
}