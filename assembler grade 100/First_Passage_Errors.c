#include "assembler.h"

/**
 * @brief Compares a label with all lists in the ASSEMBLER_TABLE to check for duplicates.
 *
 * This function checks if the given label is already defined in any of the lists within the
 * `ASSEMBLER_TABLE`, including the macro list, label list, and extern list. If a match is found,
 * it prints an error message and marks the result as `FALSE`, indicating that the label is already defined.
 *
 * @param label The label to be checked for duplicates.
 * @param assembler Pointer to the ASSEMBLER_TABLE structure that contains the macro, label, and extern lists.
 * @return Returns TRUE if the label is not found in any list, or FALSE if it is already defined.
 */
BOOLEAN compare_With_Other_Lists(char *label, ASSEMBLER_TABLE *assembler)
{
    BOOLEAN result = TRUE;

    /* Check if the label is already defined in the macro list */
    while (assembler->macro_head != NULL)
    {
        if (strcmp(label, assembler->macro_head->macro_name) == 0)
        {
            printf("%s %s\n",label, LABEL_ALREADY_DEFINED);
            result += FALSE;
        }
        assembler->macro_head = assembler->macro_head->next;
    }

    /* Check if the label is already defined in the label list */
    while (assembler->label_head != NULL)
    {
        if (strcmp(label, (assembler)->label_head->label) == 0)
        {
            printf("%s %s\n",label, LABEL_ALREADY_DEFINED);
            result += FALSE;
        }
        assembler->label_head = assembler->label_head->next;
    }

    /* Check if the label is already defined in the extern list */
    while (assembler->extern_head != NULL)
    {
        if (strcmp(label, (assembler)->extern_head->label) == 0)
        {
            printf("%s %s\n",label, LABEL_ALREADY_DEFINED);
            result += FALSE;
        }
        assembler->extern_head = assembler->extern_head->next;
    }

  

    return result;
}

/**
 * @brief Examines the validity of a label in assembly code.
 *
 * This function checks if a given label is valid according to the following rules:
 * - A label cannot be empty.
 * - A label cannot start with a digit.
 * - A label can only contain alphanumeric characters (digits, uppercase, lowercase).
 * - A label cannot be a reserved word (register name, command, or directive).
 *
 * If any of these conditions are violated, an error is printed and the function returns `FALSE`.
 * Otherwise, it returns `TRUE`.
 *
 * @param label The label to examine for validity.
 * @param line_counter The current line number in the source file (used for error reporting).
 * @return Returns TRUE if the label is valid, or FALSE if it is invalid.
 */
BOOLEAN examine_Label(char *label, int line_counter )
{
    int i;
    BOOLEAN result = TRUE;

    /* Check if the label is empty */
    if(label[0] == '\0')
    { 
        print_error(MISSING_LABEL,line_counter);
        return FALSE;
    }

    /* Check if the label starts with a digit (invalid) */
    if( isdigit(label[0]) )
    { 
        print_error(INVALID_LABEL,line_counter);
        return FALSE;
    }
     /* Loop through each character in the label and check if it is alphanumeric */
    for(i = 0; i < (int)strlen(label) && label[i] != '\0' && label[i] != '\n'; i++)
    { 
        /* If the character is not a digit, uppercase, or lowercase letter, it's invalid */
        if( ( !isdigit(label[i]) ) && ( !isupper(label[i]) )  && ( !islower(label[i]) ) )
        { 
            print_error(INVALID_LABEL,line_counter);
            return FALSE;
        }
    }
    /* Check if the label is a reserved word (e.g., command or register) */
    result += Reserved_Words_Check(label, CANNOT_BE_RESERVED_WORD , line_counter);
    return result;
}


/**
 * @brief Examines the operands of a command to verify correct syntax and operand count.
 *
 * This function checks the validity of the operands for the given command based on the command's type.
 * It distinguishes between commands that require no operands, single operands, and double operands.
 * Each type is examined by a corresponding helper function.
 *
 * - Commands with no operands (e.g., `RTS`, `STOP`) are passed to `Zero_Op_Examine`.
 * - Commands with two operands (e.g., `MOV`, `CMP`, `ADD`) are passed to `Double_Op_Examine`.
 * - Commands with a single operand (e.g., `CLR`, `JMP`) are passed to `Single_Op_Examine`.
 *
 * @param command Pointer to the COMMAND structure representing the current command.
 * @param line The line of assembly code containing the command and operands.
 * @param line_counter The current line number (used for error reporting).
 * @param registers Array of register names used for operand validation.
 * @return Returns TRUE if the command's operands are valid, or FALSE if an error is detected.
 */
BOOLEAN Command_Examine(COMMAND * command , char * line , int line_counter ,const char **registers){
    BOOLEAN result = TRUE;
    /* Check if the command has no operands (e.g., RTS, STOP) */
    if(command->code >= RTS){ 
        result += Zero_Op_Examine(line , command->code ,line_counter);
    }
     /* Check if the command requires two operands (e.g., MOV, CMP, LEA) */
    else if( command->code <= LEA){
        result += Double_Op_Examine(line , command->code  ,line_counter,registers);
    }
     /* Handle single operand commands (e.g., CLR, JMP, PRN) */
    else{  
        result += Single_Op_Examine(line, command->code  ,line_counter,registers);
    }
    
    return result;
}

/**
 * @brief Examines zero-operand commands (e.g., `RTS`, `STOP`) for extraneous text.
 *
 * This function checks if there is any extraneous text following a zero-operand command like `RTS` or `STOP`.
 * If additional text is found, it is considered an error, and an appropriate error message is printed.
 *
 * @param line The line of assembly code to examine.
 * @param command_code The code representing the current command (e.g., `RTS`, `STOP`).
 * @param line_counter The current line number (used for error reporting).
 * @return Returns TRUE if the command is valid, or FALSE if extraneous text is detected.
 */
BOOLEAN Zero_Op_Examine(char * line , int command_code , int line_counter){
    BOOLEAN result = TRUE;
    
    /* Check for extraneous text after the command */
    if(strlen(line) > 1 && line[0] != '\n' && line[0] != '\0' ){
        if(command_code == STOP){ 
            print_error(EXTRANOUS_TEXT_AFTER_STOP_COMMAND, line_counter);
        }
        else{ 
            print_error(EXTRANOUS_TEXT_AFTER_RTS_COMMAND, line_counter);
        }

        result = FALSE;
    }
    return result;
}

/**
 * @brief Examines the operand of a single-operand command for validity.
 *
 * This function checks the validity of the operand for commands that require only one operand
 * (e.g., `CLR`, `NOT`, `INC`, `DEC`, `JMP`, `PRN`). It extracts the operand from the line, identifies 
 * its type (e.g., `REGISTER`, `LABEL`, `POINTER`), and ensures the operand matches the expected type 
 * for the given command. If the operand is invalid or missing, appropriate error messages are printed.
 *
 * @param line The line of assembly code containing the command and operand.
 * @param command_code The code representing the command (e.g., `CLR`, `JMP`).
 * @param line_counter The current line number (used for error reporting).
 * @param registers Array of register names used for operand identification.
 * @return Returns TRUE if the operand is valid, or FALSE if an error is detected.
 */
BOOLEAN Single_Op_Examine(char * line , int command_code , int line_counter ,const char ** registers){

    BOOLEAN result = TRUE;
    int op_type;
    char operand[MAX_LABEL_SIZE];

    memset(operand, '\0', sizeof(operand));

    /* Extract the operand from the line */
    extract_Label(line , operand , '\n' , line_counter);

    /* Identify the type of the operand (e.g., REGISTER, LABEL, POINTER) */
    op_type = operand_Type_Identifier(registers ,operand );

    /* Examine the operand for validity */
    result += Examine_Operand(operand , op_type , line_counter);

    /* Check the command and validate the operand type */
    switch (command_code) {
        case CLR:
        case NOT:
        case INC:
        case DEC:
        case RED:
            /* These commands require the operand to be a register, label, or pointer */
            if(op_type != REGISTER && op_type != LABEL && op_type != POINTER ){ 
                print_error(INVALID_OPERAND, line_counter);
                result = FALSE;
            }
            break;

       case JMP:
       case BNE:
       case JSR:
            /* These commands require the operand to be a label or pointer */
            if( op_type != LABEL && op_type != POINTER ){ 
                print_error(INVALID_OPERAND, line_counter);
                result = FALSE;
            }
            break;

       case PRN:   
            /* PRN allows any valid operand type except NO_OP */
            if(op_type == NO_OP){
                print_error(INVALID_OPERAND, line_counter);
                result = FALSE;
            }
            break;     
        case NO_OP:
            /* If no operand is provided, print a missing operand error */
            print_error(MISSING_OPERAND, line_counter);
            result = FALSE; 
            break;
    }


    return result;
}

/**
 * @brief Examines the operands of a double-operand command for validity.
 *
 * This function checks the validity of the operands for commands that require two operands 
 * (e.g., `MOV`, `CMP`, `ADD`, `SUB`, `LEA`). It extracts both the source and destination operands 
 * from the assembly line, validates them based on their type, and ensures that they conform 
 * to the operand rules for the specific command. If there are errors, appropriate error messages 
 * are printed.
 *
 * @param line The line of assembly code containing the command and its operands.
 * @param command_code The code representing the command (e.g., `MOV`, `CMP`).
 * @param line_counter The current line number (used for error reporting).
 * @param registers Array of register names used for operand identification.
 * @return Returns TRUE if the operands are valid, or FALSE if an error is detected.
 */
BOOLEAN Double_Op_Examine(char * line , int command_code ,int line_counter ,const char ** registers ){
    BOOLEAN result = TRUE;
    char op_src[MAX_LABEL_SIZE];
    char op_dest[MAX_LABEL_SIZE];
    int op_src_type;
    int op_dest_type;
    int length;
    int i;
    
    memset(op_src, '\0', sizeof(op_src)); 
    memset(op_dest , '\0' , sizeof(op_dest));

    /* Check if the line contains any operands */
    if(strlen(line) < 1){
        print_error(MISSING_OPERAND,line_counter);
        return FALSE;
    }
    
    /* Check if there is an invalid comma at the beginning or end of the line */
    if(line[0] == ',' || line[strlen(line) - 2] == ','){ 
        print_error(INVALID_COMMA, line_counter);
        result += FALSE;
    }

    /* Check for consecutive commas (invalid comma usage) */
    i = 1;
    while(line[i] != '\0'){
        if(line[i-1] == ',' && line[i] == ','){ 
            print_error(INVALID_COMMA, line_counter);
            return FALSE;
        }
        i++;
    }
    
    
     /* Extract the source operand from the line */
    length = extract_Label(line ,op_src , ',' , line_counter);

    /* Extract the destination operand from the line */
    length += extract_Label(line + length, op_dest ,'\n' , line_counter);

    /* Identify the type of each operand */
    op_src_type = operand_Type_Identifier(registers,op_src); 
    op_dest_type = operand_Type_Identifier(registers,op_dest);

    /* Examine the validity of the source and destination operands */
    result += Examine_Operand(op_src , op_src_type ,  line_counter);
    result += Examine_Operand(op_dest, op_dest_type  ,line_counter);
    
    /* Validate the operand types based on the command */
    switch(command_code){
        case MOV:
        case ADD:
        case SUB:
             /* Source operand must be valid, and destination must not be a number */
            if(op_src_type == NO_OP || op_dest_type == NUMBER || op_dest_type == NO_OP ){
                print_error(INVALID_OPERAND, line_counter);
                result = FALSE;
            }
            break;
        case CMP:
            /* Both source and destination operands must be valid */
            if(op_src_type == NO_OP || op_dest_type == NO_OP){
                print_error(INVALID_OPERAND, line_counter);
                result = FALSE;
            }
            break;  
        case LEA:
            /* LEA requires the source to be a label, and destination must not be a number */
            if(op_src_type != LABEL || op_dest_type == NUMBER || op_dest_type == NO_OP ){
                print_error(INVALID_OPERAND, line_counter);
                result = FALSE;
            }
            break;        
    }
    return result;
}



/**
 * @brief Examines an operand to verify its validity based on its type.
 *
 * This function checks the operand's syntax and ensures that it is valid according to its type 
 * (e.g., `NUMBER`, `LABEL`, or `NO_OP`). For numbers, it checks if the value is within the acceptable 
 * range for 12-bit representation. For labels, it calls `examine_Label()` to verify if the label is valid. 
 * If the operand is missing or invalid, appropriate error messages are printed.
 *
 * @param operand The operand to examine.
 * @param type_op The type of the operand (e.g., `NUMBER`, `LABEL`, `NO_OP`).
 * @param line_counter The current line number (used for error reporting).
 * @return Returns TRUE if the operand is valid, or FALSE if an error is detected.
 */
BOOLEAN Examine_Operand(char *operand,int type_op , int line_counter ){
    BOOLEAN result = TRUE;
    int i;
    int num;
    
    /* Examine the operand based on its type */
    switch(type_op){
        case NUMBER:
            /* Check if the operand starts with a valid sign or digit */
            if(strlen(operand) > 1 && operand[1] != '-' && operand[1] != '+' && !isdigit(operand[1])){ 
                print_error(INVALID_NUMBER, line_counter);
                result = FALSE;
            }
            /* Check if the rest of the operand contains only digits */
            for( i = 2 ; i < (int)strlen(operand) ; i++){
                if(!isdigit(operand[i])){
                    print_error(INVALID_NUMBER, line_counter);
                    result = FALSE;   
                    i = strlen(operand);
                }
            }

            /* Check if the rest of the operand contains only digits */
                if(result == TRUE){
                    num = atoi(operand + 1);/* Convert the number from string to integer (ignoring the first character) */
                    /* Check if the number is within the valid range for 12-bit representation */
                    if( num >= MAX_VALUE_FOR_12_BITS  ||  num <= MIN_VALUE_FOR_12_BITS  ){
                       print_error(INVALID_NUMBER, line_counter);
                        result = FALSE;  
                    }
                }
            break;
        case LABEL:
            /* Examine the label for validity of the label */
            result += examine_Label(operand, line_counter);  
            break;

        case NO_OP:
            /* Handle cases where no operand is provided or the operand is invalid */
            if(strlen(operand) < 1){
                print_error(MISSING_OPERAND, line_counter); 
            }
            else{
                print_error(INVALID_OPERAND_NAME, line_counter);
            }
            break;      
    }

    return result;
}


/**
 * @brief Examines a string directive in the assembly code for validity.
 *
 * This function checks the validity of a `.string` directive, ensuring it follows the correct syntax:
 * - The string must be enclosed in double quotes (`"`).
 * - The string must not contain invalid characters (i.e., characters outside the standard ASCII range).
 * - The string must be of a minimum length, including the quotes.
 *
 * If any of these conditions are violated, appropriate error messages are printed, and the function
 * returns `FALSE`. Otherwise, it returns `TRUE`.
 *
 * @param line The line of assembly code containing the `.string` directive.
 * @param line_counter The current line number (used for error reporting).
 * @return Returns TRUE if the string is valid, or FALSE if an error is detected.
 */
BOOLEAN String_Examine(char * line , int line_counter){
    int i ;
    BOOLEAN result = TRUE;
   
    /* Check if the string is of minimum length (e.g., must contain at least two quotes) */
    if(strlen(line) < MIN_STR_SIZE){ 
        print_error(MISSING_STRING, line_counter);
        return FALSE;
    }
    
     /* Check if the string starts and ends with double quotes */
    if(line[0] != '"' || line[strlen(line) - 2 ] != '"'){ 
        print_error(MISSING_QUOTE, line_counter);
        result = FALSE;   
    }

     /* Validate that all characters in the string are within the valid ASCII range */
    for(i = 1 ; i < (int)strlen(line) - 1 ; i++){ 
        if( ((unsigned char)line[i]) > MAX_ASCII_VALUE ){
            print_error(INVALID_CHARACTER, line_counter);
            result = FALSE; 
        }
    }


    return result;
}


/**
 * @brief Examines a `.data` directive in the assembly code for validity.
 *
 * This function checks the validity of a `.data` directive, ensuring that it contains a valid list
 * of integers separated by commas. It validates that:
 * - The numbers are within the 15-bit range.
 * - Commas are used correctly (no consecutive or leading/trailing commas).
 * - The directive contains at least one valid number.
 *
 * If any of these conditions are violated, appropriate error messages are printed, and the function
 * returns `FALSE`. Otherwise, it returns `TRUE`.
 *
 * @param line The line of assembly code containing the `.data` directive.
 * @param line_counter The current line number (used for error reporting).
 * @return Returns TRUE if the `.data` directive is valid, or FALSE if an error is detected.
 */
BOOLEAN Data_Examine(char * line , int line_counter){
    BOOLEAN result = TRUE;
    int i = 0;
    char * char_num = 0;
    int num;
    char temp[MAX_FILE_LINE_LENGTH];
    memset(temp , '\0' , sizeof(temp));
    strcpy(temp, line);

   /* Check if the directive has a minimum size*/
    if(strlen(line) < MIN_DATA_SIZE){ 
        print_error(MISSING_NUMBER, line_counter);  
        return FALSE;
    }

    /* Check for invalid commas at the beginning or end of the line */
    if(line[0] == ',' || line[strlen(line) - 2] == ','){ 
        print_error(INVALID_COMMA, line_counter);
        result = FALSE;
    }

    /* Check for consecutive commas in the line */
    i = 1;
    while(line[i] != '\n'){
        if(line[i-1] == ',' && line[i] == ','){ 
            print_error(INVALID_COMMA, line_counter);
            result = FALSE;
        }
        i++;
    }
    
    /* Tokenize the line to extract individual numbers separated by commas */
    char_num = strtok(temp , ",");
    while(char_num != NULL){
        /* Validate each extracted number */
        result += Valid_Num_Examine(char_num , line_counter);
        if(result == TRUE){
            /* Convert the valid number to an integer and check its range */
            num = atoi(char_num);
            if(num >= MAX_VALUE_FOR_15_BITS  || num <= MIN_VALUE_FOR_15_BITS){ 
                print_error(NUMBER_OUT_OF_BOUND, line_counter);
                result = FALSE;
            }
        }
        /* Move to the next number in the list */
        char_num = strtok(NULL , ",");                 
    }
                             
    return result;
}

/**
 * @brief Examines a number in a `.data` directive to ensure it is valid.
 *
 * This function checks if the given string representing a number is valid:
 * - It allows for an optional leading sign (`+` or `-`).
 * - It ensures that the remaining characters are all digits.
 * - If the string is empty or contains non-digit characters, an appropriate error message is printed.
 *
 * @param char_num The string representing the number to examine.
 * @param line_counter The current line number (used for error reporting).
 * @return Returns TRUE if the number is valid, or FALSE if an error is detected.
 */
BOOLEAN Valid_Num_Examine( char  * char_num , int line_counter){
    BOOLEAN result = TRUE;

    /* Check if the number string is missing */
    if (*char_num == '\0'){
        print_error(MISSING_PARAMETER, line_counter);
        result = FALSE;
    }
    
    /* Skip over an optional leading sign ('-' or '+') */
    if (*char_num == '-' || *char_num == '+') {
        char_num++;
    }

    /* Check if the remaining characters are digits */
    while (*char_num && (*char_num != '\n' ) ) { 
        if (!isdigit(*char_num)){
            print_error(IS_NOT_A_NUMBER, line_counter);
            result = FALSE;
            return result;
        }
        char_num++;
    }

    return result;
}


/**
 * @brief Examines the validity of an `.extern` directive in assembly code.
 *
 * This function validates an `.extern` directive by extracting the label, ensuring it is a valid label,
 * and checking that it has not been previously defined in the label or entry lists. The function also
 * verifies that the label follows the correct syntax and is not defined as an entry elsewhere.
 * If any errors are detected (e.g., invalid label, label already defined), appropriate error messages
 * are printed.
 *
 * @param line The line of assembly code containing the `.extern` directive.
 * @param line_counter The current line number (used for error reporting).
 * @param assembler Pointer to the ASSEMBLER_TABLE, which holds the lists of labels and entries.
 * @param registers Array of register names used to check if the label is valid.
 * @return Returns TRUE if the `.extern` directive is valid, or FALSE if an error is detected.
 */
BOOLEAN Extern_Examine(char * line , int line_counter , ASSEMBLER_TABLE *assembler ,const char **resgisters){
    BOOLEAN result = TRUE;
    int type;
    char label[MAX_LABEL_SIZE];
    ENTRY_LIST * temp_ent;
    LABEL_LIST *temp_label;
    memset(label , '\0',sizeof(label));

    /* Extract the label from the line and check for errors */
    if (extract_Label(line ,label , '\n' , line_counter) ==  ERROR)
    {
        print_error(ILLEGAL_LABEL_NAME_LENGTH ,line_counter);
        return FALSE;
    }

     /* Identify the type of the operand (it should be a label) */
    type = operand_Type_Identifier(resgisters, label);
    /* Ensure the operand is a valid label */
    if(type != LABEL){
        print_error(NOT_A_LABEL, line_counter);
        result = FALSE;
    }
    else{
        result += examine_Label(label, line_counter); /* Further validate the label */
    }
    
    /* Check if the label is already defined in the label or entry lists */
    temp_label = assembler->label_head;
    temp_ent = assembler->entry_head;
    if(result == TRUE){
        while(temp_label != NULL){
            /* Check if the label is already defined in the label list */
            if( ( strcmp(label, temp_label->label) ) == 0){
                    print_error(LABEL_ALREADY_DEFINED, line_counter);
                    result = FALSE;
            }
            temp_label = temp_label->next;
        }
        while(temp_ent != NULL){
            /* Check if the label is already defined as an entry */
            if( ( strcmp(label, temp_ent->label) ) == 0){
                    print_error(ALREADY_DEFINED_AS_ENTRY, line_counter);
                    result = FALSE;
            }
            temp_ent = temp_ent->next;
        }
    }
    
    return result;
}


/**
 * @brief Examines the validity of an `.entry` directive in assembly code.
 *
 * This function validates an `.entry` directive by extracting the label, ensuring it is a valid label,
 * and checking that it has not been previously defined in the `.extern` list. The function verifies
 * that the label follows the correct syntax and is not defined as an external symbol elsewhere.
 * If any errors are detected (e.g., invalid label, label already defined as an external), 
 * appropriate error messages are printed.
 *
 * @param line The line of assembly code containing the `.entry` directive.
 * @param line_counter The current line number (used for error reporting).
 * @param assembler Pointer to the ASSEMBLER_TABLE, which holds the lists of external symbols.
 * @param registers Array of register names used to check if the label is valid.
 * @return Returns TRUE if the `.entry` directive is valid, or FALSE if an error is detected.
 */
BOOLEAN Entry_Examine(char * line , int line_counter , ASSEMBLER_TABLE *assembler ,const char **registers){
    BOOLEAN result = TRUE;
    int type;
    char label[MAX_LABEL_SIZE];
    EXTERN_LIST * temp;
    memset(label , '\0',sizeof(label));

    /* Extract the label from the line and check for errors */
    if (extract_Label(line ,label , '\n',line_counter) == ERROR)
    {
        return FALSE;
    }
    /* Identify the type of the operand (it should be a label) */
    type = operand_Type_Identifier(registers, label);
    
    /* Ensure the operand is a valid label */
    if(type != LABEL)
    {
        print_error(NOT_A_LABEL, line_counter);
        result = FALSE;
    }
    else
    {
        /* Further validate the label */
        result += examine_Label(label, line_counter);
    }
    temp  = assembler->extern_head;
    if(result == TRUE)
    {
        while(temp!= NULL)
        {
            if( ( strcmp(label, temp->label) ) == 0)
            {
                /* Check if the label is already defined in the `.extern` list */
                print_error(ALREADY_DEFINED_AS_EXTERN, line_counter);
                result = FALSE;
            }
            temp = temp->next;
        }
    }
    
    return result;
}





