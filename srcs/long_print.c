#include "ft_ls.h"

typedef size_t	(*t_formatter)(char *dst, t_file file);

static t_formatter	g_formatters[] =
{
	append_permissions,
	append_links,
	append_owner,
	append_group,
	append_size,
	append_date
};

static void	format(t_file file, unsigned int mask)
{
	char		out[1024] = {0};
	size_t		len;
	size_t		size;

	len = 0;
	size = sizeof(g_formatters) / sizeof(*g_formatters);

	for (size_t i = 0; i < size; i++)
	{
		if (!(mask & (1 << i))) continue;

		len += g_formatters[i](&out[len], file);
	}

	ft_printf("\n%s", out);

	if (mask & F_NAME) print_name(file);
}

void	long_print(t_files files)
{
	if (HAS_BIT(g_flags, MULTI_TARGET)) write(1, "\n", 1);

	ft_printf("total %u", files.file_size / 2);

	if (files.size == 0) return;

	for (size_t i = 0; i < files.size; i++)
	{
		t_file file = files.data[i];

		switch (g_flags & LONG_PRINT)
		{
			case FLAG_g: format(file, F_PERMISSIONS | F_LINKS | F_GROUP | F_SIZE | F_DATE | F_NAME); break;
			case FLAG_G: format(file, F_PERMISSIONS | F_LINKS | F_OWNER | F_SIZE | F_DATE | F_NAME); break;
			case FLAG_l: format(file, F_PERMISSIONS | F_LINKS | F_OWNER | F_GROUP | F_SIZE | F_DATE | F_NAME); break;
		}
	}
}
