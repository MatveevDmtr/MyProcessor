# Переменная CC указывает компилятор, используемый для сборки
CC=g++
#В переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS=-c -fno-omit-frame-pointer -std=c++14 -DLOGGING
OPTFLAG=-O0
MY_OPT=

all: build execute clear

leakcheck: build use_valgrind clear

build: Processor_main.o Stack_main.o Assembler.o Disassembler.o cpu.o logging.o GetPoison.o specificators.o
	$(CC) Processor_main.o Stack_main.o Assembler.o Disassembler.o cpu.o logging.o GetPoison.o specificators.o -o Processor

Processor_main.o: src/Processor_main/Processor_main.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/Processor_main/Processor_main.cpp

Stack_main.o: src/Stack/Stack_main.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/Stack/Stack_main.cpp

Assembler.o: src/Assembler/Assembler.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/Assembler/Assembler.cpp

Disassembler.o: src/Disassembler/Disassembler.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/Disassembler/Disassembler.cpp

cpu.o: src/cpu/cpu.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/cpu/cpu.cpp

logging.o: src/logging/logging.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/logging/logging.cpp 

GetPoison.o: src/GetPoison/GetPoison.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/GetPoison/GetPoison.cpp

specificators.o: src/specificators/specificators.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/specificators/specificators.cpp

clear:
	rm *.o

execute:
	./Processor

use_valgrind:
	valgrind --leak-check=full ./Processor