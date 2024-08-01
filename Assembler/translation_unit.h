#ifndef TRANSLATION_UNIT_H /*header guards*/
#define TRANSLATION_UNIT_H

#include "assembler.h"

/*---------Translation unit func  declaration start-------------------*/
/**
 * @brief Convert a decimal number to octal format.
 * 
 * This function converts a given decimal number to its octal representation,
 * stripping the sign bit if present, and formats the output as a string.
 *
 * @param binary_code The number to convert, expected to be a 16-bit integer.
 * @param addr The address associated with the binary code.
 * @return char* A dynamically allocated string containing the octal representation.
 */
char *  from_dec_to_oct(unsigned short binary_code , int addr);
/**
 * translation_unit - Finalize the assembly process by writing machine code and labels to files
 * @head_mach_code: Pointer to the head of the MachineCode list.
 * @head_label: Pointer to the head of the LABEL list.
 * @file_name_without_suffix: The base name of the input file without extension.
 * @IC: The final instruction counter value.
 * @DC: The final data counter value.
 * 
 * This function writes the assembled machine code and relevant label information
 * to output files. It handles object code, entry labels, and external labels.
 */
void translation_unit(MachineCode *head_mach_code, LABEL * head_label , char * file_name_without_suffix, int IC , int DC);
/*---------Translation unit func declaration end-----------*/

#endif