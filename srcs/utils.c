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
	if (a->st.st_mtime == b->st.st_mtime)
	{
		return (ft_strcmp(a->name, b->name) <= 0);
	}
	else
	{
		return (a->st.st_mtime > b->st.st_mtime);
	}
}
