# Custom Assembler Project

## Overview

This project is an assembler for a custom, human-invented assembly language. The assembler converts these custom assembly instructions into machine code, providing an educational tool for learning about low-level programming, assembly language design, and the intricacies of writing and understanding machine-level instructions.


## Features

- **Custom Instruction Set**: This assembler supports a unique set of instructions designed specifically for this project. Key instructions include:
  - `ADD` - Add values from two registers and store the result in another register.
  - `SUB` - Subtract the value in one register from another.
  - `PRN` - Print the value of a register.
  - `JSR` - Jump to subroutine.
  - `STOP` - Halt the execution.
  and more,
  These instructions provide a comprehensive set of operations that cover basic arithmetic, memory operations, control flow, and I/O operations.

- **Data Handling**: The assembler includes robust data handling capabilities:
  - `.data` directive is used for defining numerical data, allowing the initialization of data memory with integers.
  - `.string` directive allows the definition of strings, making it easier to handle text data within programs.
  
  This flexibility supports the creation of complex programs with varied data requirements.

- **Error Checking**: To assist developers, the assembler includes both syntax and semantic error checking:
  - **Syntax Checking**: Ensures that the code conforms to the grammar and structure rules of the custom language. This helps catch issues like misspelled instructions or incorrect formats.
  - **Semantic Checking**: Validates the logical consistency of the code. For instance, it checks for the proper use of registers and memory, correct usage of labels, and ensures that instructions have valid operands.

- **Macros Expansion**: Supports the definition and expansion of macros, allowing users to create reusable code blocks. This feature is particularly useful for reducing code duplication and managing complex assembly instructions.

- **Output Formats**: The assembler produces several output files to support different aspects of program analysis and execution:
  - **Object File (`.ob`)**: Contains the assembled machine code in a format that can be loaded into memory and executed by a runtime environment or simulator.
  - **Entry Point List (`.ent`)**: Lists all entry points defined in the assembly code. This file is crucial for linking and loading processes, indicating where the program execution can begin.
  - **External References (`.ext`)**: Lists all symbols that are declared external and need to be resolved during linking. This supports modular programming and linking multiple assembly files.

##  Importatnt Note
There also the project task booklet, emphasizing its role in guiding the development process and documenting the project's objectives and milestones. 

## 📞 Contact

**Or Saban**  
*Private Lessons and Project Guidance*

- **📧 Email:** [or8saban@gmail.com](mailto:or8saban@gmail.com)  
- **📱 Phone:** +972-54-463-6567  

For personalized tutoring and expert project guidance, feel free to reach out!



