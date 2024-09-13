#include <stdio.h>

#define REG_COUNT 32

typedef struct
{
    int registers[REG_COUNT];
    int setNum;
    int addNum;
    int subNum;
    int mulNum;
    int divNum;
    int ldNum;
    int retNum;
} Simple_Pipe;

void print_regs(Simple_Pipe simple_pipe);

int execution_time;
int instruction_count;
