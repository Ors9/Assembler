#include "assembler.h"

/**
 * @brief Adds an address to the address list.
 * 
 * This function dynamically allocates memory for a new address node and appends it 
 * to the end of the address list.
 * 
 * @param addr_list Pointer to the head of the address list.
 * @param addr The address to be added.
 */
void add_addr_to_list(ADDRESS_LIST ** addr_list ,int addr){
    ADDRESS_LIST * new_node = generic_malloc(sizeof(ADDRESS_LIST));
    ADDRESS_LIST * ptr = NULL;
    new_node->addr = addr;
    new_node->next = NULL;

    /* If the address list is empty, set the new node as the head */
    if( *addr_list == NULL){
        *addr_list = new_node;
        return;
    }
    /* Traverse the list and append the new node at the end */
    ptr = *addr_list;
    while(ptr->next != NULL){
        ptr = ptr->next;
    }
    ptr->next = new_node;

}

/**
 * @brief Checks if a label exists in the label or extern list.
 * 
 * This function searches for a given label in the label list and extern list. If the label is 
 * found, it returns TRUE. If the label is not found, it prints an error and returns FALSE.
 * 
 * @param head_label Pointer to the head of the label list.
 * @param head_extern Pointer to the head of the extern list.
 * @param addr The line number for error reporting.
 * @param label The label name to check.
 * @return BOOLEAN indicating if the label exists (TRUE) or not (FALSE).
 */
BOOLEAN Check_If_Label_Exist(LABEL_LIST * head_label,EXTERN_LIST * head_extern , int addr,char * label){
    BOOLEAN result = FALSE;

    /* Search for the label in the label list */
    while (head_label != NULL) {
        if( (strcmp(head_label->label , label) == 0)){
            result = TRUE;/* Label found */
        }

        head_label = head_label->next; 
    }
    /* Search for the label in the extern list */
    while (head_extern != NULL) {
        if( (strcmp(head_extern->label , label) == 0)){
            result = TRUE;/* Label found */
        }

        head_extern = head_extern->next; 
    }

    /* If the label was not found, print an error */
    if(result == FALSE){
        printf("Line: %d Label is not defined , and the label name: %s \n",addr , label);
    }

    return result;
}

/**
 * @brief Handles the second pass of the assembler.
 * 
 * This function performs the second pass on the assembler table, which involves resolving labels 
 * and updating machine code instructions. It handles both label resolution and extern handling, 
 * updating the machine code accordingly. If the second pass is successful, it proceeds to the 
 * translation phase.
 * 
 * @param assembler_table Pointer to the assembler table.
 * @param file_name The name of the file being processed.
 * @param IC Instruction count.
 * @param DC Data count.
 */
void Second_Pass(ASSEMBLER_TABLE ** assembler_table , char * file_name , int IC , int DC){

    LABEL_LIST * ptr_label = (*assembler_table)->label_head;
    EXTERN_LIST * ptr_extern = (*assembler_table)->extern_head;
    ENTRY_LIST * ptr_entry = (*assembler_table)->entry_head;
    MACHINE_CODE_COMMAND * ptr_mach_code = (*assembler_table)->command_head;
    BOOLEAN error_flag = TRUE;

    /* Iterate through the machine code list and resolve labels */
    while(ptr_mach_code != NULL){
         /* Check if the mila is marked for the second pass */
        if(ptr_mach_code->binary_code.mila == MARK_FOR_SECOND_PASS){
            /* Verify if the label exists in either the label or extern list */
            error_flag += Check_If_Label_Exist((*assembler_table)->label_head ,(*assembler_table)->extern_head ,ptr_mach_code->addr,ptr_mach_code->label);
            
            /* Check if the label is in the label list */
            while(ptr_label != NULL){
                if(strcmp(ptr_label->label ,ptr_mach_code->label) == 0){
                    ptr_mach_code->binary_code.mila = R;
                    ptr_mach_code->binary_code.mila |= shift_Bits(ptr_label->addr , 3);
                }

                ptr_label = ptr_label->next;
            }
            
            /* Check if the label is in the extern list */
            while(ptr_extern != NULL){
                if(strcmp(ptr_extern->label ,ptr_mach_code->label) == 0 ){
                    ptr_mach_code->binary_code.mila = E;
                    add_addr_to_list( &(ptr_extern->addr_list) ,ptr_mach_code->addr );
                }
                ptr_extern = ptr_extern->next;
            }
            
        }

        /* Reset pointers for the next iteration */
        ptr_entry = (*assembler_table)->entry_head;
        ptr_extern = (*assembler_table)->extern_head;
        ptr_label = (*assembler_table)->label_head;
        ptr_mach_code = ptr_mach_code->next;
    }

    /* Handle entry labels */
    ptr_label = (*assembler_table)->label_head;
    while(ptr_label != NULL){
        while(ptr_entry != NULL){
            if(strcmp(ptr_entry->label ,ptr_label->label) == 0 ){
                add_addr_to_list( &(ptr_entry->addr_list) ,ptr_label->addr );
            }
            ptr_entry = ptr_entry->next;
        }
        /* Reset entry pointer for the next label */
        ptr_entry = (*assembler_table)->entry_head;
        ptr_label = ptr_label->next;
    }

    /* If no errors were found, proceed to the translation phase */
    if(error_flag == TRUE){
        translationunit( assembler_table , file_name , IC , DC);
    }

}