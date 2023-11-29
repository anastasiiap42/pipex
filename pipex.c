/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apashkov <apashkov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 11:49:59 by apashkov          #+#    #+#             */
/*   Updated: 2023/11/29 18:46:44 by apashkov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	*execute(char **paths, char **args, char **envp)
{
	char	*the_path;
	int		i;

	i = -1;
	while (paths[++i])
	{
		the_path = ft_strjoin(paths[i], args[0]);
		if (!the_path)
			return (free_arr(paths), free_arr(args));
		if (access(the_path, X_OK) == 0)
		{
			free_arr(paths);
			if (execve(the_path, args, envp) == -1)
			{
				perror("Execve failed");
				free_arr(args);
				exit(1);
			}
		}
		free(the_path);
	}
	ft_putstr_fd("command not found\n", 2);
	return (free_arr(paths), free_arr(args));
}

static void	child1_p(char **paths, int end[], char **envp, char **argv)
{
	char	**args;
	int		infile;

	infile = open(argv[1], O_RDONLY, 0777);
	if (infile < 0)
		error_handle(paths, 1, "Open failed", end);
	args = ft_split(argv[2], ' ');
	if (!args || dup2(infile, 0) < 0 || dup2(end[1], 1) < 0)
	{
		if (args)
		{
			free_arr(args);
			error_handle(paths, 1, "Dup2 failed", end);
		}
		else
		{
			free_arr(args);
			error_handle(paths, 1, "Split failed", end);
		}
	}
	close(end[0]);
	close (infile);
	close(end[1]);
	execute(paths, args, envp);
	exit(0);
}

static void	child2_p(char **paths, int end[], char **envp, char **argv)
{
	char	**args;
	int		outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile < 0)
		error_handle(paths, 1, "Open failed", end);
	args = ft_split(argv[3], ' ');
	if (!args || dup2(end[0], 0) < 0 || dup2(outfile, 1) < 0)
	{
		if (args)
		{
			free_arr(args);
			error_handle(paths, 1, "Dup2 failed", end);
		}
		else
		{
			free_arr(args);
			error_handle(paths, 1, "Split failed", end);
		}
	}
	close(end[1]);
	close(outfile);
	close(end[0]);
	execute(paths, args, envp);
	exit(0);
}

static void	*pipex(char **paths, char **argv, char **envp)
{
	int		end[2];
	pid_t	child1;
	int		wstatus;
	pid_t	child2;

	if (pipe(end) == -1)
		error_handle(paths, 1, "Pipe failed", end);
	child1 = fork();
	if (child1 < 0)
		error_handle(paths, 1, "Child1 failed", end);
	else if (child1 == 0)
		child1_p(paths, end, envp, argv);
	child2 = fork();
	if (child2 < 0)
		error_handle(paths, 1, "Child2 failed", end);
	else if (child2 == 0)
		child2_p(paths, end, envp, argv);
	close(end[1]);
	close(end[0]);
	waitpid(child1, NULL, 0);
	waitpid(child2, &wstatus, 0);
	return (free_arr(paths));
}

int	main(int argc, char **argv, char **envp)
{
	char	*the_path;
	char	**poss_paths;
	int		i;

	i = 0;
	the_path = NULL;
	if (argc != 5)
		return (ft_putstr_fd("Invalid input\n", 2), 1);
	while (the_path == NULL)
	{
		if (ft_strncmp("PATH=/", envp[i], 6) == 0)
			the_path = ft_strstr(envp[i], "PATH=/");
		i++;
	}
	poss_paths = ft_split(the_path, ':');
	if (!poss_paths)
		exit(1);
	pipex(poss_paths, argv, envp);
}
