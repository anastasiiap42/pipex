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

void	child_process(int file1, char **paths, char *argv)
{
	char	**args;

	if (dup2(file1, 0) < 0)
		return (perror("Dup2: "), free_array(paths));
	if (dup2(end[1], 1) < 0)
		return (perror("Dup2: "), free_array(paths));
	while (access(*paths, X_OK) == -1)
		*paths++;
	args = ft_split(argv, ' ');
	if (execve(*paths, args, envp) == -1)
		return (perror("Execve: "), free_array(paths), free_array(args));
	close(end[0]);
	close (file1);
	exit(1);
}

void	parent_process(int file2, char **paths, char *argv)
{
	char	**args;

	if (dup2(end[0], 0) < 0)
		return (perror("Dup2: "), free_array(paths));
	if (dup2(file2, 1) < 0)
		return (perror("Dup2: "), free_array(paths));
	while (access(*paths, X_OK) == -1)
		*paths++;
	args = ft_split(argv, ' ');
	if (execve(*paths, args, envp) == -1)
		return (perror("Execve: "), free_array(paths), free_array(args));
	close(end[1]);
	close(file2);
	exit(1);
}

void	pipex(int file1, int file2, char **paths, char **argv)
{
	int		end[2];
	pid_t	child;
	int		wstatus;

	if (pipe(end) == -1)
		return (perror("Pipe: "), free_array(paths));
	child = fork();
	if (child < 0)
		return (perror("Fork: "), free_array(paths));
	if (child == 0)
		child_process(file1, paths, argv[2]);
	else
	{
		waitpid(child, &wstatus, 0);
		parent_process(file2, paths, argv[3]);
	}
	close(end[0]);
	close(end[1]);
	
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
	if (argc != 5)
		perror("error");
	while (the_path == NULL)
		the_path = ft_strstr(envp[i++], "PATH=/");
	printf("%s\n", the_path);
	poss_paths = ft_split(the_path, ':');
	if (!poss_paths)
		return (perror("Split: "), free_array(poss_paths), 1);
	infile = open(argv[1], O_RDONLY);
	outfile = open(argv[4], O_RDWR);
	if (infile < 0 || outfile < 0)
		return (perror("Open: "), 1);
	pipex(file1, file2, poss_paths, argv);
}