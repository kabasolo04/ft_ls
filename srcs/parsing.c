#include "ft_ls.h"

char	flag_error(char* input)
{
	if (input[0] != '-') return 0;

	for (size_t i = 1; input[i]; i++)
	{
		switch (input[i])
		{
			case 'a': ADD_BIT(g_flags, FLAG_a); break;
			case 'r': ADD_BIT(g_flags, FLAG_r); break;
			case 'R': ADD_BIT(g_flags, FLAG_R); break;

			case 'l': RM_BIT(g_flags, LONG_PRINT); ADD_BIT(g_flags, FLAG_l); break;
			case 'g': RM_BIT(g_flags, LONG_PRINT); ADD_BIT(g_flags, FLAG_g); break;
			case 'G': RM_BIT(g_flags, LONG_PRINT); ADD_BIT(g_flags, FLAG_G); break;
			
			case 't': RM_BIT(g_flags, SORT_FLAGS); ADD_BIT(g_flags, FLAG_t); break;
			case 'S': RM_BIT(g_flags, SORT_FLAGS); ADD_BIT(g_flags, FLAG_S); break;
			case 'U': RM_BIT(g_flags, SORT_FLAGS); ADD_BIT(g_flags, FLAG_U); break;
		
			default:
				return ft_printf("ft_ls: invalid option -- '%c'\nOpen README.md in the root of this repo for more information.\n", input[i]), 1;
		}
	}

	return 0;
}

char	target_error(char* input)
{
	struct stat	st;
	static char	firstFlag = 1;

	if (input[0] == '-' && input[1] != '\0') return 0;

	if (lstat(input, &st) == -1)
	{
		if (!firstFlag)
			write(1, "\n", 1);

		ft_printf("ft_ls: cannot access '%s': %s", input, strerror(errno));

		firstFlag = 0;
		g_exit	  = 2;
	}

	return 1;
}
