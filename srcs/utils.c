#include "ft_ls.h"

char	*join_path(const char *dir, const char *name)
{
	char	*path;
	size_t	len_dir;
	size_t	len_name;

	len_dir = ft_strlen(dir);
	len_name = ft_strlen(name);

	while (len_dir >= 1 && dir[len_dir - 1] == '/')
	{
		len_dir --;
	}

	path = malloc(len_dir + 1 + len_name + 1);

	if (!path) return (NULL);

	ft_memcpy(path, dir, len_dir);
	path[len_dir] = '/';

	ft_memcpy(path + len_dir + 1, name, len_name);
	path[len_dir + 1 + len_name] = '\0';

	return (path);
}

char	cmp_time(t_file *a, t_file *b)
{
	if (a->st.st_mtime != b->st.st_mtime)
	{
		return (a->st.st_mtime > b->st.st_mtime);
	}
	
	if (a->st.st_mtim.tv_nsec != b->st.st_mtim.tv_nsec)
	{
		return (a->st.st_mtim.tv_nsec > b->st.st_mtim.tv_nsec);
	}
	
	return (ft_strcmp(a->name, b->name) < 0);
}

char	cmp_size(t_file *a, t_file *b)
{
	return (a->st.st_size > b->st.st_size);
}

char	cmp_nothing(t_file *a, t_file *b)
{
	(void)a;
	(void)b;

	return 1;
}

char	cmp_alpha(t_file *a, t_file *b)
{
	int	i;
	int	ca, cb;

	i = 0;
	while (a->name[i] && b->name[i])
	{
		ca = ft_tolower(a->name[i]);
		cb = ft_tolower(b->name[i]);

		if (ca != cb)
			return (ca < cb);

		if (a->name[i] != b->name[i])
			return ((unsigned char)a->name[i] < (unsigned char)b->name[i]);

		i++;
	}

	return a->name[i] == '\0';
}

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

	if (S_ISBLK(mode) || S_ISCHR(mode))	return YELLOW;

	if (S_ISREG(mode))
	{
		if (is_compressed_file(name))	return RED;

		if ((mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0)	return GREEN;
	}

	return RESET;
}

void	printName(t_file data)
{
	char		buf[1024];
	ssize_t		len;
	const char *color;

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
