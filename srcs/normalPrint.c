#include "ft_ls.h"

typedef struct s_layout
{
	size_t	cols;
	size_t	rows;
	size_t	*widths;

}	t_layout;

static void	print_padded(size_t len, size_t width)
{
	char	tmp[1024];
	size_t	padding;

	if (width <= len) return;

	padding = width - len;

	for (size_t i = 0; i < padding; i++)
	{
		tmp[i] = ' ';
	}
	
	write(1, tmp, padding);
}

void	print_columns(t_files files, t_layout layout)
{
	size_t	i;

	for (size_t	row = 0; row < layout.rows; row++)
	{
		if (row != 0) write(1, "\n", 1);

		for (size_t	col = 0; col < layout.cols; col++)
		{
			i = row + col * layout.rows;
		
			if (i >= files.size) continue;
		
			printName(files.data[i]);
	
			if (col != layout.cols - 1)
			{
				print_padded(files.data[i].name_len, layout.widths[col] + 2);
			}
		}
	}

	if (layout.widths)
		free(layout.widths);
}

static size_t	get_column_width(t_files files, size_t col, size_t rows)
{
	size_t	start;
	size_t	end;
	size_t	width;

	start	= col * rows;
	end		= start + rows;
	width	= 0;

	if (end > files.size)
		end = files.size;

	while (start < end)
	{
		if (files.data[start].name_len > width)
			width = files.data[start].name_len;

		start ++;
	}

	return width;
}

static size_t	get_total_width(size_t *widths, size_t cols)
{
	size_t	total;
	size_t	i;

	if (cols == 0) return (0);

	total	= 0;
	i		= 0;

	while (i < cols - 1)
	{
		total += widths[i++] + 2;
	}

	total += widths[i];

	return (total);
}

static t_layout	calculate_layout(t_files files, size_t term_width)
{
	t_layout	layout = {0};
	size_t		*widths;
	size_t		cols;
	size_t		rows;

	widths = malloc(sizeof(size_t) * files.size);

	if (!widths)
		return layout;

	cols = files.size;

	while (cols > 0)
	{
		rows = (files.size + cols - 1) / cols;

		for (size_t i = 0; i < cols; i++)
			widths[i] = get_column_width(files, i, rows);

		if (get_total_width(widths, cols) <= term_width)
			break;

		cols--;
	}

	layout.cols = cols;
	layout.rows = (files.size + cols - 1) / cols;
	layout.widths = widths;

	return layout;
}

void	normalPrint(t_files files)
{
	static size_t		term_width = 0;
	t_layout			layout;

	if (files.size == 0) return;

	if (HAS_FLAG(g_flags, MULTI_TARGET)) write(1, "\n", 1);

	if (term_width == 0)
	{
		struct winsize	ws;

		ioctl(1, TIOCGWINSZ, &ws);
		term_width = ws.ws_col;
	}

	layout = calculate_layout(files, term_width);

	print_columns(files, layout);
}
