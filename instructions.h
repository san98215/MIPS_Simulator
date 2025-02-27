typedef struct memory
{
    char *address;
    char *data;
    int intAddress;
    int intData;
} Memory;

int Add(Memory *mainMem, int *regs, char *content, int numInst);
int BranchP(Memory *mainMem, int *regs, int rPC, int rCond, char *content, int numInst);
int Load(Memory *mainMem, int *regs, int rPC, char *content, int numInst);
int LoadR(Memory *mainMem, int *regs, int rPC, char *content, int numInst);
void Store(Memory *mainMem, int *regs, char *content, int numInst);
char *twosComplement(char *strBin);