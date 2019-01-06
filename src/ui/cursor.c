/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpetruno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 19:58:48 by mpetruno          #+#    #+#             */
/*   Updated: 2018/12/21 16:59:59 by mpetruno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	g_shell;

void	cur_mv_up(void)
{
	if (g_shell.positions.current.row > 0)
	{
		g_shell.positions.current.row--;
		tconf("up");
	}
}

void	cur_mv_down(void)
{
	g_shell.positions.current.row++;
	tconf("do");
}

void	cur_mv_left(void)
{
	if (g_shell.positions.current.col == 0 && g_shell.positions.current.row > 0)
	{
		g_shell.positions.current.col = g_shell.winsize.ws_col - 1;
		g_shell.positions.current.row--;
	}
	else
		g_shell.positions.current.col--;
	move_cursor(g_shell.positions.current.col, g_shell.positions.current.row);
}

void	cur_mv_right(void)
{
	if (g_shell.positions.current.col >= g_shell.winsize.ws_col - 1)
	{
		g_shell.positions.current.col = 0;
		if (g_shell.positions.current.row >= g_shell.winsize.ws_row - 1)
		{
			tconf("sf");
			g_shell.positions.prompt.row--;
			g_shell.positions.cmd.row--;
		}
		else
			g_shell.positions.current.row++;
	}
	else
		g_shell.positions.current.col++;
	move_cursor(g_shell.positions.current.col, g_shell.positions.current.row);
}

int	init_cursor(void)
{
	g_shell.positions.prompt.col = 0;
	g_shell.positions.prompt.row = 0;
	g_shell.positions.cmd.col = 0;
	g_shell.positions.cmd.row = 0;
	g_shell.positions.current.col = 0;
	g_shell.positions.current.row = 0;
	return (0);
}