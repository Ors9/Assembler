#include "assembler.h"

/**
 * @brief Adds a new macro to the macro list.
 * 
 * This function allocates memory for a new macro node and appends it to the end 
 * of the macro list. The macro contains a name and content linked list.
 * 
 * @param head Pointer to the head of the macro list.
 * @param macro_name Name of the macro to be added.
 * @param head_content Pointer to the content of the macro.
 */
void add_to_macro_list(MACRO_LIST ** head ,char * macro_name ,MACRO_CONTENT * head_content){
    MACRO_LIST * ptr = NULL;
    /* Allocate memory for a new macro node */
    MACRO_LIST * new_node = generic_malloc(sizeof(MACRO_LIST));
    memset(new_node->macro_name , '\0',MAX_FILE_LINE_LENGTH);
    /* Copy the macro name to the new node */
    strcpy( new_node->macro_name , macro_name);
    new_node->head_content = head_content;
    new_node->next = NULL;

    /* If the macro list is empty, set the new node as the head */
    if(*head == NULL){
        *head = new_node;
        return;
    }
    /* Traverse the list to append the new node at the end */
    ptr = *head;
    while(ptr->next != NULL){
        ptr = ptr->next;
    }
    ptr->next = new_node;
}

/**
 * @brief Adds a new line of content to the macro content list.
 * 
 * This function allocates memory for a new content node and appends a line of 
 * content to the end of the macro content list.
 * 
 * @param head_content Pointer to the head of the content list.
 * @param line_to_add Line of text to be added to the content list.
 */
void add_content_to_list(MACRO_CONTENT ** head_content , char *line_to_add){
    MACRO_CONTENT * ptr = NULL;
    /* Allocate memory for a new content node */
    MACRO_CONTENT * content_to_add = generic_malloc(sizeof(MACRO_CONTENT));
    memset(content_to_add->line , '\0' , sizeof(content_to_add->line));
    /* Copy the line of content to the new node */
    strcpy(content_to_add->line , line_to_add);
    content_to_add->next = NULL;
    /* If the content list is empty, set the new node as the head */
    if( *head_content == NULL){
        *head_content = content_to_add;
        return;
    }
    /* Traverse the content list to append the new node at the end */
    ptr = *head_content;
    while( ptr->next != NULL){
        ptr = ptr->next;
    }
    ptr->next = content_to_add;
}


/**
 * @brief Removes whitespace characters from a line of text.
 * 
 * This function removes all unnecessary spaces and tabs from a line, except when 
 * the line is inside quotation marks ("). It handles line comments by terminating 
 * the string when a semicolon is encountered.
 * 
 * @param line Pointer to the line of text to be cleaned.
 * @return Pointer to the cleaned line of text (newly allocated memory).
 */
char * remove_white_spaces(char * line)
{
    int i , j , k;
    char  * new_line = generic_malloc(MAX_FILE_LINE_LENGTH);
    j = 0;
    memset(new_line , '\0', MAX_FILE_LINE_LENGTH);
    /* Loop through the characters of the line */
    for(i = 0 ; i < (int)strlen(line) ; i++){
        /* Handle quotes: copy everything within quotes as-is */
        if(line[i] == '"'){
            new_line[j] = line[i];
            j++;
            i++;
            k = strlen(line)-1;
            while(k > i  && (line[k] != '"'  )){
                k--;
            }
            if(i != k){
                while(i <= k){
                    new_line[j] = line[i];
                    i++;
                    j++;
                }
            }

        }
        /* Handle semicolon , break the line and add '\n' */
        if(line[i] == ';'){
            new_line[j] = '\n';
            j++;
            i = strlen(line);
        }
        /* Skip spaces and tabs, add other characters to new_line */
        if(line[i] != ' ' && line[i] != '\t'){
            new_line[j] = line[i];
            j++;
        }
    }
    new_line[j] = '\0';/* Null-terminate the new line */
    return new_line;
}


/**
 * @brief Finds a macro by name in the macro list.
 * 
 * This function searches for a macro in the list by comparing the macro name 
 * with the provided name. If found, it returns the macro; otherwise, it returns NULL.
 * 
 * @param head Pointer to the head of the macro list.
 * @param name_to_find The name of the macro to search for.
 * @return Pointer to the found macro or NULL if not found.
 */
MACRO_LIST * find_macro(MACRO_LIST * head , char * name_to_find){
    /* Traverse the macro list to find the matching macro name */
    while(head != NULL){
        /* Compare the macro name with the current list node */
        if(strcmp(head->macro_name , name_to_find) == 0){
            return head;
        }
        head = head->next;
    }
    return NULL;/* Return NULL if macro not found */
}

/**
 * @brief Determines the type of a line in the assembler code.
 * 
 * This function categorizes the line into one of the following types:
 * - MACRO_DECLARATION
 * - END_MACR
 * - EMPTY_LINE
 * - MACRO_CALL
 * - NONE (default, if the line does not match any of the previous categories)
 * 
 * @param line The original line of text.
 * @param head Pointer to the macro list head.
 * @param line_clean Cleaned line without whitespaces.
 * @return The type of the line as an integer value.
 */
int line_type(char * line , MACRO_LIST *head , char *line_clean){
     /* Check if the line is a macro declaration */
    if(strncmp(line , "macr" , strlen("macr")) == 0){
        return MACRO_DECLARATION;
    }
    /* Check if the line marks the end of a macro */
    if(strncmp(line , "endmacr" , strlen("endmacr")) == 0){
        return END_MACR;
    }
    /* Check for empty line */
    if(line[0] == '\0' || line[0] == '\n'){
        return EMPTY_LINE;
    }
    /* Check if the line calls an existing macro */
    if(find_macro(head , line_clean) != NULL){
        return MACRO_CALL;
    }
    /* Default case if no conditions are met (ordinary line)*/
    return NONE;
}

/**
 * @brief Handles the pre-processing phase of the assembler.
 * 
 * This function reads an assembly source file, processes macros, and writes the 
 * processed content to a new file with ".am" suffix. It also handles macro 
 * declaration and calls, while reporting errors and cleaning up as necessary.
 * 
 * @param head_table Pointer to the assembler table containing macros and other information.
 * @param file_name Name of the source file to process.
 */
void Pre_Proc(ASSEMBLER_TABLE ** head_table , char *file_name){
    char * file_am ,* file_as,  line[MAX_FILE_LINE_LENGTH] ,* line_clean , macro_name[MAX_FILE_LINE_LENGTH];
    FILE * fptr_am , * fptr_as;
    int type = NONE ,  line_count = 1;
    BOOLEAN error_flag = TRUE , final_result = TRUE;
   
    MACRO_CONTENT *head_content = NULL;
    MACRO_LIST * head_macro= NULL;
  
    memset(line , '\0',sizeof(line));
    memset(macro_name , '\0',sizeof(macro_name));
    if(File_Name_Check(file_name) == FALSE){
        return;
    }
    /* Add file suffixes for the input and output files */
    file_as = add_suffix(file_name , ".as");
    file_am = add_suffix(file_name , ".am");

    /* Open the source file for reading */
    fptr_as  = fopen(file_as , "r");

    if(fptr_as == NULL){
        print_error(FAILED_TO_OPEN_FILE , NO_LINE);
        exit(1);
    }
    /* Open the output file for writing */
    fptr_am = fopen(file_am , "w");
    if(fptr_am == NULL){
        print_error(FAILED_TO_OPEN_FILE , NO_LINE);
        exit(1);
    }

    /* Main loop to scan the source file line by line */
    while(fgets(line , sizeof(line) , fptr_as) != NULL){
        line_clean = remove_white_spaces(line);
        /* Determine the type of the line */
        type = line_type(line_clean , ((*head_table)->macro_head) , line_clean);
        
        switch(type){
            case MACRO_DECLARATION:
                /* Check macro name validity and handle error flags */
                error_flag += Macro_Name_Check(line_clean + strlen("macr"),  ((*head_table)->macro_head) , line_count);
                if(error_flag >= FALSE){
                    final_result = FALSE;
                }
                /*Copy the macro name*/
                if(error_flag == TRUE){
                    strcpy(macro_name , (line_clean + strlen("macr")) ); 
                }
                free(line_clean);
                line_clean = NULL;
                memset(line , '\0',sizeof(line));
                /* Save macro content until endmacr is found */
                while(fgets(line , sizeof(line) , fptr_as) != NULL && type != END_MACR){
                    line_count++;
                    line_clean = remove_white_spaces(line);

                    type = line_type(line_clean , ((*head_table)->macro_head) , line_clean);
                   
                    if(type == END_MACR){
                         /* Check for the validity of the macro end line */
                        error_flag += End_Macr_Check(line_clean + strlen("endmacr") , line_count);
                        
                        if(error_flag >= FALSE){
                            final_result = FALSE;
                        }
                       
                        break;
                    }
                    /* Add the line to the macro is valid */
                    if(error_flag  == TRUE){
                        add_content_to_list( &head_content , line_clean);
                    }
                    
                    memset(line , '\0',sizeof(line));
                    free(line_clean);
                    line_clean = NULL;
                }
                /* Add the macro to the macro list if no errors occurred */
                if(error_flag == TRUE){
                    add_to_macro_list( &((*head_table)->macro_head) , macro_name ,head_content);
                }
                
                break;

            case MACRO_CALL:
                /* Handle macro call by writing its content to the output file */
                head_macro = find_macro((*head_table)->macro_head , line_clean);
                if(head_macro != NULL){
                    head_content =  head_macro->head_content;
                    while(head_content != NULL){
                        fprintf(fptr_am ,"%s" ,head_content->line );
                        head_content = head_content->next;
                    }
                }
                break;

            case EMPTY_LINE:
                /* Skip empty lines */
                break;  

            case NONE:
                /* Write the ordinary line from the source to the output file */
                fprintf(fptr_am ,"%s" ,line_clean);
                break;  
        }

        line_count++;
        memset(line , '\0',sizeof(line));
        memset(macro_name , '\0',sizeof(macro_name));
        free(line_clean);
        line_clean = NULL;
        error_flag = TRUE;
    }
    line_count = 0;
    free(line_clean);
    line_clean = NULL;

    free(file_as);
    file_as = NULL;
    fprintf(fptr_am ,"%s" ,"\n");

    fclose(fptr_am);
    fclose(fptr_as);
   
    /* If there was an error, remove the output file */
    if(final_result == FALSE){
        if( remove(file_am) != 0 ){  
            print_error(FAILED_TO_REMOVE_FILE , NO_LINE);
            exit(1);
        }
    }
    
    /* If everything is successful, proceed to the first pass of the assembler */
    if(final_result == TRUE){
        firstpass(head_table, file_am , file_name );
    }

    free(file_am);
    file_am = NULL;

}