#include "assembler.h"
#include "first_phase.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>


/*-------------COMMAND SECTION START-----------*/

/**
 * @brief Check for duplicate labels in the label list.
 * 
 * This function iterates over the label list to check if there are any
 * duplicate labels with the `DEFINE` status. If duplicates are found, it
 * prints an error message and sets the result to `FAIL`.
 *
 * @param labels_head Pointer to the head of the LABEL list.
 * @return int Returns SUCCESS (0) if no duplicates are found, otherwise FAIL (1).
 */
int check_for_label_repetition(LABEL * labels_head ){
    int result = SUCCESS;
    LABEL * temp;
    LABEL * ptr;

    /* 
     * Iterate through each label in the list to check for duplicates.
     * A nested loop is used where 'temp' holds the current label and 'ptr' iterates
     * through the remaining labels to find duplicates.
     */
    while(labels_head  != NULL){
        temp = labels_head;
        ptr = labels_head->next_label;
        while(ptr != NULL){
           
            if( (strcmp(ptr->label   , temp->label ) == 0)  && (ptr->define_flag == DEFINE) && (temp->define_flag == DEFINE) ){
                printf("\n label %s has already defined at address %d \n"  ,temp->label , ptr->addr);
                result = FAIL;/*Found labels with same name and in definition 'DEFINE'*/
            }
            ptr = ptr->next_label;
        }
        labels_head = labels_head->next_label;
    }
    return result;
}




/**
 * @brief Validate a label's name.
 * 
 * This function checks if a label's name meets all criteria:
 * - It does not exceed the maximum allowed length.
 * - It does not start with a digit.
 * - It consists only of alphanumeric characters.
 * - It does not conflict with reserved words or existing macro names.
 *
 * @param label A string containing the label to be examined.
 * @param head_macro Pointer to the head of the macro list.
 * @param line_count The line number in the source file where the label was found.
 * @return int Returns SUCCESS (0) if the label is valid, otherwise FAIL (1).
 */
int label_examine(char * label ,  Macro_list * head_macro , int line_count){
    int i = 0;
    int result = SUCCESS;
    Macro_list * temp_macro = head_macro;

    
    /* Check if the label's length exceeds the maximum allowed size */
    if( strlen(label) >= MAX_LABEL_SIZE){ 
        print_error_message(ILLEGAL_LABEL_NAME_LENGTH, line_count);
        return FAIL;
    }

    /* Check if the label conflicts with reserved words */
    result += cmp_with_reserved_words( label , INVALID_LABEL_NAME , line_count);     

    /* Check if the label is empty */
    if( label[0] == '\n'){ 
        print_error_message(MISSING_LABEL, line_count);
        return FAIL;
    }

    /* Check if the label starts with a digit */
    if( isdigit(label[0]) ){ 
        print_error_message(INVALID_LABEL_NAME, line_count);
        result = FAIL;
    }

    /* Check if the label contains only valid characters */
    for(i = 0 ; i < strlen(label) && label[i] != '\0' && label[i] != '\n'; i++){ 
        if( ( !isdigit(label[i]) ) && ( !isupper(label[i]) )  && ( !islower(label[i]) ) ){ 
            print_error_message(INVALID_LABEL_NAME, line_count);
            result = FAIL;
            i = strlen(label);/* Terminate the loop early if an invalid character is found */
        }
        i++;
    }

    
    /* Check if the label conflicts with existing macro names */
    while( temp_macro != NULL){
        if( strcmp(temp_macro->macro_name ,  label ) == 0 ){
            print_error_message(INVALID_LABEL_NAME, line_count);
            result = FAIL;
        }

        temp_macro = temp_macro->next_macro;
    }

    return result;
}


/**
 * @brief Examine the validity of a command line.
 * 
 * This function checks if a command is valid based on the current section of the code.
 * It verifies if the command exists in the `commands_table` and checks for commands in the
 * data section if `stop_flag` is set.
 *
 * @param line A string containing the command line.
 * @param stop_flag An integer flag indicating if the stop command has been encountered.
 * @param line_count The current line number in the source file.
 * @return int Returns SUCCESS (0) if the command is valid, otherwise FAIL (1).
 */
int command_examine(char * line , int stop_flag , int line_count){
    int i = 0;
    int result = SUCCESS;
    int flag = 0;

    /* 
     * Check if the command is present in the commands table.
     * If found, set the flag to indicate a valid command.
     */
    for(i = 0 ; i < TOTAL_COMMANDS ; i++ ){
        if( strncmp(line , get_command(i)->name , strlen(get_command(i)->name)) == 0){
            flag = 1;     
        }
    }

    /* If a command is found in the data section, print an error */
    if(flag == 1 && stop_flag == 1){ 
        print_error_message(COMMAND_IN_DATA_SECTION, line_count);
        result = FAIL;
    }

    /* If the command is invalid in the code section, print an error */
    if(flag == 0 && stop_flag == 0){ 
        print_error_message(ILLEGAL_COMMAND, line_count);
        result = FAIL;
    }
    return result;
}


/**
 * @brief Examine the validity of a zero-operand command line.
 * 
 * This function checks for extraneous text after a zero-operand command like `STOP` or `RTS`.
 * It ensures that no additional characters follow the command.
 *
 * @param line A string containing the command line.
 * @param command_code The command code from the `commands_table`.
 * @param line_count The current line number in the source file.
 * @return int Returns SUCCESS (0) if the command line is valid, otherwise FAIL (1).
 */
int zero_op_examine(char * line , int command_code , int line_count){
    int result = SUCCESS;
    
    /* Check for extraneous text after the command */
    if(strlen(line) > 1 && line[0] != '\n' && line[0] != '\0' ){
        if(command_code == STOP){ 
            print_error_message(EXTRANOUS_TEXT_AFTER_STOP_COMMAND, line_count);
        }
        else{ 
            print_error_message(EXTRANOUS_TEXT_AFTER_RTS_COMMAND, line_count);
        }

        result = FAIL;
    }
    return result;
}


/**
 * @brief Examine the validity of a single-operand command line.
 * 
 * This function checks the operand for single-operand commands, ensuring that it is valid
 * and appropriate for the given command code.
 *
 * @param line A string containing the command line.
 * @param command_code The command code from the `commands_table`.
 * @param head Pointer to the head of the macro list.
 * @param line_count The current line number in the source file.
 * @return int Returns SUCCESS (0) if the command line is valid, otherwise FAIL (1).
 */
int single_op_examine(char * line , int command_code , Macro_list * head, int line_count){
    int result = SUCCESS;
    int op_type;
    char op[MAX_LABEL_SIZE];

    memset(op, '\0', sizeof(op));
    operand_extractor(line , op);/* Extract the operand from the command line */
    op_type = operand_type_identifier(op); /* Identify the type of the operand */
    result += examine_operand(op , op_type , head , line_count);
    
     /* Check the validity of the operand based on the command*/
    switch (command_code) {
        case CLR:
        case NOT:
        case INC:
        case DEC:
        case RED:
         /* For CLR, NOT, INC, DEC, RED: Operand must be a register, label, or pointer */
            if(op_type != REGISTERS && op_type != LABELS && op_type != POINTERS ){ 
                print_error_message(ILLEGAL_OPERAND, line_count);
                result = FAIL;
            }
            break;

       case JMP:
       case BNE:
       case JSR:
       /* For JMP, BNE, JSR: Operand must be a label or pointer */
            if( op_type != LABELS && op_type != POINTERS ){ 
                print_error_message(ILLEGAL_OPERAND, line_count);
                result = FAIL;
            }
            break;

       case PRN:   
        /* For PRN: Operand can be any type. */
            if(op_type == NO_OP){
                print_error_message(ILLEGAL_OPERAND, line_count);
                result = FAIL;
            }
            break;     
        case NO_OP:
        /* If command requires an operand but none is provided */ 
            print_error_message(MISSING_OPERAND, line_count);
            result = FAIL; 
            break;
    }


    return result;
}



/**
 * @brief Examine the validity of a double-operand command line.
 * 
 * This function checks the source and destination operands for double-operand commands,
 * ensuring they are valid and appropriate for the given command code.
 *
 * @param line A string containing the command line.
 * @param command_code The command code from the `commands_table`.
 * @param head Pointer to the head of the macro list.
 * @param line_count The current line number in the source file.
 * @return int Returns SUCCESS (0) if the command line is valid, otherwise FAIL (1).
 */
int double_op_examine(char * line , int command_code , Macro_list * head,int line_count ){
    int result = SUCCESS;
    char op_src[MAX_LABEL_SIZE];
    char op_dest[MAX_LABEL_SIZE];
    int op_src_type;
    int op_dest_type;
    int length;
    
    memset(op_src, '\0', sizeof(op_src));  /* Initialize the source operand string */
    memset(op_dest , '\0' , sizeof(op_dest));/* Initialize the destination operand string */

    length = operand_extractor(line, op_src);/* Extract the source operand */
    length += operand_extractor(line + length, op_dest);/* Extract the destination operand */
    op_src_type = operand_type_identifier(op_src); /* Identify the type of source operand */
    op_dest_type = operand_type_identifier(op_dest);/* Identify the type of destination operand */

    result += examine_operand(op_src , op_src_type ,head , line_count);/* examine  source operand */
    result += examine_operand(op_dest, op_dest_type ,head ,line_count);/* examine destination operand */
    
    /* Check the validity of the source and destination operands based on the command */
    switch(command_code){
        case MOV:
        case ADD:
        case SUB:
         /* For MOV, ADD, SUB: Source operand must be defined and destination cannot be a number or undefined */
            if(op_src_type == NO_OP || op_dest_type == NUMBERS || op_dest_type == NO_OP ){
                print_error_message(ILLEGAL_OPERAND, line_count);
                result = FAIL;
            }
            break;
        case CMP:
        /* For CMP: Both source and destination operands must be defined */
            if(op_src_type == NO_OP || op_dest_type == NO_OP){
                print_error_message(ILLEGAL_OPERAND, line_count);
                result = FAIL;
            }
            break;  
        case LEA:
        /* For LEA: Source operand must be a label, and destination cannot be a number or undefined */
            if(op_src_type != LABELS || op_dest_type == NUMBERS || op_dest_type == NO_OP ){
                print_error_message(ILLEGAL_OPERAND, line_count);
                result = FAIL;
            }
            break;        
    }
    return result;
}


/**
 * @brief Examine a command and its operands for validity.
 * 
 * This function determines the type of command (zero, single, or double operand) and calls
 * the appropriate examination function to validate the command and its operands.
 *
 * @param command Pointer to the Command structure representing the command.
 * @param line A string containing the command line.
 * @param head Pointer to the head of the macro list.
 * @param line_count The current line number in the source file.
 * @return int Returns SUCCESS (0) if the command and operands are valid, otherwise FAIL (1).
 */
int examine_command_cases(Command * command , char * line ,Macro_list *head , int line_count){
    int result = SUCCESS;

    if(command->code <= DOUBLE_OP){/** Two operand command **/
        result += double_op_examine(line , command->code ,head ,line_count);
    }
    else if(command->code <= SINGLE_OP){  /** One operand command **/
        result += single_op_examine(line, command->code ,head ,line_count);
    }
    else{ /** Zero operand command **/
        result += zero_op_examine(line , command->code ,line_count);
    }
    return result;
}


/**
 * @brief Examine the validity of an operand.
 * 
 * This function checks if an operand is valid based on its type (number, label, etc.)
 * and ensures that it adheres to the expected format and constraints.
 *
 * @param op A string containing the operand.
 * @param type_op The type of the operand (NUMBERS, LABELS, etc.).
 * @param head Pointer to the head of the macro list.
 * @param line_count The current line number in the source file.
 * @return int Returns SUCCESS (0) if the operand is valid, otherwise FAIL (1).
 */
int examine_operand(char *op,int type_op , Macro_list * head, int line_count ){
    int result = SUCCESS;
    int i;
    int num;
    
     /* Check if the operand is a number and validate its format and range */
    switch(type_op){
        case NUMBERS:
            /* Validate the number format and range */
            if(strlen(op) > 1 && op[1] != '-' && op[1] != '+' && !isdigit(op[1])){ 
                print_error_message(INVALID_NUMBER, line_count);
                result = FAIL;
            }
            for( i = 2 ; i < strlen(op) ; i++){
                if(!isdigit(op[i])){
                    print_error_message(INVALID_NUMBER, line_count);
                    result = FAIL;   
                    i = strlen(op);/* Exit the loop early if an invalid character is found */
                }
            }
                if(result == SUCCESS){
                    num = atoi(op + 1);
                    if( num >= MAX12_BITS  ||  num <= MIN12_BITS  ){
                       print_error_message(INVALID_NUMBER, line_count);
                        result = FAIL;  
                    }
                }
            break;
        case LABELS:
            /* Check the validity of the label */
            result += label_examine(op , head, line_count);    
            break;
        case NO_OP:
            /* Check for missing operand or invalid operand name */ 
            if(strlen(op) < 1){
                print_error_message(MISSING_OPERAND, line_count); 
            }
            else{
                print_error_message(INVALID_OPERAND_NAME, line_count);
            }
            break;      
    }

    return result;
}


/*-------------COMMAND SECTION END-----------*/


/*-------------DATA SECTION START-----------*/

/**
 * @brief Examine a string directive for validity.
 * 
 * This function checks if a string directive is properly formatted, including the presence
 * of quotes and valid ASCII characters.
 *
 * @param line A string containing the string directive.
 * @param line_count The current line number in the source file.
 * @return int Returns SUCCESS (0) if the string directive is valid, otherwise FAIL (1).
 */
int string_examine(char * line , int line_count){
    int i ;
    int result = SUCCESS;
   
    /* Check if the string is long enough to contain quotes */
    if(strlen(line) < 3){ 
        print_error_message(MISSING_STRING, line_count);
        result = FAIL;
        return result;
    }
    
    /* Check for opening and closing quotes */ 
    if(line[0] != '"' || line[strlen(line) - 2 ] != '"'){ 
        print_error_message(MISSING_QUOTE, line_count);
        result = FAIL;   
    }

    /* Check for valid ASCII characters in the string */
    for(i = 1 ; i < strlen(line) - 1 ; i++){ 
        if( ((unsigned char)line[i]) > MAX_ASCII_VALUE ){
            print_error_message(ILLEGAL_CHARACTER, line_count);
            result = FAIL; 
        }
    }


    return result;
}


/**
 * @brief Examine a data directive for validity.
 * 
 * This function checks if a data directive is properly formatted, including valid numbers
 * and correct usage of commas.
 *
 * @param line A string containing the data directive.
 * @param line_count The current line number in the source file.
 * @return int Returns SUCCESS (0) if the data directive is valid, otherwise FAIL (1).
 */
int data_examine(char * line , int line_count){
    int result = SUCCESS;
    int i = 0;
    char * data_num = 0;
    int num;
    char temp_line[MAX_FILE_LINE_LENGTH];

    strcpy(temp_line, line);

    /* Check if the data directive contains at least one number */
    if(strlen(line) < MIN_SIZE_OF_DATA){ 
        print_error_message(MISSING_NUMBER, line_count);
        result = FAIL;  
        return result;
    }

    /* Check for illegal comma at the start or end of the line */
    if(line[0] == ',' || line[strlen(line) - 2] == ','){ 
        print_error_message(ILLEGAL_COMMA, line_count);
        result = FAIL;
    }

    /** Check for consecutive commas **/
    i = 1;
    while(line[i] != '\n'){
        if(line[i-1] == ',' && line[i] == ','){ 
            print_error_message(ILLEGAL_COMMA, line_count);
            result = FAIL;
        }
        i++;
    }
    
    /** Tokenize the line and validate each number **/
    data_num = strtok(temp_line , ",");
    while(data_num != NULL){
        result += is_valid_number(data_num , line_count);
        if(result == SUCCESS){
            num = atoi(data_num);
            if(num >= MAX15_BITS  || num <= MIN15_BITS){ 
                print_error_message(NUMBER_OUT_OF_BOUND, line_count);
                result = FAIL;
            }
        }
        data_num = strtok(NULL , ",");                 
    }
                             
    return result;
}

/**
 * @brief Validate a number in a data directive.
 * 
 * This function checks if a given string represents a valid number, ensuring it does not
 * contain invalid characters and falls within the expected range.
 *
 * @param str A string containing the number to validate.
 * @param line_count The current line number in the source file.
 * @return int Returns SUCCESS (0) if the number is valid, otherwise FAIL (1).
 */
int is_valid_number( char  * str , int line_count){
    int result = SUCCESS;

    /* Check if the string is empty */ 
    if (*str == '\0'){
        print_error_message(MISSING_PARAMETER, line_count);
        result = FAIL;
    }
    
    /* Check if the number has a sign and then validate the digits */
    if (*str == '-' || *str == '+') {
        str++;
    }

    
    while (*str && (*str != '\n' ) ) { 
        if (!isdigit(*str)){
            print_error_message(IS_NOT_A_NUMBER, line_count);
            result = FAIL;
            return result;
        }
        str++;
    }

    return result;
}


/**
 * @brief Examine entry or extern directives for validity.
 * 
 * This function checks if an entry or extern directive is valid, ensuring that the label
 * is defined and does not conflict with other label definitions.
 *
 * @param line A string containing the entry or extern directive.
 * @param line_count The current line number in the source file.
 * @param head_macro Pointer to the head of the macro list.
 * @param head_label Pointer to the head of the label list.
 * @param instruction The instruction type (ENTRY or EXTERN).
 * @return int Returns SUCCESS (0) if the directive is valid, otherwise FAIL (1).
 */
int entry_or_extern_examine(char * line , int line_count , Macro_list * head_macro , LABEL * head_label ,int instruction){
    int result = SUCCESS;
    int type = ERROR;
    
    type = operand_type_identifier(line);

    /* Check if the directive uses a valid label */ 
    if(type != LABELS){
        print_error_message(NOT_A_LABEL, line_count);
        result = FAIL;
    }
    else{
        result += label_examine(line, head_macro, line_count);
    }
    /* Check for conflicts with existing labels */
    if(result == SUCCESS){
        while(head_label != NULL){
            if( (strncmp(line, head_label->label,(strlen(line)-1)) == 0)){
                if((head_label->type == EXTERN && instruction == ENTRY)){ 
                    print_error_message(ALREADY_DEFINED_AS_EXTERN, line_count);
                    result = FAIL;
                }
                if((head_label->type == ENTRY && instruction == EXTERN)){ 
                    print_error_message(ALREADY_DEFINED_AS_ENTRY, line_count);
                    result = FAIL;
                }
                
            }
            head_label = head_label->next_label;
        }
    }
    return result;
}



/*-------------DATA SECTION END-----------*/
