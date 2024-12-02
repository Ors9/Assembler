#include "assembler.h"

/**
 * @brief Extracts a label from a line of assembly code.
 *
 * This function extracts a label from the given line of code up to a specified delimiter.
 * The label is copied into the `label` parameter, and the function returns the length
 * of the label (including the delimiter) if successful. If no label is found or the 
 * label exceeds the maximum allowed size, an error is returned.
 *
 * @param line The line of assembly code to extract the label from.
 * @param label The output buffer where the extracted label will be stored.
 * @param delimiter The character used to indicate the end of the label (typically ':' for labels).
 * @param line_counter The current line number in the source file (used for error reporting).
 * @return The length of the extracted label including the delimiter, or ERROR if the label is invalid.
 */
int extract_Label(char *line, char *label, char delimiter , int line_counter)
{
    int j, label_flag = 0;
    int i;
    
    /* Iterate through the line to extract the label */
    for(i = 0, j = 0; i < (int)strlen(line) && j < MAX_LABEL_SIZE && label_flag != 1; i++, j++)
    {
        if(line[i] != delimiter)
        {
            label[j] = line[i];/* Extract label characters */
        }
        else
        {
            label_flag = 1;/* Stop extraction at colon */
        }
    }
    /* If label extraction was successful, return the length of the label */
    if(label_flag == 1)
    {
        return strlen(label) + 1; /* Length includes delimiter */
    }
    /* Check if the label exceeds maximum size and delimiter is not ':' */
    if (label_flag != 1 && j == MAX_LABEL_SIZE && delimiter != ':')
    {
        print_error(ILLEGAL_LABEL_NAME_LENGTH , line_counter);/* Error for illegal label length */
        return ERROR;
    }

    return 0; /* Return 0 if no label is found */
}

/**
 * @brief Inserts a new label into the LABEL_LIST.
 *
 * This function inserts a new label into the linked list of labels (`LABEL_LIST`). 
 * It allocates memory for the new node, initializes the label and address, 
 * and appends the node to the end of the list.
 *
 * @param label_list Pointer to the head of the LABEL_LIST linked list.
 * @param label The label name to insert into the list.
 * @param address The address associated with the label.
 */
void insert_Label_List(LABEL_LIST **label_list, char *label, int address)
{
    LABEL_LIST *ptr = NULL;
    LABEL_LIST *new_node = generic_malloc(sizeof(LABEL_LIST));

    /* Initialize the new label node */
    memset(new_node->label, '\0', sizeof(new_node->label));
    strcpy(new_node->label, label);  
    new_node->label[MAX_LABEL_SIZE - 1] = '\0';           
    new_node->addr = address;
    new_node->next = NULL;

    /* If the list is empty, set the new node as the head */
    if (*label_list == NULL) 
    {
        *label_list = new_node;
        
    } else 
    {
        /* Traverse the list to find the last node */
        ptr = *label_list;
        while (ptr->next != NULL) 
        {
            ptr = ptr->next;
        }
        /* Append the new node to the end of the list */
        ptr->next = new_node;  
    }

}

/**
 * @brief Determines the type of line in the assembly source code.
 *
 * This function identifies the type of line from the given assembly code, whether it is
 * a directive (e.g., `.entry`, `.extern`, `.data`, `.string`) or a command from the 
 * commands table. The function also updates the `skip` parameter, which indicates how 
 * many characters were processed in determining the line type. If the line matches a command,
 * the corresponding command is returned through the `command` parameter.
 *
 * @param line The line of assembly code to analyze.
 * @param commands_table Array of command structures containing supported command names.
 * @param skip A pointer to an integer to track how many characters to skip after processing the line type.
 * @param command Pointer to a `COMMAND` structure that will point to the matched command (if applicable).
 * @return An integer representing the type of the line (e.g., IS_ENTRY, IS_EXTERN, IS_COMMAND, IS_STRING, IS_DATA, UNDEFINED_INSTRUCTION).
 */
int get_Line_Type(char *line, COMMAND *commands_table, int *skip, COMMAND **command)
{
    int i;
    /* Check if the line is an .entry directive */
    if (strncmp(line, ".entry", strlen(".entry")) == 0)
    {
        (*skip) += strlen(".entry"); /* Update skip to indicate how many characters to skip */
        return IS_ENTRY; /* Return the type as IS_ENTRY */
    }
    /* Check if the line is an .extern directive */
    if (strncmp(line, ".extern", strlen(".extern")) == 0)
    {
        (*skip) += strlen(".extern"); /* Update skip for .extern */
        return IS_EXTERN; /* Return the type as IS_EXTERN */
    } 
    /* Loop through the commands table to check if the line matches any command */
    for (i = 0; i < TOTAL_COMMANDS; i++)
    {
        if (strncmp(line, commands_table[i].name, strlen(commands_table[i].name)) == 0)
        {
            *command = &commands_table[i];/* Set the command pointer to the matched command */
            (*skip) += strlen(commands_table[i].name); /* Update skip for the command */
            return IS_COMMAND; /* Return the type as IS_COMMAND */
        }
    }
    /* Check if the line is a .string directive */
    if (strncmp(line, ".string", strlen(".string")) == 0)
    {
        (*skip) += strlen(".string");/* Update skip for .string */
        return IS_STRING; /* Return the type as IS_STRING */
    } 

    if (strncmp(line, ".data", strlen(".data")) == 0 ) /* Check if the line is a .data directive */
    {
        (*skip) += strlen(".data"); /* Update skip for .data */
        return IS_DATA;/* Return the type as IS_DATA */
    }
    /* If no match is found, return UNDEFINED_INSTRUCTION */
    return UNDEFINED_INSTRUCTION;
}

/**
 * @brief Inserts a new entry into the ENTRY_LIST.
 *
 * This function inserts a new entry (label) into the linked list of entries (`ENTRY_LIST`).
 * It dynamically allocates memory for a new node, extracts the label from the provided line,
 * and appends the node to the end of the list. If the list is empty, the new node becomes the head.
 *
 * @param entry_list Pointer to the head of the ENTRY_LIST linked list.
 * @param line The line containing the label to insert into the entry list.
 * @param line_counter The current line number (used for error reporting).
 */
void insert_Entry_List(ENTRY_LIST **entry_list, char *line , int line_counter) {
    ENTRY_LIST *ptr = NULL;
    ENTRY_LIST *new_node = generic_malloc(sizeof(ENTRY_LIST));
    char entry[MAX_LABEL_SIZE];

    memset(entry , '\0' , sizeof(entry));

    /* Extract the label from the line */
    extract_Label(line , entry , '\n',line_counter);

    strcpy(new_node->label, entry );
    new_node->addr_list = NULL;
    new_node->next = NULL;

    /* If the list is empty, set the new node as the head */
    if (*entry_list == NULL) {
        *entry_list = new_node;
    } else {
        /* Traverse the list to find the last node */
        ptr = *entry_list;
        while (ptr->next != NULL) {
            ptr = ptr->next;
        }
        /* Append the new node to the end of the list */
        ptr->next = new_node;
    }

}

/**
 * @brief Inserts a new external label into the EXTERN_LIST.
 *
 * This function inserts a new external label into the linked list of externals (`EXTERN_LIST`).
 * It dynamically allocates memory for a new node, extracts the label from the provided line,
 * and appends the node to the end of the list. If the list is empty, the new node becomes the head.
 *
 * @param extern_list Pointer to the head of the EXTERN_LIST linked list.
 * @param line The line containing the external label to insert into the list.
 * @param line_counter The current line number (used for error reporting in label extraction).
 */
void insert_Extern_List(EXTERN_LIST **extern_list, char *line , int line_counter) {
    char external[MAX_LABEL_SIZE];
    EXTERN_LIST *ptr = NULL;
    EXTERN_LIST *new_node = generic_malloc(sizeof(EXTERN_LIST));
    new_node->addr_list = NULL;
    
    memset(external , '\0',sizeof(external) );

     /* Extract the label from the line*/
    extract_Label(line , external , '\n' , line_counter);

    strcpy(new_node->label, external );
    new_node->next = NULL;

    /* If the list is empty, set the new node as the head */
    if (*extern_list == NULL) {
        *extern_list = new_node;
    } else {
        /* Traverse the list to find the last node */
        ptr = *extern_list;
        while (ptr->next != NULL) {
            ptr = ptr->next;
        }
        /* Append the new node to the end of the list */
        ptr->next = new_node;
    }

}

void add_to_instruction_list(MACHINE_CODE_INSTRUCTION **instruction_list , int address , MILA mila){
    MACHINE_CODE_INSTRUCTION *ptr = NULL;
    MACHINE_CODE_INSTRUCTION *new_node = generic_malloc(sizeof(MACHINE_CODE_INSTRUCTION));

    /* Initialize the new node */
    new_node->addr = address;
    new_node->mila = mila;
    new_node->next = NULL;

    /* If the list is empty, set the new node as the head */
    if (*instruction_list == NULL) {
        *instruction_list = new_node;
    } else {
        /* Traverse the list to find the last node */
        ptr = *instruction_list;
        while (ptr->next != NULL) {
            ptr = ptr->next;
        }
        /* Append the new node to the end of the list */
        ptr->next = new_node;
    }

}

/**
 * @brief Extracts a string from a line of assembly and adds it to the instruction list.
 *
 * This function parses a string literal from the provided assembly line, converts each character 
 * into a `MILA` (instruction representation), and adds each character as an instruction to the 
 * `MACHINE_CODE_INSTRUCTION` list. If the string is correctly formatted (enclosed in quotes), 
 * it adds each character followed by a terminating zero (null character) to the list.
 *
 * @param instruction_list Pointer to the head of the MACHINE_CODE_INSTRUCTION linked list.
 * @param line The line of assembly code containing the string to extract.
 * @param address Pointer to the current address, which is incremented as new instructions are added.
 * @return Returns FALSE if the string is correctly extracted, TRUE if there is an error (i.e., no opening quote).
 */
int extract_String(MACHINE_CODE_INSTRUCTION **instruction_list, char *line, int *address) {
    int i;
    MILA new_mila;

    /* Check if the line starts with a double quote (indicating a string literal) */
    if (line[0] != '"') {
        return FALSE;
    }

    /* Iterate over the characters in the string, excluding the surrounding quotes */
    for (i = 1; i < (int)(strlen(line) - strlen("\"\n")); i++) {
        new_mila.mila = line[i];/* Set the MILA to the current character */
        add_to_instruction_list(instruction_list , *address , new_mila);/* Add character as instruction */
        (*address)++;
    }
     /* Add a terminating null character (MILA = 0) at the end of the string */
    new_mila.mila = 0;
    add_to_instruction_list(instruction_list , *address , new_mila);
    (*address)++;
    return FALSE;
}


/**
 * @brief Extracts data values from a line of assembly and adds them to the instruction list.
 *
 * This function parses a list of comma-separated numeric values from the provided assembly line, 
 * converts each value into a `MILA` (instruction representation), and adds it as an instruction 
 * to the `MACHINE_CODE_INSTRUCTION` list. The address is incremented as each new instruction is added.
 *
 * @param instruction_list Pointer to the head of the MACHINE_CODE_INSTRUCTION linked list.
 * @param line The line of assembly code containing the data values to extract.
 * @param address Pointer to the current address, which is incremented as new instructions are added.
 * @return Always returns FALSE to indicate successful extraction.
 */
int extract_Data(MACHINE_CODE_INSTRUCTION **instruction_list, char *line, int *address)
{
    char *number;
    MILA mila;

    /* Extract the first token (number) from the line */
    number = strtok(line, ",\n");
   
    /* Walk through the other tokens (numbers separated by commas) */
    while(number != NULL ) 
    {
         /* Convert the token (string) to an integer and store it in the MILA struct */
        mila.mila = atoi(number);
        add_to_instruction_list(instruction_list , *address , mila);
        
        (*address)++;
        /* Get the next token (number) from the line */
        number = strtok(NULL, ",\n");
    }

    return FALSE;
}

/**
 * @brief Converts an assembly command line into machine code and adds it to the command list.
 *
 * This function processes a line of assembly code containing a command and its operands. It identifies
 * the command type and extracts the source and destination operands (if applicable). The operands are
 * converted into machine code using the `generate_Mila` function, which appends the result to the command list.
 *
 * @param command_list Pointer to the head of the MACHINE_CODE_COMMAND linked list.
 * @param registers Array of register names for operand identification.
 * @param line The line of assembly code containing the command and its operands.
 * @param command Pointer to the `COMMAND` structure representing the current command.
 * @param IC Pointer to the instruction counter (used to track the address of each command).
 * @param line_counter The current line number in the source file (used for error reporting).
 * @return Always returns FALSE to indicate successful conversion.
 */
int convert_Command(MACHINE_CODE_COMMAND **command_list, const char **registers, char *line, COMMAND *command, int *IC , int line_counter)
{
    char src_op[MAX_LABEL_SIZE], dest_op[MAX_LABEL_SIZE];
    int type_src = NO_OP , type_dest = NO_OP;

    memset(src_op , '\0' , MAX_LABEL_SIZE);
    memset(dest_op , '\0' , MAX_LABEL_SIZE);

    /* Handle different command types based on the command code */
    switch(command->code)
    {
        /* Two operand commands (e.g., MOV, CMP, ADD, SUB, LEA) */
        case MOV:
        case CMP:
        case ADD:
        case SUB:
        case LEA:
            /* Extract the source operand (before the comma) */
            extract_Label(line, src_op, ',' , line_counter);
            /* Extract the destination operand (after the comma) */
            extract_Label( (line + strlen(src_op) + strlen(",")) , dest_op, '\n' , line_counter);
            /* Identify the types of the source and destination operands */
            type_src =  operand_Type_Identifier(registers, src_op);
            type_dest =  operand_Type_Identifier(registers, dest_op);
            /* Generate the machine code (MILA) for the command and operands */
            generate_Mila(command_list, command->code, src_op, type_src, dest_op, type_dest, IC);
            break;
        
         /* Single operand commands (e.g., CLR, NOT, INC, JMP, etc.) */
        case CLR:
        case NOT:
        case INC:
        case DEC:
        case JMP:
        case BNE:
        case RED:
        case PRN:
        case JSR:
            /* Extract the destination operand */
            extract_Label(line, dest_op, '\n' , line_counter);
            /* Identify the type of the destination operand */
            type_dest =  operand_Type_Identifier(registers, dest_op);
            /* Generate the machine code (MILA) for the command and destination operand */
            generate_Mila(command_list, command->code, NULL, NONE, dest_op, type_dest, IC);
            break;

        /* Commands with no operands (e.g., RTS, STOP) */
        case RTS:
        case STOP:
            /* Generate the machine code (MILA) for commands with no operands */
            generate_Mila(command_list, command->code, NULL, NONE, NULL, NONE, IC);
    }


    return FALSE;
}


/**
 * @brief Inserts a new command into the MACHINE_CODE_COMMAND list.
 *
 * This function adds a new command node, containing an address, label, binary code, and type,
 * to the end of the linked list of machine code commands. If the list is empty, the new node 
 * becomes the head.
 *
 * @param command_list Pointer to the head of the MACHINE_CODE_COMMAND linked list.
 * @param addr The address associated with the command.
 * @param label The label associated with the command, if any.
 * @param binary_code The binary representation of the machine code (MILA).
 * @param type The type of the command (e.g., type of addressing or instruction format).
 */
void insert_Command_List(MACHINE_CODE_COMMAND **command_list , int addr, char label[MAX_LABEL_SIZE] , MILA binary_code , int type)
{
    MACHINE_CODE_COMMAND * ptr = NULL;
    MACHINE_CODE_COMMAND * new_node = generic_malloc(sizeof(MACHINE_CODE_COMMAND) );
    new_node->addr = addr;
    memset(new_node->label , '\0' ,sizeof(new_node->label));
    strcpy(new_node->label , label);
    new_node->type = type;
    new_node->binary_code = binary_code;
    new_node->next = NULL;

    /* If the list is empty, set the new node as the head */
    if(*command_list == NULL){
        *command_list = new_node;
        return;
    }
    /* Traverse the list to find the last node */
    ptr = *command_list;
    while(ptr->next != NULL){
        ptr = ptr->next;
    }
    /* Append the new node to the end of the list */
    ptr->next = new_node;
    
}

/**
 * @brief Generates machine code (MILA) for a given command and its operands.
 *
 * This function generates the binary code (MILA) for the specified assembly command and its operands.
 * It handles different command types based on the number of operands, addressing modes, and updates
 * the instruction counter (IC) after inserting the generated machine code into the command list.
 *
 * @param command_list Pointer to the head of the MACHINE_CODE_COMMAND linked list.
 * @param command_code The code of the assembly command (e.g., MOV, ADD, RTS).
 * @param src_op The source operand (if applicable).
 * @param type_src The addressing type of the source operand (e.g., REGISTER, LABEL).
 * @param dest_op The destination operand (if applicable).
 * @param type_dest The addressing type of the destination operand.
 * @param IC Pointer to the instruction counter (incremented as new instructions are added).
 */
void generate_Mila(MACHINE_CODE_COMMAND **command_list, CODES command_code, char *src_op, int type_src, char *dest_op, int type_dest, int *IC)
{
    MILA new_mila;
    new_mila.mila = A;/* Initialize the MILA value with default value A */

     /* Handle commands with no operands (e.g., RTS, STOP) */
    if(command_code >= RTS)
    {
        new_mila.mila |= shift_Bits(command_code, OPCODE);  /* Encode the command opcode */               
        insert_Command_List(command_list, *IC, "\0", new_mila, type_src);   /* Insert the command into the list */
        (*IC)++;
        return;  
    }
    
    /* Handle commands with two operands (e.g., MOV, CMP, ADD, LEA) */
    if (command_code <= LEA)
    {
        /* Encode source and destination operand types, and command opcode */
        new_mila.mila |= shift_Bits(MASK128, type_src);
        new_mila.mila |= shift_Bits(MASK8, type_dest);
        new_mila.mila |= shift_Bits(command_code, OPCODE);                
        insert_Command_List(command_list, *IC, "\0", new_mila, type_src);/* Insert the command */
        
        (*IC)++;

        new_mila.mila = 0;/* Reset MILA value for further operand handling */

         /* Handle special case when both operands are registers or pointers */
        if ((type_src == REGISTER || type_src == POINTER) && (type_dest == POINTER || type_dest == REGISTER))
        {
        
            new_mila.mila = A;
            new_mila.mila |= shift_Bits(get_Operand_Value(type_dest, dest_op), 3);/* Encode destination operand */
            new_mila.mila |= shift_Bits(get_Operand_Value(type_src, src_op), 6);/* Encode source operand */
            insert_Command_List(command_list, *IC, "\0", new_mila, NONE);/* Insert operands as a single instruction */
            (*IC)++;
            return;
        }
        /* Handle source operand */
        if(type_src == LABEL){
           new_mila.mila = MARK_FOR_SECOND_PASS;  /* Mark for the second pass */
        }
        else if (type_src == NUMBER)
        {
           new_mila.mila = A;
           new_mila.mila |= shift_Bits(get_Operand_Value(type_src, src_op), 3);/* Encode immediate number */
        }
        else
        {
           new_mila.mila = A;
           new_mila.mila |= shift_Bits(get_Operand_Value(type_src, src_op), 6); /* Encode register/pointer */
        }

        insert_Command_List(command_list, *IC, src_op, new_mila, type_src);/* Insert source operand */
        new_mila.mila = 0;
        (*IC)++;

        /* Handle destination operand */
        if(type_dest == LABEL){
            new_mila.mila = MARK_FOR_SECOND_PASS;/* Mark for the second pass */
        }
        else 
        {
            new_mila.mila = A;
            new_mila.mila |= shift_Bits( get_Operand_Value(type_dest, dest_op)  , 3);/* Encode destination operand */
            
        }
        
        insert_Command_List(command_list, *IC, dest_op, new_mila, type_dest);
        (*IC)++;
    }
    /* Handle single operand commands (e.g., PRN, JMP) */
    else
    {
        new_mila.mila |= shift_Bits(MASK8, type_dest);
        new_mila.mila |= shift_Bits(command_code, OPCODE);           
        insert_Command_List(command_list, *IC, "\0", new_mila, type_src);
       
        (*IC)++;

        if(type_dest == LABEL)
        {
            new_mila.mila = MARK_FOR_SECOND_PASS; /* Mark for the second pass */
        }
        else
        {
            new_mila.mila = A;
            new_mila.mila |= shift_Bits(get_Operand_Value(type_dest, dest_op), 3);/* Encode operand */
        }
        insert_Command_List(command_list, *IC, dest_op, new_mila, type_dest);
        (*IC)++;
        
    }
    
}

/**
 * @brief Retrieves the integer value of an operand based on its type.
 *
 * This function extracts the integer value of an operand from a string representation,
 * depending on the type of operand (e.g., `REGISTER`, `POINTER`, `NUMBER`). It handles
 * different cases for registers, pointers, and numbers, returning an appropriate integer
 * or a constant for cases like `NO_OP`.
 *
 * @param type_op The type of the operand (e.g., `REGISTER`, `POINTER`, `NUMBER`, `NO_OP`).
 * @param op The operand as a string (e.g., "r3" for a register or "#5" for a number).
 * @return The integer value of the operand, or a constant (e.g., `NO_OP`) if applicable.
 */
int get_Operand_Value(int type_op , char op[MAX_LABEL_SIZE])
{
    int result = 0;
    switch (type_op) {
        /* Convert register number to an integer (ignoring the 'r' prefix) */
        case REGISTER:
            result = atoi( &op[1]); /* Convert register number to integer */
            break;/* Example: "r3" -> 3 */
        /* Convert pointer number to an integer (ignoring the '*r' prefix) */
        case POINTER:
            result = atoi( &op[2]);/* Example: "*r2" -> 2 */
            break;
        /* Convert number to an integer (ignoring the '#' prefix) */
        case NUMBER:
            result =  atoi( (op+1) ); /* Example: "#5" -> 5 */
            break;
        case NO_OP:
            result = NO_OP; /* No operand provided, return NO_OP constant */
            break;
        default:  
            /* Handle any unrecognized operand types */
            result = NONE;        
    
    }
    return result;
}


/**
 * @brief Identifies the type of an operand in assembly code.
 *
 * This function analyzes an operand string and determines its type. It checks whether the operand 
 * is a register, pointer, number, or label based on its initial character and compares it against 
 * the list of known registers. If the operand is invalid, the function returns `NO_OP`.
 *
 * @param registers Array of register names used for operand identification.
 * @param operand The operand string (e.g., "r3", "*r2", "#5").
 * @return An integer representing the operand type (`REGISTER`, `POINTER`, `NUMBER`, `LABEL`, or `NO_OP` if invalid).
 */
int operand_Type_Identifier(const char **registers, char operand[MAX_LABEL_SIZE]){
    int i;
    int result = NO_OP;
    int flag = 0;
    
    /* Check if the first character is legal (must be a digit, letter, '*' for pointers, or '#' for numbers) */
    if( !isdigit(operand[0]) && !isupper(operand[0])  && !islower(operand[0]) && operand[0] != '*' && operand[0] != '#'){
        return NO_OP;/* Return NO_OP for invalid operands */
    }
    /* Determine the operand type based on the first character */
    switch (operand[0]){
        /* Case for register operands (e.g., "r3") */
        case 'r':
            for(i = 0 ; i < TOTAL_REGISTERS ; i++){
                if( ( strcmp(registers[i], operand) ) == 0 ){
                    result = REGISTER; /* Register operand */
                }
            }
            
            break;
        /* Case for pointer operands (e.g., "*r2") */
        case '*':
            for(i = 0 ; i < TOTAL_REGISTERS ; i++){
                if( ( strcmp(registers[i], operand + strlen("*" )) ) == 0 ){
                    result = POINTER; /* Pointer operand */
                    flag = 1;/* Set flag if valid pointer */
                }
                
            }
            if(flag == 0){
                result = NO_OP;/* Invalid pointer operand */
            }
            break;

        /* Case for number operands (e.g., "#5") */
        case '#':
            result = NUMBER; /* Number operand */
            break;

        /* Default case for label operands */
        default:
            result = LABEL;/* Label operand */
    }
    return result;/* Return the identified operand type */
}

/**
 * @brief Shifts a number left by a specified number of steps.
 *
 * This function shifts the bits of an unsigned short integer left by a specified number 
 * of steps. This is equivalent to multiplying the number by 2 raised to the power of the 
 * number of steps.
 *
 * @param num The number to shift.
 * @param steps The number of positions to shift the bits.
 * @return The result of shifting the bits of `num` left by `steps` positions.
 */
unsigned short shift_Bits(unsigned short num,unsigned short steps)
{
    return (num << steps);/* Perform left bit shift */
}



/**
 * @brief Performs the first pass over the assembly file to process labels, directives, and commands.
 *
 * This function reads the assembly source file line by line, extracting labels, directives, and commands.
 * It performs checks on the syntax of labels, `.entry`, `.extern`, `.string`, `.data`, and commands. 
 * The information is stored in the ASSEMBLER_TABLE, including labels, instructions, and machine code commands.
 * 
 * If the first pass completes without errors, it triggers the second pass.
 *
 * @param assembler Pointer to the ASSEMBLER_TABLE that stores labels, commands, instructions, etc.
 * @param file_name The name of the assembly source file.
 * @param file_name_no_ending The base name of the file without the file extension (used for the second pass).
 */
void firstpass(ASSEMBLER_TABLE **assembler, char *file_name ,char * file_name_no_ending)
{
    FILE *fptr = NULL;
    char line[MAX_FILE_LINE_LENGTH], label[MAX_LABEL_SIZE];
    int IC = 100, DC = 1, skip = 0, line_counter = 1,label_skip = 0 , type = 0;
    BOOLEAN error_flag = TRUE , error = TRUE; /* Flags to track errors during the first pass */

    /* Register names for operand identification */
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

    /* Pointer to hold the current command being processed */
    COMMAND *command = NULL;

    /* Command table with supported assembly commands */
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

    memset(label, '\0', sizeof(label));
    memset(line, '\0', sizeof(line));

    /* Open the am file */
    fptr = fopen(file_name, "r");
    if(fptr == NULL)
    {
        print_error(FAILED_TO_OPEN_FILE , NO_LINE);
        return;
    }
   
   /* Process each line of the am file */
    while( fgets(line , MAX_FILE_LINE_LENGTH , fptr) != NULL)
    {   
        /* Extract the label (if any) from the line */
        skip = extract_Label(line, label, ':' , line_counter);
        if(skip != NO_LABEL && skip != ERROR)
        {
            error += examine_Label(label, line_counter);/* Check the validity of the label */
            if(error >= FALSE){
                type = IS_ERROR;
            }
            
        }

         /* If no label error, determine the line type (directive or command) */
        if (error == TRUE && skip != ERROR)
        {
            label_skip = skip;
            type = get_Line_Type(line + skip, commands_table, &skip, &command);
        }
        
        /* Process the line based on its type (command, directive, etc.) */
        switch(type)
        {
            case IS_ENTRY:
                /* Handle .entry directive */
                if (label_skip != NO_LABEL)
                {
                    /* Add the label to the label list with IC + DC */
                    insert_Label_List(&((*assembler)->label_head), label, IC + DC ); /* Might cause an error (*assembler)->label_head */
                }
                /* Examine the .entry directive for errors */
                error += Entry_Examine(line + label_skip +strlen(".entry"), line_counter , *assembler , registers);
                if(error == TRUE){
                    /* Add entry to the entry list */
                    insert_Entry_List(&((*assembler)->entry_head), line + skip,line_counter);
                }
                break;
                

            case IS_EXTERN:
                /* Handle .extern directive */
                if (label_skip != NO_LABEL)
                {
                    /* Add the label to the label list with IC + DC */
                    insert_Label_List(&((*assembler)->label_head), label, IC + DC ); /* Might cause an error (*assembler)->label_head */
                }
                error += Extern_Examine(line + label_skip +strlen(".extern"), line_counter , *assembler , registers);
                if(error == TRUE){
                    /* Add extern to the extern list */
                    insert_Extern_List(&((*assembler)->extern_head), line + skip , line_counter);
                }
                break;

            case IS_STRING:
                /* Handle .string directive */
                if (label_skip != NO_LABEL)
                {
                    insert_Label_List(&((*assembler)->label_head), label, IC + DC - 1); /* Might cause an error (*assembler)->label_head */
                }
                /* Examine the .string directive for errors */
                error += String_Examine(line + label_skip + strlen(".string"), line_counter);
                if(error == TRUE){
                    /* Extract the string data into the instruction list */
                    extract_String(&((*assembler)->instruction_head), line + skip, &DC);
                }
                break;

            case IS_DATA:
                /* Handle .data directive */
                if (label_skip != NO_LABEL)
                {
                    /* Add the label to the label list */
                    insert_Label_List(&((*assembler)->label_head), label,IC + DC - 1); /* Might cause an error (*assembler)->label_head */
                }
                /* Examine the .data directive for errors */
                error += Data_Examine(line + label_skip + strlen(".data") , line_counter);
                if(error == TRUE){
                    /* Extract the data values into the instruction list */
                    extract_Data(&((*assembler)->instruction_head), line + skip, &DC);
                }
                break;

            case IS_COMMAND:
                /* Handle assembly commands */
                if (label_skip != NO_LABEL)
                {
                    /* Add the label to the label list with IC */
                    insert_Label_List(&((*assembler)->label_head), label, IC ); /* Might cause an error (*assembler)->label_head */
                }
                /* Examine the command for errors */
                error += Command_Examine(command , line + label_skip + strlen(command->name), line_counter,registers);
                if(error == TRUE){
                    /* Convert the command into machine code */
                    convert_Command(&((*assembler)->command_head), registers, line + skip, command, &IC , line_counter);
                }
                break;

            case IS_ERROR:
                /* Error case: Skip further processing if the line has been marked as an error */
                break;

            default:
                /* Handle undefined instructions (lines that don't match any valid directive or command) */
                if(line[0] != '\n'){
                    printf("Line %d: undefined instruction\n", line_counter);
                }

        }

        /* Reset variables for the next line */
        label_skip = 0;
        error_flag += error;
        error = TRUE;
        line_counter++;
        command = NULL;
        memset(label, '\0', sizeof(label));
        memset(line, '\0', sizeof(line));
    }
    /* Close the file after processing */
    fclose(fptr);
    fptr = NULL;

    /* If no errors were encountered, proceed to the second pass */
    if(error_flag == TRUE){
        Second_Pass(assembler ,file_name_no_ending , IC , DC);
    }
}

