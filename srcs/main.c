#include "ft_ls.h"

char	g_flags = 0;
char	g_exit = 0;

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

static void	printFiles(t_files files, char *input)
{
	static char		first = 1;

	if (!first)
		write(1, "\n\n", 2);

	first = 0;

	if (HAS_FLAG(g_flags, MULTI_TARGET) || HAS_FLAG(g_flags, FLAG_R))
	{
		ft_printf("%s:", input);
	}

	if (HAS_FLAG(g_flags, FLAG_l))
		longPrint(files);
	else
		normalPrint(files);
}

static void	displayInfo(char* input);

static void	handleRecursive(t_files	files)
{
	if (files.size == 0) return;

	if (!HAS_FLAG(g_flags, FLAG_R)) return ;

	for (size_t i = 0; i < files.size; i++)
	{
		if (ft_strncmp(files.data[i].name, ".", 2) == 0) continue;
	
		if (ft_strncmp(files.data[i].name, "..", 3) == 0) continue;

		if (S_ISDIR(files.data[i].st.st_mode)) displayInfo(files.data[i].path);
	}
}

static void	displayInfo(char* input)
{
	struct stat st;

	if (stat(input, &st) == -1 || !S_ISDIR(st.st_mode)) return;

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

	handleRecursive(files);

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

	if (targetNumber == 0)
		displayInfo(".");
	else
	{
		for (int i = 1; i < argc; i++)
		{
			if (argv[i][0] != '-' || argv[i][1] == ' ')
				displayInfo(argv[i]);
		}
	}

	return write(1, "\n", 1), g_exit;
}
