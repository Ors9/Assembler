#include "assembler.h"
#include "first_phase.h"
#include "second_phase.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Add a new label to the label list.
 * 
 * This function creates a new LABEL node and appends it to the linked list.
 * If the list is empty, the new node becomes the head.
 * 
 * @param head Double pointer to the head of the LABEL list.
 * @param label A null-terminated string containing the label to be added.
 * @param addr The address associated with the label.
 * @param type The type of the label (e.g., DATA, CODE).
 * @param definition The definition status of the label (e.g., defined or not).
 */
void add_to_label_list(LABEL **head , char label[MAX_LABEL_SIZE] , int addr , int type, int definition){
    LABEL * label_to_add= NULL;
    LABEL * temp = *head;
    
    
    label_to_add = generic_malloc(sizeof(LABEL));/* Allocate memory for new label */
    label_to_add->addr = addr;/* Set the address */
    label_to_add->type = type;/* Set the type */
    label_to_add->define_flag = definition;/* Set the definition flag */
    strcpy(label_to_add->label , label);/* Copy the label name */
    label_to_add->next_label = NULL;
    
    if( *head == NULL){
        *head = label_to_add; /* If list is empty, new label becomes head */
    }
    else{
        while( temp->next_label != NULL ){ /* Find the end of the list */
            temp = temp->next_label;
        }
        temp->next_label = label_to_add; /* Add new label to the end */
    }

    return;  

}




/**
 * @brief Extract a label from a line of code.
 * 
 * This function extracts a label from the provided line, stopping at the first colon (`:`).
 * The function allocates memory for the label and returns it. If no label is found, NULL is returned.
 * 
 * @param line The input line from which the label is to be extracted.
 * @return char* A pointer to the extracted label (null-terminated string), or NULL if no label is found.
 */
char * label_extractor( char *line ){
    char  * label = generic_malloc(MAX_LABEL_SIZE*sizeof(char));
    int j  , flag = 0;
    int i;
    

    memset(label, '\0', MAX_LABEL_SIZE);/* Initialize the label with null characters */

    for(i = 0 , j = 0; i < strlen(line) && j < MAX_LABEL_SIZE && flag != 1 ; i++ , j++){
        if(line[i] != ':'){
            label[j] = line[i];/* Extract label characters */
        }
        else{
            flag = 1;/* Stop extraction at colon */
        }
    }
    label[j] = '\0';
    if(flag == 1){
        return label;
    }

    return NULL; /* Return NULL if no label is found */
}



/**
 * @brief Finds a command in the commands table.
 * 
 * This function searches for a command in the global commands table by comparing the command name 
 * from the input line with the names stored in the commands table.
 *
 * @param line The input line containing the command to be searched for.
 * @return Command* A pointer to the found Command structure, or NULL if the command is not found.
 */
Command * find_command(char *line){
    int i;
    
    for(i = 0 ; i < TOTAL_COMMANDS ; i++){
        if(strncmp( (get_command(i))->name , line,strlen((get_command(i))->name)) == 0){
            return get_command(i); /* Return the found command */
        }
        
    }
    
    return NULL; /* Return NULL if no command is found */
}


/**
 * @brief Extract an operand from the line.
 * 
 * This function extracts an operand from the given line, stopping at a comma or newline character.
 *
 * @param line The line containing the operand.
 * @param operand The buffer to store the extracted operand.
 * @return int The length of the extracted operand plus one, or 0 if no operand is found.
 */
int  operand_extractor(char * line , char * operand){
    int i ,  j , flag = 0;

    for(i = 0 , j = 0; i < strlen(line) && j < MAX_LABEL_SIZE && flag != 1 ; i++ , j++){
        if(line[i] != ',' && line[i] != '\n'){
            operand[j] = line[i]; /* Extract operand characters */
        }
        else{
            flag = 1; /* Stop extraction at comma or newline */
        }
    }
    operand[j] = '\0';
    if(flag == 1){
        return strlen(operand) + 1; /* Return the length of the operand plus one */
    }
    
    return 0; /* Return 0 if no operand is found */
}



/**
 * @brief Find an instruction in the instruction types array.
 * 
 * This function searches for an instruction in the global instruction_types array by comparing the instruction name.
 *
 * @param line The line containing the instruction.
 * @return int The index of the found instruction, or ERROR if not found.
 */
int find_instruction(char *line){
    int i;
    for(i = 0 ; i < TOTAL_INSTRUCTIONS ; i++){
        if(strncmp( get_instruction_type(i) , line,strlen(get_instruction_type(i))) == 0){
            return i; /* Return the index of the instruction in the table*/
        }
        
    }
    return ERROR; /* Return ERROR if no instruction is found */
}



/**
 * @brief Extract data values from a line for data, string, entry, and extern instructions.
 * 
 * This function extracts and processes data values from the given line, based on the instruction type.
 * - For `DATA`, it extracts integers.
 * - For `STRING`, it extracts characters and stores them as integer values.
 * - For `ENTRY` and `EXTERN`, it extracts labels and adds them to the label list.
 * 
 * @param line The input line containing the data values.
 * @param instruction The instruction type (e.g., DATA, STRING, ENTRY, EXTERN).
 * @param skipping_length The number of characters to skip at the start of the line.
 * @param head Double pointer to the head of the LABEL list.
 * @param counter_size Pointer to an integer to store the size of the extracted data array.
 * @return int* A pointer to the array of extracted data values (integers), or NULL if no data is found.
 */
int * data_extractor(char * line , int instruction , int skipping_length , LABEL ** head , int * counter_size){
    char * data_num = NULL;
    int i = 1;
    int * array_data = NULL;
    LABEL temp_label;

    memset(temp_label.label, '\0', sizeof(temp_label.label)); /* Initialize temporary label */
    switch(instruction){
                case DATA:
                    skipping_length = strlen(get_instruction_type(DATA));
                    data_num = strtok(line + skipping_length, ",");/* Extract first data value */
                    while(data_num != NULL){
                        array_data = realloc(array_data , i*sizeof(int)); /* Resize the data array */
                        array_data[i-1] = atoi(data_num);  /* Convert and store the data value */
                        i++;
                        data_num = strtok(NULL , ","); /* Extract the next data value */
                        
                    }
                    *counter_size = i-1;
                    
                    break;

                case STRING:
                    skipping_length = strlen(get_instruction_type(STRING));
                    for(i = 1 ; i < strlen(line+skipping_length) -2 ; i++){
                        array_data = realloc(array_data, i * sizeof(int));  /* Resize the data array */
                        if (array_data == NULL) {
                            print_critical_error_message(MEMORY_ALLOCATION_FAILED);
                            exit(1);
                        }
                        array_data[i-1] = line[skipping_length + i]; /* Store the character value */
                    
                    }
                    array_data = realloc(array_data, i * sizeof(int)); /* Resize the data array for null terminator */
                    array_data[i-1] = 0;  /* Add null terminator */
                    *counter_size = i;
                    
                    break;

                case ENTRY:
                    skipping_length = strlen(get_instruction_type(ENTRY));
                    /* Extract entry label */
                    strcpy(temp_label.label, strtok(line+skipping_length , "\n"));  
                    add_to_label_list(head , temp_label.label    , NONE , ENTRY , NOT_DEFINE);
                    
                    break;

                case EXTERN:
                    skipping_length = strlen(get_instruction_type(EXTERN));
                    /* Extract extern label */
                    strcpy(temp_label.label, strtok(line+skipping_length , "\n")); 
                    add_to_label_list(head , temp_label.label    , NONE , EXTERN , NOT_DEFINE);
                    break;
                    
                
    }
        
    return array_data;  
}



/**
 * @brief Create a Mila object with the given number as the bit field.
 * 
 * This function allocates memory for a Mila object and sets its bit field with the given number.
 *
 * @param number The number to be stored in the Mila object's bit field.
 * @return Mila* A pointer to the created Mila object.
 */
Mila * save_data(int  number){
    Mila * mila = generic_malloc(sizeof(Mila) ); /*Memory allocation*/
    mila->bit_field = number;
    return mila; /*A pointer to the created Mila object*/
}



/**
 * @brief Add a new machine code entry to the machine code list.
 * 
 * This function creates a new MachineCode node, representing a line of machine code, and appends it to the linked list.
 * If the list is empty, the new node becomes the head.
 * 
 * @param head Double pointer to the head of the MachineCode list.
 * @param add_to_list The Mila object containing the binary code to be added.
 * @param addr The address associated with the machine code.
 * @param label_src The source label associated with the machine code.
 * @param label_dest The destination label associated with the machine code.
 * @param type_src The type of the source operand (e.g., register, label).
 * @param type_dest The type of the destination operand.
 */
void add_to_machine_code_list(MachineCode ** head , Mila * add_to_list , int  addr , char label_src[MAX_LABEL_SIZE] ,char label_dest[MAX_LABEL_SIZE],int type_src,int type_dest){
    MachineCode * temp = *head;
    MachineCode * add_value = generic_malloc(sizeof(MachineCode)); /*Memory allocation to a new node*/
    add_value->addr = addr;
    add_value->binary_code =  add_to_list;
    add_value->next_entry = NULL;
    
    strcpy(add_value->label_dest,label_dest);  /*copy the label_dest to be the new node label_dest*/
    strcpy(add_value->label_src,label_src); /*copy the label_src to be the new node label_src*/
    add_value->type_dest = type_dest;
    add_value->type_src = type_src; 
    
     
    if(*head == NULL){  /* If list is empty, new entry becomes head */
        *head = add_value;
        return;
    }
    
    while(temp->next_entry != NULL){ /* Find the end of the list */
        temp = temp->next_entry;
    }

    temp->next_entry = add_value; /* Add new entry to the end */
    return;
}

/**
 * @brief Add a new machine code entry to the machine code list.
 * 
 * This function creates a new MachineCode node and appends it to the linked list.
 * If the list is empty, the new node becomes the head.
 *
 * @param head Double pointer to the head of the MachineCode list.
 * @param add_to_list The Mila object containing the binary code.
 * @param addr The address associated with the machine code.
 * @param label_src The source label associated with the machine code.
 * @param label_dest The destination label associated with the machine code.
 * @param type_src The type of the source operand.
 * @param type_dest The type of the destination operand.
 */
void add_to_mach_data_list(MachCodeData ** head , Mila * add_to_list, int  addr){
    MachCodeData * temp = *head;
    MachCodeData * add_value = generic_malloc(sizeof(MachCodeData)); /*Memory allocation to a new node*/
    add_value->addr = addr;
    add_value->binary_code =  add_to_list;
    add_value->next = NULL;
    
    if(*head == NULL){  /* If list is empty, new entry becomes head */
        *head = add_value;
        return;
    }
    
    while(temp->next != NULL){ /* Find the end of the list */
        temp = temp->next;
    }

    temp->next = add_value; /* Add new entry to the end */
    return;
}

/**
 * @brief Identify the type of an operand in a line of code.
 * 
 * This function determines whether an operand is a label, number, register, pointer, or none,
 * based on its format and the first character in the operand.
 * 
 * @param operand The operand to be examined.
 * @return int An integer representing the operand type, such as:
 *         - `NO_OP` for no operand
 *         - `REGISTERS` for register operands
 *         - `LABELS` for label operands
 *         - `POINTERS` for pointer operands
 *         - `NUMBERS` for numeric operands
 */
int operand_type_identifier(char operand[MAX_LABEL_SIZE]){
    int i;
    int result = NO_OP;
    int flag = 0;

    /*Check if the first character is legal*/
    if( !isdigit(operand[0]) && !isupper(operand[0])  && !islower(operand[0]) && operand[0] != '*' && operand[0] != '#'){
        return NO_OP;
    }

    switch (operand[0]){

        case 'r':
            for(i = 0 ; i < TOTAL_REGISTERS ; i++){
                if( ( strcmp(get_register(i), operand) ) == 0 ){
                    result = REGISTERS; /* Register operand */
                }
            }
            
            break;

        case '*':
            for(i = 0 ; i < TOTAL_REGISTERS ; i++){
                if( ( strcmp(get_register(i), operand + strlen("*" )) ) == 0 ){
                    result = POINTERS; /* Pointer operand */
                    flag = 1;
                }
                
            }
            if(flag == 0){
                result = NO_OP;/* Invalid pointer operand */
            }
            break;

        case '#':
            result = NUMBERS; /* Number operand */
            break;

        default:
            result = LABELS;/* Label operand */
    }
    return result;
}




/**
 * @brief Find the data type of an operand and convert it to an integer representation.
 * 
 * This function converts the operand type (register, pointer, number, etc.) to an integer
 * representation that can be used in machine code.
 * 
 * @param type_op The type of the operand (as determined by `operand_type_identifier`).
 * @param op The operand to be converted.
 * @return int The integer representation of the operand value, or an error code if the type is `NO_OP`.
 */
int find_data_type(int type_op , char op[MAX_LABEL_SIZE]){
    int result = 0;

    switch (type_op) {
        
        case REGISTERS:
            result = atoi( &op[1]); /* Convert register number to integer */
            break;

        case POINTERS:
            result = atoi( &op[2]);/* Convert pointer number to integer */
            break;

        case NUMBERS:
            result =  atoi( (op+1) ); /* Convert number to integer */
            break;
        case NO_OP:
            result = NO_OP; /* No operand */
            break;
        default:  
            result = NONE;        
    
    }
    return result;
}



/**
 * @brief Shift the bits of a number to the left by a specified number of steps.
 * 
 * This function performs a left bitwise shift on a number.
 *
 * @param num The number to be shifted.
 * @param steps The number of positions to shift the bits.
 * @return unsigned short The result of the bitwise shift.
 */
unsigned short move_bits(unsigned short num,unsigned short steps){
    return (num << steps);
}


/**
 * @brief Convert command operands to machine code and add to the machine code list.
 * 
 * This function processes the source and destination operands, converts them to machine code,
 * and adds them to the machine code list. The conversion handles different cases, such as:
 * - Both operands are registers or pointers.
 * - A single operand is present.
 * - The operand is a label, number, or other type.
 * 
 * @param head Double pointer to the head of the MachineCode list.
 * @param head_label Double pointer to the head of the LABEL list.
 * @param src_op The source operand.
 * @param dest_op The destination operand.
 * @param IC Pointer to the instruction counter.
 */
void convert_command_to_mach_code(MachineCode ** head ,LABEL **head_label , char src_op[MAX_LABEL_SIZE] , char dest_op[MAX_LABEL_SIZE] ,int * IC){
    int type_src  = operand_type_identifier(src_op); /*convert the src_op to its number type*/
    int type_dest = operand_type_identifier(dest_op); /*convert the dest_op to its number type*/
    int result_src = find_data_type(type_src, src_op); /* convert it to an integer representation.*/
    int result_dest = find_data_type(type_dest  , dest_op);  /* convert it to an integer representation.*/
    
    Mila * mila_one= NULL;
    Mila * mila_two = NULL;

    if( (type_src == REGISTERS || type_src == POINTERS) && (type_dest == REGISTERS || type_dest == POINTERS) ){ /* Two register/pointer operands */
         /* Case: Both operands are registers/pointers so we have only one of machine code line*/
        mila_one = generic_malloc(sizeof(Mila));/* Allocate memory for the Mila object that will represent the binary code */
        mila_one->bit_field = A;  /* Set A, R, E bits (absolute addressing) */
        mila_one->bit_field |= move_bits(result_dest , 3); /* Set bits for destination operand */
        mila_one->bit_field |= move_bits(result_src , 6); /* Set bits for source operand */
        /*Add the machine code to the list*/
        add_to_machine_code_list(head , mila_one , *IC , src_op,dest_op,type_src,type_dest);
        (*IC)++;
        
    }
    else if(type_src != NO_OP && type_dest == NO_OP ){ /* Case: Single operand */
        mila_one = generic_malloc(sizeof(Mila));/* Allocate memory for the Mila object that will represent the binary code */
        if(type_src == LABELS){
            /* Case: Source operand is a label */
            mila_one->bit_field = NONE; /*Initialize to zero in the first phase (will handle in second phase)*/
            add_to_label_list(head_label , src_op  , *IC , NONE , NOT_DEFINE);/* Add label to label list */
            add_to_machine_code_list(head , mila_one , *IC, src_op,dest_op,type_src,type_dest);/*Add the machine code to the list*/
        }
        else{
            /* Case: Source operand is a register, number, or pointer */
            mila_one->bit_field = A; /* Set A, R, E bits */
            mila_one->bit_field |= move_bits(result_src , 3); /* Set bits for source operand */
            add_to_machine_code_list(head , mila_one , *IC , src_op,dest_op,type_src,type_dest);/*Add the machine code to the list*/
        }
    
        (*IC)++;
    }
    else{
        /* Case: Handling the source operand cases  */
        if(type_src == REGISTERS || type_src == POINTERS ){ 
         /* Source operand is a register or pointer  */
            mila_one = generic_malloc(sizeof(Mila));
            mila_one->bit_field = A;  /* Set A, R, E bits */
            mila_one->bit_field |= move_bits(result_src, 6);/* Set bits for source operand */
            add_to_machine_code_list(head , mila_one , *IC , src_op,"\0",type_src,NONE);/*Add the machine code to the list*/
            
        }
        else if(type_src == NUMBERS){
            /* Source operand is a number */
            mila_one = generic_malloc(sizeof(Mila));/* Allocate memory for the Mila object that will represent the binary code */
            mila_one->bit_field = A;/* Set A, R, E bits */
            mila_one->bit_field |= move_bits(result_src, 3);/* Set bits for source operand */
            add_to_machine_code_list(head , mila_one , *IC , src_op,"\0",type_src,NONE);;/*Add the machine code to the list*/

        }
        else{/* Source operand is a label*/
            mila_one = generic_malloc(sizeof(Mila));/* Allocate memory for the Mila object that will represent the binary code */
            mila_one->bit_field = NONE; /*Initialize to zero in the first phase (will handle in second phase)*/
            add_to_label_list(head_label , src_op  , *IC , NONE , NOT_DEFINE);/*Add label to the list*/
            add_to_machine_code_list(head , mila_one , *IC , src_op,"\0",type_src,NONE);/*Add the machine code to the list*/
        }
        
        (*IC)++;
        
        /* Case: Handling the destination operand */
        if(type_dest == REGISTERS || type_dest == POINTERS || type_dest == NUMBERS ){ 
            /* Destination operand is a register, pointer, or number */
            mila_two = generic_malloc(sizeof(Mila));/* Allocate memory for the Mila object that will represent the binary code */
            mila_two->bit_field = A;/* Set A, R, E bits */
            mila_two->bit_field |= move_bits(result_dest, 3);/* Set bits for destination operand */
            add_to_machine_code_list(head , mila_two , *IC , "\0",dest_op,NONE,type_dest);/*Add the machine code to the list*/
            
        }
        else{
            /* Destination operand is a label */
            mila_two = generic_malloc(sizeof(Mila));/* Allocate memory for the Mila object that will represent the binary code */
            mila_two->bit_field = NONE;  /*Initialize to zero in the first phase (will handle in second phase)*/
            add_to_label_list(head_label , dest_op  , *IC , NONE , NOT_DEFINE);/*Add label to the list*/
            add_to_machine_code_list(head , mila_two , *IC , "\0",dest_op,NONE,type_dest);/*Add the machine code to the list*/
            
        }


        (*IC)++;
    }


}



/**
 * @brief Analyze and process a valid line of code in the first phase.
 * 
 * This function analyzes a line of code, determines if it's a command or instruction,
 * and processes it accordingly by extracting operands and converting them to machine code.
 *
 * @param valid_line The line of code to be processed.
 * @param head Pointer to the head of the macro list.
 * @param IC Pointer to the instruction counter.
 * @param DC Pointer to the data counter.
 * @param labels_head Pointer to the head of the LABEL list.
 * @param mach_code_head Pointer to the head of the MachineCode list.
 * @param label_skip The number of characters to skip for label extraction.
 */
void valid_line_examine(char * valid_line , Macro_list * head , int * IC,int * DC , LABEL ** labels_head  , MachineCode ** mach_code_head ,int label_skip , MachCodeData ** mach_data_head){
    int instruction = 0;
    Command * command = NULL;
    char src_op[MAX_LABEL_SIZE];
    char dest_op[MAX_LABEL_SIZE];
    int skipping_length = 0;
    int * array_data = NULL;
    int i;
    Mila * mila = NULL;
    int  counter_size = 0;
    int type_src;
    int type_dest;

    memset(src_op, '\0', sizeof(src_op)); 
    memset(dest_op,'\0', sizeof(dest_op));
    
    /** 
     * Skip the label part if it exists and start processing the actual command or instruction.
     * The label_skip parameter indicates how many characters to skip from the start of the line.
     */
    skipping_length = label_skip; 
    command = find_command(valid_line + skipping_length); 
        
    if(command != NULL){
        /*
         * Move the pointer forward by the length of the command name.
         * This prepares for extracting operands that follow the command.
         */
        skipping_length += strlen(command->name);

         /*
         * Extract source and destination operands if they exist. 
         * These operands are part of the machine code instruction.
         */
        skipping_length += operand_extractor( (valid_line + skipping_length) , src_op);/* Extract source operand */
        skipping_length += operand_extractor((valid_line + skipping_length) , dest_op);/* Extract destination operand */

        /*
         * Allocate memory for Mila structure, which represents a single line of machine code.
         * This will hold the bit-field representation of the instruction and its operands.
         */
        mila = generic_malloc(sizeof(Mila));
        mila->bit_field = A; /* Set A ,R,E bits*/
        mila->bit_field |= move_bits( command->code , OPCODE );/* Set the bits corresponding to the command's opcode */
            
        /* 
        * Handle the source and destination operands.
        * Determine their types and set the appropriate bits in the machine code.
        */
        type_dest = operand_type_identifier(dest_op);
        type_src = operand_type_identifier(src_op);
        if(type_src != NO_OP && type_dest == NO_OP){
            /* Only source operand is present, no destination operand */
            mila->bit_field |= move_bits(MASK8, type_src);
        }
        else{
            /* Both source and destination operands are present */
            mila->bit_field |= move_bits(MASK128, type_src);
            mila->bit_field |= move_bits(MASK8, type_dest);  
                    
        }

        
        /* Add machine code line for the command */
        add_to_machine_code_list(mach_code_head, mila, *IC , src_op,dest_op,type_src,type_dest);
        (*IC)++;/* Increment the instruction counter for the next line */

        /* Convert operands to machine code */
        if( strcmp(command->name , "stop") != 0 &&  strcmp(command->name , "rts") != 0 ){
            convert_command_to_mach_code(mach_code_head, labels_head ,src_op , dest_op , IC);
        }

    }       
    else{/* Handle instructions like DATA, STRING, ENTRY, and EXTERN */
           
        instruction = find_instruction(valid_line + skipping_length);
        array_data = data_extractor(valid_line + skipping_length, instruction , skipping_length , labels_head ,&counter_size);
        

        if(array_data != NULL){
            /** 
             * Process data array elements and add them to the machine code list.
             * This handles initializing memory with data values or string literals.
             */
            for(i = 0 ; i < counter_size ;i++){  
                mila = save_data(array_data[i]);
                add_to_mach_data_list(mach_data_head , mila , ( (*DC) + (*IC) ) );
                (*DC)++;
            }
                
            free(array_data);
            array_data = NULL;
                
        }
            
            
    }
    
    return;

}



/**
 * @brief The first phase of processing the input file, identifying labels and converting commands.
 * 
 * This function reads the input file line by line, identifying labels, commands, and instructions.
 * It then processes the lines accordingly, updating the machine code list and label list.
 *
 * @param file_am The name of the input file with the .am extension.
 * @param file_name_witout_suffix The base name of the input file without extension.
 * @param head Pointer to the head of the macro list.
 */
void first_phase(char * file_am , char * file_name_witout_suffix , Macro_list * head){
    FILE * fp;
    char line[MAX_FILE_LINE_LENGTH];
    int IC = 100;
    int DC = 0;
    int instruction = 0;
    LABEL * labels_head = NULL;
    Command * command = NULL;
    char src_op[MAX_LABEL_SIZE];
    char dest_op[MAX_LABEL_SIZE];
    char * label;
    int skipping_length = 0;
    MachineCode * mach_code_head = NULL;
    MachCodeData * mach_data_head = NULL;
    
    int line_count = 1;
    int flag_error = SUCCESS;
    int result_flag = SUCCESS;
    int label_skip = 0;

    /* Open the input file for reading */
    fp = fopen( file_am , "r");

    if(fp == NULL){
        print_critical_error_message(FAILED_TO_OPEN_FILE);
        return;/* Exit the program if the file cannot be opened */
    }
    
    memset(line, '\0', sizeof(line));
    memset(src_op, '\0', sizeof(src_op));
    memset(dest_op, '\0', sizeof(dest_op));
    
    /*
     * Read the file line by line.
     * Each line is processed to identify labels, commands, and instructions.
     */
    while(fgets(line, MAX_FILE_LINE_LENGTH, fp) != NULL){
        /**
         * Extract labels if present and prepare for further processing.
         * The skipping_length accounts for characters that should be ignored after label extraction.
         */
        label = label_extractor(line);
        if(label != NULL){
            skipping_length += strlen( label ) + 1;
            label_skip = strlen( label ) + 1;
            flag_error += label_examine(label , head , line_count);
        }
        
        if(flag_error == SUCCESS){
            /* Find command or instruction in the line */
            command = find_command(line + skipping_length); 
            instruction = find_instruction(line + skipping_length);

            if(command != NULL){
                /* Command examination */
                if(label != NULL){
                    add_to_label_list(&labels_head , label , IC , NONE , DEFINE);
                }
                
                flag_error += command_examine( line + skipping_length, line_count);
                if(flag_error == SUCCESS){
                    skipping_length += strlen(command->name);
                    flag_error += examine_command_cases(command, line + skipping_length , head, line_count);
                }
                
                
            }
            else if(instruction == DATA || instruction == STRING){
                if(label != NULL){
                    add_to_label_list(&labels_head , label , IC + DC , NONE , DEFINE);
                }
                
                /* Data or string examination */
                if(instruction == DATA){
                    flag_error += data_examine((line + strlen(get_instruction_type(DATA)) + skipping_length)  ,line_count);
                }
                else{
                    flag_error += string_examine((line +  strlen(get_instruction_type(STRING)) + skipping_length) ,  line_count);
                }
                
                
            }
            else if(instruction == ENTRY || instruction == EXTERN){
                /* Entry or extern examination */
                if(instruction == ENTRY){
                    flag_error += entry_or_extern_examine(line + strlen(get_instruction_type(ENTRY)) + skipping_length , line_count , head,labels_head,instruction);
                }
                else{
                    flag_error += entry_or_extern_examine(line + strlen(get_instruction_type(EXTERN)) + skipping_length , line_count , head,labels_head,instruction);
                }
            }
            else{
                /* Handle undefined instruction */
                if(line[0] != '\n'){
                    print_error_message(UNDEFINED_INSTRUCTION, line_count);
                    flag_error += FAIL;
                }
            }


            if(flag_error == SUCCESS){
                /* Process the line if no errors */
                valid_line_examine(line , head ,  &IC, &DC , &labels_head ,&mach_code_head,label_skip , &mach_data_head);
            }

        }
        
        line_count++;
        skipping_length = 0;
        label_skip = 0;
        memset(line, '\0', sizeof(line));
        memset(src_op, '\0', sizeof(src_op));  
        memset(dest_op , '\0' , sizeof(dest_op));


        result_flag += flag_error; /* Accumulate error flags */
        flag_error = SUCCESS;
        
    }
    
    fclose(fp);/* Close the input file */

    result_flag += check_for_label_repetition(labels_head);

    if(result_flag == SUCCESS){
        /* Proceed to the second phase if no errors */
        second_phase( &mach_code_head, labels_head , file_name_witout_suffix,IC - 100 ,DC , &mach_data_head );
    }

     /** Free allocated memory for labels and machine code lists **/
    free_label_list( &labels_head);
    free_machine_code( &mach_code_head);
    free_mach_code_data(&mach_data_head);
    labels_head = NULL;
    mach_code_head = NULL;

    return ;
}



