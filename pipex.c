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

/*void print_split(char **argv) 
{
	int i = 0;
	while (argv[i])
	{
		printf("[%s]\n", argv[i]);
		i++;
	}
	printf("[%s]\n", argv[i]);
	free_arr(argv);
}*/

void	execute(char **paths, char **args, char **envp)
{
	char	*the_path;
	int		i;

	i = -1;
	while (paths[++i])
	{
		the_path = ft_strjoin(paths[i], args[0]);
		if (!the_path)
		{
			free_arr(paths);
			free(the_path);
		}
		if (access(the_path, X_OK) == 0)
		{
			if (execve(the_path, args, envp) == -1)
			{
				perror("Execve failed");
				free_arr(args);
			}
		}
		free(the_path);
	}
}

void	*child_p(char **paths, int end[], char **envp, char **argv)
{
	char	**args;
	int		infile;

	infile = open(argv[1], O_RDONLY);
	if (infile < 0)
	{
		perror("Open failed: ");
		free_arr(paths);
		exit(1);
	}
	args = ft_split(argv[2], ' ');
	if (!args || dup2(infile, 0) < 0 || dup2(end[1], 1) < 0)
	{
		if (args)
			perror("Dup2 failed");
		free_arr(paths);
		free_arr(args);
		exit(1);
	}
	close(end[0]);
	close (infile);
	execute(paths, args, envp);
	exit(0); //do I need it?
}

void	*parent_p(char **paths, int end[], char **envp, char **argv)
{
	char	**args;
	int		outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC);
	if (outfile < 0)
	{
		perror("Open failed: ");
		free_arr(paths);
		exit(1);
	}
	args = ft_split(argv[3], ' ');
	if (!args || dup2(end[0], 0) < 0 || dup2(outfile, 1) < 0)
	{
		if (args)
			perror("Dup2 failed");
		free_arr(paths);
		free_arr(args);
		exit(1);
	}
	close(end[1]);
	close(outfile);
	execute(paths, args, envp);
	exit(0); //do I need it?
}

void	pipex(char **paths, char **argv, char **envp)
{
	int		end[2];
	pid_t	child;
	int		wstatus;

	if (pipe(end) == -1)
	{
		perror("Pipe: ");
		free_arr(paths);
		exit(1);
	}
	child = fork();
	if (child < 0)
	{
		perror("Fork: ");
		free_arr(paths);
		exit(1);
	}
	else if (child == 0)
		child_p(paths, end, envp, argv);
	else
	{
		waitpid(child, &wstatus, 0);
		parent_p(paths, end, envp, argv);
	}
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
	{
		free_arr(poss_paths);
		exit(1);
	}
	pipex(poss_paths, argv, envp);
}

//Special edge cases: /dev/urandom and /dev/stdin?