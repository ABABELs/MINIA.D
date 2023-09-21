/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dilovancandan <dilovancandan@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 10:16:38 by arthurabel        #+#    #+#             */
/*   Updated: 2023/09/21 17:57:12 by dilovancand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_or_not(t_crust *crust)
{
	if ((crust->pipe) >= 0)
		lanch_pipe(crust);
	else
		return ;
	print_lst_parsing(crust->lst_cmd->first);
}

void	wait_all_process(t_crust *crust)
{
	t_list	*list;
	t_core	*core;
	int		exit_code;

	if (!crust->lst_cmd)
		return ;
	list = ft_lstlast(crust->lst_cmd->first);
	core = (t_core *)list->content;
	while (list && !ft_isbuiltins(core))
	{
		exit_code = 0;
		waitpid(core->child, &exit_code, 0);
		ft_printf("1");
		if (core->exit_code == 0)
			core->exit_code = WEXITSTATUS(exit_code);
		list = list->prev;
		if (list)
			core = (t_core *)list->content;
	}
}

void	run_my_child(t_core *cmd, t_crust *crust)
{
	if (dup2(cmd->infile, STDIN_FILENO) == -1
		|| dup2(cmd->outfile, STDOUT_FILENO) == -1)
		exit(1);
	close_fd(cmd);
	path_in_cmd(crust, cmd);
	if (execve(cmd->pathed, cmd->tab, crust->env) == -1)
		return ;
}

void	exec_my_pipe(t_core *core, t_crust *crust)
{
	core->child = fork();
	if (core->child > 0)
		ft_signal_in_fork();
	if (core->child < 0)
	{
		close_all_fd(crust);
		return (perror("fork failed"), (void)1);
	}
	else if (core->child == 0 && !core->error && !core->exit_code)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (dup2(core->infile, STDIN_FILENO) == -1
			|| dup2(core->outfile, STDOUT_FILENO) == -1)
			exit(1);
		close_all_fd(crust);
		path_in_cmd(crust, core);
		if (execve(core->pathed, core->tab, crust->env) == -1)
			exit(127);
	}
	if (core->child == 0)
		exit(1);
}

void	lanch_pipe(t_crust *crust)
{
	t_core	*content;
	t_list	*list;
//	int		exit_code;

	//exit_code = 0;
	list = crust->lst_cmd->first;
	// not_used_pipe(crust);
	remove_pipe(crust);
	while (list)
	{
		content = (t_core *)list->content;
		if (content->type == CMD && content->tab)
		{
			if (ft_isbuiltins(content) == 1)
			{
				if (!content->error)
					exec_my_builtins(content->tab[0], content, crust);
			}
			else if (ft_strncmp("", content->str, ft_strlen(content->str)))
				exec_my_pipe(content, crust);
			close_fd(content);
		}
		list = list->next;
	}
	wait_all_process(crust);
	ft_signal();
}
