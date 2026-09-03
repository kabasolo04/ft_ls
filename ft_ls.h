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
#include <sys/sysmacros.h>

//______________________//
//________COLORS________//
#define RESET		"\033[0m"
#define BLUE		"\033[34m"
#define CYAN		"\033[36m"
#define GREEN		"\033[32m"
#define MAGENTA		"\033[35m"
#define YELLOW		"\033[33m"
#define RED			"\033[31m"

#define RED_BG		"\033[41m"
#define WHITE		"\033[97m"

//_____________________________//
//________FLAG BIT-MASK________//
#define FLAG_l		(1 << 0)
#define FLAG_g		(1 << 1)
#define FLAG_G		(1 << 2)

#define FLAG_a		(1 << 3)
#define FLAG_R		(1 << 4)
#define FLAG_r		(1 << 5)

#define FLAG_t		(1 << 6)
#define FLAG_S		(1 << 7)
#define FLAG_U		(1 << 8)

#define MULTI		(1 << 9)

//_______________________________//
//________PRINT BIT-MASK_________//
#define F_PERMISSIONS	(1 << 0)
#define F_LINKS			(1 << 1)
#define F_OWNER			(1 << 2)
#define F_GROUP			(1 << 3)
#define F_SIZE			(1 << 4)
#define F_DATE			(1 << 5)
#define F_NAME			(1 << 6)

//________________________________//
//________BIT-MASK HELPERS________//
#define ALL_TRUE		(~0u)
#define MULTI_TARGET	(MULTI  | FLAG_R)
#define LONG_PRINT		(FLAG_l | FLAG_g | FLAG_G)
#define SORT_FLAGS		(FLAG_t | FLAG_S | FLAG_U)

//_________________________________//
//________BIT-MASK FUNTIONS________//
#define HAS_BIT(bit_mask, bit)		((bit_mask & bit) != 0)
#define ADD_BIT(bit_mask, bit)		(bit_mask |= bit)
#define RM_BIT(bit_mask, bit)		(bit_mask &= ~bit)

extern int	g_flags;
extern char	g_exit;

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

//_______________________//
//________PARSING________//
char	flag_error(char* input);
char	target_error(char* input);

//_____________________//
//________PRINT________//
void	normal_print(t_files files);
void	long_print(t_files files);

//___________________________//
//________PRINT_UTILS________//
size_t	append_permissions(char *buf, t_file file);
size_t	append_links(char *dst, t_file file);
size_t	append_owner(char *dst, t_file file);
size_t	append_group(char *dst, t_file file);
size_t	append_size(char *dst, t_file file);
size_t	append_date(char *dst, t_file file);
void	print_name(t_file file);

//__________________________//
//________SORT_FILES________//
void	merge_sort(t_files* f, int l, int r);
t_files	get_files(DIR* dir, char* path);
void	free_files(t_files* f);

//__________________________//
//________SORT_UTILS________//
typedef	char (*t_cmp)(t_file* , t_file* );
t_cmp	get_comparator(void);
char*	join_path(const char* dir, const char* name);

//_____________________//
//________CACHE________//
char*	get_user_cached(uid_t uid);
char*	get_group_cached(gid_t gid);
void	free_cache();

#endif //FT_LS.H
