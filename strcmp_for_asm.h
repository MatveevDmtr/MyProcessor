#ifndef COMMANDS_FOR_CPU_H_INCLUDED
#define COMMANDS_FOR_CPU_H_INCLUDED

#define WRITE_CMD_NUM  {(asm_code->Ptr)[(asm_code->Ip)++] = cmd_code;};

#define WRITE_ARG_JUMP {FuncJump(cursor, asm_code);};

DEF_CMD(HLT, 0, WRITE_CMD_NUM)

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
    WRITE_CMD_NUM;

    WRITE_ARG_JUMP;
})

DEF_CMD(JB, 10,
{
    WRITE_CMD_NUM;

    WRITE_ARG_JUMP;
})

DEF_CMD(JBE, 11,
{
    WRITE_CMD_NUM;

    WRITE_ARG_JUMP;
})

DEF_CMD(JA, 12,
{
    WRITE_CMD_NUM;

    WRITE_ARG_JUMP;
})

DEF_CMD(JAE, 13,
{
    WRITE_CMD_NUM;

    WRITE_ARG_JUMP;
})

DEF_CMD(JE, 14,
{
    WRITE_CMD_NUM;

    WRITE_ARG_JUMP;
})

DEF_CMD(JNE, 15,
{
    WRITE_CMD_NUM;

    WRITE_ARG_JUMP;
})

DEF_CMD(NAME, 16,
{
    FuncName(cursor, asm_code);
})

DEF_CMD(LBL , 17,
{
    FuncLab(cursor, asm_code);
})

DEF_CMD(DRAW, 18, WRITE_CMD_NUM)

DEF_CMD(CALL, 19,
{
    WRITE_CMD_NUM;

    WRITE_ARG_JUMP;
})

DEF_CMD(RET, 20, WRITE_CMD_NUM)

DEF_CMD(COPY, 21, WRITE_CMD_NUM)


#endif // COMMANDS_FOR_CPU_H_INCLUDED
