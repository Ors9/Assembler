#include "assembler.h"
#include "second_phase.h"
#include "first_phase.h"
#include "translation_unit.h"

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
int update_label_address( char label[MAX_LABEL_SIZE] , LABEL *head_label){
    int result = 0;
    LABEL * temp = head_label;
    int temp_type = 0;

    

    /*Find the maximum type of the label to determine the appropriate addressing mode */
    while(temp != NULL){
        if(strcmp(temp->label , label) == 0){
            temp_type = (temp->type > temp_type) ? temp->type : temp_type;
        }

        temp = temp->next_label;
    }

    /* Update the address and addressing mode for the label */
    while(head_label != NULL){
        if( (strcmp(label , head_label->label)) == 0 && (head_label->define_flag == DEFINE || head_label->type == EXTERN)){
            if(temp_type == EXTERN){
                result = E;/* External addressing mode */
            }
            else{
                result = R;/* Relocatable addressing mode */
                result |= move_bits(head_label->addr , ARE_SHIFT);/* Shift address bits to A,R,E field*/
            }
        }

        head_label = head_label->next_label;
    }    

    return result;
}



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
void second_phase(MachineCode ** head_mach_code , LABEL * head_label , char * file_name_without_suffix , int IC , int DC){
    MachineCode * temp = *head_mach_code;
    int error_flag = SUCCESS;
    int final_result = SUCCESS;

    /** 
    * Iterate through the machine code list to resolve label addresses.
    * For each machine code entry, if the binary code's bit field is 0 and
    * the source or destination operand is a label, update the binary code
    * with the correct address and addressing mode.
    */
    while ( temp != NULL ) {
        /* Check if the current entry has a label that needs addressing */
        if(temp->binary_code->bit_field == 0 && (temp->type_src == LABELS || temp->type_dest == LABELS ) ){
            /* If the source operand is a label, update its address */
            if(temp->type_src == LABELS){
                error_flag = check_if_label_exist(head_label,temp->addr,temp->label_src);
                if(error_flag == SUCCESS){
                    temp->binary_code->bit_field = update_label_address( temp->label_src , head_label);
                }
            }
            /* If the destination operand is a label, update its address */
            if(temp->type_dest == LABELS){
                error_flag = check_if_label_exist(head_label,temp->addr,temp->label_dest);
                if(error_flag == SUCCESS){
                    temp->binary_code->bit_field = update_label_address( temp->label_dest , head_label);
                }
            }
        }
        /* Accumulate any errors found during the label update process */
        final_result += error_flag;
        error_flag = SUCCESS;
        /* Move to the next machine code entry */
        temp= temp->next_entry;
    }


    /** 
    * After processing all machine code entries, check if there were any errors.
    * If no errors occurred, proceed with translating the final machine code output.
    */
    if(final_result == SUCCESS){
        translation_unit(*head_mach_code,head_label ,file_name_without_suffix ,IC , DC);
    }

    return;
}