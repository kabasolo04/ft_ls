#include "libft.h"

char	*ft_ultoa(unsigned long n)
{
	char			*str;
	unsigned long	tmp;
	size_t			len;

	tmp = n;
	len = 1;
	while (tmp >= 10)
	{
		tmp /= 10;
		len++;
	}

	str = malloc(sizeof(char) * (len + 1));

	if (!str)
		return (NULL);

	str[len] = '\0';
	while (len > 0)
	{
		str[--len] = '0' + (n % 10);
		n /= 10;
	}

	return (str);
}
