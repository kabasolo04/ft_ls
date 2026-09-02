#include "ft_ls.h"

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

t_cmp	get_comparator(void)
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

char	*join_path(const char *dir, const char *name)
{
	char	*path;
	size_t	len_dir;
	size_t	len_name;

	len_dir  = ft_strlen(dir);
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
