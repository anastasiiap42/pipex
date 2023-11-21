/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apashkov <apashkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 11:49:59 by apashkov          #+#    #+#             */
/*   Updated: 2023/11/21 19:39:04 by apashkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* void	child1_process(int file1, char *cmd1)
{
	if (dup2(file1, STDIN_FILENO) < 0)
		return (perror("Dup2: "));
	dup2(end[1], );
	close(end[0]);
	exit(1);
}

void	parent_process(int file2)
{
	int	status;

	waitpid(-1, &status, 0);
	if (dup2(file2, ) < 0)
		return (perror("Dup2: "));
	dup2(end[0], );
	close(end[1]);
	exit(1);
} */

/* void	pipex(int file1, int file2)
{
	int		end[2];
	pid_t	child1;
	pid_t	child2;
	int		wstatus;

	if (pipe(end) == -1)
		return (perror("Pipe: "));
	child1 = fork();
	if (child1 < 0)
		return (perror("Fork1: "));
	if (child1 == 0)
		//child1_process(file1, char *cmd1);
	child2 = fork();
	if (child2 < 0)
		return (perror("Fork2: "));
	if (child2 == 0)
		//child2_process(file2, char *cmd2);
	waitpid(child1, &status, 0);
	waitpid(child2, &status, 0);
} */

void print_split(char **argv) 
{
	char **result = ft_split(argv[1], argv[2][0]);

	for (int i = 0; result[i]; i++)
	{
		printf("[%s]\n", result[i]);
	}
	free_array(result);
}

int	main(int argc, char** argv, char **envp)
{
	/* int	infile;
	int outfile; */
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
	print_split(poss_paths);
	/* if (argc)
	{
		infile = open(argv[1], O_RDONLY);
		outfile = open(argv[4], O_RDWR);
		if (infile < 0 || outfile < 0)
			return(perror("Open: "), 1);
		pipex(file1, file2);
	} */
}