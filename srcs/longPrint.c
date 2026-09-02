#include "ft_ls.h"

#define CACHE_SIZE 256

static struct
{
	uid_t uid;
	char *name;
}
g_uid_cache[CACHE_SIZE];

static struct
{
	gid_t gid;
	char *name;
}
g_gid_cache[CACHE_SIZE];

static unsigned g_uid_cache_count = 0;
static unsigned g_gid_cache_count = 0;

void free_cache()
{
	for (unsigned i = 0; i < g_uid_cache_count; i++)
	{
		free(g_uid_cache[i].name);
	}

	for (unsigned i = 0; i < g_gid_cache_count; i++)
	{
		free(g_gid_cache[i].name);
	}
}

static char *get_user_cached(uid_t uid)
{
	for (unsigned i = 0; i < g_uid_cache_count; i++)
	{
		if (g_uid_cache[i].uid == uid)
		{
			return g_uid_cache[i].name;
		}
	}

	struct passwd *pw = getpwuid(uid);

	if (g_uid_cache_count < CACHE_SIZE)
	{
		g_uid_cache[g_uid_cache_count].uid = uid;
		g_uid_cache[g_uid_cache_count].name = pw ? ft_strdup(pw->pw_name) : ft_strdup("nobody");
		g_uid_cache_count++;
	}

	return pw ? pw->pw_name : "nobody";
}

static char *get_group_cached(gid_t gid)
{
	for (unsigned i = 0; i < g_gid_cache_count; i++)
	{
		if (g_gid_cache[i].gid == gid)
		{
			return g_gid_cache[i].name;
		}
	}

	struct group *gr = getgrgid(gid);

	if (g_gid_cache_count < CACHE_SIZE)
	{
		g_gid_cache[g_gid_cache_count].gid = gid;
		g_gid_cache[g_gid_cache_count].name = gr ? ft_strdup(gr->gr_name) : ft_strdup("nogroup");
		g_gid_cache_count++;
	}

	return gr ? gr->gr_name : "nogroup";
}

static void	print_permissions(t_file f)
{
	char	buf[12];

	build_permissions(buf, f.st.st_mode);

	if (HAS_FLAG(g_flags, FLAG_g))
		ft_printf("%s %u %s %d ", buf, f.st.st_nlink, get_group_cached(f.st.st_gid), (long)f.st.st_size);
	else if (HAS_FLAG(g_flags, FLAG_G))
		ft_printf("%s %u %s %d ", buf, f.st.st_nlink, get_user_cached(f.st.st_uid), (long)f.st.st_size);
	else
		ft_printf("%s %u %s %s %d ", buf, f.st.st_nlink, get_user_cached(f.st.st_uid), get_group_cached(f.st.st_gid), (long)f.st.st_size);

	char *date;
	date = ctime(&f.st.st_mtime);
	write(1, date + 4, 12);
	write(1, " ", 1);
}

void	longPrint(t_files files)
{
	if (HAS_FLAG(g_flags, MULTI_TARGET)) write(1, "\n", 1);

	ft_printf("total %u", files.file_size / 2);

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
