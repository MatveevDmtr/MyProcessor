#ifndef COMMANDS_FOR_CPU_H_INCLUDED
#define COMMANDS_FOR_CPU_H_INCLUDED

//#define DO_JUMP        {cpu->ip = cpu->code[cpu->ip+1] - 1;}

#define DO_JUMP        {cpu->ip = cpu->code[(cpu->ip)+1] - 2;}

#define DO_POP         ({StackPop(stk);})

#define DO_PUSH(arg)   StackPush(stk, arg);

#define REMEMBER_CALL  {StackPush(cpu->StkCalls, cpu->ip);};

#define RETURN_TO_CALL {cpu->ip = StackPop(cpu->StkCalls) + 1;};

#define SKIP_ARG       {(cpu->ip)++;};

DEF_CMD(HLT, 0, 0,
{
    printf("Goodbye!\n");

    return 0;
})

DEF_CMD(PUSH, 1, 1,
{
    DO_PUSH(*GetArg(cpu));
})

DEF_CMD(POP, 2, 1,
{
    int* ptr = GetArg(cpu);

    log("get_arg in POP: %d\n", ptr);

    *ptr = DO_POP;
})

DEF_CMD(ADD, 3, 0,
{
    log("in add\n");

    DO_PUSH(DO_POP + DO_POP);
})

DEF_CMD(SUB, 4, 0,
{
    log("in sub\n");

    DO_PUSH(DO_POP - DO_POP);
})

DEF_CMD(MUL, 5, 0,
{
    log("in mul\n");

    DO_PUSH(DO_POP * DO_POP);
})

DEF_CMD(DIV, 6, 0,
{
    log("in div\n");

    int a = DO_POP;
    int b = DO_POP;

    if (b == 0)
    {
        printf("ZERO DIVISION ERROR: can't divide by zero\n");
        return ZERO_DIVISION;
    }
    else
    {
        DO_PUSH(a * b);
    }

})

DEF_CMD(IN, 7, 0,
{
    printf("enter a number");

    int value = 0;

    while (!scanf("%d", &value))
    {
        printf("please, enter a fucking number:\n");
        SkipNewLines();
    }

    SkipNewLines();

    DO_PUSH(value);
})

DEF_CMD(OUT, 8, 0,
{
    log("cuming out\n");

    printf("Out print is %d\n", DO_POP);
})

DEF_CMD(JUMP, 9, 1,
{
    DO_JUMP;
})

DEF_CMD(JB, 10, 1,
{
    if (DO_POP < DO_POP)
    {
        DO_JUMP;
    }
    else
    {
        SKIP_ARG;
    }
})

DEF_CMD(JBE, 11, 1,
{
    if (DO_POP <= DO_POP)
    {
        DO_JUMP;
    }
    else
    {
        SKIP_ARG;
    }
})

DEF_CMD(JA, 12, 1,
{
    if (DO_POP > DO_POP)
    {
        DO_JUMP;
    }
    else
    {
        SKIP_ARG;
    }
})

DEF_CMD(JAE, 13, 1,
{
    if (DO_POP >= DO_POP)
    {
        DO_JUMP;
    }
    else
    {
        SKIP_ARG;
    }
})

DEF_CMD(JE, 14, 1,
{
    if (DO_POP == DO_POP)
    {
        DO_JUMP;
    }
    else
    {
        SKIP_ARG;
    }
})

DEF_CMD(JNE, 15, 1,
{
    if (DO_POP != DO_POP)
    {
        DO_JUMP;
    }
    else
    {
        SKIP_ARG;
    }
})

DEF_CMD(DRAW, 18, 0,
{
    PrintRAM(BIN_FORMAT, cpu, 10);
})

DEF_CMD(CALL, 19, 1,
{
    REMEMBER_CALL;

    DO_JUMP;
})

DEF_CMD(RET, 20, 0,
{
    RETURN_TO_CALL;
})

DEF_CMD(COPY, 21, 0,
{
    int val = DO_POP;

    DO_PUSH(val);
    DO_PUSH(val);
})

DEF_CMD(SQRT, 22, 0,
{
    int val = DO_POP;

    val = floor(sqrt(val));

    DO_PUSH(val);
})


#endif // COMMANDS_FOR_CPU_H_INCLUDED
