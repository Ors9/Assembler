#ifndef PRE_PROCESSOR_H/*header guards*/
#define PRE_PROCESSOR_H

#include "assembler.h"

/*-------------------Pre proc function declaration start------------------------*/

/**
 * @brief Create a new line of macro content.
 * 
 * This function allocates memory for a new Macro_content struct,
 * copies the provided line into the struct, and initializes the 
 * next_line pointer to NULL.
 *
 * @param line A string containing the line to be added to the macro content.
 * @return Macro_content* A pointer to the newly created Macro_content node.
 */
Macro_content * create_new_content(char line[MAX_FILE_LINE_LENGTH]);
/**
 * @brief Add a line to the macro content list.
 * 
 * This function creates a new node containing the given line and 
 * adds it to the end of the linked list starting at head.. If the list is empty, the new node becomes the head.
 *
 * @param line A string containing the line to be added.
 * @param head A double pointer to the head of the macro content linked list.
 */
void add_to_macro_content(char line[MAX_FILE_LINE_LENGTH], Macro_content ** head);
/**
 * @brief Create a new macro with a given name and content.
 * 
 * This function allocates memory for a new Macro_list struct,
 * sets the macro name and content, and initializes the next_macro
 * pointer to NULL.
 *
 * @param macro_name A string containing the name of the macro.
 * @param macro_value A pointer to the content of the macro.
 * @return Macro_list* A pointer to the newly created Macro_list node.
 */
Macro_list * create_macro(char * macro_name , Macro_content * macro_value);
/**
 * @brief Add a macro to the macro list.
 * 
 * This function adds a new Macro_list node to the end of the list
 * pointed to by head. If the list is empty, the new node becomes the head.
 *
 * @param head A double pointer to the head of the macro list.
 * @param node_to_add A pointer to the macro to be added.
 */
void add_macro_to_list( Macro_list ** head , Macro_list * node_to_add);
/**
 * @brief Find a macro in the list of macros by its name.
 * 
 * This function searches through the list of macros for a macro with
 * a name matching name_to_find. If found, a pointer to the macro is returned.
 * Otherwise, NULL is returned.
 *
 * @param name_to_find A string containing the name of the macro to find.
 * @param head A pointer to the head of the macro list.
 * @return Macro_list* A pointer to the found Macro_list node, or NULL if not found.
 */
Macro_list * find_macro(char *name_to_find , Macro_list * head);
/**
 * @brief Finds the last double-quote (") in a string.
 *
 * Scans the string `line` from end to start, returning the index of the last
 * double-quote (`"`). Returns `0` if no quote is found.
 *
 * @param line Input string to be scanned, with a maximum length of `MAX_FILE_LINE_LENGTH`.
 * 
 * @return int Index of the last double-quote, or `0` if none is found.
 */
int quoto_case(char line[MAX_FILE_LINE_LENGTH]);
/**
 * @brief Count the quotes in the string
 *
 * Irritate over the line and count the quotes
 *
 * @param line Input string to be scanned, with a maximum length of `MAX_FILE_LINE_LENGTH`.
 * 
 * @return the qunatity of quotes.
 */
int count_quote(char line[MAX_FILE_LINE_LENGTH]);
/**
 * @brief Removes all tab and space characters from a given string.
 *
 * This function processes the input string `line` by removing all tab and space
 * characters. If a semicolon `;` is encountered, the function terminates the string
 * at that point by adding a newline character and stops further processing.
 *
 * @param line The input string to be processed. The string is modified in place.
 */
void  remove_tab_and_spaces(char line[MAX_FILE_LINE_LENGTH]);
/**
 * @brief The preprocessor phase to process the input file, expanding macros, and writing the output file.
 *
 * This function processes the input file line by line, identifying macro definitions and uses.
 * Macro definitions are added to a list, while uses are replaced by the corresponding macro content.
 * The processed content is written to the output file.
 *
 * @param as_file A string containing the input file name.
 * @param am_file A string containing the output file name.
 * @param macro_list A double pointer to the head of the macro list.
 * @return int Returns SUCCESS (0) if successful, otherwise returns error code.
 */
int  preprocessor_phase(char * file,char * file2 ,  Macro_list ** macro_list);
/*pre proc error examine start*/
/**
 * @brief Examines a macro name to ensure it meets all the required criteria.
 *
 * This function checks the macro name length, ensures it does not start with a digit,
 * and that it consists only of alphanumeric characters and underscores. It also checks
 * if the name conflicts with existing commands, registers, or macro names.
 *
 * @param macro_name_line A string containing the macro name to be checked.
 * @param line_count The current line number in the source file.
 * @param head A pointer to the head of the macro list.
 * @return int Returns SUCCESS (0) if the macro name is valid, otherwise FAIL (1).
 */
int macro_examine(char * macro_name_line , int line_count , Macro_list * head);
/**
 * @brief Checks for extraneous text after the end of a macro.
 *
 * This function checks if there is any text after the "endmacr" line in the source file.
 * If such text is found, it prints an error message.
 *
 * @param endmacr_line A string containing the line after "endmacr".
 * @param line_count The current line number in the source file.
 * @return int Returns SUCCESS (0) if there is no extraneous text, otherwise FAIL (1).
 */
int end_macro_examine(char * line , int );
/**
 * @brief Checks if a macro name already exists in the list.
 *
 * This function iterates through the list of macros and checks if the provided
 * macro name already exists. If it does, it prints an error message.
 *
 * @param head A pointer to the head of the macro list.
 * @param macro_name A string containing the macro name to be checked.
 * @param line_count The current line number in the source file.
 * @return int Returns SUCCESS (0) if the macro name is unique, otherwise FAIL (1).
 */
int check_if_macro_name_already_exit(Macro_list * head , char * macro_name , int line_count);
/*pre proc error examine end*/
/*----------------Pre proc function declaration end---------------------*/

#endif
