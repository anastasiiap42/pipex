/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apashkov <apashkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 18:43:34 by apashkov          #+#    #+#             */
/*   Updated: 2023/11/21 19:03:06 by apashkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strstr(char *str, char *to_find)
{
	int	i;
	int	l;

	if (to_find[0] == '\0')
		return (str);
	i = 0;
	l = 0;
	if (str[0] == to_find[0])
	{
		while (str[i])
		{
			if (str[i] == to_find[l])
			{
				if (to_find[l + 1] == '\0')
					return (&str[i]);
				l++;
			}
			else
				l = 0;
			i++;
		}
	}
	return (0);
}

/*#include <stdio.h>
#include <string.h>

int	main()
{
	char	str[] = "Hi there";
	char	to_find[] = "the";

	printf("%s ", ft_strstr(str, to_find));
	printf("%s", strstr(str, to_find));
}*/
