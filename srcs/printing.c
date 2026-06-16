#include "ft_ls.h"

static void	print_permissions(struct stat st)
{
	if (S_ISREG(st.st_mode))		write(1, "-", 1);
	else if (S_ISDIR(st.st_mode))	write(1, "d", 1);
	else if (S_ISLNK(st.st_mode))	write(1, "l", 1);

	write(1, (st.st_mode & S_IRUSR) ? "r" : "-", 1);
	write(1, (st.st_mode & S_IWUSR) ? "w" : "-", 1);
	write(1, (st.st_mode & S_IXUSR) ? "x" : "-", 1);

	write(1, (st.st_mode & S_IRGRP) ? "r" : "-", 1);
	write(1, (st.st_mode & S_IWGRP) ? "w" : "-", 1);
	write(1, (st.st_mode & S_IXGRP) ? "x" : "-", 1);

	write(1, (st.st_mode & S_IROTH) ? "r" : "-", 1);
	write(1, (st.st_mode & S_IWOTH) ? "w" : "-", 1);
	write(1, (st.st_mode & S_IXOTH) ? "x" : "-", 1);
	write(1, ". ", 2);

	ft_printf("%u ", st.st_nlink);
	
	struct passwd *pw = getpwuid(st.st_uid);
	ft_printf("%s ", pw ? pw->pw_name : "nobody");
	struct group *gr = getgrgid(st.st_gid);
	ft_printf("%s ", gr ? gr->gr_name : "nogroup");
	ft_printf("%d ", st.st_size);

	char *date;
	date = ctime(&st.st_mtime);
	write(1, date + 4, 12);
	write(1, " ", 1);
}

static void	printName(t_file data)
{
	ft_printf("%s", data.name);
	if (S_ISLNK(data.st.st_mode))
	{
		char	buf[1024];
		ssize_t len;

		len = readlink(data.path, buf, sizeof(buf) - 1);
		if (len >= 0)
		{
			buf[len] = '\0';
			ft_printf(" -> %s", buf);
		}
	}
}

static void	longPrint(t_files f)
{
	print_permissions(f.data[0].st);
	printName(f.data[0]);

	for (size_t i = 1; i < f.size; i++)
	{
		write(1, "\n", 1);
		print_permissions(f.data[i].st);
		printName(f.data[i]);
	}
}

static void	normalPrint(t_files f)
{
	ft_printf("%s", f.data[0].name);

	for (size_t i = 1; i < f.size; i++)
	{
		ft_printf("  %s", f.data[i].name);
	}
}

void	printFiles(t_files f, char *input)
{
	static char		prefix = 0;

	if (prefix) write(1, "\n", 1);

	if (HAS_FLAG(g_flags, MULTI_TARGET) || HAS_FLAG(g_flags, FLAG_R))
	{
		if (prefix) write(1, "\n", 1);

		ft_printf("%s:", input);

		if (f.size > 0)
			write(1, "\n", 1);
		else
			return;
	}

	prefix = 1;

	if (f.size == 0) return;

	if (HAS_FLAG(g_flags, FLAG_l))
		longPrint(f);
	else
		normalPrint(f);
}