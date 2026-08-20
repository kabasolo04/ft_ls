#include "ft_ls.h"

int		g_flags = 0;
char	g_exit = 0;

static t_cmp	get_comparator(void)
{
	static const struct
	{
		int		flag;
		t_cmp	cmp;
	}
	sorts[] =
	{
		{FLAG_U, cmp_nothing},
		{FLAG_t, cmp_time},
		{FLAG_S, cmp_size},
	};

	for (size_t i = 0; i < sizeof(sorts) / sizeof(sorts[0]); i++)
	{
		if (HAS_FLAG(g_flags, sorts[i].flag)) return (sorts[i].cmp);
	}

	return (cmp_alpha);
}

static t_files	list_directory(DIR *dir, char *input)
{
	t_files	files;

	files = getFiles(dir, input);

	closedir(dir);

	merge_sort(&files, 0, files.size - 1, get_comparator());

	return	files;
}

static void	printFiles(t_files files, char *input)
{
	static char	first = 1;

	if (!first)
		write(1, "\n\n", 2);

	first = 0;

	if (HAS_FLAG(g_flags, MULTI_TARGET)) ft_printf("%s:", input);

	if (HAS_FLAG(g_flags, LONG_PRINT))
	{
		longPrint(files);
	}
	else
	{
		normalPrint(files);
	}
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
	struct stat	st;
	DIR*		dir;
	t_files		files;

	if (stat(input, &st) == -1 || !S_ISDIR(st.st_mode)) return;

	dir = opendir(input);

	if (!dir)
	{
		ft_printf("\nft_ls: cannot open directory '%s': %s", input, strerror(errno));

		if (g_exit < 2)
			g_exit = 2 - HAS_FLAG(g_flags, MULTI_TARGET); // in MULTI_TARGET the error code is 1 instead of 2
	
		return ;
	}

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
	}

	if (targetNumber == 0)
		return displayInfo("."), write(1, "\n", 1), free_cache(), g_exit;

	ADD_FLAG(g_flags, MULTI * (targetNumber > 1));
	
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] != '-' || argv[i][1] == ' ') displayInfo(argv[i]);
	}

	return write(1, "\n", 1), free_cache(), g_exit;
}
