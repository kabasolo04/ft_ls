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

static void	print_permissions(struct stat st)
{
	char buf[20];
	int idx = 0;

	if (S_ISREG(st.st_mode))		buf[idx++] = '-';
	else if (S_ISDIR(st.st_mode))	buf[idx++] = 'd';
	else if (S_ISLNK(st.st_mode))	buf[idx++] = 'l';

	buf[idx++] = (st.st_mode & S_IRUSR) ? 'r' : '-';
	buf[idx++] = (st.st_mode & S_IWUSR) ? 'w' : '-';
	buf[idx++] = (st.st_mode & S_IXUSR) ? 'x' : '-';
	buf[idx++] = (st.st_mode & S_IRGRP) ? 'r' : '-';
	buf[idx++] = (st.st_mode & S_IWGRP) ? 'w' : '-';
	buf[idx++] = (st.st_mode & S_IXGRP) ? 'x' : '-';
	buf[idx++] = (st.st_mode & S_IROTH) ? 'r' : '-';
	buf[idx++] = (st.st_mode & S_IWOTH) ? 'w' : '-';
	buf[idx++] = (st.st_mode & S_IXOTH) ? 'x' : '-';
	buf[idx++] = '.';
	buf[idx++] = ' ';
	write(1, buf, idx);

	ft_printf("%u %s %s %d ", st.st_nlink, get_user_cached(st.st_uid), get_group_cached(st.st_gid), st.st_size);

	char *date;
	date = ctime(&st.st_mtime);
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

static void	longPrint(t_files f)
{
	write(1, "\n", 1);

	print_permissions(f.data[0].st);
	printName(f.data[0]);

	for (size_t i = 1; i < f.size; i++)
	{
		write(1, "\n", 1);
		print_permissions(f.data[i].st);
		printName(f.data[i]);
	}
}

static void	normalPrint(t_files f)
{
	ft_printf("%s", f.data[0].name);

	for (size_t i = 1; i < f.size; i++)
	{
		ft_printf("  %s", f.data[i].name);
	}
}

void	printFiles(t_files f, char *input)
{
	static char		prefix = 0;

	if (prefix) write(1, "\n", 1);

	if (HAS_FLAG(g_flags, MULTI_TARGET) || HAS_FLAG(g_flags, FLAG_R))
	{
		if (prefix) write(1, "\n", 1);

		ft_printf("%s:\n", input);
	}

	if (HAS_FLAG(g_flags, FLAG_l))
		ft_printf("total %u", f.file_size / 2);

	prefix = 1;
	
	if (f.size == 0) return;

	if (HAS_FLAG(g_flags, FLAG_l))
		longPrint(f);
	else
		normalPrint(f);
}