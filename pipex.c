/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apashkov <apashkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 11:49:59 by apashkov          #+#    #+#             */
/*   Updated: 2023/11/23 17:56:51 by apashkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_process(int file1)
{
	if (dup2(file1, 0) < 0)
		return (perror("Dup2: "));
	if (dup2(end[1], 1) < 0)
		return (perror("Dup2: "));
	close(end[0]);
	close (file1);
	exit(1);
}

void	parent_process(int file2)
{
	int	status;

	if (dup2(end[0], 0) < 0)
		return (perror("Dup2: "));
	if (dup2(file2, 1) < 0)
		return (perror("Dup2: "));
	close(end[1]);
	close(file2);
	exit(1);
}

void	pipex(int file1, int file2)
{
	int		end[2];
	pid_t	child1;
	pid_t	child2;
	int		wstatus;

	if (pipe(end) == -1)
		return (perror("Pipe: "));
	child = fork();
	if (child < 0)
		return (perror("Fork: "));
	if (child == 0)
		child_process(file1);
	else
	{
		waitpid(child, &status, 0);
		parent_process(file2);
	}
}

/* void print_split(char **argv) 
{
	int i = 0;
	while (argv[i])
	{
		printf("[%s]\n", argv[i]);
		i++;
	}
	printf("[%s]\n", argv[i]);
	free_array(argv);
} */

int	main(int argc, char** argv, char **envp)
{
	int	infile;
	int outfile;
	char *the_path;
	char **poss_paths;
	int	i;

	argv = NULL;
	argc = 1;
	i = 0;
	the_path = NULL;
	while (the_path == NULL)
		the_path = ft_strstr(envp[i++], "PATH=/");
	printf("%s\n", the_path);
	poss_paths = ft_split(the_path, ':');
	if (!poss_paths)
		return(perror("Split: "), 1);
	infile = open(argv[1], O_RDONLY);
	outfile = open(argv[4], O_RDWR);
	if (infile < 0 || outfile < 0)
		return(perror("Open: "), 1);
	pipex(file1, file2);
}