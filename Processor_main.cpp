#include <stdio.h>
//#include <TXLib.h>

#include "assembler.h"
#include "cpu.h"
#include "disassembler.h"

#ifndef LOGGING
    #define log();
#endif

int main(int argc, char* argv[])
{
    Assemble(argc, argv);

    Run();

    //MakeReadableCode();

    return 0;
}
