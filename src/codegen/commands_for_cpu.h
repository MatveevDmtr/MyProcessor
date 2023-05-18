#ifndef COMMANDS_FOR_CPU_H_INCLUDED
#define COMMANDS_FOR_CPU_H_INCLUDED

//#include <TXLib.h>
//#define DO_JUMP        {cpu->ip = cpu->code[cpu->ip+1] - 1;}

#define DO_JUMP        {cpu->ip = cpu->code[(cpu->ip)+1] - 2;}

#define DO_POP         ({(double)StackPop(stk) / ACCURACY;})

#define DO_PUSH(arg)   StackPush(stk, (arg) * ACCURACY);

#define REMEMBER_CALL  {StackPush(cpu->StkCalls, cpu->ip);};

#define RETURN_TO_CALL {cpu->ip = StackPop(cpu->StkCalls) + 1;};

#define SKIP_ARG       {(cpu->ip)++;};

#define COND_JUMP(condition)                                 \
{                                                            \
    if (condition)                                           \
    {                                                        \
        DO_JUMP;                                             \
    }                                                        \
    else                                                     \
    {                                                        \
        SKIP_ARG;                                            \
    }                                                        \
}

DEF_CMD(HLT, 0, 0,
{
    printf("Goodbye!\n");

    return 0;
})

DEF_CMD(PUSH, 1, 1,
{
    DO_PUSH(((double)*GetArg(cpu)) / ACCURACY);
})

DEF_CMD(POP, 2, 1,
{
    int* ptr = GetArg(cpu);

    *ptr = DO_POP * ACCURACY;
})

DEF_CMD(ADD, 3, 0,
{
    log("in add\n");
    double a = DO_POP;
    double b = DO_POP;

    log("a: %lg, b: %lg\n", a, b);

    log("a+b: %lg", a+b);

    DO_PUSH(a + b);
})

DEF_CMD(SUB, 4, 0,
{
    log("in sub\n");

    double a = DO_POP;
    double b = DO_POP;

    DO_PUSH(b - a);
})

DEF_CMD(MUL, 5, 0,
{
    log("in mul\n");

    DO_PUSH(DO_POP * DO_POP);
})

DEF_CMD(DIV, 6, 0,
{
    log("in div\n");

    double a = DO_POP;
    double b = DO_POP;

    log("a: %lg, b: %lg\n", a, b);

    if (a == 0)
    {
        printf("ZERO DIVISION ERROR: can't divide by zero\n");

        return ZERO_DIVISION;
    }
    else
    {
        DO_PUSH(b / a);
    }

})

DEF_CMD(IN, 7, 0,
{
    printf("enter a number: ");

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

    double val = DO_POP;

    log("val to out: %lg\n", val);

    if (floor(val) == val)          printf("Out print is %d \n", (int)val);
    else                            printf("Out print is %lg\n",      val);
})

DEF_CMD(JUMP, 9, 1,
{
    DO_JUMP;
})

DEF_CMD(JB, 10, 1,
{
    COND_JUMP(DO_POP < DO_POP);
})

DEF_CMD(JBE, 11, 1,
{
    COND_JUMP(DO_POP <= DO_POP);
})

DEF_CMD(JA, 12, 1,
{
    COND_JUMP(DO_POP > DO_POP);
})

DEF_CMD(JAE, 13, 1,
{
    COND_JUMP(DO_POP >= DO_POP);
})

DEF_CMD(JE, 14, 1,
{
    COND_JUMP(DO_POP == DO_POP);
})

DEF_CMD(JNE, 15, 1,
{
    COND_JUMP(DO_POP != DO_POP);
})

DEF_CMD(DRAW, 18, 0,
{
    PrintRAM(BIN_FORMAT, cpu, 100); //const, static screen
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
    double val = DO_POP;

    log("value in copy: %lg\n", val);

    DO_PUSH(val);
    DO_PUSH(val);
})

DEF_CMD(SQRT, 22, 0,
{
    double val = DO_POP;

    double sqrt_val = sqrt(val);

    log("sqrt from val: %lg\n", sqrt_val);

    DO_PUSH(sqrt_val);
})

DEF_CMD(CLEAR, 23, 0,
{
    ;
    txClearConsole();
})


#endif // COMMANDS_FOR_CPU_H_INCLUDED
