#include "ft_ls.h"

char	*join_path(const char *dir, const char *name)
{
	char	*path;
	size_t	len_dir;
	size_t	len_name;

	len_dir = ft_strlen(dir);
	len_name = ft_strlen(name);

	while (len_dir > 1 && dir[len_dir - 1] == '/')
		len_dir--;

	path = malloc(len_dir + 1 + len_name + 1);
	if (!path)
		return (NULL);

	ft_memcpy(path, dir, len_dir);
	path[len_dir] = '/';
	ft_memcpy(path + len_dir + 1, name, len_name);
	path[len_dir + 1 + len_name] = '\0';

	return (path);
}

static int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

static int	my_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

char	cmp_alpha(t_file *a, t_file *b)
{
	int	i;
	int	ca, cb;

	i = 0;
	while (a->name[i] && b->name[i])
	{
		ca = my_tolower(a->name[i]);
		cb = my_tolower(b->name[i]);
		if (ca != cb)
			return (ca < cb);

		if (a->name[i] != b->name[i])
			return ((unsigned char)a->name[i] < (unsigned char)b->name[i]);
		i++;
	}
	return (a->name[i] == '\0');
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

typedef struct s_color_rule
{
	mode_t		mask;
	mode_t		value;
	const char	*color;
}	t_color_rule;

static const t_color_rule	g_rules[] = {
	{S_IFMT, S_IFDIR, BLUE},
	{S_IFMT, S_IFLNK, RED},
	{S_IFMT, S_IFIFO, YELLOW},
	{S_IFMT, S_IFSOCK, MAGENTA},
	{S_IFMT, S_IFBLK, YELLOW},
	{S_IFMT, S_IFCHR, YELLOW},
	{S_IXUSR | S_IXGRP | S_IXOTH, S_IXUSR | S_IXGRP | S_IXOTH, GREEN},
};

static const char	*getColor(const mode_t mode)
{
	size_t	i;

	i = 0;
	while (i < sizeof(g_rules) / sizeof(*g_rules))
	{
		if ((mode & g_rules[i].mask) == g_rules[i].value)
			return (g_rules[i].color);
		i++;
	}
	return (RESET);
}

void	printName(t_file data)
{
	char		buf[1024];
	ssize_t		len;
	const char *color;

	color = getColor(data.st.st_mode);

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
