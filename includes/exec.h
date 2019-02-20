/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpetruno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 18:25:59 by mpetruno          #+#    #+#             */
/*   Updated: 2019/02/20 18:59:45 by mpetruno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

#include "parser.h"


//typedef int			(*t_exec_func)(t_ast *node);


/*
** tkn_add.c
*/
int					tkn_add(t_token **tkn, char **inp);

/*
** expansion.c
*/
int					expand_token(t_token *tkn);

/*
** expand_str.c
*/
int					expand_str(t_token **tkn, char **input);

/*
** get_argv.c
*/
char				**get_arg_vector(t_token *lst);

/*
** exec_func.c
*/
int					eval_logic(t_ast *node);
int					exec_wait(t_ast *node);
int					exec_no_wait(t_ast *node);

#endif
