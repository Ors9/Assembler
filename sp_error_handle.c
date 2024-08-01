#include "assembler.h"

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
int check_if_label_exist(LABEL * head_label,int addr,char * label){
    int result = FAIL;

    /* Traverse the label list to check for the label's existence */
    while (head_label != NULL) {
         /* Check if the current label matches the searched label and is either defined or external */
        if( (strcmp(head_label->label , label) == 0)  && ((head_label->define_flag == DEFINE) || (head_label->type == EXTERN)) ){
            result = SUCCESS;/* Label found and valid */
        }

        head_label = head_label->next_label; /* Move to the next label in the list */
    }

    /* If the label is not found, print an error message */
    if(result == FAIL){
        printf("Address %d: Label %s does not exit\n",addr , label);
    }

    return result;
}