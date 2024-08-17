#include "assembler.h"
#include "pre_processor.h"



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
Macro_content * create_new_content(char line[MAX_FILE_LINE_LENGTH]){
    Macro_content * new_file = generic_malloc( sizeof(Macro_content) );
    strcpy(new_file->line, line); 
    new_file->next_line = NULL;
    return new_file;
}

/**
 * @brief Add a line to the macro content list.
 * 
 * This function creates a new node containing the given line and 
 * adds it to the end of the linked list starting at head.. If the list is empty, the new node becomes the head.
 *
 * @param line A string containing the line to be added.
 * @param head A double pointer to the head of the macro content linked list.
 */
void add_to_macro_content(char line[MAX_FILE_LINE_LENGTH], Macro_content ** head){
    Macro_content * new_line = create_new_content(line);
    Macro_content * temp = *head;
    if(*head == NULL){/*Check if the list is empty ,if so ,add the new object as the head*/
        *head = new_line;
        return;
    }
    while(temp->next_line != NULL){/*irrirate through all the list and add to end*/
        temp = temp->next_line;
    }
    temp->next_line = new_line; /* Link the new line to the end of the list */
    return;
}

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
Macro_list * create_macro(char * macro_name , Macro_content  * macro_value){
    Macro_list * new_macro = generic_malloc(sizeof(Macro_list ));
    strcpy(new_macro->macro_name , macro_name); /* Set the macro name */
    new_macro->macro_content = macro_value; 
    new_macro->next_macro = NULL;
    return new_macro;
}

/**
 * @brief Add a macro to the macro list.
 * 
 * This function adds a new Macro_list node to the end of the list
 * pointed to by head. If the list is empty, the new node becomes the head.
 *
 * @param head A double pointer to the head of the macro list.
 * @param node_to_add A pointer to the macro to be added.
 */
void add_macro_to_list( Macro_list ** head , Macro_list * node_to_add){
    Macro_list * temp = NULL;
    if( *head ==  NULL){ /*If the list is empty , set the new macro as the head*/
        *head = node_to_add;
        return;
    }
    temp = *head;
    while (temp->next_macro != NULL){/*Irritate to the last member of the list and add the new object to the end*/
        temp = temp->next_macro;
    }
    temp->next_macro = node_to_add; /* Link the new macro to the end of the list */
    return;  
}


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
Macro_list * find_macro(char *name_to_find , Macro_list * head){
    while( head != NULL ){/*Irritate through the list*/

        if(strcmp(name_to_find, head->macro_name) == 0){/*Check if the macro name matches the seatch query*/
            return head; /* Return the found macro */
        }

        head = head->next_macro; /* Return NULL if not found */
    }
    return NULL;
}





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
int quoto_case(char line[MAX_FILE_LINE_LENGTH]){
    int i = strlen(line) - 1;
    while(i > 0){
        if(line[i] == '"'){
            return i;
        }

        i--;
    }
    return i;
}

/**
 * @brief Count the quotes in the string
 *
 * Irritate over the line and count the quotes
 *
 * @param line Input string to be scanned, with a maximum length of `MAX_FILE_LINE_LENGTH`.
 * 
 * @return the qunatity of quotes.
 */
int count_quote(char line[MAX_FILE_LINE_LENGTH]){
    int i = 0;
    int quote_count  = 0;
    while(line[i] != '\0'){
        if(line[i] == '"'){
            quote_count++;
        }
        i++;
    }
    return quote_count ;
}


/**
 * @brief Removes all tab and space characters from a given string.
 *
 * This function processes the input string `line` by removing all tab and space
 * characters. If a semicolon `;` is encountered, the function terminates the string
 * at that point by adding a newline character and stops further processing.
 *
 * @param line The input string to be processed. The string is modified in place.
 */
void  remove_tab_and_spaces(char line[MAX_FILE_LINE_LENGTH]){
    char ch;
    char  new_line[MAX_FILE_LINE_LENGTH];
    int i = 0;
    int j = 0;
    int quote = count_quote(line);;

    
    memset(new_line, '\0', sizeof(new_line));/* Initialize new_line with null characters */

    while( ((ch = line[i]) != '\0') ){ /* Iterate through each character in the line */
        if(ch == '"' && quote >= 2){
           
           new_line[j] = ch;
           i++;
           j++;

           
           while(i < quoto_case(line)  ){/*Handle string case (dont want to delete spaces and tab inside)*/
                new_line[j] = line[i];
                i++;
                j++; 
           }
           

        }
             
        if(ch == ';'){ /* If a semicolon is found, terminate the line because the line is note*/
            new_line[j] = '\n';
            break;
        }
        if( ch != ' ' && ch != '\t'){ /* If the character is not a blank, add it to new_line */
            new_line[j] = ch;
            j++;
        }
        i++;
    }
    
    strcpy(line, new_line); /* Copy the processed content back to the original line */
    
    return;
}


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
int  preprocessor_phase(char * as_file , char * am_file , Macro_list ** macro_list){
    Macro_list * macro = NULL; 
    Macro_list * temp_macro = NULL;
    Macro_content * macro_content = NULL; 
    Macro_content *temp_macro_content = NULL;
    FILE * fp_as , * fp_am;
    char line[MAX_FILE_LINE_LENGTH];
    char macro_name[MAX_FILE_LINE_LENGTH];
    char macro_line[MAX_FILE_LINE_LENGTH];
    int flag_error = SUCCESS;
    int line_counter = 1;

    /* Initialize  with null characters */
    memset(line, '\0', sizeof(line));
    memset(macro_name, '\0', sizeof(macro_name));
    memset(macro_line, '\0', sizeof(macro_line));

    /* Open the input and output files */
    fp_as = fopen(as_file , "r"); /*Read from the as file*/

    if(fp_as == NULL){/* Check if the file opened correctly */
        print_critical_error_message(FAILED_TO_OPEN_FILE);
        return FAIL;
        
    }

    fp_am = fopen(am_file , "w"); /* Write to the am file */

    if(fp_am == NULL){/* Check if the file opened correctly */
        print_critical_error_message(FAILED_TO_OPEN_FILE);
        return FAIL;
        
    }

    /*
    * This loop iterates through each line of the assembly source file (`fp_as`) and processes
    * macros and regular lines accordingly. The macro definitions are identified, stored, and 
    * expanded in the output file (`fp_am`).
    */
    while(fgets(line , MAX_FILE_LINE_LENGTH , fp_as) != NULL){/* Read each line of the as file */
        remove_tab_and_spaces(line);/* Remove tab and spaces from a line */

        if(strncmp(line, "macr" , strlen("macr")) == 0 ){/*If in macro definition*/
            
            flag_error += macro_examine(line + strlen("macr") , line_counter,*macro_list);/*Error examine in macro definition*/
            strcpy(macro_name, line + strlen("macr"));/*Save the macro name*/

            while (fgets(macro_line, MAX_FILE_LINE_LENGTH, fp_as ) && strncmp(macro_line, "endmacr" , strlen("endmacr")) != 0) {
                remove_tab_and_spaces(macro_line);/*Remove tab and spaces from a line that inside macro*/
                /*While  in macro definition ,save the content of the macro*/
                add_to_macro_content(macro_line, &macro_content); /*Add each line of content to the end of the list*/
                line_counter++;

                memset(macro_line, '\0', sizeof(macro_line));
            }
            if(strncmp(macro_line, "endmacr" , strlen("endmacr")) == 0){
                line_counter++;
                 /* Check for errors at endmacr */
                remove_tab_and_spaces(macro_line);/*Remove tab and spaces from a line that inside macro*/
                flag_error += end_macro_examine(macro_line + strlen("endmacr") , line_counter);
            }
            
            if(flag_error == SUCCESS){
                macro =  create_macro(macro_name , macro_content); /*Create new macro object that contain the name and content*/
                add_macro_to_list( macro_list, macro);/*Add the macro object to the list of macros*/
            
            }

        }
        else if( (temp_macro = find_macro( line , *macro_list) ) != NULL ){/*If found the macro name in the code*/
            
            temp_macro_content = temp_macro->macro_content;
            while(temp_macro_content != NULL){/*Print all the macro content to the am file*/
                fprintf(fp_am, "%s" ,temp_macro_content->line);
                temp_macro_content = temp_macro_content->next_line;
            }
            temp_macro_content = NULL;/*Resset to null for the nexts irritates*/
            temp_macro = NULL;

        }
        else{/*This ordinary code line ,so ,print it into the am file*/
            if(line[0] != '\n'){
                fprintf(fp_am, "%s" , line);
            }
        }
        
        memset(line, '\0', sizeof(line));/* Initialize the 'line' buffer to null characters */
        memset(macro_name, '\0', sizeof(macro_name));/* Initialize the 'macro_name' buffer to null characters */
        memset(macro_line, '\0', sizeof(macro_line));/* Initialize the 'macro_line' buffer to null characters */
        line_counter++;   /* Increment the line counter to track the number of lines processed */
    }
    
    fprintf(fp_am , "\n");/*Add new line in case that missing in last line*/
    fclose(fp_as);/* Close the as file */
    fclose(fp_am);/* Close the am file */
    
    if(flag_error >= FAIL){/*Delete the .am file if error found*/
        if( remove(am_file) != 0 ){  
            perror("Error deleting the file");
        }
    }
    
    return flag_error; /* Return the error flag status */
}

