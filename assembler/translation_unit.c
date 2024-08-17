#include "assembler.h"
#include "translation_unit.h"

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
char *  from_dec_to_oct(unsigned short binary_code , int addr){
    char * result = generic_malloc(MAX_FILE_LINE_LENGTH);/* Allocate memory for the result string */
    binary_code = binary_code &  (~(1 << ((sizeof(unsigned short) * 8) - 1)));/* Remove the sign bit if present */
    sprintf(result , "0%d\t%05o\n" , addr , binary_code );/* Format the result as an octal string */
    
    return result;
}




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
void translation_unit(MachineCode *head_mach_code, LABEL * head_label , char * file_name_without_suffix, int IC , int DC,MachCodeData *mach_data_head){
    char * line_ob;
    char * line_ext;
    char * line_ent;
    char * name_ob = add_file(file_name_without_suffix,".ob");/* Create the name for the object file */
    char * name_ext = add_file(file_name_without_suffix,".ext") ;/* Create the name for the external file */
    char * name_ent = add_file(file_name_without_suffix, ".ent");/* Create the name for the entry file */
    FILE * ptr_ob = fopen( name_ob , "w");/* Open the object file for writing */
    FILE * ptr_ext = fopen( name_ext , "w");/* Open the external file for writing */
    FILE * ptr_ent = fopen(name_ent , "w");/* Open the entry file for writing */
    int flag_ext = 0;/* Flag to check if any external labels were written */
    int flag_ent = 0;/* Flag to check if any entry labels were written */
    MachineCode * head_mach = head_mach_code;
    LABEL * temp = head_label;
    LABEL *ptr = head_label;

    /* Check if files are opened successfully */
    if(ptr_ent == NULL || ptr_ext == NULL || ptr_ob == NULL){
        print_critical_error_message(FAILED_TO_OPEN_FILE);
        return;
    }  
    
    /* Write header with IC and DC counts */
    fprintf(ptr_ob,"\n\t%d %d\n" , IC , DC);
    
    /* Write machine code to the .ob file */
    while (head_mach != NULL){
        line_ob = (from_dec_to_oct(head_mach->binary_code->bit_field, head_mach->addr) );
        fprintf(ptr_ob , "%s" ,line_ob);

        free(line_ob); /* Free the memory allocated for the line */
        line_ob = NULL;
        head_mach = head_mach->next_entry;/* Move to the next machine code entry */
    }

    while(mach_data_head != NULL){
        line_ob = (from_dec_to_oct(mach_data_head->binary_code->bit_field, mach_data_head->addr) );
        fprintf(ptr_ob , "%s" ,line_ob);

        free(line_ob); 
        line_ob = NULL;
        mach_data_head = mach_data_head->next;
    }

    fclose(ptr_ob);/* Close the object file */
    free(name_ob);/* Free the allocated memory for the file name */
    name_ob = NULL;

    /* Write entry labels to the .ent file */
    while(temp != NULL){
        
        if(temp->type == ENTRY && temp->addr == 0){/* Check for entry labels not yet resolved */
            ptr = head_label;
            while(ptr != NULL){
                
                if(strcmp(temp->label,ptr->label) == 0 && ptr->define_flag == DEFINE){/* Match label and definition */
                    line_ent = generic_malloc(MAX_FILE_LINE_LENGTH);
                    sprintf(line_ent,"%s\t0%d\n", ptr->label , ptr->addr );
                    fprintf(ptr_ent, "%s ", line_ent);

                    flag_ent = 1;/* Mark that an entry label has been written */
                    free(line_ent);/* Free the memory allocated for the line */
                    line_ent = NULL;
                }
                ptr = ptr->next_label;/* Move to the next label */
            }
        }    

        temp = temp->next_label;/* Move to the next label */
    }


    fclose(ptr_ent);/* Close the entry file */

    /* Remove the .ent file if no entries were written */
    if(flag_ent == 0){
        if( remove(name_ent) != 0 ){  
            perror("Error deleting the file");
        }
    }

    free(name_ent);/* Free the allocated memory for the file name */
    name_ent = NULL;

    /* Process external labels and write them to the .ext file */
    ptr = head_label;
    while(ptr !=NULL){

        if(ptr->type == EXTERN && ptr->addr == 0){/* Check for external labels not yet resolved */

            temp = head_label;

            while(temp != NULL){
                if((strcmp(ptr->label , temp->label) == 0) &&  temp->addr != 0){/* Match label and definition */
                    line_ext = generic_malloc(MAX_FILE_LINE_LENGTH);
                    sprintf(line_ext,"%s\t0%d\n", temp->label , temp->addr );
                    fprintf(ptr_ext, "%s ", line_ext);
                    flag_ext = 1;/* Mark that an external label has been written */

                    free(line_ext); /* Free the memory allocated for the line */
                    line_ext = NULL;
                }    
                temp = temp->next_label;/* Move to the next label */
            }

        }

        ptr = ptr->next_label;/* Move to the next label */
    }


    fclose(ptr_ext);/* Close the external file */

    /* Remove the .ext file if no external labels were written */
    if (flag_ext == 0) {
        if( remove(name_ext) != 0 ){  
            perror("Error deleting the file");
        }
    }

    free(name_ext);  /* Free the allocated memory for the file name */
    name_ext = NULL;
    
}
