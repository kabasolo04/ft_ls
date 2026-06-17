#include "ft_ls.h"

static void	files_init(t_files *f)
{
	f->data = NULL;
	f->size = 0;
	f->capacity = 0;
	f->file_size = 0;
}

void	free_files(t_files *f)
{
	for (size_t i = 0; i < f->size; i++)
	{
		free(f->data[i].name);
		free(f->data[i].path);
	}

	free(f->data);
}

static int	files_push(t_files *f, t_file file)
{
	t_file*	new_data;

	if (f->size == f->capacity)
	{
		size_t new_cap = (f->capacity == 0) ? 8 : f->capacity * 2;

		new_data = malloc(sizeof(t_file) * new_cap);
		if (!new_data)
			return (0);

		for (size_t i = 0; i < f->size; i++)
			new_data[i] = f->data[i];

		free(f->data);
		f->data = new_data;
		f->capacity = new_cap;
	}

	f->data[f->size++] = file;
	return (1);
}

t_files	getFiles(DIR *dir, char *path)
{
	struct dirent*	entry;
	t_files			files;
	t_file			file;

	files_init(&files);

	while ((entry = readdir(dir)))
	{
		if (entry->d_name[0] == '.' && !HAS_FLAG(g_flags, FLAG_a))	continue;
	
		file.name = ft_strdup(entry->d_name);
		if (!file.name) { continue;}
		file.path = join_path(path, entry->d_name);
		if (!file.path) { free(file.name); continue ;}

		if (HAS_FLAG(g_flags, FLAG_l) || HAS_FLAG(g_flags, FLAG_R) || HAS_FLAG(g_flags, FLAG_t))
		{
			lstat(file.path, &file.st);
		}
		else
		{
			stat(file.path, &file.st);
//			memset(&file.st, 0, sizeof(file.st));
		}

		files.file_size += file.st.st_blocks;

		files_push(&files, file);
	}

	return files;
}

static void	merge(t_files *f, t_file *temp, int l, int m, int r, t_cmp cmp)
{
	int n1 = m - l + 1;
	int n2 = r - m;

	for (int i = 0; i < n1; i++)
		temp[l + i] = f->data[l + i];

	for (int j = 0; j < n2; j++)
		temp[m + 1 + j] = f->data[m + 1 + j];

	int i = 0, j = 0, k = l;

	while (i < n1 && j < n2)
	{
		if (cmp(&temp[l + i], &temp[m + 1 + j]) ^ HAS_FLAG(g_flags, FLAG_r)) // 0 0 -> 0 // 1 0 -> 1 // 0 1 -> 1 // 1 1 -> 0
		{
			f->data[k++] = temp[l + i++];
		}
		else
		{
			f->data[k++] = temp[m + 1 + j++];
		}
	}

	while (i < n1)
		f->data[k++] = temp[l + i++];

	while (j < n2)
		f->data[k++] = temp[m + 1 + j++];
}

static void	merge_sort_helper(t_files *f, t_file *temp, int l, int r, t_cmp cmp)
{
	if (l >= r)
		return;

	int m = (l + r) / 2;

	merge_sort_helper(f, temp, l, m, cmp);
	merge_sort_helper(f, temp, m + 1, r, cmp);
	merge(f, temp, l, m, r, cmp);
}

void	merge_sort(t_files *f, int l, int r, t_cmp cmp)
{
	if (l >= r || f->size == 0)
		return;

	t_file *temp = malloc(sizeof(t_file) * f->size);
	if (!temp)
		return;

	merge_sort_helper(f, temp, l, r, cmp);
	free(temp);
}
