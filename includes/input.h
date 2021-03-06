/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpetruno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/04 09:44:43 by mpetruno          #+#    #+#             */
/*   Updated: 2019/03/05 19:08:39 by mpetruno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# define K_RIGHT		"\x1b\x5b\x43"
# define K_LEFT			"\x1b\x5b\x44"
# define K_CTRL_D		"\x04"
# define K_TAB			"\x09"
# define K_RETURN		"\x0a"
# define K_BACK_SP		"\x7f"
# define K_DEL			"\x1b\x5b\x33\x7e"

# define K_CTRL_R		"\x12"
# define K_UP			"\x1b\x5b\x41"
# define K_DOWN			"\x1b\x5b\x42"
# define K_SH_RIGHT		"\x1b\x5b\x31\x3b\x32\x43"
# define K_SH_LEFT		"\x1b\x5b\x31\x3b\x32\x44"

# define K_SH_HOME		"\x1b\x5b\x48"
# define K_SH_END		"\x1b\x5b\x46"
# define K_SH_PGUP		"\x1b\x5b\x35\x7e"
# define K_SH_PGDOWN	"\x1b\x5b\x36\x7e"

# define K_CTRL_B		"\x2"
# define K_CTRL_F		"\x6"

# define K_CTRL_U		"\x15"
# define K_CTRL_K		"\x0b"
# define K_CTRL_A		"\x01"
# define K_CTRL_X		"\x18"
# define K_CTRL_P		"\x10"

# define KEY_NUM			23

# define INP_FD				0
# define INP_BUFF_SIZE		8
# define SYM_SIZE			6
# define TAB_SIZE			4

# include <dirent.h>

typedef struct	s_inp_buff
{
	char		**data;
	int			pos;
	int			len;
	int			size;
}				t_inp_buff;

typedef int	(*t_key_act)(t_inp_buff *buff, char *sym);

typedef	struct	s_key
{
	char		*code;
	t_key_act	action;
}				t_key;

/*
** terminal_outp.c
*/
int				tconf(char *s);
int				techo(char *s);
void			move_cursor(int col, int row);
void			clear_from_cursor(t_inp_buff *buff);

int				is_control(char *str);

char			*inp_to_str(char **inp);
int				increase_buff(t_inp_buff *buff);

int				inp_paste(t_inp_buff *buff, char *sym);
int				inp_copy_all(t_inp_buff *buff, char *sym);
int				inp_cut_all(t_inp_buff *buff, char *sym);
int				inp_cut_backward(t_inp_buff *buff, char *sym);
int				inp_cut_forward(t_inp_buff *buff, char *sym);
int				inp_copy_backward(t_inp_buff *buff, char *sym);
int				inp_copy_forward(t_inp_buff *buff, char *sym);

int				inp_insert_clipboard(t_inp_buff *buff);
int				inp_insert(t_inp_buff *buff, char *sym);
int				inp_control(t_inp_buff *buff, char *sym);
int				inp_home(t_inp_buff *buff, char *sym);
int				inp_end(t_inp_buff *buff, char *sym);
int				inp_up(t_inp_buff *buff, char *sym);
int				inp_down(t_inp_buff *buff, char *sym);
int				inp_eof(t_inp_buff *buff, char *sym);

int				inp_movel(t_inp_buff *buff, char *sym);
int				inp_mover(t_inp_buff *buff, char *sym);
int				inp_delete(t_inp_buff *buff, char *sym);
int				inp_backsp(t_inp_buff *buff, char *sym);
int				inp_tab(t_inp_buff *buff, char *sym);
int				inp_isearch(t_inp_buff *buff, char *sym);

int				inp_ignore(t_inp_buff *buff, char *sym);

void			read_symbol(char *buff, int fd);
int				get_input(char **str, int fd);
void			inp_buff_free(t_inp_buff *buff);

#endif
