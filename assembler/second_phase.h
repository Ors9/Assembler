#ifndef SECOND_PHASE_H/*header guards*/
#define SECOND_PHASE_H

#include "assembler.h"

/*-------------second phase function declaration start------------------*/
/**
 * @brief Update the address of a label in the label list.
 * 
 * This function searches through the label list to find the specified label and update its address
 * based on its type. It sets the appropriate addressing mode (E for external, R for relocatable) 
 * and shifts the address as needed.
 *
 * @param label A string containing the label to update.
 * @param head_label Pointer to the head of the LABEL list.
 * @return int The updated address with the appropriate addressing mode.
 */
int update_label_address( char label[MAX_LABEL_SIZE] , LABEL *head_label);
/**
 * @brief The second phase of processing, handling label addresses and final translation.
 * 
 * This function processes the machine code list and updates labels with their addresses. 
 * It checks for any errors in label usage and, if successful, proceeds to translate the code.
 *
 * @param head_mach_code Pointer to the head of the MachineCode list.
 * @param head_label Pointer to the head of the LABEL list.
 * @param file_name_without_suffix The base name of the input file without extension.
 * @param IC Instruction counter value.
 * @param DC Data counter value.
 */
void second_phase(MachineCode ** head_mach_code , LABEL * head_label ,char *,int IC , int DC,MachCodeData **mach_data_head);
/*second phase error start*/
/**
 * @brief Check if a label exists in the label list.
 * 
 * This function searches through the label list to find if a specific label exists,
 * and whether it is defined or declared as external. If the label is not found, 
 * an error message is printed.
 *
 * @param head_label Pointer to the head of the LABEL list.
 * @param addr The address where the label was expected to be found.
 * @param label The label string to be searched.
 * @return int Returns SUCCESS (0) if the label is found, otherwise FAIL (1).
 */
int check_if_label_exist(LABEL * head_label,int addr,char * label);
/*second phase error end*/
/*---------------second phase declaration end--------------*/

#endif