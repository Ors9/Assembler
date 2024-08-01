#include "assembler.h"


/*Registers type*/
const char * registers[] = {
    "r0",
    "r1",
    "r2",
    "r3",
    "r4",
    "r5",
    "r6",
    "r7"
};


/**
 * @brief Retrieves the name of a register.
 * 
 * This function returns the name of a register from the global registers array based on the provided index.
 *
 * @param i The index of the register in the registers array.
 * @return const char* A string containing the name of the register.
 */
const char * get_register(int i){
    return registers[i];
}



/* Instruction types */
const char * instruction_types[] = {
    ".data",
    ".string",
    ".entry",
    ".extern"
};

/**
 * @brief Retrieves the name of an instruction type.
 * 
 * This function returns the name of an instruction type from the global instruction_types array based on the provided index.
 *
 * @param i The index of the instruction type in the instruction_types array.
 * @return const char* A string containing the name of the instruction type.
 */
const char * get_instruction_type(int i){
    return instruction_types[i];
}



/* Initialize an array of commands */
Command commands_table[] = {
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


/**
 * @brief Retrieves a command from the commands table.
 * 
 * This function returns a pointer to a Command structure from the global commands_table array based on the provided index.
 *
 * @param i The index of the command in the commands_table array.
 * @return Command* A pointer to the Command structure.
 */
Command * get_command(int i){
    return commands_table+i;
}

/* Error messages for various error conditions */
const  char  *error_messages[] = {
"Failed to open file",
"Memory allocation failed",
"Failed to remove file",
"Invalid macro name",
"Extranous text after end of macro",
"Extranous text after macro call",
"Exceeded macro name",
"Invalid label name",
"Illegal label name length",
"Invalid multiple commas",
"Missing comma",
"Command in data section",
"Illegal command",
"Extranous text after stop command",
"Extranous text after rts command",
"Illegal operand",
"Missing operand",
"Invalid number",
"Invalid operand name",
"Macro name already defined",
"Undefined instruction",
"Missing quote",
"Missing string",
"Illegal character",
"Not a label",
"Missing parameter",
"Is not a number",
"Illegal comma",
"Missing number",
"Missing macro name",
"Number out of bound",
"Already defined as extern",
"Already defined as entry",
"Missing label",
"Data in command section"


};



/**
 * @brief Prints an error message with line number.
 * 
 * This function prints an error message corresponding to the given error number along with the line number where the error occurred.
 *
 * @param error The error number, corresponding to an index in the error_messages array.
 * @param line_count The line number where the error occurred.
 */
void print_error_message(error_num error, int line_count){
    printf("\nline %d: %s \n" , line_count ,error_messages[error]);
    return;
}



/**
 * @brief Prints a critical error message and exits the program.
 * 
 
 * This function prints an error message corresponding to the given error number and then exits the program.
 *
 * @param error The error number, corresponding to an index in the error_messages array.
 */
void print_critical_error_message(error_num error){
    puts(error_messages[error]);
    return;
}








/**
 * @brief Compares a given line with known command, register, and instruction names to ensure it is not used as a name.
 *
 * This function checks if the provided line matches any of the names in the commands_table,
 * registers, or instruction_types arrays. If a match is found, it prints an error message and
 * returns FAIL; otherwise, it returns SUCCESS.
 *
 * @param line A string containing the name to be checked.
 * @param error_message An error number representing the type of error to display.
 * @param line_count The current line number in the source file.
 * @return int Returns SUCCESS (0) if the name is valid, otherwise FAIL (1).
 */
int cmp_with_reserved_words(char * line , error_num error_message , int line_count){
    char  temp_label[MAX_LABEL_SIZE];
    int i;
    int result = SUCCESS;

    memset(temp_label, '\0', sizeof(temp_label));/* Initialize temp_label with null characters */

     /* Remove newline character at the end if present */
    if(line[strlen(line)-1] == '\n'){
        strncpy(temp_label,line,(strlen(line)-1) );
    }
    else{
        strcpy(temp_label,line);   
    }
    
    
    /* Check if the name matches any command */
    for(i = 0 ; i < TOTAL_COMMANDS ; i++ ){
        if( strcmp(temp_label , commands_table[i].name ) == 0){
            printf("\nline %d: %s \n" , line_count ,error_messages[error_message]);
            result = FAIL;/* If the name is a command, mark as FAIL */
        }
    }

    /* Check if the name matches any register */
    for(i = 0 ; i < TOTAL_REGISTERS ; i++ ){
        if( strcmp(temp_label , registers[i] ) == 0){
            printf("\nline %d: %s\n " , line_count ,error_messages[error_message]);
            result = FAIL; /* If the name is a register, mark as FAIL */
        }
    }

    /* Check if the name matches any instruction type */
    for(i = 0 ; i < TOTAL_INSTRUCTIONS ; i++ ){
        if( strcmp(temp_label , instruction_types[i] ) == 0){
            printf("\nline %d: %s \n" , line_count ,error_messages[error_message]);
            result = FAIL;/* If the name is an instruction type, mark as FAIL */     
        }
    }

    /* Check if the name matches macro defenition command */
    if( strcmp(temp_label , "macr" ) == 0  || strcmp(temp_label , "endmacr" ) == 0 ){
        printf("\nline %d: %s \n" , line_count ,error_messages[error_message]);
        result = FAIL;/* If the name is an macro defenition command, mark as FAIL */     
    }
    

    return result; /* Return the result of the checks */
}




/**
 * @brief Allocate memory and handle errors if the allocation fails.
 *
 * This function attempts to allocate a specified amount of memory and checks if the allocation
 * was successful. If not, it prints an error message and exits the program.
 *
 * @param size The size of memory to allocate.
 * @return void* A pointer to the allocated memory.
 */
void * generic_malloc(long size){
    void * ptr = malloc(size);
    if(ptr == NULL){ /*Check if memory allocation failed*/
        puts(error_messages[MEMORY_ALLOCATION_FAILED]);
        exit(1);/* Exit the program if memory allocation fails */
    }    
    return ptr; /* Return void pointer to allocated memory */
}

/**
 * @brief Add a suffix to a file name.
 * 
 * This function creates a new string by appending the suffix to the
 * original file name and returns the new string.
 *
 * @param file_name A string containing the original file name.
 * @param suffix A string containing the suffix to be added.
 * @return char* A new file name with the suffix added.
 */
char * add_file(char * file_name , char * suffix){
    char * new_file_name = generic_malloc( (MAX_FILE_LINE_LENGTH*sizeof(char)) );

    /*Copy the file name to new_file_name*/
    strcpy(new_file_name, file_name);

    /*Add the suffix to the new_file_name*/
    strcat(new_file_name, suffix);

    return new_file_name; 
}




/**
 * @brief Free the linked list of macro content nodes.
 *
 * This function iterates through the linked list of macro content nodes and frees each node.
 * 
 * @param head A pointer to the head of the macro content linked list.
 */
void free_macro_content(Macro_content **head){
    Macro_content * temp = NULL;
 
    while( *head != NULL){
        temp = *head;
        *head = (*head)->next_line;/* Move the head pointer to the next node */
        free(temp);/* Free the current node */
        temp = NULL;
    }
    
    return;
}



/**
 * @brief Free the linked list of macro list nodes.
 *
 * This function iterates through the linked list of macro list nodes and frees each node along with its content.
 * 
 * @param head A pointer to the head of the macro list.
 */
void free_macro_list(Macro_list **head){
    Macro_list *temp = NULL;
    while( *head != NULL){
        temp = *head;
        *head = (*head)->next_macro;/* Move the head pointer to the next node */
        free_macro_content(&temp->macro_content->next_line);/* Free the content of the macro */
        free(temp);/* Free the current macro node */
        temp->macro_content = NULL;
        temp = NULL;

    }
    return;
}




/**
 * @brief Free the linked list of label nodes.
 *
 * This function iterates through the linked list of label nodes and frees each node.
 * 
 * @param head A pointer to the head of the label list.
 */
void free_label_list(LABEL ** head){
    LABEL *temp = NULL;
    while( *head != NULL){
        temp = *head;
        *head = (*head)->next_label;/* Move the head pointer to the next node */
        free(temp);/* Free the current label node */
    }
    return;
}

/**
 * @brief Free the linked list of machine code nodes.
 *
 * This function iterates through the linked list of machine code nodes and frees each node.
 * It also frees the memory allocated for the binary code within each machine code node.
 * 
 * @param head A pointer to the head of the machine code linked list.
 */
void free_machine_code(MachineCode **head){
    MachineCode *temp = NULL;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next_entry;/* Move the head pointer to the next node */
        free(temp->binary_code);/* Free the binary code of the current machine code node */
        free(temp);/* Free the current machine code node */
    }
    return;
}

