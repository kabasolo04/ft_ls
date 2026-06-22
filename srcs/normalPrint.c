#include "ft_ls.h"

typedef struct s_layout
{
	size_t	cols;
	size_t	rows;
	size_t	*widths;
}	t_layout;

static void print_padded(size_t len, size_t width)
{
	size_t i;

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
				printName(files.data[i]);
				if (col != layout.cols - 1)
					print_padded(files.data[i].name_len, layout.widths[col] + 2);
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

void	normalPrint(t_files files)
{
	static size_t		term_width = 0;
	t_layout			layout;

	if (files.size == 0) return;

	if (HAS_FLAG(g_flags, MULTI_TARGET) || HAS_FLAG(g_flags, FLAG_R))
	{
		write(1, "\n", 1);
	}

	if (term_width == 0)
	{
		struct winsize		ws;
		ioctl(1, TIOCGWINSZ, &ws);
		term_width = ws.ws_col;
	}

	layout = calculate_layout(files, term_width);
	print_columns(files, layout);
}