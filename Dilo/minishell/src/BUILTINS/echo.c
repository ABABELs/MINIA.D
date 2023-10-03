/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dilovancandan <dilovancandan@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 13:56:03 by aabel             #+#    #+#             */
/*   Updated: 2023/10/03 15:00:38 by dilovancand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_mini_sig;

void	ft_echo(t_core *core)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (core->infile > 0)
	{
		close(core->infile);
		core->infile = 0;
	}	
	if (core->tab[1] && ft_strncmp(core->tab[1], "-n", 2) == 0)
	{
		flag = 1;
		i++;
	}
	while (core->tab[++i])
	{
		write(core->outfile, core->tab[i], ft_strlen(core->tab[i]));
		if (core->tab[i + 1] != NULL)
			write(core->outfile, " ", 1);
	}
	if (flag == 0)
		write(core->outfile, "\n", 1);
	core->exit_code = 0;
}
