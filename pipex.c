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

void print_split(char **argv) 
{
	int i = 0;
	while (argv[i])
	{
		printf("[%s]\n", argv[i]);
		i++;
	}
	printf("[%s]\n", argv[i]);
	free_arr(argv);
}

void	*child_p(int file1, char **paths, int end[], char **envp, char **argv)
{
	int		i;
	char	*the_path;
	char	**args;

	i = 0;
	args = ft_split(argv[2], ' ');
	if (!args)
		return (perror("Split failed:"), free_arr(paths));
	if (dup2(file1, 0) < 0 || dup2(1, end[1]) < 0)
		return (perror("Dup2: "), free_arr(args));
	while (access(paths[i], F_OK | X_OK) == -1)
		i++;
	the_path = ft_strjoin(paths[i], args[0]);
	if (!the_path)
		return (perror("Strjoin failed"), free_arr(paths));
	if (execve(the_path, args, envp) == -1)
		return (perror("Execve"), free_arr(args));
	close(end[0]);
	close (file1);
	exit(1);
}


void	*parent_p(int file2, char **paths, int end[], char **envp, char **argv)
{
	int		i;
	char	*the_path;
	char	**args;

	i = 0;
	args = ft_split(argv[3], ' ');
	if (!args)
		return (perror("Split failed:"), free_arr(paths));
	if (dup2(end[0], 0) < 0 || dup2(1, file2) < 0)
		return (perror("Dup2: "), free_arr(args));
	while (access(paths[i], X_OK) == -1)
		i++;
	the_path = ft_strjoin(paths[i], args[0]);
	if (!the_path)
		return (perror("Strjoin failed"), free_arr(paths));
	if (execve(the_path, args, envp) == -1)
		return (perror("Execve: "), free_arr(args));
	close(end[1]);
	close(file2);
	exit (1);
}

void	*pipex(int file1, int file2, char **paths, char **argv, char **envp)
{
	int		end[2];
	pid_t	child;

	if (pipe(end) == -1)
		return (perror("Pipe: "), free_arr(paths));
	child = fork();
	if (child < 0)
		return (perror("Fork: "), free_arr(paths));
	else if (child == 0)
	{
		printf("Child executed\n");
		child_p(file1, paths, end, envp, argv);
	}
	/*else
	{
		waitpid(child, &wstatus, 0);
		parent_p(file2, cmd, end, envp, argv);
	}*/
	close(end[0]);
	close(end[1]);
	close(file2);
	exit(0);
}

int	main(int argc, char **argv, char **envp)
{
	int	infile;
	int outfile;
	char *the_path;
	char **poss_paths;
	int	i;

	i = 0;
	the_path = NULL;
	if (argc != 5)
	{
		perror("Invalid input");
		exit(EXIT_FAILURE);
	}
	while (the_path == NULL)
		the_path = ft_strstr(envp[i++], "PATH=/");
	poss_paths = ft_split(the_path, ':');
	if (!poss_paths)
		return (perror("Split: "), free_arr(poss_paths), 1);
	infile = open(argv[1], O_RDONLY);
	outfile = open(argv[4], O_RDWR);
	if (infile < 0 || outfile < 0)
		return (perror("Open failed: "), 1);
	pipex(infile, outfile, poss_paths, argv, envp);
}