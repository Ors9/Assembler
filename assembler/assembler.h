#ifndef ASSEMBLER_H /*header guards*/
#define ASSEMBLER_H

/*---Standart librarys declaration---*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*------------Macro defenitions start----------*/

#define MAX_ASCII_VALUE 127 /*Max ascii value in the ascii table*/
#define TOTAL_REGISTERS 8 /* Number of registers in the system */
#define TOTAL_COMMANDS 16/* Number of supported commands */
#define MAX_LABEL_SIZE 32/* Maximum size for labels */
#define TOTAL_INSTRUCTIONS 4/* Number of instruction types */
#define SINGLE_OP 13 /* Code for commands with a single operand */
#define DOUBLE_OP 4 /* Code for commands with two operands */
#define BITS 15 /* Number of bits in the code */
#define MASK8 8 /* 8-bit mask */
#define MASK128 128 /* 128-bit mask */
#define NUMBERS 0 /* Code for number type */
#define LABELS 1   /* Code for label type */
#define POINTERS 2 /* Code for pointers type */
#define REGISTERS 3 /* Code for registers type */
#define OPCODE 11 /* Position of opcode in binary code */
#define NO_OP -1 /* Value for no operand instructions */
#define DEFINE 1/* Value for defined */
#define NOT_DEFINE 0 /* Value for not defined */
#define NONE 0 /* Undefined value */
#define ERROR -1  /* Error value */
#define DATA 0 /* Data type */
#define STRING 1  /* String type */
#define ENTRY 2 /* Entry type */
#define EXTERN 3 /* External type */
#define A 4   /* Value for 'A' operand */
#define R 2  /* Value for 'R' operand */
#define E 1  /* Value for 'E' operand */
#define ARE_SHIFT 3 /* Position of ARE in binary code */
#define MAX_FILE_LINE_LENGTH 81 /* Maximum length of a file line */
#define MAX12_BITS 2048 /* Maximum value for 12 bits */
#define MIN12_BITS -2047/* Minimum value for 12 bits */
#define MAX15_BITS 32767 /* Maximum value for 15 bits */
#define MIN15_BITS -16384 /* Minimum value for 15 bits */
#define MIN_SIZE_OF_DATA 2  /*Missing parameter in data*/
#define MIN_SIZE_OF_STRING 3  /*Missing parameter in string*/

/*------------Macro defenitions end----------*/


/*-----------Definnition of structure data start--------------*/

/**
 * Struct for representing a line of macro content.
 */
typedef struct Macro_content {
    char line[MAX_FILE_LINE_LENGTH]; /* Line content */
    struct Macro_content * next_line; /* Pointer to the next line in the macro */
} Macro_content;

/**
 * Struct for representing a macro with its name and content.
 */
typedef struct Macro_list {
    char  macro_name [MAX_FILE_LINE_LENGTH];  /* Macro name */
    Macro_content * macro_content; /* Macro content */
    struct Macro_list * next_macro; /* Pointer to the next macro in the list */
} Macro_list;

/**
 * Struct for representing a command with a name and a code.
 */
typedef struct {
    const char *name;/* Name of the command */
    const int code; /* Code of the command */
}Command;

/**
 * Struct for representing a label with its name, address, type, and define flag.
 */
typedef struct LABEL{
    char label[MAX_LABEL_SIZE]; /* Label name */
    int addr;  /* Address of the label */
    int type; /* Type of the label */
    int define_flag; /* Flag indicating if the label has been defined */
    struct LABEL * next_label; /* Pointer to the next label in the list */
} LABEL;

/**
 * Enum for boolean values used for success or failure.
 */
typedef enum boolean{
     SUCCESS, /* Operation succeeded */
     FAIL /* Operation failed */
}boolean;





/**
 * Struct for representing binary machine code using bit fields 
 *(i will use only 15 bits every system have at least 16 bits as unsign short
 * that way it will be machine independed)
 * 
 */
typedef struct Mila{
    unsigned short bit_field; /* Binary code in 15 bit field */
}Mila;


/**
 * Struct for representing machine code with address, binary code, source and destination labels, and types for command section.
 */
typedef struct MachineCode{
    int  addr;/* Address of the machine code */
    Mila * binary_code; /* Binary code */
    char label_src[MAX_LABEL_SIZE];/* Source label */
    char label_dest[MAX_LABEL_SIZE];/* Destination label */
    int type_src; /* Type of the source operand */
    int type_dest; /* Type of the destination operand */
    struct MachineCode * next_entry; /* Pointer to the next machine code entry */
}MachineCode;

/**
 * Struct for representing machine code with address, binary code, source and destination labels, and types for data section.
 */
typedef struct MachCodeData{
    int addr;/* Address of the data */
    Mila * binary_code;/* Pointer to the binary code */
    struct MachCodeData *next;/* Pointer to the next data entry in the list */
}MachCodeData;

/**
 * Enum for various error numbers used in the program 
 * represent as index of the error in the error_messages array.
 */
typedef enum error_num{
    FAILED_TO_OPEN_FILE  ,
    MEMORY_ALLOCATION_FAILED,
    FAILED_TO_REMOVE_FILE,
    INVALID_MACRO_NAME,
    EXTRANOUS_TEXT_AFTER_END_OF_MACRO,
    EXTRANOUS_TEXT_AFTER_MACRO_CALL,
    EXCEEDED_MACRO_NAME,
    INVALID_LABEL_NAME,
    ILLEGAL_LABEL_NAME_LENGTH,
    INVALID_MULTIPLE_COMMAS,
    MISSING_COMMA,
    ILLEGAL_COMMAND,
    EXTRANOUS_TEXT_AFTER_STOP_COMMAND,
    EXTRANOUS_TEXT_AFTER_RTS_COMMAND,
    ILLEGAL_OPERAND,
    MISSING_OPERAND,
    INVALID_NUMBER,
    INVALID_OPERAND_NAME,
    MACRO_NAME_ALREADY_DEFINED,
    UNDEFINED_INSTRUCTION,
    MISSING_QUOTE,
    MISSING_STRING,
    ILLEGAL_CHARACTER,
    NOT_A_LABEL,
    MISSING_PARAMETER,
    IS_NOT_A_NUMBER,
    ILLEGAL_COMMA,
    MISSING_NUMBER,
    MISSING_MACRO_NAME,
    NUMBER_OUT_OF_BOUND,
    ALREADY_DEFINED_AS_EXTERN,
    ALREADY_DEFINED_AS_ENTRY,
    MISSING_LABEL,
    FILE_LENGTH_EXCEED

}error_num;

/**
 * Enum for command names used in the instruction set each of
 * them represent the command code and index of the commands_table array.
 */
typedef enum Command_names{
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

}Command_names;




/*-----------Definnition of structure data end--------------*/

/*----------general funcs declaration start-------------------*/

/**
 * @brief Retrieves the name of a register.
 * 
 * This function returns the name of a register from the global registers array based on the provided index.
 *
 * @param i The index of the register in the registers array.
 * @return const char* A string containing the name of the register.
 */
const char * get_register(int i);

/**
 * @brief Retrieves the name of an instruction type.
 * 
 * This function returns the name of an instruction type from the global instruction_types array based on the provided index.
 *
 * @param i The index of the instruction type in the instruction_types array.
 * @return const char* A string containing the name of the instruction type.
 */
const char * get_instruction_type(int i);

/**
 * @brief Retrieves a command from the commands table.
 * 
 * This function returns a pointer to a Command structure from the global commands_table array based on the provided index.
 *
 * @param i The index of the command in the commands_table array.
 * @return Command* A pointer to the Command structure.
 */
Command * get_command(int i);

/**
 * @brief Prints an error message with line number.
 * 
 * This function prints an error message corresponding to the given error number along with the line number where the error occurred.
 *
 * @param error The error number, corresponding to an index in the error_messages array.
 * @param line_count The line number where the error occurred.
 */
void print_error_message(error_num error, int line_count);

/**
 * @brief Prints a critical error message and exits the program.
 * 
 * This function prints an error message corresponding to the given error number and then exits the program.
 *
 * @param error The error number, corresponding to an index in the error_messages array.
 */
void print_critical_error_message(error_num error);

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
int cmp_with_reserved_words(char * line , error_num error_message , int);
/**
 * @brief Allocate memory and handle errors if the allocation fails.
 *
 * This function attempts to allocate a specified amount of memory and checks if the allocation
 * was successful. If not, it prints an error message and exits the program.
 *
 * @param size The size of memory to allocate.
 * @return void* A pointer to the allocated memory.
 */
void * generic_malloc(long size); 
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
char * add_file(char * file_name , char * suffix);
/**
 * @brief Free the linked list of macro content nodes.
 *
 * This function iterates through the linked list of macro content nodes and frees each node.
 * 
 * @param head A pointer to the head of the macro content linked list.
 */
void free_macro_content(Macro_content **head);
/**
 * @brief Free the linked list of macro list nodes.
 *
 * This function iterates through the linked list of macro list nodes and frees each node along with its content.
 * 
 * @param head A pointer to the head of the macro list.
 */
void free_macro_list(Macro_list **head);
/**
 * @brief Free the linked list of label nodes.
 *
 * This function iterates through the linked list of label nodes and frees each node.
 * 
 * @param head A pointer to the head of the label list.
 */
void free_label_list(LABEL ** head);
/**
 * @brief Free the linked list of machine code nodes.
 *
 * This function iterates through the linked list of machine code nodes and frees each node.
 * It also frees the memory allocated for the binary code within each machine code node.
 * 
 * @param head A pointer to the head of the machine code linked list.
 */
void free_machine_code(MachineCode **head);

/**
 * @brief Free the linked list of MachCodeData nodes.
 *
 * This function iterates through the linked list of MachCodeData nodes and frees each node.
 * It also frees the memory allocated for the binary code within each MachCodeData node.
 * 
 * @param head A pointer to the head of the MachCodeData linked list.
 */
void free_mach_code_data(MachCodeData **head);

/*--------------general funcs declaration end------------------*/

#endif
