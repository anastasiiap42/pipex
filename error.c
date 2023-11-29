/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apashkov <apashkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 18:19:11 by apashkov          #+#    #+#             */
/*   Updated: 2023/11/29 18:46:38 by apashkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_handle(char **arr, int e, char *str, int end[])
{
	perror(str);
	free_arr(arr);
	close(end[1]);
	close(end[0]);
	if (e == 1)
		exit(1);
}
