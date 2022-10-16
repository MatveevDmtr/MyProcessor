#include "cpu.h"

/*static char* CpuErrorNames[] {
    "Wrong signature",
    "Undefined command",
    "Can't divide by zero",
    "Invalid reg index",
    "Invalid RAM index"
};*/


const char* CPU_SIGNATURE  = "MDA";
const char* NAME_FILE_CODE = "ASM.txt";

const size_t RAM_SIZE = 100;

const size_t REG_SIZE = 10;

int Run()
{
    log("\n----------CPU----------\n");

    stack_t stk1 = StructStackInit(stk1);

    stack_t stkCalls1 = StructStackInit(stk1);

    StackCtor(&stk1);

    StackCtor(&stkCalls1);

    CPU cpu = {NULL, NULL, NULL, CPU_SIGNATURE, 0, 0, &stk1, &stkCalls1};

    log("stk1 created\n");

    getCode(&cpu);

    log("Code was read\n");

    if (Execute(&cpu))
    {
        print_log(FRAMED, "EXECUTION ERROR");
    }

    log("Finish running\n");

    return 0;
}

int Execute(CPU* cpu)
{
    Assert(cpu == NULL);

    stack_t* stk = cpu->Stk;

    Assert(stk == NULL);

    log("ip: %d, Size: %d\n", cpu->ip, cpu->Size);

    while (cpu->ip < cpu->Size)
    {
        log("cmd_code wo mask: %d\n", cpu->code[cpu->ip]);

        switch(cpu->code[cpu->ip] & CMD_MASK)
        {
            #include "commands_for_cpu.h"

            default:

                print_log(FRAMED, "COMMAND ERROR: Unexpected command");

                log("ERROR: Unexpected command, ip: %zd\n", cpu->ip);

                return UNDEFINED_CMD;
        }

        cpu->ip++;

        CPUDump(cpu);
    }

    log("end of cycle\n");

    #undef DEF_CMD

    return 0;
}

void CPUDumpEmExit()
{
    log("\n?????????? EMERGENCY FINISH [FULL MASTER] DUMP ??????????\n\n");
}

int FuckingCPUDump(CPU* cpu,
                   const char* funcname,
                   const char* filename,
                   int         line)
{
    log("\n\n*********** Start [FULL MASTER] Dump **********\n");

    if (cpu == NULL)
    {
        print_log(FRAMED, "POINTER ERROR: CPU pointer is NULL");

        CPUDumpEmExit();

        return -1;
    }

    if (funcname == NULL || filename == NULL || line == 0)
    {
        log("STACKDUMP CALL ERROR: StackDump can't recognize parameters of call\n");
    }
    else
    {
        log("\n%s at %s (line %d):\n", funcname, filename, line);
    }

    log("CPU struct at [0x%p]\n", cpu);

    log("Signature:             %p\n"
        "Code:                  %p\n"
        "Size of code:          %d\n"
        "Registers:             %p\n"
        "RAM:                   %p\n"
        "Stack:                 %p\n"
        "Stack of Calls:        %p\n"
        "Instruction Pointer:   %d\n",
        cpu->signature,
        cpu->code,
        cpu->Size,
        cpu->regs,
        cpu->RAM,
        cpu->Stk,
        cpu->StkCalls,
        cpu->ip);

    size_t numbers_in_size = (size_t) ceil(log10 ((double) cpu->Size));

    elem_t curr_elem = 0;

    log("\n");

    for (size_t i = 0; i < cpu->Size; i++)
    {
        curr_elem = (cpu->code)[i];

        log("\t code[%0*d] = %11d = %x\n", numbers_in_size, i, curr_elem, curr_elem);
    }

    log("\n\n");

    for (size_t i = 0; i < REG_SIZE; i++)
    {
        log("reg[%d] = %d\n", i, cpu->regs[i]);
    }

    StackDump(cpu->Stk);

    log("\n---------- Finish [FULL MASTER] Dump ----------\n\n");

    return 0;
};

void SkipNewLines()
{
    char sym = 0;

    while((sym = getchar()) != '\n')            continue;
}

int PrintRAM(size_t format, CPU* cpu, size_t len_line)
{
    int* ptr_RAM = cpu->RAM;

    switch (format)
    {
        case BIN_FORMAT:
            log("start printing in BIN format");

            for (size_t i = 0; i < RAM_SIZE; i++)
            {
                if (i % len_line == 0)     printf("\n");

                if (ptr_RAM[i])            printf("%c", '#');

                else                       printf("%c", ' ');
            }
            break;

        case SYM_FORMAT:
            for (size_t i = 0; i < RAM_SIZE; i++)
            {
                if (i % len_line == 0)     printf("\n");

                printf("%c", ptr_RAM[i]);
            }
            break;

        default:
            print_log(FRAMED, "FORMAT ERROR: Unexpected format or printing RAM");
    }

    printf("\n");
}

void PushArg(CPU* cpu)
{
    log("start push\n");

    size_t cmd_ip = cpu->ip++;

    log("code of push: %d\n", cpu->code[cmd_ip]);

    if (cpu->code[cmd_ip] & ARG_RAM)
    {
        int index = 0;

        if (cpu->code[cmd_ip] & ARG_REG)
        {
            index += cpu->regs[cpu->code[(cpu->ip)++]];
        }

        if (cpu->code[cmd_ip] & ARG_IMMED)
        {
            index += cpu->code[cpu->ip++];
        }

        StackPush(cpu->Stk, cpu->RAM[index]);
    }
    else
    {
        int arg = 0;

        log("push wo RAM\n");

        if (cpu->code[cmd_ip] & ARG_REG)
        {
            arg += cpu->regs[cpu->code[(cpu->ip)++]];
        }

        log("arg after push rcx: %d\n", arg);

        if (cpu->code[cmd_ip] & ARG_IMMED)
        {
            arg += cpu->code[cpu->ip++];
        }

        StackPush(cpu->Stk, arg);
    }

    cpu->ip--;

    log("finish push\n");
}

int PopArg(CPU* cpu)
{
    log("start pop\n");

    int arg = StackPop(cpu->Stk);

    log("cmd_code wo mask in pop: %d\n", cpu->code[cpu->ip]);

    size_t cmd_ip = cpu->ip++;

    int index = 0;

    if (cpu->code[cmd_ip] & ARG_RAM)
    {
        if (cpu->code[cmd_ip] & ARG_REG)
        {
            index += cpu->regs[cpu->code[(cpu->ip)++]];
        }

        if (cpu->code[cmd_ip] & ARG_IMMED)
        {
            index += cpu->code[cpu->ip++];
        }

        cpu->RAM[index] = arg;
    }
    else
    {
        log("pop wo ram\n");

        log("pop code: %d\n", cpu->code[cmd_ip]);

        if (cpu->code[cmd_ip] & ARG_REG)
        {
            cpu->regs[cpu->code[(cpu->ip)++]] = arg;

            log("rcxing\n");
        }

        if (cpu->code[cmd_ip] & ARG_IMMED)
        {
            print_log(FRAMED, "SYNTAX ERROR: Invalid usage of function pop");
        }
    }

    cpu->ip--;

    log("pop done\n");
}

void CpuCtor(CPU* cpu)
{
    cpu->code = (int*)(calloc(cpu->Size, sizeof(int)));
    Assert(cpu->code == nullptr);

    cpu->RAM = (int*)(calloc(RAM_SIZE, sizeof(int)));
    Assert(cpu->RAM == nullptr);

    cpu->regs = (int*)(calloc(REG_SIZE, sizeof(int)));
    Assert(cpu->regs == nullptr);
}

int checkSign(CPU* cpu, FILE* file_asm)
{
    char asm_sign[MAX_LEN_SIGN] = {};

    int num_read_lines = fscanf(file_asm, "%3s", asm_sign);

    log("Lines read: %d\n", num_read_lines);

    log("sign read: %s, sign cpu: %s\n", asm_sign, cpu->signature);

    if (strcmp(asm_sign, cpu->signature))
    {
        print_log(FRAMED, "SIGNATURE ERROR");

        return WRONG_SIGNATURE;
    }

    return 0;
}

int getCode(CPU* cpu)
{
    FILE* file_asm = open_file_rmode(NAME_FILE_CODE);

    Assert(file_asm == NULL);

    printf("ftell: %d\n", ftell(file_asm));

    if (checkSign(cpu, file_asm))    return WRONG_SIGNATURE;

    printf("ftell: %d\n", ftell(file_asm));

    //fscanf(file_asm, "\n");

    /*int ch = 0;
    while( true )
    {
        if( ch != '\n' ) break;
        ch = fgetc( file_asm );
    }
    */

    printf("ftell: %d\n", ftell(file_asm));

    fread(&cpu->Size, sizeof(int), 1, file_asm);

    log("Size already read: %d\n", cpu->Size);

    CpuCtor(cpu);

    fscanf(file_asm, "\n");

    fread(cpu->code, sizeof(int), cpu->Size, file_asm);

    printf("\n");

    fclose(file_asm);

}
