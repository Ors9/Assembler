#ifndef FIRST_PHASE_H/*header guards*/
#define FIRST_PHASE_H

#include "assembler.h"
/*----------------First phase function declaration start---------------------*/
/**
 * @brief Add a new label to the label list.
 * 
 * This function creates a new LABEL node and appends it to the linked list.
 * If the list is empty, the new node becomes the head.
 *
 * @param head Double pointer to the head of the LABEL list.
 * @param label The label string to be added.
 * @param ic The address associated with the label.
 * @param type The type of the label.
 * @param definition The definition status of the label.
 */
void add_to_label_list(LABEL **head , char label[MAX_LABEL_SIZE] , int ic , int type, int definition);

/**
 * @brief Extract a label from a line of code.
 * 
 * This function extracts a label from the provided line, stopping at the first colon (`:`).
 * The function allocates memory for the label and returns it. If no label is found, NULL is returned.
 * 
 * @param line The input line from which the label is to be extracted.
 * @return char* A pointer to the extracted label (null-terminated string), or NULL if no label is found.
 */

char * label_extractor( char *line );
/**
 * @brief Find a command in the commands table.
 * 
 * This function searches for a command in the global commands table by comparing the command name.
 *
 * @param line The line containing the command.
 * @return Command* A pointer to the found Command structure, or NULL if not found.
 */

Command * find_command(char *line );
/**
 * @brief Extract an operand from the line.
 * 
 * This function extracts an operand from the given line, stopping at a comma or newline character.
 *
 * @param line The line containing the operand.
 * @param operand The buffer to store the extracted operand.
 * @return int The length of the extracted operand plus one, or 0 if no operand is found.
 */
int  operand_extractor(char * line , char * operand);
/**
 * @brief Find an instruction in the instruction types array.
 * 
 * This function searches for an instruction in the global instruction_types array by comparing the instruction name.
 *
 * @param line The line containing the instruction.
 * @return int The index of the found instruction, or ERROR if not found.
 */
int find_instruction(char *line);
/**
 * @brief Extract data values from a line for data, string, entry, and extern instructions.
 * 
 * This function extracts and processes data values from the given line, based on the instruction type.
 *
 * @param line The line containing the data values.
 * @param instruction The instruction type.
 * @param skipping_length The number of characters to skip at the start of the line.
 * @param head Double pointer to the head of the LABEL list.
 * @param counter_size Pointer to an integer to store the size of the extracted data array.
 * @return int* A pointer to the array of extracted data values, or NULL if no data is found.
 */
int * data_extractor(char * line , int instruction , int skipping_length , LABEL ** head , int * counter_size);
/**
 * @brief Create a Mila object with the given number as the bit field.
 * 
 * This function allocates memory for a Mila object and sets its bit field with the given number.
 *
 * @param number The number to be stored in the Mila object's bit field.
 * @return Mila* A pointer to the created Mila object.
 */
Mila * save_data(int  number);
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
void add_to_machine_code_list(MachineCode ** head , Mila * add_to_list , int  addr , char label_src[MAX_LABEL_SIZE] ,char label_dest[MAX_LABEL_SIZE],int type_src,int type_dest);

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
void add_to_mach_data_list(MachCodeData ** head , Mila * add_to_list, int  addr);

/**
 * @brief Convert a command to its type number
 * 
 * This function determines the type number representation of a command based on its operand.
 *
 * @param operand The operand to be converted.
 * @return int The binary representation of the operand type.
 */
int operand_type_identifier(char operand[MAX_LABEL_SIZE]);
/**
 * @brief Find the data type of an operand and convert it to an integer representation.
 * 
 * This function converts the operand type to an integer representation based on the operand type.
 *
 * @param type_op The type of the operand.
 * @param op The operand to be converted.
 * @return int The integer representation of the operand value.
 */
int find_data_type(int type_op , char op[MAX_LABEL_SIZE]);

/**
 * @brief Shift the bits of a number to the left by a specified number of steps.
 * 
 * This function performs a left bitwise shift on a number.
 *
 * @param num The number to be shifted.
 * @param steps The number of positions to shift the bits.
 * @return unsigned short The result of the bitwise shift.
 */
unsigned short move_bits(unsigned short num,unsigned short steps);
/**
 * @brief Convert command operands to machine code and add to the machine code list.
 * 
 * This function processes the source and destination operands, converts them to machine code,
 * and adds them to the machine code list.
 *
 * @param head Double pointer to the head of the MachineCode list.
 * @param head_label Double pointer to the head of the LABEL list.
 * @param src_op The source operand.
 * @param dest_op The destination operand.
 * @param IC Pointer to the instruction counter.
 */
void convert_command_to_mach_code(MachineCode ** head ,LABEL **head_label , char src_op[MAX_LABEL_SIZE] , char dest_op[MAX_LABEL_SIZE] ,int * IC);

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
void valid_line_examine(char * valid_line , Macro_list * head , int * IC,int * DC , LABEL ** labels_head ,MachineCode ** mach_head,int label_skip , MachCodeData **mach_data_head);
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
void  first_phase(char * file_am , char *,Macro_list *);
/*First phase error examine start*/
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


int check_for_label_repetition(LABEL * labels_head );
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
int label_examine(char * label ,  Macro_list * head_macro , int line_count);
/**
 * @brief Validate a command in the command line.
 * 
 * This function checks if the command in the provided line is valid by
 * comparing it against a list of known commands. If the command is valid,
 * the function returns `SUCCESS`; otherwise, it prints an error message
 * and returns `FAIL`.
 *
 * @param line A string containing the command to be validated.
 * @param line_count The line number in the source file where the command was found.
 * @return int Returns SUCCESS (0) if the command is valid, otherwise FAIL (1).
 */
int command_examine(char * line  , int line_count);
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
int zero_op_examine(char * line , int command_code , int line_count);
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
int single_op_examine(char * line , int command_code , Macro_list * head, int line_count);
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
int double_op_examine(char * line , int command_code , Macro_list * head,int line_count );
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
int examine_command_cases(Command * command , char * line ,Macro_list *head , int );
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
int examine_operand(char *op,int type_op ,Macro_list * , int line_count);
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
int string_examine(char * line , int line_count);
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
int data_examine(char * line , int line_count);
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
int is_valid_number( char  * str , int line_count);
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
int entry_or_extern_examine(char * line , int line_count , Macro_list * head_macro,LABEL * head_label, int instruction);
/*First phase error examine end*/
/*----------------First phase function declaration end---------------------*/

#endif