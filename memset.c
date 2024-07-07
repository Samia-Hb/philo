# include <stdio.h>
# include <string.h>

int main()
{
    char buffer[5];
    memset(buffer, 65, sizeof(buffer));
    printf("%s\n", buffer);
}