#include <stdio.h>
#include <stdint.h>

uint16_t swap16(uint16_t x)
{
    return (x << 8) | (x >> 8);
}

int main(void)
{
    uint16_t program[] =
    {
        0x3000,
        0x2002,
        0xF021,
        0xF025,
        0x0041
    };

    FILE *f = fopen("print_a.obj", "wb");
    size_t count = sizeof program / sizeof program[0];

    for (size_t i=0; i<count; i++)
    {
        uint16_t big_endian = swap16(program[i]);
        fwrite(&big_endian, sizeof(uint16_t), 1, f);
    }

    fclose(f);
    return 0;
}