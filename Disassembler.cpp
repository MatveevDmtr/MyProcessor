#include "cpu.h"
#include "disassembler.h"
#include "onegin.h"

#undef DEF_CMD

#define DEF_CMD(name, num, arg, ...)                      \
    case num:                                             \
        PutCmd(disasm, #name);                            \
        switch (arg)                                      \
        {                                                 \
            case 2:                                       \
                log("2 args in case\n");                  \
                PutStackCmd(disasm);                      \
                break;                                    \
            case 1:                                       \
                disasm->ip++;                             \
                WRITE_NUMBER;                             \
                break;                                    \
            default:                                      \
                log("No argument of cmd\n");              \
                disasm->ip++;                             \
        }                                                 \
        disasm->buf_code[disasm->cursor++] = '\n';        \
    break;

#define WRITE_NUMBER                                      \
{                                                         \
                                                          \
        PutNumToCharBuf(disasm);                          \
}

//#include <TXLib.h>

/*int PutArgJump(disasm_t* disasm)
{
    WRITE_NUMBER;
}*/

const char* DISASM_SIGNATURE = "MDA";

const char* ASM_FILE_NAME_DISASM = "ASM.txt";

const int MAX_LEN_CMD = 6;

int MakeReadableCode()
{
    log("\n----------DISASM----------\n");

    disasm_t disasm = {NULL, 0, 0, DISASM_SIGNATURE, NULL, 0};

    log("disasm created\n");

    getCodeForDisasm(&disasm);

    log("Code was read\n");

    if (Disassemble(&disasm))
    {
        print_log(FRAMED, "EXECUTION ERROR");
    }

    WriteUserCode(&disasm, "Disassembled.txt");

    log("Finish disassembling\n");

    return 0;
}

int PutCmd(disasm_t* disasm, const char* cmd_name)
{
    strcpy(disasm->buf_code + disasm->cursor, cmd_name);

    log("finish strcpy\n");

    disasm->cursor += strlen(cmd_name);

    disasm->buf_code[disasm->cursor++] = ' ';

    return 0;
}

int HandleRegsDisasm(disasm_t* disasm, size_t reg_num)
{
    char reg_name[MAX_LEN_REG_NAME] = {'r', 'z', 'x'};

    reg_name[1] = (char)(reg_num + (int)'a');

    log("reg_name: %s\n\n", reg_name);

    strcpy(disasm->buf_code + disasm->cursor, reg_name);

    disasm->cursor += strlen(reg_name);

    return 0;
}

int PutNumToCharBuf(disasm_t* disasm)
{
    char* line = itoa((disasm->asm_code)[disasm->ip++], disasm->buf_code + disasm->cursor, 10);

    disasm->cursor += strlen(line);

    return 0;
}

int PutStackCmd(disasm_t* disasm)
{
    log("start PutStackArg\n");

    size_t cmd_ip = disasm->ip++;

    log("code of push: %d\n", disasm->asm_code[cmd_ip]);

    if (disasm->asm_code[cmd_ip] & ARG_RAM)   disasm->buf_code[disasm->cursor++] = '[';

    if (disasm->asm_code[cmd_ip] & ARG_REG)
    {
        HandleRegsDisasm(disasm, (disasm->asm_code)[disasm->ip++]);
    }

    if (disasm->asm_code[cmd_ip] & ARG_IMMED)
    {
        WRITE_NUMBER;
    }

    if (disasm->asm_code[cmd_ip] & ARG_RAM)   disasm->buf_code[disasm->cursor++] = ']';

    log("finish PutStackArg\n");

    return 0;
}


int Disassemble(disasm_t* disasm)
{
    Assert(disasm == NULL);

    log("ip: %d, Size: %d\n", disasm->ip, disasm->Size);

    while (disasm->ip < disasm->Size)
    {
        log("cmd_code wo mask: %d\n", disasm->asm_code[disasm->ip]);

        switch(disasm->asm_code[disasm->ip] & CMD_MASK)
        {
            #include "strcmp_for_asm.h"

            default:

                print_log(FRAMED, "COMMAND ERROR: Unexpected command");

                log("ERROR: Unexpected command, ip: %zd\n", disasm->ip);

                return UNDEFINED_CMD;
        }

        //CPUDump(cpu);
    }

    log("end of cycle\n");

    #undef DEF_CMD

    return 0;
}

int WriteUserCode(disasm_t* disasm, const char* filename)
{
    FILE* w_file = open_Wfile(filename);

    size_t num_written_sym = fwrite(disasm->buf_code,
                                    sizeof(char),
                                    disasm->cursor + 1,
                                    w_file);

    if (num_written_sym != disasm->cursor + 1)
    {
        print_log(FRAMED, "Error in writing to file\n");

        return -1;
    }

    log("File is written successfully\n");

    return 0;
}

int checkSignDisasm(disasm_t* disasm, FILE* file_asm)
{
    char asm_sign[MAX_LEN_SIGN] = {};

    int num_read_lines = fscanf(file_asm, "%3s", asm_sign);

    log("Lines read: %d\n", num_read_lines);

    log("sign read: %s, sign cpu: %s\n", asm_sign, disasm->signature);

    if (strcmp(asm_sign, disasm->signature))
    {
        print_log(FRAMED, "SIGNATURE ERROR");

        return -1;
    }

    return 0;
}

void BufCtor(disasm_t* disasm)
{
    disasm->asm_code = (int*)(calloc(disasm->Size, sizeof(int)));
    Assert(disasm->asm_code == nullptr);

    disasm->buf_code = (char*)(calloc(disasm->Size * MAX_LEN_CMD, sizeof(char)));
    Assert(disasm->buf_code == nullptr);
}

int getCodeForDisasm(disasm_t* disasm)
{
    FILE* file_asm = open_file_rmode(ASM_FILE_NAME_DISASM);

    Assert(file_asm == NULL);

    if (checkSignDisasm(disasm, file_asm))    return -1;

    fread(&disasm->Size, sizeof(int), 1, file_asm);

    log("Size already read: %d\n", disasm->Size);

    BufCtor(disasm);

    fscanf(file_asm, "\n");

    fread(disasm->asm_code, sizeof(int), disasm->Size, file_asm);

    printf("\n");

    fclose(file_asm);

    return 0;
}
