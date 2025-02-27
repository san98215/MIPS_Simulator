# MIPS Simulator

This project is a C implementation designed to emulate a MIPS processor and execute MIPS instructions.

## Features

- **Instruction Input**: The simulator accepts a file with MIPS instructions formatted as a standard MIPS assembly file.
- **Execution Output**: After each instruction execution, the simulator presents the processor and memory state. The detailed output includes:
  - Instruction Number
  - Executed Instruction
  - Register States
  - Memory States
- **Supported Instructions**: 
  - Arithmetic: `add`, `addi`, `sub`, `mul`, `div`
  - Memory: `lw`, `sw`
  - Branching: `beq`, `bne`
  - Jumps: `j`, `jal`
  - System: `syscall`

## Getting Started

The simulator is compatible with Linux Ubuntu and uses GCC for compilation.
To compile the simulator, simply run:
```bash
make
```

### Execution

- Run the simulator:
  ```bash
  ./instructions [file_name]
  ```
- For memory leak checks, execute with Valgrind:
  ```bash
  valgrind --leak-check=full ./instructions [file_name]
  ```

