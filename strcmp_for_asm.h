#ifndef COMMANDS_FOR_CPU_H_INCLUDED
#define COMMANDS_FOR_CPU_H_INCLUDED

#define WRITE_CMD_NUM {(asm_code->Ptr)[(asm_code->Ip)++] = cmd_code;};

DEF_CMD(HLT, 0)

DEF_CMD(PUSH, 1,
{
   PutArg(cmd_code, cursor, asm_code);
})

DEF_CMD(POP, 2,
{
    PutArg(cmd_code, cursor, asm_code);
})

DEF_CMD(ADD , 3, WRITE_CMD_NUM)

DEF_CMD(SUB , 4, WRITE_CMD_NUM)

DEF_CMD(MUL , 5, WRITE_CMD_NUM)

DEF_CMD(DIV , 6, WRITE_CMD_NUM)

DEF_CMD(IN  , 7, WRITE_CMD_NUM)

DEF_CMD(OUT , 8, WRITE_CMD_NUM)

DEF_CMD(JUMP, 9,
{
    FuncJump(cursor, asm_code);
})

DEF_CMD(NAME, 16,
{
    FuncName(cursor, asm_code);
})

DEF_CMD(LBL , 17,
{
    FuncLab(cursor, asm_code);
})


#endif // COMMANDS_FOR_CPU_H_INCLUDED
