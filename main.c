#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instructions.h"

int main(int argc, char *argv[])
{
    char *fileName = argv[1];

    FILE *fp;

    // Opcodes
    char *LD = "0010";
    char *LDR = "0110";
    char *ADD = "0001";
    char *BRp = "0000";
    char *STR = "0111";
    char *FIN = "1101";

    // Initialize registers
    int regs[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int rPC = 0;
    int rPbit = 0;

    fp = fopen(fileName, "r");

    // Number of instructions
    int numInst;
    fscanf(fp, "%d", &numInst);

    // Used for strtol function calls
    char *end;

    // Iterate through file and store as main memory
    Memory *mainMem = (Memory *)malloc(numInst * sizeof(Memory));

    for (int i = 0; i < numInst; i++)
    {
        // Store address and data as strings
        char *address = (char *)malloc(17 * sizeof(char));
        char *data = (char *)malloc(17 * sizeof(char));
        fscanf(fp, "%s %s", address, data);
        address[16] = '\0';
        data[16] = '\0';

        mainMem[i].address = address;
        mainMem[i].data = data;

        // Store address and data as ints - check for two's comlement
        int intAddress;
        if (address[0] == '1')
        {
            char *negAddress = twosComplement(address);
            intAddress = -1 * strtol(negAddress, &end, 2);
            free(negAddress);
        }
        else
        {
            intAddress = strtol(address, &end, 2);
        }

        int intData;
        if (data[0] == '1')
        {
            char *negData = twosComplement(data);
            intData = -1 * strtol(negData, &end, 2);
            free(negData);
        }
        else
        {
            intData = strtol(data, &end, 2);
        }

        mainMem[i].intAddress = intAddress;
        mainMem[i].intData = intData;
    }

    fclose(fp);

    // Start program
    int running = 1;
    while (running)
    {
        // Fetch instruction from current PC and increment PC
        char *content = mainMem[rPC].data;
        rPC++;

        // Decode instruction
        char opcode[5];
        strncpy(opcode, content, 4);
        opcode[4] = '\0';


        if (strcmp(opcode, LD) == 0)
        {
            // Execute load instruction and set P bit based on result
            rPbit = Load(mainMem, regs, rPC, content, numInst);
        }
        else if (strcmp(opcode, LDR) == 0)
        {
            // Execute loadR instruction and set P bit based on result
            rPbit = LoadR(mainMem, regs, rPC, content, numInst);
        }
        else if (strcmp(opcode, ADD) == 0)
        {
            // Execute add instruction and set P bit based on result
            rPbit = Add(mainMem, regs, content, numInst);
        }
        else if (strcmp(opcode, BRp) == 0)
        {
            // Execute branchP instruction only if pBit is set to 1
            if (rPbit == 1)
            {
                // If branchP is exexcuted, set PC to the result
                rPC = BranchP(mainMem, regs, rPC, rPbit, content, numInst);
            }
        }
        else if (strcmp(opcode, STR) == 0)
        {
            // Execute store instruction
            Store(mainMem, regs, content, numInst);
        }
        else if (strcmp(opcode, FIN) == 0)
        {
            // Execute FIN instruction and exit program
            running = 0;
        }
    }

    // Free memory
    for (int i = 0; i < numInst; i++)
    {
        free(mainMem[i].address);
        free(mainMem[i].data);
    }

    free(mainMem);
}