# MIPS Simulator

This project is a C implementation designed to emulate a MIPS processor and execute MIPS instructions.

## Features

- **Instruction Input**: The simulator accepts a text file with MIPS instructions encoded in binary.
- **Supported Instructions**: 
  - `LD`, `LDR`, `ADD`, `BRp`, `STR`

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

