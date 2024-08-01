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

## Role in the Project

As a key contributor to this project, your responsibilities include:

- **Syntax and Semantic Error Checking**: Developing and implementing robust mechanisms for detecting and reporting syntax errors (e.g., incorrect instruction formats, illegal characters) and semantic errors (e.g., misuse of registers, undefined labels). This helps ensure that users receive clear feedback on errors, facilitating easier debugging and learning.

- **Macro Expansion**: Implementing a feature for defining and expanding macros in the assembly code. Macros are a powerful tool for code reuse, allowing users to define complex sequences of instructions as a single macro command. This not only simplifies code management but also enhances the flexibility of the assembler.

- **Documentation and User Guidance**: Creating detailed documentation and examples to help users understand the custom assembly language and how to effectively use the assembler. This includes providing sample programs, explaining the syntax and semantics of the language, and offering tips for avoiding common errors.

- **Testing and Validation**: Conducting comprehensive testing of the assembler to ensure it accurately converts assembly instructions to machine code. This includes creating test cases that cover a wide range of scenarios, from basic operations to complex macro expansions, and verifying that the output is correct.

## Project Goals and Learning Outcomes

This project is designed as a learning tool for understanding the foundational concepts of computer architecture and low-level programming. By working on this assembler, users will gain experience in:

- **Language Design**: Understanding how to design an assembly language, including defining a custom instruction set and syntax rules.
- **Assembler Development**: Learning how assemblers work, including parsing source code, error checking, and generating machine code.
- **Systems Programming**: Gaining insights into low-level systems programming, which is crucial for fields such as operating systems, embedded systems, and compiler development.
- **Debugging and Optimization**: Developing skills in debugging and optimizing code at the assembly level, which is valuable for performance-critical applications.

## Usage and Examples

To get started, users write their programs in a `.as` file using the custom assembly language syntax. The assembler then converts this source code into machine code. The following is an example of a simple program:

```assembly
; file example.as
.entry LIST
.extern fn1

MAIN:   add r3, LIST
        jsr fn1
        prn r3
        stop

LIST:   .data 6, -9
STR:    .string "abcd"
```

This program demonstrates basic operations such as data definition, arithmetic operations, and function calls. Users can explore more complex functionalities by extending the instruction set and using the assembler's error checking and output capabilities to refine their code.


## Contact

Created by [Or Saban] - [or8saban@gmail.com] - feel free to contact me!
