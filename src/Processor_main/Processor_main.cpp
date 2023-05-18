#include <stdio.h>
//#include <TXLib.h>

#include "../Assembler/assembler.h"
#include "../cpu/cpu.h"
#include "../Disassembler/disassembler.h"

#ifndef LOGGING
    #define log();
#endif

int main(int argc, char* argv[])
{
    Assemble(argc, argv);

    Run();

    MakeReadableCode();

    return 0;
}
