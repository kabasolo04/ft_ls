#include "ft_ls.h"

char	g_flags = 0;
char	g_exit = 0;

static char	flagError(char* input)
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

static char	targetError(char* input)
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

static t_files	list_directory(DIR *dir, char *input)
{
	t_files			files;

	files = getFiles(dir, input);

	closedir(dir);

	if (HAS_FLAG(g_flags, FLAG_t))
		merge_sort(&files, 0, files.size - 1, cmp_time);
	else
		merge_sort(&files, 0, files.size - 1, cmp_alpha);

	return	files;
}

static void	displayInfo(char* input);

static void	handleRecursive(t_files	files)
{
	for (size_t i = 0; i < files.size; i++)
	{
		if (ft_strncmp(files.data[i].name, ".", 2) == 0) continue;
	
		if (ft_strncmp(files.data[i].name, "..", 3) == 0) continue;

		if (S_ISDIR(files.data[i].st.st_mode)) displayInfo(files.data[i].path);
	}
}

static void	displayInfo(char* input)
{
	if (input[0] == '-' && input[1] != ' ') return;

	struct stat st;

	if (lstat(input, &st) == -1 || !S_ISDIR(st.st_mode)) return;

	DIR	*dir;

	dir = opendir(input);
	if (!dir)
	{
		ft_printf("\nft_ls: cannot open directory '%s': %s", input, strerror(errno));

		if (g_exit < 2) g_exit = 2 - HAS_FLAG(g_flags, MULTI_TARGET) || HAS_FLAG(g_flags, FLAG_R);
	
		return ;
	}

	t_files	files;

	files = list_directory(dir, input);

	printFiles(files, input);

	if (files.size == 0) return;

	if (HAS_FLAG(g_flags, FLAG_R)) handleRecursive(files);

	free_files(&files);
}

int	main(int argc, char** argv)
{
	size_t	targetNumber = 0;

	for (int i = 1; i < argc; i++)
	{
		if (flagError(argv[i])) return 2;
	}

	for (int i = 1; i < argc; i++)
	{
		targetNumber += targetError(argv[i]);
	
		if (targetNumber > 1)
			ADD_FLAG(g_flags, MULTI_TARGET);
	}

	if (!targetNumber) return displayInfo("."), write(1, "\n", 1), g_exit;

	for (int i = 1; i < argc; i++)
	{
		displayInfo(argv[i]);
	}

	write(1, "\n", 1);

	return g_exit;
}
