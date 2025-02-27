#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instructions.h"

int Load(Memory *mainMem, int *regs, int rPC, char *content, int numInst)
{
    char *end;
    int rPbit;
    int contOffset = 4; // Used to iterate through 16 bit instruction

    // Fetch operand for destination register
    int destReg;
    char *strDest = (char *)malloc(4 * sizeof(char));
    strncpy(strDest, content + contOffset, 3);
    strDest[3] = '\0';

    destReg = strtol(strDest, &end, 2);
    contOffset += 3;

    // Fetch operand for PC offset
    int offset;
    char *strOffset;

    // Check for negative offset
    if (content[contOffset] == '1')
    {
        // Calculate offset with two's complement
        strOffset = (char *)malloc(17 * sizeof(char));
        strncpy(strOffset + 7, content + contOffset, 9);

        // Sign extend offset
        for (int i = 0; i < 7; i++)
        {
            strOffset[i] = '1';
        }
        strOffset[16] = '\0';

        // Calculate two's complement
        char *negOffset = twosComplement(strOffset);
        offset = -1 * strtol(negOffset, &end, 2);
        free(negOffset);
    }
    else
    {
        // Offset is positive
        strOffset = (char *)malloc(10 * sizeof(char));
        strncpy(strOffset, content + contOffset, 9);
        strOffset[9] = '\0';
        offset = strtol(strOffset, &end, 2);
    }

    // Calulate address
    int address = rPC + offset;
    int result = 0;

    // Fetch data from address and store in destination register
    for (int i = 0; i < numInst; i++)
    {
        if (mainMem[i].intAddress == address)
        {
            result = mainMem[i].intData;
            break;
        }
    }
    regs[destReg] = result;

    // Check if P bit needs to be set to 1 or reset to 0
    if (result > 0)
    {
        rPbit = 1;
    }
    else
    {
        rPbit = 0;
    }

    free(strDest);
    free(strOffset);

    return rPbit;
}

int LoadR(Memory *mainMem, int *regs, int rPC, char *content, int numInst)
{
    char *end;
    int rPbit;
    int contOffset = 4;

    // Fetch operand for destination register
    int destReg;
    char *strDest = (char *)malloc(4 * sizeof(char));
    strncpy(strDest, content + contOffset, 3);
    strDest[3] = '\0';

    destReg = strtol(strDest, &end, 2);
    contOffset += 3;

    // Fetch operand for base register
    int base;
    char *baseReg = (char *)malloc(4 * sizeof(char));
    strncpy(baseReg, content + contOffset, 3);
    baseReg[3] = '\0';

    base = strtol(baseReg, &end, 2);
    contOffset += 3;

    // Fetch operand for base register offset
    int offset;
    char *strOffset;

    // Check for negative offset
    if (content[contOffset] == '1')
    {
        // Sign extend offset
        strOffset = (char *)malloc(17 * sizeof(char));
        strncpy(strOffset + 10, content + contOffset, 6);

        for (int i = 0; i < 10; i++)
        {
            strOffset[i] = '1';
        }
        strOffset[16] = '\0';

        // Calculate two's complement
        char *negOffset = twosComplement(strOffset);
        offset = -1 * strtol(negOffset, &end, 2);
        free(negOffset);
    }
    else
    {
        // Offset is positive
        strOffset = (char *)malloc(7 * sizeof(char));
        strncpy(strOffset, content + contOffset, 6);
        strOffset[6] = '\0';
        offset = strtol(strOffset, &end, 2);
    }

    // Calulate address
    int address = regs[base] + offset;
    int result = 0;

    // Fetch data from address and store in destination register
    for (int i = 0; i < numInst; i++)
    {
        if (mainMem[i].intAddress == address)
        {
            result = mainMem[i].intData;
            break;
        }
    }
    regs[destReg] = result;

    // Check if P bit needs to be set to 1 or reset to 0
    if (result > 0)
    {
        rPbit = 1;
    }
    else
    {
        rPbit = 0;
    }

    free(strDest);
    free(baseReg);
    free(strOffset);

    return rPbit;
}

int Add(Memory *mainMem, int *regs, char *content, int numInst)
{
    char *end;
    int rPbit;
    int contOffset = 4;

    // Fetch operand for destination register
    int destReg;
    char *strDest = (char *)malloc(4 * sizeof(char));
    strncpy(strDest, content + contOffset, 3);
    strDest[3] = '\0';

    destReg = strtol(strDest, &end, 2);
    contOffset += 3;

    // Fetch operand for source register 1
    int reg1;
    char *strReg1 = (char *)malloc(4 * sizeof(char));
    strncpy(strReg1, content + contOffset, 3);
    strReg1[3] = '\0';

    reg1 = strtol(strReg1, &end, 2);
    contOffset += 3;

    // Fetch the mode: 0 for regs, 1 for immediate
    int result;
    char mode = *(content + contOffset);

    // Register mode
    if (mode == '0')
    {
        // Skip remaining two 0s
        contOffset += 3;

        // Fetch source register 2
        int reg2;
        char *strReg2 = (char *)malloc(4 * sizeof(char));
        strncpy(strReg2, content + contOffset, 3);
        strReg2[3] = '\0';

        // Add value stored by source reg1 and source reg2, then store in destination reg
        reg2 = strtol(strReg2, &end, 2);
        result = regs[reg1] + regs[reg2];
        regs[destReg] = result;
        free(strReg2);
    }
    // Immediate mode
    else
    {
        // Skip mode bit
        contOffset += 1;

        char *strImm;
        int imm;

        // Check if immediate value is negative
        if (content[contOffset] == '1')
        {
            // Sign extend and calculate two's complement for imm
            char *sgnExt = (char *)malloc(17 * sizeof(char));
            strncpy(sgnExt + 11, content + contOffset, 5);

            for (int i = 0; i < 11; i++)
            {
                sgnExt[i] = '1';
            }
            sgnExt[16] = '\0';

            strImm = twosComplement(sgnExt);
            imm = strtol(strImm, &end, 2) * -1;
            free(sgnExt);
        }
        else
        {
            // Imm is positive
            strImm = (char *)malloc(6 * sizeof(char));
            strncpy(strImm, content + contOffset, 5);
            strImm[5] = '\0';
            imm = strtol(strImm, &end, 2);
        }

        // Add source reg1 and immediate and store in destination reg
        result = regs[reg1] + imm;
        regs[destReg] = result;
        free(strImm);
    }

    // Check if P bit needs to be set to 1 or reset to 0
    if (result > 0)
    {
        rPbit = 1;
    }
    else
    {
        rPbit = 0;
    }

    free(strDest);
    free(strReg1);

    return rPbit;
}

void Store(Memory *mainMem, int *regs, char *content, int numInst)
{
    char *end;
    int contOffset = 4;

    // Fetch operand for destination register
    int sourceReg;
    char *strSource = (char *)malloc(4 * sizeof(char));
    strncpy(strSource, content + contOffset, 3);
    strSource[3] = '\0';

    sourceReg = strtol(strSource, &end, 2);
    contOffset += 3;

    // Fetch operand for base register
    int baseReg;
    char *strBase = (char *)malloc(4 * sizeof(char));
    strncpy(strBase, content + contOffset, 3);
    strBase[3] = '\0';

    baseReg = strtol(strBase, &end, 2);
    contOffset += 3;

    // Fetch operand for base register offset
    int offset;
    char *strOffset;

    // Check for negative offset
    if (content[contOffset] == '1')
    {
        // Sign extend offset and calculate two's complement
        strOffset = (char *)malloc(17 * sizeof(char));
        strncpy(strOffset + 10, content + contOffset, 6);

        for (int i = 0; i < 10; i++)
        {
            strOffset[i] = '1';
        }
        strOffset[16] = '\0';

        char *negOffset = twosComplement(strOffset);
        offset = -1 * strtol(negOffset, &end, 2);
        free(negOffset);
    }
    else
    {
        // Offset is positive
        strOffset = (char *)malloc(7 * sizeof(char));
        strncpy(strOffset, content + contOffset, 6);
        strOffset[6] = '\0';
        offset = strtol(strOffset, &end, 2);
    }

    // Data in register should be an address, so if negative recalculate the address as an int and store as a positive address
    if (regs[baseReg] < 0) {
        for (int i = numInst-1; i >= 0; i--) {
            if (mainMem[i].intData == regs[baseReg]) {
                regs[baseReg] = strtol(mainMem[i].data, &end, 2);
            }
        }
    }
    int address = (regs[baseReg] + offset);

    // Print address and the value being stored to that address
    printf("%X %d\n", address, regs[sourceReg]);

    free(strSource);
    free(strBase);
    free(strOffset);
}

int BranchP(Memory *mainMem, int *regs, int rPC, int rPbit, char *content, int numInst)
{
    char *end;
    int contOffset = 4;

    // Fetch operand for P bit
    int pBit;
    char *strBit = (char *)malloc(4 * sizeof(char));
    strncpy(strBit, content + contOffset, 3);
    strBit[3] = '\0';

    pBit = strtol(strBit, &end, 2);
    contOffset += 3;

    // Fetch operand for PC offset
    int offset;
    char *strOffset;

    // Check for negative offset
    if (content[contOffset] == '1')
    {
        // Sign extend offset and calculate two's complement
        strOffset = (char *)malloc(17 * sizeof(char));
        strncpy(strOffset + 7, content + contOffset, 9);

        for (int i = 0; i < 7; i++)
        {
            strOffset[i] = '1';
        }
        strOffset[16] = '\0';

        char *negOffset = twosComplement(strOffset);
        offset = -1 * strtol(negOffset, &end, 2);

        free(negOffset);
    }
    else
    {
        // Positive offset
        strOffset = (char *)malloc(10 * sizeof(char));
        strncpy(strOffset, content + contOffset, 9);
        strOffset[9] = '\0';
        offset = strtol(strOffset, &end, 2);
    }

    // Update PC since P bit was checked before executing this function
    rPC += offset;

    free(strBit);
    free(strOffset);

    return rPC;
}

char *twosComplement(char *strBin)
{
    // Calculate and store one's complement of input
    char *negOffset = (char *)malloc(17 * sizeof(char));
    negOffset[16] = '\0';

    for (int i = 15; i >= 0; i--)
    {
        if (strBin[i] == '1')
        {
            negOffset[i] = '0';
        }
        else
        {
            negOffset[i] = '1';
        }
    }

    // Add 1 to one's complement to get two's complement
    int carry = 1;
    for (int i = 15; i >= 0; i--)
    {
        if (negOffset[i] == '0' && carry == 1)
        {
            negOffset[i] = '1';
            carry = 0;
        }
        else if (negOffset[i] == '1' && carry == 1)
        {
            negOffset[i] = '0';
        }
    }

    return negOffset;
}