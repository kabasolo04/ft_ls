#include "ft_ls.h"

static int	is_compressed_file(const char *name)
{
	const char*	dot;

	static const char	*extensions[] =
	{
		".zst",
		".gz",
		".bz2",
		".xz",
		".zip",
		".tar",
		".tgz",
		".7z",
		".rar",
		".lz4",
		NULL
	};

	dot = ft_strrchr(name, '.');

	if (!dot || dot == name) return 0;

	for (int i = 0; extensions[i]; i ++)
	{
		if (ft_strcmp(dot, extensions[i]) == 0) return 1;
	}
	
	return 0;
}

static const char	*getColor(const mode_t mode, const char *name)
{
	if (S_ISDIR(mode))	return BLUE;
	if (S_ISLNK(mode))	return RED;
	if (S_ISFIFO(mode))	return YELLOW;
	if (S_ISSOCK(mode))	return MAGENTA;

	if (S_ISBLK(mode) || S_ISCHR(mode)) return YELLOW;

	if (S_ISREG(mode))
	{
		if (is_compressed_file(name)) return RED;

		if ((mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0) return GREEN;
	}

	return RESET;
}

void	printName(t_file data)
{
	char		buf[1024];
	ssize_t		len;
	const char*	color;

	color = getColor(data.st.st_mode, data.name);

	ft_printf("%s%s%s", color, data.name, RESET);

	if (HAS_FLAG(g_flags, FLAG_l) && S_ISLNK(data.st.st_mode))
	{
		len = readlink(data.path, buf, sizeof(buf) - 1);

		if (len >= 0)
		{
			buf[len] = '\0';
			ft_printf(" -> %s%s%s%s", RED_BG, WHITE, buf, RESET);
		}
	}
}

static char	file_tipe(mode_t mode)
{
	if (S_ISREG(mode))
		return ('-');
	if (S_ISDIR(mode))
		return ('d');
	if (S_ISLNK(mode))
		return ('l');
	if (S_ISBLK(mode))
		return ('b');
	if (S_ISCHR(mode))
		return ('c');
	if (S_ISSOCK(mode))
		return ('s');
	if (S_ISFIFO(mode))
		return ('p');
	return ('-');
}

void	build_permissions(char *buf, mode_t mode)
{
	buf[0] = file_tipe(mode);

	buf[1] = (mode & S_IRUSR) ? 'r' : '-';
	buf[2] = (mode & S_IWUSR) ? 'w' : '-';
	buf[3] = (mode & S_IXUSR) ? 'x' : '-';
	buf[4] = (mode & S_IRGRP) ? 'r' : '-';
	buf[5] = (mode & S_IWGRP) ? 'w' : '-';
	buf[6] = (mode & S_IXGRP) ? 'x' : '-';
	buf[7] = (mode & S_IROTH) ? 'r' : '-';
	buf[8] = (mode & S_IWOTH) ? 'w' : '-';
	buf[9] = (mode & S_IXOTH) ? 'x' : '-';

	buf[10] = '.';
	buf[11] = '\0';
}
