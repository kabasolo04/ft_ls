#include "ft_ls.h"

static char	file_tipe(mode_t mode)
{
	if (S_ISREG(mode))
		return '-';
	if (S_ISDIR(mode))
		return 'd';
	if (S_ISLNK(mode))
		return 'l';
	if (S_ISBLK(mode))
		return 'b';
	if (S_ISCHR(mode))
		return 'c';
	if (S_ISSOCK(mode))
		return 's';
	if (S_ISFIFO(mode))
		return 'p';
	return '-';
}

size_t	append_permissions(char *buf, t_file file)
{
	unsigned int	mode = file.st.st_mode;
	size_t			i = 0;

	buf[i++] = file_tipe(mode);
	buf[i++] = (mode & S_IRUSR) ? 'r' : '-';
	buf[i++] = (mode & S_IWUSR) ? 'w' : '-';
	buf[i++] = (mode & S_IXUSR) ? 'x' : '-';
	buf[i++] = (mode & S_IRGRP) ? 'r' : '-';
	buf[i++] = (mode & S_IWGRP) ? 'w' : '-';
	buf[i++] = (mode & S_IXGRP) ? 'x' : '-';
	buf[i++] = (mode & S_IROTH) ? 'r' : '-';
	buf[i++] = (mode & S_IWOTH) ? 'w' : '-';
	buf[i++] = (mode & S_IXOTH) ? 'x' : '-';

	buf[i++] = '.';
	buf[i]   = ' ';

	return i;
}

size_t	append_links(char *dst, t_file file)
{
	char	*str;
	size_t	len;

	str = ft_uitoa(file.st.st_nlink);

	if (!str)
		return (0);

	len = ft_strlen(str);

	ft_memcpy(dst, str, len);
	free(str);

	dst[len++] = ' ';

	return len;
}

size_t	append_owner(char *dst, t_file file)
{
	char	*name;
	size_t	len;

	name = get_user_cached(file.st.st_uid);
	len = ft_strlen(name);

	ft_memcpy(dst, name, len);

	dst[len++] = ' ';

	return len;
}

size_t	append_group(char *dst, t_file file)
{
	char	*name;
	size_t	len;

	name = get_group_cached(file.st.st_gid);
	len = ft_strlen(name);

	ft_memcpy(dst, name, len);

	dst[len++] = ' ';

	return len;
}

static size_t	append_ulong(char *dst, unsigned long n)
{
	char	*str;
	size_t	len;

	str = ft_ultoa(n);

	if (!str)
		return (0);

	len = ft_strlen(str);

	ft_memcpy(dst, str, len);
	free(str);

	return len;
}

size_t	append_size(char *dst, t_file file)
{
	size_t	len;

	len = 0;

	if (S_ISCHR(file.st.st_mode) || S_ISBLK(file.st.st_mode))
	{
		len += append_ulong(dst + len, major(file.st.st_rdev));
		dst[len++] = ',';
		dst[len++] = ' ';
		len += append_ulong(dst + len, minor(file.st.st_rdev));
	}
	else
	{
		len += append_ulong(dst + len, file.st.st_size);
	}

	dst[len++] = ' ';

	return len;
}

size_t	append_date(char *dst, t_file file)
{
	char	*date;

	date = ctime(&file.st.st_mtime);

	ft_memcpy(dst, date + 4, 12);

	dst[12] = ' ';
	
	return 13;
}

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

static int	is_compressed_file(const char *name)
{
	const char*	dot;

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

void	print_name(t_file file)
{
	char		buf[1024];
	ssize_t		len;
	const char*	color;

	color = getColor(file.st.st_mode, file.name);

	ft_printf("%s%s%s", color, file.name, RESET);

	if (HAS_BIT(g_flags, FLAG_l) && S_ISLNK(file.st.st_mode))
	{
		len = readlink(file.path, buf, sizeof(buf) - 1);

		if (len >= 0)
		{
			buf[len] = '\0';
			ft_printf(" -> %s%s%s%s", RED_BG, WHITE, buf, RESET);
		}
	}
}
