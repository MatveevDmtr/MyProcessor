#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <string.h>
#include <sys\stat.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>

#include "stack.h"
#include "Cpu_Config.h"
#include "logging.h"

#define DEF_CMD(name, num, arg, ...) \
    case num:                        \
    __VA_ARGS__                      \
    break;

#define CPUDump(cpu);                                          \
            FuckingCPUDump(cpu,                                \
                        __PRETTY_FUNCTION__,                   \
                        __FILE__,                              \
                        __LINE__);

#define RETURN_PTR_ARG(arg)          \
{                                    \
    cpu->ip--;                       \
                                     \
    return arg;                      \
}


const int CMD_MASK = 31; /*11111*/

const size_t MAX_LEN_SIGN = 20;

enum CPU_ERRORS
{
    WRONG_SIGNATURE   = 1,
    UNDEFINED_CMD     = 2,
    ZERO_DIVISION     = 3,
    INVALID_REG_INDEX = 4,
    INVALID_MEM_INDEX = 5
};

enum PrintRAMFromats
{
    BIN_FORMAT = 0,
    SYM_FORMAT = 1,
    INT_FORMAT = 2
};


typedef struct CPU_struct
{
    int*        code;
    int*        regs;
    int*        RAM;
    const char* signature;
    size_t      ip;
    size_t      Size;
    stack_t*    Stk;
    stack_t*    StkCalls;
} CPU;

int  Execute(CPU* cpu);

void SkipNewLines();

int FuckingCPUDump(CPU* cpu,
                   const char* funcname,
                   const char* filename,
                   int         line);

void CPUDumpEmExit();

int PrintRAM(size_t format, CPU* cpu, size_t len_line);

int* GetArg(CPU* cpu);

void PushArg(CPU* cpu);

int PopArg(CPU* cpu);

void CpuCtor(CPU* cpu);

int  checkSign(CPU* cpu, FILE* a_code);

int  getCode(CPU* cpu);

FILE* open_file_rmode(const char* filename);

int Run();

#endif //guard
