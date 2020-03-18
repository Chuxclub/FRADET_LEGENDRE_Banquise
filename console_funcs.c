#include "console_funcs.h"

void center_printf_col(int width)
{
    for(int i = 0; i < width; i++)
        printf(" ");
}

void center_printf_row(int height)
{
    for(int i = 0; i < height; i++)
        printf("\n");
}
