#include <stdio.h>
int	ft_atoi_base(const char *str, int str_base)
{
    int i = 0;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    while (str[i] == '+' || str[i] == '-')
        i++;
    int j = 0;
    int result = 0;
    while (j < ft_strlen(str))
    {
        result += pow( , );
    }
}
int main(int argc, char **argv)
{
    (void)argc;
    char *str = argv[1];
    printf("%d\n", ft_atoi_base(str, 16));
}