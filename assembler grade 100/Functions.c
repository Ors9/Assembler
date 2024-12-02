#include "assembler.h"

/**
 * @brief Checks if a label is a reserved word.
 *
 * This function checks whether a given label is a reserved word in the assembler.
 * It checks against command names, register names, and assembler directives. If
 * the label is reserved, an error message is printed.
 *
 * @param line The label to check.
 * @param error_message The error message to print if the label is reserved.
 * @param line_count The line number where the error occurred (used for reporting).
 * @return TRUE if the label is valid, FALSE if it is reserved.
 */
BOOLEAN Reserved_Words_Check(char * line , const char * error_message , int line_count){
     /* List of reserved register names */
    const char *registers[] = 
    {
        "r0",
        "r1",
        "r2",
        "r3",
        "r4",
        "r5",
        "r6",
        "r7"
    };
    /* List of reserved command names with corresponding codes */
    COMMAND commands_table[] = 
    {
        { "mov", 0 },
        { "cmp", 1 },
        { "add", 2 },
        { "sub", 3 },
        { "lea", 4 },
        { "clr", 5 },
        { "not", 6 },
        { "inc", 7 },
        { "dec", 8 },
        { "jmp", 9 },
        { "bne", 10 },
        { "red", 11 },
        { "prn", 12 },
        { "jsr", 13 },
        { "rts", 14 },
        { "stop", 15 }
    };

    char label[MAX_LABEL_SIZE];
    int i;
    BOOLEAN result = TRUE;

    memset(label, '\0', sizeof(label));

    
    /* Remove newline from label if present */
    if(line[strlen(line)-1] == '\n'){
        strncpy(label,line,(strlen(line)-1) );
    }
    else{
        strcpy(label,line);   
    }
    

    /* Check if label matches any command */
    for(i = 0 ; i < TOTAL_COMMANDS ; i++ ){
        if( strcmp(label , commands_table[i].name ) == 0){
            /* If the label matches a command, print an error and mark as invalid */
            print_error(error_message , line_count);
            result = FALSE;
        }
    }

   /* Check if label matches any register */
    for(i = 0 ; i < TOTAL_REGISTERS ; i++ ){
        if( strcmp(label , registers[i] ) == 0){
            /* If the label matches a register, print an error and mark as invalid */
            print_error(error_message , line_count);
            result = FALSE; 
        }
    }

    /* Check if label matches assembler directives */
    if( strcmp(label , ".data" ) == 0 || strcmp(label , ".string" ) == 0  || strcmp(label , ".entry" ) == 0  || strcmp(label , ".extern" ) == 0 ){
        print_error(error_message , line_count);
        result = FALSE;     
    }
    
    /* Check for reserved words specific to macros */
    if( strcmp(label , "macr" ) == 0  || strcmp(label , "endmacr" ) == 0 ){
        print_error(error_message , line_count);
        result = FALSE;    
    }
    
    return result; 
}


/**
 * @brief Prints an error message with an optional line number.
 *
 * This function handles the printing of error messages to the console. If a valid
 * line number is provided, it is included in the output; otherwise, only the error
 * message is printed.
 *
 * @param error_message The error message to print.
 * @param line The line number associated with the error (or NO_LINE if not applicable).
 */
void print_error(const char *error_message, int  line){
    if (line != NO_LINE) {
        /* Print the error message along with the line number */
        printf("Line: %d , Error: %s\n", line , error_message);
    } else {
        /* Print the error message without a line number */
        printf("Error: %s\n", error_message);
    }
}

/**
 * @brief Frees the memory allocated for the MACRO_CONTENT list.
 *
 * This function frees all the memory used by the MACRO_CONTENT linked list.
 *
 * @param head Pointer to the head of the MACRO_CONTENT list.
 */
void free_macro_content(MACRO_CONTENT **head){
    MACRO_CONTENT *temp;
    /* Loop through each element of the list and free it */
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
        temp = NULL;
    }

}

/**
 * @brief Frees the memory allocated for the MACRO_LIST list.
 *
 * This function frees all the memory used by the MACRO_LIST linked list. It also
 * frees the associated MACRO_CONTENT for each macro.
 *
 * @param head Pointer to the head of the MACRO_LIST.
 */
void free_macro_list(MACRO_LIST **head){
    MACRO_LIST *temp;
    /* Loop through the macro list and free each node */
    while (*head != NULL) {
        /* Free the macro content associated with each macro */
        if((*head)->head_content->next != NULL){
            free_macro_content(&((*head)->head_content->next));
        }
        temp = *head;
        *head = (*head)->next;
        free(temp);
        temp = NULL;
    }

}

/**
 * @brief Frees the memory allocated for the LABEL_LIST list.
 *
 * This function frees all the memory used by the LABEL_LIST linked list.
 *
 * @param head Pointer to the head of the LABEL_LIST.
 */
void free_label_list(LABEL_LIST **head){
    LABEL_LIST *temp;
    /* Loop through the label list and free each node */
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
        temp = NULL;
    }

}

/**
 * @brief Frees the memory allocated for the ADDRESS_LIST list.
 *
 * This function frees all the memory used by the ADDRESS_LIST linked list.
 *
 * @param head Pointer to the head of the ADDRESS_LIST.
 */
void free_address_list(ADDRESS_LIST **head){
    ADDRESS_LIST *temp;
    /* Loop through the address list and free each node */
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
        temp = NULL;
    }
 
}

/**
 * @brief Frees the memory allocated for the ENTRY_LIST list.
 *
 * This function frees all the memory used by the ENTRY_LIST linked list. It also
 * frees the associated ADDRESS_LIST for each entry.
 *
 * @param head Pointer to the head of the ENTRY_LIST.
 */
void free_entry_list(ENTRY_LIST **head){
    ENTRY_LIST *temp;
    /* Loop through the entry list and free each node */
    while (*head != NULL) {
        /* Free the address list associated with each entry */
        if((*head)->addr_list != NULL && (*head)->addr_list->next != NULL){
            free_address_list(&(*head)->addr_list->next);
        }
        temp = *head;
        *head = (*head)->next;
        free(temp);
        temp = NULL;
    }
 
}

/**
 * @brief Frees the memory allocated for the EXTERN_LIST list.
 *
 * This function frees all the memory used by the EXTERN_LIST linked list. It also
 * frees the associated ADDRESS_LIST for each external label.
 *
 * @param head Pointer to the head of the EXTERN_LIST.
 */
void free_extern_list(EXTERN_LIST **head){
    EXTERN_LIST *temp;
    /* Loop through the extern list and free each node */
    while (*head != NULL) {
        /* Free the address list associated with each extern */
        if( (*head)->addr_list != NULL && (*head)->addr_list->next != NULL){
            free_address_list(&(*head)->addr_list->next);
        }
        temp = *head;
        *head = (*head)->next;
        free(temp);
        temp = NULL;
    }
    
}

/**
 * @brief Frees the memory allocated for the EXTERN_LIST list.
 *
 * This function frees all the memory used by the EXTERN_LIST linked list. It also
 * frees the associated ADDRESS_LIST for each external label.
 *
 * @param head Pointer to the head of the EXTERN_LIST.
 */
void free_machine_code_command(MACHINE_CODE_COMMAND **head){
    MACHINE_CODE_COMMAND *temp;
    /* Loop through the command list and free each node */
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
        temp = NULL;
    }
    
}

/**
 * @brief Frees the memory allocated for the MACHINE_CODE_INSTRUCTION list.
 *
 * This function frees all the memory used by the MACHINE_CODE_INSTRUCTION linked list.
 *
 * @param head Pointer to the head of the MACHINE_CODE_INSTRUCTION list.
 */
void free_machine_code_instruction(MACHINE_CODE_INSTRUCTION **head){
    MACHINE_CODE_INSTRUCTION *temp;
    /* Loop through the instruction list and free each node */
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
        temp = NULL;
    }
    
}

/**
 * @brief Frees all memory associated with an ASSEMBLER_TABLE.
 *
 * This function frees all the linked lists associated with an ASSEMBLER_TABLE, including
 * macros, labels, entries, externs, commands, and instructions.
 *
 * @param table Pointer to the ASSEMBLER_TABLE to free.
 */
void free_assembler_table(ASSEMBLER_TABLE **table){
    if (table == NULL || *table == NULL) {
        return;
    }

    /* Free each component of the assembler table if it is not NULL */
    if((*table)->macro_head != NULL && (*table)->macro_head->head_content != NULL && (*table)->macro_head->head_content->next != NULL){
        free_macro_content(&(*table)->macro_head->head_content->next);
    }
    if ((*table)->macro_head != NULL && (*table)->macro_head->next != NULL  ) {
        free_macro_list(&((*table)->macro_head->next));
    }

    if ((*table)->label_head != NULL && (*table)->label_head->next != NULL ) {
        free_label_list(&((*table)->label_head->next));
    }
    if((*table)->entry_head != NULL && (*table)->entry_head->addr_list != NULL && (*table)->entry_head->addr_list->next != NULL){
        free_address_list(&(*table)->entry_head->addr_list->next);
    }

    if ((*table)->entry_head != NULL && (*table)->entry_head->next != NULL ) {
        free_entry_list(&((*table)->entry_head->next));
    }

    if((*table)->extern_head != NULL && (*table)->extern_head->addr_list && (*table)->extern_head->addr_list->next != NULL){
        free_address_list(&(*table)->extern_head->addr_list->next);
    }

    if ((*table)->extern_head != NULL && (*table)->extern_head->next != NULL ) {
        free_extern_list(&((*table)->extern_head->next));
    }

    if ((*table)->command_head != NULL && (*table)->command_head->next != NULL) {
        free_machine_code_command(&((*table)->command_head->next));
    }

    if ((*table)->instruction_head != NULL &&(*table)->instruction_head->next != NULL ) {
        free_machine_code_instruction(&((*table)->instruction_head->next));
    }

    /* Free the table itself */
    free(*table);
    *table = NULL;
}


/**
 * @brief Allocates memory dynamically and checks for allocation failure.
 *
 * This function allocates memory of the specified size and checks if the allocation
 * was successful. If memory allocation fails, an error message is printed and the
 * program exits.
 *
 * @param size The size of the memory to allocate (in bytes).
 * @return Pointer to the allocated memory.
 */
void * generic_malloc(long size){
    void * ptr = malloc(size);
    /*Check if memory allocation failed*/
    if(ptr == NULL){ 
        print_error(MEMORY_ALLOCATION_FAILED , NO_LINE);
        exit(1);/* Exit the program if memory allocation fails */
    }    
    return ptr; /* Return void pointer to allocated memory */
}

/**
 * @brief Adds a suffix to a file name.
 *
 * This function creates a new string by appending the specified suffix (file extension)
 * to the provided file name.
 *
 * @param file_name The base file name.
 * @param ending The suffix (file extension) to add to the file name.
 * @return Pointer to the new file name with the added suffix.
 */
char * add_suffix(char * file_name , char * ending){
    /* Allocate memory for the new file name */
    char * new_name = generic_malloc(MAX_FILE_LINE_LENGTH);

    /* Initialize the memory with null characters */
    memset(new_name , '\0' , MAX_FILE_LINE_LENGTH);

    /* Copy the base file name */
    strcpy(new_name,file_name);

    /* Append the suffix to the file name */
    strcat(new_name , ending);
   
    /* Return the new file name with the suffix */
    return new_name;
}