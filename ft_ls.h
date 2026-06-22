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
#include <sys/ioctl.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define RESET	"\033[0m"
#define BLUE	"\033[34m"
#define CYAN	"\033[36m"
#define GREEN	"\033[32m"
#define MAGENTA	"\033[35m"
#define YELLOW	"\033[33m"
#define RED		"\033[31m"

#define RED_BG	"\033[41m"
#define WHITE	"\033[97m"

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
	size_t		name_len;
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

typedef	char (*t_cmp)(t_file *, t_file *);
void	merge_sort(t_files *f, int l, int r, t_cmp cmp);

t_files	getFiles(DIR *dir, char *path);
void	free_files(t_files *f);

char	*join_path(const char *dir, const char *name);
char	cmp_alpha(t_file *a, t_file *b);
char	cmp_time(t_file *a, t_file *b);
void	printName(t_file data);

char	flagError(char* input);
char	targetError(char* input);

void	normalPrint(t_files files);
void	longPrint(t_files files);

#endif //FT_LS.H
