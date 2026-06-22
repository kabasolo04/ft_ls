#include "ft_ls.h"

char	flagError(char* input)
{
	if (input[0] != '-')
		return 0;

	for (size_t i = 1; input[i]; i++)
	{
		switch (input[i])
		{
			case 'l': ADD_FLAG(g_flags, FLAG_l); break;
			case 'R': ADD_FLAG(g_flags, FLAG_R); break;
			case 'a': ADD_FLAG(g_flags, FLAG_a); break;
			case 'r': ADD_FLAG(g_flags, FLAG_r); break;
			case 't': ADD_FLAG(g_flags, FLAG_t); break;
			default:
				return ft_printf("ft_ls: invalid option -- '%c'\nTry 'ls --help' for more information.\n", input[i]), 1;
		}
	}
	return 0;
}

char	targetError(char* input)
{
	if (input[0] == '-' && input[1] != '\0')
	{
		return 0;
	}

	struct stat st;

	static char	firstFlag = 1;

	if (lstat(input, &st) == -1)
	{
		if (!firstFlag)
			write(1, "\n", 1);

		ft_printf("ft_ls: cannot access '%s': %s", input, strerror(errno));
		g_exit = 2;
	}

	firstFlag = 0;

	return (1);
}