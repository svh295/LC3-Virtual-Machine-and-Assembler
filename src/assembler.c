#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

//limits
#define MAX_SYMBOLS 256
#define MAX_LINE 256

typedef struct {
    char name[64];
    uint16_t addr;
} symbol;

symbol sym_table[MAX_SYMBOLS];
int sym_count = 0;

//swaps 2 bytes of 16 bit number
uint16_t swap16(uint16_t x)
{
    return (x << 8) | (x >> 8);
}

//writes to output file
void write_word(FILE *out, uint16_t word)
{
    uint16_t swapped = swap16(word);
    fwrite(&swapped, sizeof(uint16_t), 1, out);
}

//add symbol to symbol table
void add_symbol(const char* name, uint16_t addr)
{
    strcpy(sym_table[sym_count].name, name);
    sym_table[sym_count].addr = addr;
    sym_count++;
}

//searches symbol table to find a label
int find_symbol(const char *name, uint16_t *addr)
{
    for (int i=0; i<sym_count; i++)
    {
        if (strcmp(sym_table[i].name, name) == 0)
        {
            *addr = sym_table[i].addr;
            return 1;
        }
    }
    return 0;
}

void to_upper(char *s)
{
    for (; *s; ++s) *s = toupper((unsigned char)*s);
}

//converts numbers from assembly to C integers
int parse_num(const char *s)
{
    if (s[0] == 'x' || s[0] == 'X') return (int)strtol(s + 1, NULL, 16);
    if (s[0] == '#') return (int)strtol(s + 1, NULL, 10);
    return (int)strtol(s, NULL, 10);
}

//extracts register number from assembly register
int parse_reg(const char *s)
{
    return s[1] - '0';
}

//determines whether the first token in line is an instruction or an assembler directive
int opcode_or_dir(const char *tok)
{
    char upper[64];
    strcpy(upper, tok);
    to_upper(upper);

    if (upper[0] == '.') return 1;
    if (strncmp(upper, "BR", 2) == 0) return 1;

    const char *keywords[] = 
    {
        "ADD", "AND", "NOT", "LD", "LDI", "LDR", "LEA", "ST", "STI",
        "STR", "JMP", "JSR", "JSRR", "RET", "GETC", "OUT", "PUTS", "IN",
        "PUTSP", "HALT", "TRAP", NULL
    };

    for (int i=0; keywords[i] != NULL; i++) if (strcmp(upper, keywords[i]) == 0) return 1;
    
    return 0;
}

void extract_string(const char *raw, char *dest)
{
    const char *start = strchr(raw, '"');
    if (!start)
    {
        dest[0] = '\0';
        return;
    }
    start++;
    const char *end = strchr(start, '"');
    if (!end)
    {
        dest[0] = '\0';
        return;
    }
    size_t len = end - start;

    memcpy(dest, start, len);
    dest[len] = '\0';
}

//calculates distance between PC and address of start
int get_pc_offset(uint16_t target, uint16_t pc, int bits)
{
    int offset = (int)target - (int)pc;

    int min = -(1 << (bits - 1));
    int max = (1 << (bits - 1)) - 1;

    if (offset < min || offset > max)
    {
        printf("Error: PC-relative offset out of range\n");
        return -1;
    }
    return offset & ((1 << bits) - 1);
}

int main(int argc, char *argv[])
{
    if ( argc < 3)
    {
        printf("Usage: %s <input.asm> <output.obj>\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in)
    {
        perror("Failed to open input file");
        return 1;
    }

    char line[MAX_LINE];
    char raw_copy[MAX_LINE];
    uint16_t pc = 0;

    //build symbol table
    while (fgets(line, sizeof line, in))
    {
        char *comment = strchr(line, ';');
        if (comment) *comment = '\0';       //get rid of comments
        strcpy(raw_copy, line);

        char *tokens[10];
        int count = 0;
        char *tok = strtok(line, " \t\r\n,");   //tokenize the line (seperate into individual tokens)
        while (tok && count < 10)
        {
            tokens[count++] = tok;
            tok = strtok(NULL, " \t\r\n,");
        }
        if (count == 0) continue;

        char op[64];
        strcpy(op, tokens[0]);
        to_upper(op);

        if (strcmp(op, ".ORIG") == 0)       //handle orgi (specifies where program starts)
        {
            pc = (uint16_t)parse_num(tokens[1]);
            continue;
        }
        if (strcmp(op, ".END") == 0) break;     //handle end (specifies progra end)

        int ind = 0;        //identify labels
        if (!opcode_or_dir(tokens[0]))
        {
            char label[64];
            strcpy(label, tokens[0]);
            size_t len = strlen(label);
            if (len > 0 && label[len - 1] == ':') label[len - 1] = '\0';
            add_symbol(label, pc);
            ind = 1;
        }
        if (ind >= count) continue;

        strcpy(op, tokens[ind]);
        to_upper(op);

        if (strcmp(op, ".STRINGZ") == 0)    //update counter
        {
            char str[MAX_LINE];
            extract_string(raw_copy, str);
            int length = 0;

            for (size_t i=0; i<strlen(str); i++)
            {
                if (str[i] == '\\' && str[i+1] == 'n') i++;
                length++;
            }
            pc += (uint16_t)(length + 1);
        }
        else if (strcmp(op, ".BLKW") == 0) pc += (uint16_t)parse_num(tokens[ind + 1]);
        else pc += 1;
    }

    //encode and write .obj file (second pass)
    rewind(in);
    FILE *out = fopen(argv[2], "wb");
    if (!out)
    {
        perror("Failed to open output file");
        fclose(in);
        return 1;
    }

    pc = 0;
    while (fgets(line, sizeof line, in))
    {
        char * comment = strchr(line, ';');
        if (comment) *comment = '\0';
        strcpy(raw_copy, line);

        char *tokens[10];
        int count = 0;
        char *tok = strtok(line, " \t\r\n,");
        while(tok && count < 10)
        {
            tokens[count++] = tok;
            tok = strtok(NULL, " \t\r\n,");
        }
        if (count == 0) continue;

        char op[64];
        strcpy(op, tokens[0]);
        to_upper(op);

        if (strcmp(op, ".ORIG") == 0)
        {
            pc = (uint16_t)parse_num(tokens[1]);
            write_word(out, pc);
            continue;
        }
        if (strcmp(op, ".END") == 0) break;

        int ind = 0;
        if (!opcode_or_dir(tokens[0])) ind = 1;
        if (ind >= count) continue;

        strcpy(op, tokens[ind]);
        to_upper(op);

        if (strcmp(op, ".STRINGZ") == 0)
        {
            char str[MAX_LINE];
            extract_string(raw_copy, str);

            //added checking for newline
            for (size_t i = 0; i<strlen(str); i++)
            {
                //temp debugging
                //printf("Writing Character: %c | Hex: x%04X\n", str[i], (unsigned char)str[i]);
                if (str[i] == '\\' && str[i+1] == 'n')
                {
                    write_word(out, 0x000A);
                    i++;
                }
                else write_word(out, (uint16_t)str[i]);
                pc++;
            }

            write_word(out, 0x0000);
            pc++;
            continue;
        }

        if (strcmp(op, ".BLKW") == 0)
        {
            int n = parse_num(tokens[ind + 1]);
            for (int i=0; i<n; i++)
            {
                write_word(out, 0x0000);
                pc++;
            }
            continue;
        }

        pc++;

        if (strcmp(op, ".FILL") == 0)
        {
            uint16_t target;
            if (find_symbol(tokens[ind + 1], &target)) write_word(out, target);
            else write_word(out, (uint16_t)parse_num(tokens[ind + 1]));
        }
        else if (strcmp(op, "ADD") == 0 || strcmp(op, "AND") == 0)
        {
            uint16_t opcode = (strcmp(op, "ADD") == 0) ? 0x1 : 0x5;
            uint16_t dr = parse_reg(tokens[ind + 1]);
            uint16_t sr1 = parse_reg(tokens[ind + 2]);
            if (toupper((unsigned char)tokens[ind + 3][0]) == 'R')
            {
                uint16_t sr2 = parse_reg(tokens[ind + 3]);
                write_word(out, (opcode << 12) | (dr << 9) | (sr1 << 6) | sr2);
            }
            else
            {
                //uint16_t imm5 = parse_num(tokens[ind + 3]) & 0x1F;
                int imm5 = parse_num(tokens[ind + 3]);
                if (imm5 < -16 || imm5 > 15)
                {
                    printf("Error: ADD/AND immediate out of range: %d", imm5);
                    return 1;
                }
                uint16_t encoded_imm5 = imm5 & 0x1F;
                write_word(out, (opcode << 12) | (dr << 9) | (sr1 << 6) | (1 << 5) | encoded_imm5);
            }
        }
        else if (strcmp(op, "NOT") == 0)
        {
            uint16_t dr = parse_reg(tokens[ind + 1]);
            uint16_t sr = parse_reg(tokens[ind + 2]);
            write_word(out, (0x9 << 12) | (dr << 9) | (sr << 6) | 0x3F);
        }
        else if (strcmp(op, "LEA") == 0 || strcmp(op, "LD") == 0 || strcmp(op, "ST") == 0) //LEA is load effective address
        {
            uint16_t opcode = (strcmp(op, "LEA") == 0) ? 0xE: (strcmp(op, "LD") == 0) ? 0x2 : 0x3;
            uint16_t reg = parse_reg(tokens[ind + 1]);
            uint16_t target;
            if (!find_symbol(tokens[ind + 2], &target))
            {
                printf("Error: Undefined Label %s\n", tokens[ind + 2]);
                return 1;
            }
            int offset = get_pc_offset(target, pc, 9);
            if (offset < 0) return 1;
            write_word(out, (opcode << 12) | (reg << 9) | offset);
            //temp debugging
            //printf("LEA Label: %s | Target: x%04X | PC: x%04X\n", tokens[ind + 2], target, pc);
        }
        else if (strncmp(op, "BR", 2) == 0)
        {
            uint16_t nzp = 0;
            if (strchr(op + 2, 'N')) nzp |= 0x4;
            if (strchr(op + 2, 'Z')) nzp |= 0x2;
            if (strchr(op + 2, 'P')) nzp |= 0x1;
            if (nzp == 0) nzp = 0x7;
            uint16_t target;
            if (!find_symbol(tokens[ind + 1], &target))
            {
                printf("Error: Undefined label %s\n", tokens[ind + 1]);
                return 1;
            }
            int offset = get_pc_offset(target, pc, 9);
            if (offset < 0) return 1;
            write_word(out, (nzp << 9) | offset);
        }
        else if (strcmp(op, "JMP") == 0)
        {
            uint16_t base = parse_reg(tokens[ind + 1]);
            write_word(out, (0xC << 12) | (base << 6));
        }
        else if (strcmp(op, "LDI") == 0)
        {
            uint16_t dr = parse_reg(tokens[ind + 1]);
            uint16_t target;

            if (!find_symbol(tokens[ind + 2], &target))
            {
                printf("Error: Undefined label %s\n", tokens[ind + 2]);
                return 1;
            }

            uint16_t offset = get_pc_offset(target, pc, 9);
            if (offset < 0) return 1;
            write_word(out, (0xA << 12) | (dr << 9) | offset);
        }
        else if (strcmp(op, "STI") == 0)
        {
            uint16_t sr = parse_reg(tokens[ind + 1]);
            uint16_t target;

            if (!find_symbol(tokens[ind +2], &target))
            {
                printf("Error: Undefined label %s\n", tokens[ind + 2]);
                return 1;
            }

            uint16_t offset = get_pc_offset(target, pc, 9);
            if (offset < 0) return 1;
            write_word(out, (0xB << 12) | (sr << 9) | offset);
        }
        else if (strcmp(op, "LDR") == 0)    //load register
        {
            uint16_t dr = parse_reg(tokens[ind + 1]);
            uint16_t base = parse_reg(tokens[ind+2]);
            int offset = parse_num(tokens[ind+3]);
            if (offset < -32 || offset > 31)
            {
                printf("Error: LDR offset out of range");
                return 1;
            }
            uint16_t offset6 = offset & 0x3F;
            write_word(out, (0x6 << 12) | (dr << 9) | (base << 6) | offset6);
        }
        else if (strcmp(op, "STR") == 0)    //store relative
        {
            uint16_t sr = parse_reg(tokens[ind+1]);
            uint16_t base = parse_reg(tokens[ind+2]);
            int offset = parse_num(tokens[ind + 3]);
            if (offset < -32 || offset > 31)
            {
                printf("Error: STR offset out of range");
                return 1;
            }
            uint16_t offset6 = offset & 0x3F;
            write_word(out, (0x7 << 12) | (sr << 9) | (base << 6) | offset6);
        }
        else if (strcmp(op, "JSR") == 0)    //jump to subroutine
        {
            uint16_t target;
            if (!find_symbol(tokens[ind+1], &target))
            {
                printf("Error: Undefined label %s\n", tokens[ind+1]);
                return 1;
            }
            uint16_t offset = get_pc_offset(target, pc, 11);
            if (offset < 0) return 1;
            write_word(out, (0x4 << 12) | (1 << 11) | offset);
        }
        else if (strcmp(op, "JSRR") == 0)   //jump to subroutine using a register
        {
            uint16_t base = parse_reg(tokens[ind+1]);
            write_word(out, (0x4 << 12) | (base << 6));
        }
        else if (strcmp(op, "TRAP") == 0)   //general trap
        {
            int trapvect = parse_num(tokens[ind + 1]);

            //temp debugging
            //printf("TRAP detected: %s | Vector: x%02X\n", tokens[ind + 1], trapvect);

            if (trapvect < 0 || trapvect > 255)
            {
                printf("Error: Invalid trap vector\n");
                return 1;
            }
            write_word(out, (0xF << 12) | (trapvect & 0xFF));
        }
        else if (strcmp(op, "RET") == 0) write_word(out, 0xC1C0);
        else if (strcmp(op, "GETC") == 0) write_word(out, 0xF020);
        else if (strcmp(op, "OUT") == 0)  write_word(out, 0xF021);
        else if (strcmp(op, "PUTS") == 0) write_word(out, 0xF022);
        else if (strcmp(op, "IN") == 0)   write_word(out, 0xF023);
        else if (strcmp(op, "PUTSP") == 0) write_word(out, 0xF024);
        else if (strcmp(op, "HALT") == 0) write_word(out, 0xF025);
    }

    fclose(in);
    fclose(out);
    printf("Assembled %s to %s successfully\n", argv[1], argv[2]);
    return 0;
}
