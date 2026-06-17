#ifndef FT_LS_H
#define FT_LS_H

#include "libft/libft.h"
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define FLAG_l			(1 << 0)  // 00000001
#define FLAG_R			(1 << 1)  // 00000010
#define FLAG_a			(1 << 2)  // 00000100
#define FLAG_r			(1 << 3)  // 00001000
#define FLAG_t			(1 << 4)  // 00010000

#define MULTI_TARGET	(1 << 5)  // 00100000

extern char	g_flags;
extern char	g_exit;

#define HAS_FLAG(flags, flag) ((flags & (flag)) != 0)
#define ADD_FLAG(flags, flag) flags |= flag

typedef struct s_file
{
	char*		name;
	char*		path;
	struct stat	st;
}
t_file;

typedef struct s_files
{
	t_file*	data;
	size_t	size;
	size_t	capacity;
	size_t	file_size;
}
t_files;

typedef char (*t_cmp)(t_file *, t_file *);

t_files	getFiles(DIR *dir, char *path);
void	merge_sort(t_files *f, int l, int r, t_cmp cmp);
void	printFiles(t_files f, char *input);
void	free_files(t_files *f);

char	*join_path(const char *dir, const char *name);
char	cmp_alpha(t_file *a, t_file *b);
char	cmp_time(t_file *a, t_file *b);

void	printFiles(t_files f, char *input);

#endif //FT_LS.H
