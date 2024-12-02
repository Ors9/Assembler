#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*---------------Define  declaration start-------------------*/

/* Maximum number of characters allowed in a file line */
#define MAX_FILE_LINE_LENGTH 81

/* Maximum label size for symbols */
#define MAX_LABEL_SIZE 32

/* Line types for preprocessor */
#define NONE 0
#define MACRO_DECLARATION 1
#define END_MACR 2
#define EMPTY_LINE 3
#define MACRO_CALL 4

/* Command and register count */
#define TOTAL_COMMANDS 16
#define TOTAL_REGISTERS 8

/* Types of instructions in fp */
#define IS_ENTRY 10
#define IS_EXTERN 20
#define IS_STRING 30
#define IS_DATA 40
#define IS_COMMAND 50
#define UNDEFINED_INSTRUCTION 60
#define IS_ERROR 70

/* Entry and Extern type definitions */
#define ENTRY 0
#define EXTERN 1

/* Operand types */
#define LABEL 1
#define REGISTER 3
#define POINTER 2
#define NUMBER 0 
#define NO_OP -1

/* Label validation and error handling */
#define NO_LABEL 0
#define ERROR -1

/* Second pass marker */
#define MARK_FOR_SECOND_PASS 0

/* Bit values for command encoding */
#define E 1
#define R 2
#define A 4

/* OPCODE and mask definitions */
#define OPCODE 11
#define MASK128 128
#define MASK8 8

/* Value limits for data representation */
#define MAX_VALUE_FOR_12_BITS 2048
#define MIN_VALUE_FOR_12_BITS -2047
#define MAX_VALUE_FOR_15_BITS 32767 
#define MIN_VALUE_FOR_15_BITS -16384
#define MIN_DATA_SIZE 2
#define MIN_STR_SIZE 2
#define MAX_ASCII_VALUE 127

/* Error messages */
#define NO_LINE -1
#define FAILED_TO_OPEN_FILE "Failed to open file"
#define MEMORY_ALLOCATION_FAILED "Failed to allocate memory"
#define FAILED_TO_REMOVE_FILE "Failed to remove file"
#define INVALID_MACRO_NAME "Invalid macro name"
#define CANNOT_BE_RESERVED_WORD "Name cannot be reserved word"
#define EXTRANOUS_TEXT_AFTER_END_OF_MACRO "Externous text after endmacr"
#define EXCEEDED_MACRO_NAME_LENGTH "Exceeded length for macro name"
#define INVALID_LABEL  "Invalid label"
#define ILLEGAL_LABEL_NAME_LENGTH "Exceeded length for label"
#define EXTRANOUS_TEXT_AFTER_STOP_COMMAND "Externous text after stop command"
#define EXTRANOUS_TEXT_AFTER_RTS_COMMAND "Externous text after rts command"
#define INVALID_OPERAND "Invalid operand"
#define MISSING_OPERAND "Missing operand"
#define INVALID_NUMBER "Invalid number"
#define INVALID_OPERAND_NAME "Invalid Operand name"
#define MACRO_NAME_ALREADY_DEFINED "Macro name has already been defined"
#define LABEL_ALREADY_DEFINED "Label has already been defined"
#define MISSING_QUOTE "Missing \""
#define MISSING_STRING "Missing string"
#define INVALID_CHARACTER "Invalid character"
#define NOT_A_LABEL "Not a label"
#define MISSING_PARAMETER "Missing parameter"
#define IS_NOT_A_NUMBER "Not a number"
#define INVALID_COMMA "Invalid comma"
#define MISSING_NUMBER "Missing number"
#define MISSING_MACRO_NAME "Missing macro name"
#define NUMBER_OUT_OF_BOUND "Number out of bounds"
#define ALREADY_DEFINED_AS_EXTERN "Already defined as extern"
#define ALREADY_DEFINED_AS_ENTRY "Already defined entry"
#define MISSING_LABEL "Missing label"
#define FILE_LENGTH_EXCEED "Exceeded length for file"

/*------------Define  declaration END----------------*/

/*---------------Data structures declaration start-----------------*/

/**
 * @brief Struct for macro content lines.
 */
typedef struct MACRO_CONTENT {
    char line[MAX_FILE_LINE_LENGTH]; /* Line of macro content */
    struct MACRO_CONTENT * next; /* Pointer to the next line */
} MACRO_CONTENT;

/**
 * @brief Struct for a macro in the macro list.
 */
typedef struct MACRO_LIST {
    char  macro_name [MAX_FILE_LINE_LENGTH]; /* Name of the macro */
    MACRO_CONTENT * head_content; /* Head of macro content */
    struct MACRO_LIST * next; /* Pointer to the next macro */
} MACRO_LIST;

/**
 * @brief Struct for labels in the label list.
 */
typedef struct LABEL_LIST{
    char label[MAX_LABEL_SIZE]; /* Label name */
    int addr;  /* Address of the label */
    struct LABEL_LIST * next; /* Pointer to the next label */
} LABEL_LIST;

typedef struct ADDRESS_LIST{
    int addr;/* Address */
    struct ADDRESS_LIST * next;/* Pointer to the next address */
}ADDRESS_LIST;

typedef struct ENTRY_LIST{
    char label[MAX_LABEL_SIZE]; /* Label name */
    ADDRESS_LIST * addr_list; /* List of addresses for this entry */
    struct ENTRY_LIST * next; /* Pointer to the next entry */
} ENTRY_LIST;

typedef struct EXTERN_LIST{
    char label[MAX_LABEL_SIZE]; /* Label name */
    ADDRESS_LIST * addr_list;/* List of addresses for this extern */
    struct EXTERN_LIST * next;  /* Pointer to the next extern */
} EXTERN_LIST;

/**
 * @brief Enum for boolean values (TRUE/FALSE).
 */
typedef enum BOOLEAN{
     TRUE, /* True value */
     FALSE /* False value */
}BOOLEAN;

/**
 * @brief Struct for machine code word.
 */
typedef struct MILA{
    unsigned short mila;   /* Binary code (mila) */
}MILA;


/**
 * @brief Struct for machine code commands.
 */
typedef struct MACHINE_CODE_COMMAND{
    int  addr; /* Address of the command */
    MILA  binary_code;  /* Binary code for the command */
    char label[MAX_LABEL_SIZE];  /* Label associated with the command */
    int type;  /* Type of command */
    struct MACHINE_CODE_COMMAND * next;  /* Pointer to the next command */
}MACHINE_CODE_COMMAND;

/**
 * @brief Struct for machine code instructions.
 */
typedef struct MACHINE_CODE_INSTRUCTION{
    int addr;/* Address of the instruction */
    MILA  mila;/* Binary code (mila) */
    struct MACHINE_CODE_INSTRUCTION *next;/* Pointer to the next instruction */
}MACHINE_CODE_INSTRUCTION;

/**
 * @brief Assembler table containing macro, label, entry, extern, and machine code data.
 */
typedef struct ASSEMBLER_TABLE {
    MACRO_LIST *  macro_head;  /* Head of macro list */
    LABEL_LIST *  label_head; /* Head of label list */
    ENTRY_LIST * entry_head; /* Head of entry list */
    EXTERN_LIST * extern_head; /* Head of extern list */
    MACHINE_CODE_COMMAND * command_head; /* Head of command list */
    MACHINE_CODE_INSTRUCTION * instruction_head; /* Head of instruction list */
} ASSEMBLER_TABLE;

typedef struct {
    const char *name;/* Name of the command */
    const int code;  /* Command code */
}COMMAND;

typedef enum Command_Codes{
    MOV,
    CMP,
    ADD,
    SUB,
    LEA,
    CLR,
    NOT,
    INC,
    DEC,
    JMP,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    STOP

}CODES;

/*-----Data structures declaration END------*/

/*-----------Function declaration pre processor--------------*/

/**
 * @brief Adds a new macro to the macro list.
 * 
 * @param head Pointer to the head of the macro list.
 * @param macro_name Name of the macro to be added.
 * @param head_content Pointer to the macro content.
 */
void add_to_macro_list(MACRO_LIST ** head ,char * macro_name ,MACRO_CONTENT * head_content);

/**
 * @brief Adds a new line of content to the macro content list.
 * 
 * @param head_content Pointer to the head of the macro content list.
 * @param line_to_add Line of content to add.
 */
void add_content_to_list(MACRO_CONTENT ** head_content , char *line_to_add);

/**
 * @brief Removes unnecessary whitespaces from a line.
 * 
 * @param line Line to be cleaned.
 * @return Cleaned line with whitespaces removed.
 */
char * remove_white_spaces(char * line);

/**
 * @brief Finds a macro by its name.
 * 
 * @param head Pointer to the head of the macro list.
 * @param name_to_find Name of the macro to find.
 * @return Pointer to the found macro or NULL if not found.
 */
MACRO_LIST * find_macro(MACRO_LIST * head , char * name_to_find);

/**
 * @brief Determines the type of a line (macro declaration, macro call, etc.).
 * 
 * @param line Line of code to analyze.
 * @param head Pointer to the macro list.
 * @param macro_name Name of the macro (if any).
 * @return The type of the line.
 */
int line_type(char * line , MACRO_LIST *head , char *macro_name);

/**
 * @brief Preprocesses a file by processing macros.
 * 
 * @param head_table Pointer to the assembler table.
 * @param file_name Name of the file to preprocess.
 */
void Pre_Proc(ASSEMBLER_TABLE ** head_table , char *file_name);


/*-----------Function declaration pre processor errors--------------*/

/**
 * @brief Checks for extraneous text after "endmacro".
 * 
 * @param line Line to check.
 * @param line_count Line number for error reporting.
 * @return TRUE if valid, FALSE otherwise.
 */
BOOLEAN End_Macr_Check(char * line , int line_count);

/**
 * @brief Checks if a macro name is valid.
 * 
 * @param line Line to check for the macro name.
 * @param head Pointer to the macro list.
 * @param line_count Line number for error reporting.
 * @return TRUE if valid, FALSE otherwise.
 */
BOOLEAN Macro_Name_Check(char * line ,MACRO_LIST * head , int line_count);

/**
* @brief Check if file name is valid.
* @param name Name of the file
* @return TRUE if valid, FALSE otherwise.
 */
BOOLEAN File_Name_Check(char * name);

/*---------Function declaration first pass--------*/

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
int extract_Label(char *line, char *label, char delimiter , int line_counter);

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
void insert_Label_List(LABEL_LIST **label_list, char *label, int address);

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
int get_Line_Type(char *line, COMMAND *commands_table, int *skip, COMMAND **command);

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
void insert_Entry_List(ENTRY_LIST **entry_list, char *line , int line_counter);

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
void insert_Extern_List(EXTERN_LIST **extern_list, char *line ,  int line_counter);

/**
 * @brief Adds a new instruction to the MACHINE_CODE_INSTRUCTION list.
 *
 * This function adds a new instruction node, containing an address and a `MILA` value,
 * to the end of the linked list of machine code instructions. If the list is empty, 
 * the new node becomes the head.
 *
 * @param instruction_list Pointer to the head of the MACHINE_CODE_INSTRUCTION linked list.
 * @param address The address associated with the instruction.
 * @param mila The MILA (instruction representation) to store in the node.
 */
void add_to_instruction_list(MACHINE_CODE_INSTRUCTION **instruction_list , int address , MILA mila);

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
int extract_String(MACHINE_CODE_INSTRUCTION **instruction_list, char *line, int *address);

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
int extract_Data(MACHINE_CODE_INSTRUCTION **instruction_list, char *line, int *address);

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
int convert_Command(MACHINE_CODE_COMMAND **command_list, const char **registers, char *line, COMMAND *command, int *IC, int line_counter);

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
void insert_Command_List(MACHINE_CODE_COMMAND **command_list , int addr, char label[MAX_LABEL_SIZE] , MILA binary_code , int type);

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
void generate_Mila(MACHINE_CODE_COMMAND **command_list, CODES command_code, char *src_op, int type_src, char *dest_op, int type_dest, int *IC);

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
int get_Operand_Value(int type_op , char op[MAX_LABEL_SIZE]);

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
int operand_Type_Identifier(const char **registers, char operand[MAX_LABEL_SIZE]);

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
unsigned short shift_Bits(unsigned short num,unsigned short steps);

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
void firstpass(ASSEMBLER_TABLE **assembler, char *file_name ,char * file_name_no_ending);




/*------Function declaration first pass errors------------*/

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
BOOLEAN compare_With_Other_Lists(char *label, ASSEMBLER_TABLE *assembler);

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
BOOLEAN examine_Label(char *label, int line_counter);

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
BOOLEAN Command_Examine(COMMAND * command , char * line , int line_counter ,const char **registers);

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
BOOLEAN Zero_Op_Examine(char * line , int command_code , int line_count);

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
BOOLEAN Single_Op_Examine(char * line , int command_code ,  int line_counter ,const char ** registers);

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
BOOLEAN Double_Op_Examine(char * line , int command_code , int line_counter ,const char ** registers );

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
BOOLEAN Examine_Operand(char *operand,int type_op , int line_counter );

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
BOOLEAN String_Examine(char * line , int line_counter);

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
BOOLEAN Data_Examine(char * line , int line_counter);

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
BOOLEAN Valid_Num_Examine( char  * char_num , int line_counter);

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
BOOLEAN Extern_Examine(char * line , int line_counter , ASSEMBLER_TABLE *assembler ,const char **resgisters);

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
BOOLEAN Entry_Examine(char * line , int line_counter , ASSEMBLER_TABLE *assembler ,const char **resgisters);


/*------------------Function declaration second pass-------------------*/

/**
 * @brief Adds an address to an address list.
 *
 * This function allocates a new node and adds the given address
 * to the end of the address list.
 *
 * @param addr_list Pointer to the head of the address list.
 * @param addr Address to be added.
 */
void add_addr_to_list(ADDRESS_LIST ** addr_list ,int addr);

/**
 * @brief Checks if a label exists in the label or extern lists.
 *
 * This function checks if a label exists in either the label list or
 * the extern list. It also prints an error if the label is not found.
 *
 * @param head_label Pointer to the head of the label list.
 * @param head_extern Pointer to the head of the extern list.
 * @param addr Address for error reporting.
 * @param label Name of the label to check.
 * @return TRUE if the label exists, FALSE otherwise.
 */
BOOLEAN Check_If_Label_Exist(LABEL_LIST * head_label,EXTERN_LIST * head_extern , int addr,char * label);
/**
 * @brief Executes the second pass of the assembler.
 *
 * The second pass resolves labels and updates the machine code
 * instructions based on label addresses and externals. This
 * function also calls the translation unit if no errors occur.
 *
 * @param assembler_table Pointer to the assembler table.
 * @param file_name Name of the input file.
 * @param IC Instruction counter.
 * @param DC Data counter.
 */
void Second_Pass(ASSEMBLER_TABLE ** assembler_table , char * file_name , int IC , int DC);


/*--------------Function declaration in translation unit------------------------*/

/**
 * @brief Prints a binary code to a file in octal format.
 *
 * Converts the given binary code to octal format and prints it
 * to the specified file with its associated address.
 *
 * @param binary_code The binary code to print.
 * @param addr The address associated with the code.
 * @param fp_file Pointer to the output file.
 */
void print_dec_to_file_in_octal(unsigned short binary_code , int addr , FILE * fp_file);

/**
 * @brief Processes and writes the object file.
 *
 * Writes the binary code instructions and commands to the object (.ob)
 * file in octal format.
 *
 * @param instruction_list List of machine code instructions.
 * @param command_list List of machine code commands.
 * @param ob_file Name of the object file to write.
 * @param IC Instruction count.
 * @param DC Data count.
 */
void process_ob_file(MACHINE_CODE_INSTRUCTION *instruction_list , MACHINE_CODE_COMMAND * command_list ,char * ob_file, int IC , int DC);

/**
 * @brief Processes and writes the entry file.
 *
 * Writes the entry labels and their addresses to the entry (.ent) file.
 * If the entry file is empty, it is removed.
 *
 * @param ent_list List of entry labels.
 * @param ent_file Name of the entry file to write.
 */
void process_ent_file(ENTRY_LIST *ent_list ,char * ent_file);

/**
 * @brief Processes and writes the external file.
 *
 * Writes the external labels and their addresses to the external (.ext) file.
 * If the external file is empty, it is removed.
 *
 * @param ext_list List of external labels.
 * @param ext_file Name of the external file to write.
 */
void process_ext_file(EXTERN_LIST *ext_list ,char * ext_file);

/**
 * @brief Executes the translation unit.
 *
 * Calls the respective functions to process and write the object, entry,
 * and external files.
 *
 * @param assembler Pointer to the assembler table.
 * @param file_name Name of the input file.
 * @param IC Instruction count.
 * @param DC Data count.
 */
void translationunit(ASSEMBLER_TABLE ** assembler , char *file_name , int IC , int DC);

/*-------------Function declaration in Functions.c---------------*/

/**
 * @brief Checks if a word is a reserved word.
 *
 * This function checks if the given word is a reserved word and prints
 * an error message if so.
 *
 * @param line The word to check.
 * @param error_message Error message to print if the word is reserved.
 * @param line_count Line number for error reporting.
 * @return TRUE if the word is valid, FALSE if it's a reserved word.
 */
BOOLEAN Reserved_Words_Check(char * line , const char * error_message , int line_count);

/**
 * @brief Prints an error message with the line number.
 *
 * Prints the specified error message to stderr, with the given line number.
 *
 * @param error_message The error message to print.
 * @param line The line number where the error occurred.
 */
void print_error(const char *error_message, int line);

/**
 * @brief Frees memory allocated for the assembler table.
 *
 * This function frees all memory associated with the assembler table,
 * including macros, labels, entries, externals, and machine code.
 *
 * @param table Pointer to the assembler table.
 */
void free_assembler_table(ASSEMBLER_TABLE **table);

/**
 * @brief Frees memory allocated for machine code instructions.
 *
 * Frees the linked list of machine code instructions.
 *
 * @param head Pointer to the head of the machine code instruction list.
 */
void free_machine_code_instruction(MACHINE_CODE_INSTRUCTION **head);

/**
 * @brief Frees memory allocated for machine code commands.
 *
 * Frees the linked list of machine code commands.
 *
 * @param head Pointer to the head of the machine code command list.
 */
void free_machine_code_command(MACHINE_CODE_COMMAND **head);

/**
 * @brief Frees memory allocated for the extern list.
 *
 * Frees the linked list of extern labels and addresses.
 *
 * @param head Pointer to the head of the extern list.
 */
void free_extern_list(EXTERN_LIST **head);

/**
 * @brief Frees memory allocated for the entry list.
 *
 * Frees the linked list of entry labels and addresses.
 *
 * @param head Pointer to the head of the entry list.
 */
void free_entry_list(ENTRY_LIST **head);

/**
 * @brief Frees memory allocated for the address list.
 *
 * Frees the linked list of addresses.
 *
 * @param head Pointer to the head of the address list.
 */
void free_address_list(ADDRESS_LIST **head);

/**
 * @brief Frees memory allocated for the label list.
 *
 * Frees the linked list of labels.
 *
 * @param head Pointer to the head of the label list.
 */
void free_label_list(LABEL_LIST **head);

/**
 * @brief Frees memory allocated for the macro list.
 *
 * Frees the linked list of macros.
 *
 * @param head Pointer to the head of the macro list.
 */
void free_macro_list(MACRO_LIST **head);

/**
 * @brief Frees memory allocated for the macro content.
 *
 * Frees the linked list of macro content.
 *
 * @param head Pointer to the head of the macro content list.
 */
void free_macro_content(MACRO_CONTENT **head);

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
void * generic_malloc(long size);

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
char * add_suffix(char * file_name , char * ending);

