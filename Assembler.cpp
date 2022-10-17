#include "assembler.h"

#define DEF_CMD(name, num, ...)        \
    else if (!stricmp(cmd, #name))    \
        {                            \
            cmd_code = num;          \
            __VA_ARGS__              \
        }                            \

//#include <TXLib.h>

//warnings
const size_t MAX_LEN_CMD    = 30;

const size_t MAX_NUM_LABELS = 30;

const char* INPUT_FILE_NAME = "factorial.txt";

const char* SIGNATURE = "MDA";

int Assemble()
{
    type_buf_char      user_code         = {NULL, 0, 0};
    type_buf_structs   arr_structs       = {NULL, 0   };

    label_field labels[MAX_NUM_LABELS] = {{87, ""}};

    asm_t asm_code = {NULL, NULL, 0, labels};

    read_file(INPUT_FILE_NAME, &user_code, &arr_structs);

    asm_code.Ptr = allocate_array(int, user_code.Num_lines * 3);

    Assert(asm_code.Ptr == NULL);

    UserCodeToASM(&user_code, &arr_structs, &asm_code);

    UserCodeToASM(&user_code, &arr_structs, &asm_code);

    WriteASM(asm_code.Ptr, "ASM.txt", asm_code.Size);
          //free
    return 0;
}

int Free(void* ptr, size_t num_bytes)
{
    ;
}

int UserCodeToASM(type_buf_char*    ptr_user_code,
                  type_buf_structs* ptr_arr_structs,
                  asm_t*            asm_code)
{
    asm_code->Ip = 0;

    char   cmd[MAX_LEN_CMD] = "";

    size_t read_res     = 0;

    size_t cmd_code     = 0;

    char*  cursor = NULL;

    for (size_t i = 0; i < ptr_user_code->Num_lines; i++)
    {
        cursor = (ptr_arr_structs->Ptr)[i].Loc;

        sscanf(cursor, "%s", cmd);

        cursor += strlen((const char*)cmd);

        log("Command found: %s\n", cmd);

        if (false)
        {
            log("Never gonna give you up");
        }

        #include "strcmp_for_asm.h"

        else
        {
            print_log(FRAMED, "COMMAND ERROR: Unexpected command while assembling");

            return -1;
        }

        #undef DEF_CMD
    }

    asm_code->Size = asm_code->Ip;

    PrintASM(asm_code);

    return 0;
}

int PrintASM(asm_t* asm_code)
{
    log("\n\n^^^^^^^ START ASM CODE ^^^^^^^\n\n");

    size_t numbers_in_size = (size_t) ceil(log10 ((double) asm_code->Size));

    for (size_t i = 0; i < asm_code->Ip; i++)
    {
        log("ASM[%0*d]: %d\n", numbers_in_size, i, (asm_code->Ptr)[i]);
    }

    log("\n=======  END ASM CODE =======\n\n");
}

int SkipSpace(char** cursor)
{
    while (**cursor == ' ') {(*cursor)++;}

    return 0;
}

int SearchLabelByName(label_field* labels, char* name)
{
    size_t i = 0;

    while (i < MAX_NUM_LABELS)
    {
        if (!strcmp(labels[i].Name, name))     return i;

        i++;
    }

    print_log(FRAMED, "LABEL ERROR: No such label found");

    return -1;
}

int FuncLab(char*        ptr_arg,
            asm_t*       asm_code)
{
    size_t num_label = IdentifyNumLabel(ptr_arg, asm_code);

    asm_code->Labels[num_label].Value = (asm_code->Ip) + 1;

    return 0;
}

int FuncName(char*        ptr_arg,
             asm_t*       asm_code)
{
    log("case name\n");

    SkipSpace(&ptr_arg);

    size_t num_label = -1;

    char label_name[MAX_LEN_LABEL_NAME] = {};

    if (!sscanf(ptr_arg, "%s", label_name))
    {
        print_log(FRAMED, "LABEL ERROR: Unreadable name");

        return -1;
    }

    SkipSpace(&ptr_arg);

    ptr_arg += strlen((const char*)label_name);

    if (!sscanf(ptr_arg, "%d", &num_label))
    {
        print_log(FRAMED, "LABEL ERROR: Unreadable name");

        return -1;
    }

    strcpy(asm_code->Labels[num_label].Name, label_name);

    return 0;
}

int FuncJump(char*        ptr_arg,
             asm_t*       asm_code)
{
    log("case jump\n");

    size_t num_label = IdentifyNumLabel(ptr_arg, asm_code);

    (asm_code->Ptr)[(asm_code->Ip)++] = asm_code->Labels[num_label].Value;

    return 0;
}

size_t IdentifyNumLabel(char*        ptr_arg,
                        asm_t*       asm_code)
{
    SkipSpace(&ptr_arg);

    size_t num_label = -1;

    if (!isdigit(*ptr_arg))
    {
        char label_name[MAX_LEN_LABEL_NAME] = {};

        if (!sscanf(ptr_arg, "%s", label_name))
        {
            print_log(FRAMED, "LABEL ERROR: Unreadable name");

            return -1;
        }

        num_label = SearchLabelByName(asm_code->Labels, label_name);
    }
    else
    {
        if (!sscanf(ptr_arg, "%d", &num_label))
        {
            print_log(FRAMED, "LABEL ERROR: Unreadable digit");

            return -1;
        }
    }

    Assert(num_label == -1);

    if (num_label == -1)
    {
        return -1;
    }

    return num_label;
}

elem_t PutArg(size_t       cmd_code,
              char*        ptr_arg,
              asm_t*       asm_code)
{
    Assert(ptr_arg == NULL);

    log("cmd_code: %d\n", cmd_code);

    int  arg         = 0;

    int  read_res    = 0;

    int  reg_num     = 0;

    char reg_name[5] = {};

    SkipSpace(&ptr_arg);

    log("ptr_arg: %p, first sym to scan: %c\n", ptr_arg, *ptr_arg);

    read_res = sscanf(ptr_arg, "[%d+%[a-z]]", &arg, reg_name);

    if (!read_res)
        read_res = sscanf(ptr_arg, "[%[a-z]+%d]", &reg_name, &arg);

    if (read_res == 2)
    {
        log("case [d+rcx]\n");

        (asm_code->Ptr)[(asm_code->Ip)++] = cmd_code + ARG_IMMED + ARG_REG + ARG_RAM; //|

        HandleRegs(asm_code, reg_name);

        asm_code->Ptr[(asm_code->Ip)++] = arg;

        return arg;
    }
    //DSL

    read_res = sscanf(ptr_arg, "%d+%[a-z]", &arg, reg_name);

    if (!read_res)
    {
        read_res = sscanf(ptr_arg, "%[a-z]+%d", reg_name, &arg);
    }

    if (read_res == 2)
    {
        log("case d+rcx\n");

        (asm_code->Ptr)[(asm_code->Ip)++] = cmd_code + ARG_IMMED + ARG_REG;

        HandleRegs(asm_code, reg_name);

        (asm_code->Ptr)[(asm_code->Ip)++] = arg;

        return arg;
    }

    read_res = sscanf(ptr_arg, "[%d]", &arg);

    if (read_res == 1)
    {
        log("case [d]\n");

        (asm_code->Ptr)[(asm_code->Ip)++] = cmd_code + ARG_IMMED + ARG_RAM;
        (asm_code->Ptr)[(asm_code->Ip)++] = arg;

        return NULL;
    }

    read_res = sscanf(ptr_arg, "%[a-z]", reg_name);

    if (read_res == 1)
    {
        log("case rcx\n");

        (asm_code->Ptr)[(asm_code->Ip)++] = cmd_code + ARG_REG;

        HandleRegs(asm_code, reg_name);

        return NULL;
    }

    read_res = sscanf(ptr_arg, "%d", &arg);

    if (read_res == 1)
    {
        log("simple-dimple\n");

        (asm_code->Ptr)[(asm_code->Ip)++] = cmd_code + ARG_IMMED;

        (asm_code->Ptr)[(asm_code->Ip)++] = arg;

        return arg;
    }

    log("No argument found\n");

    return -1;
}

int HandleRegs(asm_t* asm_code, char* reg_name)
{
    if (reg_name[0] != 'r' || reg_name[2] != 'x')
    {
        print_log(FRAMED, "Syntax Error: name of register is invalid");
    }

    int reg_num = *(reg_name + 1) - 'a';

    log("reg_num: %d\n\n", reg_num);

    (asm_code->Ptr)[(asm_code->Ip)++] = reg_num;

    return 0;
}

int read_file(const char* filename,
              type_buf_char* ptr_text_buf,
              type_buf_structs* ptr_arr_structs)
{
    FILE* text_file = open_file_rmode(filename);

    if (!text_file)    return -1;

    ptr_text_buf->Size = safe_def_int(get_file_size(text_file) + 1, NULL + 1);

    log("Size of file is: %d bytes.\n", ptr_text_buf->Size);

    ptr_text_buf->Ptr = allocate_array(char, ptr_text_buf->Size);

    text_to_buffer(text_file, ptr_text_buf);

    fclose(text_file);

    make_pointers_to_lines(ptr_text_buf,
                           ptr_arr_structs);

    return 0;
}

int make_pointers_to_lines(type_buf_char* ptr_text_buf,
                           type_buf_structs* ptr_arr_structs)
{
    ptr_text_buf   ->Num_lines = count_lines(ptr_text_buf);

    ptr_arr_structs->Ptr       = allocate_array(type_prop_line , ptr_text_buf->Num_lines);

    ptr_arr_structs->Size      = ptr_text_buf->Num_lines;

    create_array_structs(ptr_text_buf, ptr_arr_structs);
}

FILE* open_file_rmode(const char* filename)
{
    FILE* r_file = fopen(filename, "rb");

    if(r_file == NULL)
    {
        print_log(FRAMED, "File not found");

        log("File %s not found\n", filename);

        return NULL;
    }

    return r_file;
}

int text_to_buffer(FILE* file, type_buf_char* ptr_text_buf)
{
    size_t num_read_sym = fread(ptr_text_buf->Ptr,
                                sizeof(char),
                                ptr_text_buf->Size - 1,
                                file);

    Assert(num_read_sym == 0);

    log("buffer size: %d\n"
        "fread number of symbols: %d\n", ptr_text_buf->Size, num_read_sym);

    if (ptr_text_buf->Size - 1 != num_read_sym)
    {
        printf("Program dies from cringe encoding\n");

        return -1;
    }

    return 0;
}

int create_array_structs(type_buf_char*    ptr_text_buf,
                         type_buf_structs* ptr_arr_structs)
{
    unsigned long int index_line = 0;

    char* ptr_prev_line = ptr_text_buf->Ptr;

    for (size_t i = 0; i < ptr_text_buf->Size; i++)
    {
        if (end_of_line((ptr_text_buf->Ptr)[i]))
        {
            if (!is_line_empty(ptr_prev_line))
            {
                (ptr_arr_structs->Ptr)[index_line] = {ptr_prev_line,
                                                     (ptr_text_buf->Ptr) + i - ptr_prev_line};

                index_line++;
            }

            ptr_prev_line = (ptr_text_buf->Ptr) + i + 1;
        }
    }

    return 1;
}

int count_lines(type_buf_char* ptr_text_buf)
{
    Assert(ptr_text_buf == NULL);

    size_t num_lines = 0;

    char* ptr_prev_line = ptr_text_buf->Ptr;

    for(size_t i = 0; i < ptr_text_buf->Size; i++)
    {
        if (*(ptr_text_buf->Ptr + i) == '\n')
        {
            if (!is_line_empty(ptr_prev_line))
            {
                num_lines++;
            }
            ptr_prev_line = ptr_text_buf->Ptr + i + 1;
        }
    }

    if (!is_line_empty(ptr_prev_line))
    {
        num_lines++;
    }

    ptr_text_buf->Num_lines = num_lines;

    log("Num lines: %d\n", num_lines);

    return num_lines;
}

static int is_line_empty(char* ptr_line)
{
    while (!end_of_line(*ptr_line))
    {
        if (isletter(*ptr_line))
        {
            return 0;
        }
        ptr_line++;
    }
    return 1;
}


int get_file_size(FILE* file)
{
    Assert(file == NULL);

    struct stat buf;

    int errcode = fstat(fileno(file), &buf);

    Assert(errcode != NULL);

    printf("File size = %d\n", buf.st_size);

    return buf.st_size;
}

bool isletter(char sym)
{
    if (('a' <= sym && sym <= 'z') ||
        ('A' <= sym && sym <= 'Z') ||
        ('à' <= sym && sym <= 'ÿ') ||
        ('À' <= sym && sym <= 'ß'))
    {
        return true;
    }

    return false;
}

int end_of_line(char sym)
{
    return (sym == '\0' ||
            sym == '\n' ||
            sym == '\r') ?
            true : false;
}

int WriteASM(int* ptr_asm, char* filename, size_t buf_size)
{
    FILE* file = open_Wfile(filename);

    WriteHead(file, buf_size);

    put_buffer(file, ptr_asm, buf_size);

    fclose(file);

    return 0;
}

int WriteHead(FILE* file, size_t buf_size)
{
    log("length signature: %d\n", strlen(SIGNATURE));

    fwrite(SIGNATURE, sizeof(char), strlen(SIGNATURE), file);

    //fputc('\n', file);

    log("written size: %d\n", buf_size);

    fwrite(&buf_size, sizeof(int), 1, file);

    //fputc('\n', file);

    return 0;
}

FILE* open_Wfile(char* filename)
{
    FILE* w_file = fopen(filename, "wb");

    if(w_file == NULL)
    {
        log("Opening file to write %s failed\n", filename);

        return NULL;
    }

    return w_file;
}

int put_buffer(FILE* w_file, int* ptr_asm, size_t buf_size)
{
    size_t num_written_sym = fwrite(ptr_asm,
                                    sizeof(int),
                                    buf_size,
                                    w_file);

    if (num_written_sym != buf_size)
    {
        print_log(FRAMED, "Error in writing to file\n");

        return -1;
    }

    log("File is written successfully\n");

    return 0;
}
