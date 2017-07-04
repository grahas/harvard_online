#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
int main(void)
{
FILE* out = fopen("out.txt", "w+");

string buffer = "Milo Banana\n";

fwrite(buffer, 12, 1, out);
fwrite(buffer, 12, 1, out);
fwrite(buffer, 12, 1, out);

fclose(out);
}