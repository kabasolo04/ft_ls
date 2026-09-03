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

void	free_cache()
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

char*	get_user_cached(uid_t uid)
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

char*	get_group_cached(gid_t gid)
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