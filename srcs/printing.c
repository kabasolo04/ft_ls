#include "ft_ls.h"

#define CACHE_SIZE 256

static struct {
	uid_t uid;
	char *name;
} g_uid_cache[CACHE_SIZE];

static struct {
	gid_t gid;
	char *name;
} g_gid_cache[CACHE_SIZE];

static unsigned g_uid_cache_count = 0;
static unsigned g_gid_cache_count = 0;

static char *get_user_cached(uid_t uid)
{
	for (unsigned i = 0; i < g_uid_cache_count; i++)
	{
		if (g_uid_cache[i].uid == uid)
			return g_uid_cache[i].name;
	}

	struct passwd *pw = getpwuid(uid);
	if (g_uid_cache_count < CACHE_SIZE)
	{
		g_uid_cache[g_uid_cache_count].uid = uid;
		g_uid_cache[g_uid_cache_count].name = pw ? pw->pw_name : "nobody";
		g_uid_cache_count++;
	}
	return pw ? pw->pw_name : "nobody";
}

static char *get_group_cached(gid_t gid)
{
	for (unsigned i = 0; i < g_gid_cache_count; i++)
	{
		if (g_gid_cache[i].gid == gid)
			return g_gid_cache[i].name;
	}

	struct group *gr = getgrgid(gid);
	if (g_gid_cache_count < CACHE_SIZE)
	{
		g_gid_cache[g_gid_cache_count].gid = gid;
		g_gid_cache[g_gid_cache_count].name = gr ? gr->gr_name : "nogroup";
		g_gid_cache_count++;
	}
	return gr ? gr->gr_name : "nogroup";
}


static char get_attr_indicator(const char *path)
{
	(void)path;
	return '.';
}

static char	file_tipe(mode_t mode)
{
	if (S_ISREG(mode))
		return ('-');
	else if (S_ISDIR(mode))
		return ('d');
	else if (S_ISLNK(mode))
		return ('l');
	else if (S_ISBLK(mode))
		return ('b');
	else if (S_ISCHR(mode))
		return ('c');
	else if (S_ISSOCK(mode))
		return ('s');
	else if (S_ISFIFO(mode))
		return ('p');
	else
		return ('-');
}

static void	print_permissions(t_file f)
{
	char buf[20];
	int idx = 0;

	buf[idx++] = file_tipe(f.st.st_mode);

	buf[idx++] = (f.st.st_mode & S_IRUSR) ? 'r' : '-';
	buf[idx++] = (f.st.st_mode & S_IWUSR) ? 'w' : '-';
	buf[idx++] = (f.st.st_mode & S_IXUSR) ? 'x' : '-';
	buf[idx++] = (f.st.st_mode & S_IRGRP) ? 'r' : '-';
	buf[idx++] = (f.st.st_mode & S_IWGRP) ? 'w' : '-';
	buf[idx++] = (f.st.st_mode & S_IXGRP) ? 'x' : '-';
	buf[idx++] = (f.st.st_mode & S_IROTH) ? 'r' : '-';
	buf[idx++] = (f.st.st_mode & S_IWOTH) ? 'w' : '-';
	buf[idx++] = (f.st.st_mode & S_IXOTH) ? 'x' : '-';

	buf[idx++] = get_attr_indicator(f.path);
	buf[idx++] = ' ';
	
	write(1, buf, idx);

	ft_printf("%u %s %s %d ", f.st.st_nlink, get_user_cached(f.st.st_uid), get_group_cached(f.st.st_gid), f.st.st_size);

	char *date;
	date = ctime(&f.st.st_mtime);
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

static void	longPrint(t_files files)
{
	ft_printf("\ntotal %u", files.file_size / 2);

	if (files.size == 0) return;

	write(1, "\n", 1);

	print_permissions(files.data[0]);
	printName(files.data[0]);

	for (size_t i = 1; i < files.size; i++)
	{
		write(1, "\n", 1);
		print_permissions(files.data[i]);
		printName(files.data[i]);
	}
}

typedef struct s_layout
{
	size_t	cols;
	size_t	rows;
	size_t	*widths;
}	t_layout;

static void print_padded(const char *str, size_t len, size_t width)
{
	size_t i;

	write(1, str, len);

	if (width > len)
	{
		i = 0;
		while (i < (width - len))
		{
			write(1, " ", 1);
			i++;
		}
	}
}

void	print_columns(t_files files, t_layout layout)
{
	size_t	row;
	size_t	col;
	size_t	i;

	row = 0;
	while (row < layout.rows)
	{
		if (row != 0)
			write(1, "\n", 1);
		col = 0;
		while (col < layout.cols)
		{
			i = row + col * layout.rows;
			if (i < files.size)
			{
				if (col == layout.cols - 1)
					ft_printf("%s", files.data[i].name);
				else
					print_padded(files.data[i].name, files.data[i].name_len, layout.widths[col] + 2);
			}
			col++;
		}
		row++;
	}
}

static t_layout calculate_layout(t_files files, size_t term_width)
{
	t_layout layout;
	size_t cols;
	size_t rows;
	size_t col;
	size_t i;
	size_t total_width;

	static size_t *widths = NULL;
	static size_t cap = 0;

	cols = files.size;

	while (cols > 0)
	{
		rows = (files.size + cols - 1) / cols;

		if (cap < cols)
		{
			free(widths);
			widths = malloc(sizeof(size_t) * cols);
			cap = cols;
		}

		memset(widths, 0, sizeof(size_t) * cols);

		total_width = 0;

		for (col = 0; col < cols; col++)
		{
			size_t start = col * rows;
			size_t end = start + rows;

			if (end > files.size)
				end = files.size;

			for (i = start; i < end; i++)
				if (files.data[i].name_len > widths[col])
					widths[col] = files.data[i].name_len;

			total_width += widths[col] + 2;
		}

		total_width -= 2;

		if (total_width <= term_width)
		{
			layout.cols = cols;
			layout.rows = rows;
			layout.widths = widths;
			return layout;
		}

		cols--;
	}

	layout.cols = 1;
	layout.rows = files.size;
	layout.widths = widths;
	return layout;
}

static void	normalPrint(t_files files)
{
	static size_t		term_width = 0;
	t_layout			layout;

	if (files.size == 0) return;

	write(1, "\n", 1);

	if (term_width == 0)
	{
		struct winsize		ws;
		ioctl(1, TIOCGWINSZ, &ws);
		term_width = ws.ws_col;
	}

	layout = calculate_layout(files, term_width);
	print_columns(files, layout);
}

void	printFiles(t_files files, char *input)
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
