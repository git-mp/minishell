/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpetruno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 18:26:31 by mpetruno          #+#    #+#             */
/*   Updated: 2019/01/30 19:10:13 by mpetruno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Tokens:
 * NEWLINE
 * IO_NUMBER (I/O redirection: pipe, >, <, >> or <<)
 * WORD
 * ASSIGN (variable assignment - only if it is first token, or there is preceeding NEWLINE
 * AND
 * OR
 */

/*
 * States: S_GEN - general (inside word)
 *         S_WSP - whitespace (between words) // not needed? close token upon witespace and go further
 *         S_IOR - I/O redirection (after > or <)
 *         S_SQT - single quote
 *         S_DQT - double quote
 *         S_ESC - escape sequense
 *         S_LOG - logical operator? (after | or &) check if it's pipe, &, && or ||
 */

// Add hash # recognition for comments handling

t_state_trans	g_fsm_table[4][12] =
{
	[S_GEN][CH_GEN] = {S_GEN, &tkn_append},
	[S_GEN][CH_NLN] = {S_GEN, &tkn_newline},
	[S_GEN][CH_SCL] = {S_GEN, &tkn_newline}, // semicolon
	[S_GEN][CH_ESC] = {S_GEN, &tkn_escape},
	[S_GEN][CH_SQT] = {S_SQT, &tkn_append},
	[S_GEN][CH_DQT] = {S_DQT, &tkn_append},
	[S_GEN][CH_EXP] = {S_GEN, &tkn_expans},
	[S_GEN][CH_IOR] = {S_GEN, &tkn_ionumb},
	[S_GEN][CH_LOG] = {S_GEN, &tkn_logic},
	[S_GEN][CH_EQU] = {S_GEN, &tkn_assign},
	[S_GEN][CH_HSH] = {S_HSH, NULL},
	[S_GEN][CH_WSP] = {S_GEN, &tkn_complete},

	[S_SQT][CH_GEN] = {S_SQT, &tkn_append},
	[S_SQT][CH_NLN] = {S_SQT, &tkn_append},
	[S_SQT][CH_ESC] = {S_SQT, &tkn_append},
	[S_SQT][CH_SQT] = {S_GEN, &tkn_append},
	[S_SQT][CH_DQT] = {S_SQT, &tkn_append},
	[S_SQT][CH_EXP] = {S_SQT, &tkn_append},
	[S_SQT][CH_IOR] = {S_SQT, &tkn_append},
	[S_SQT][CH_LOG] = {S_SQT, &tkn_append},
	[S_SQT][CH_EQU] = {S_SQT, &tkn_append},
	[S_SQT][CH_HSH] = {S_SQT, &tkn_append},
	[S_SQT][CH_WSP] = {S_SQT, &tkn_append},

	[S_DQT][CH_GEN] = {S_DQT, &tkn_append},
	[S_DQT][CH_NLN] = {S_DQT, &tkn_append},
	[S_DQT][CH_ESC] = {S_DQT, &tkn_escape},
	[S_DQT][CH_SQT] = {S_DQT, &tkn_append},
	[S_DQT][CH_DQT] = {S_GEN, &tkn_append},
	[S_DQT][CH_EXP] = {S_DQT, &tkn_expans},
	[S_DQT][CH_IOR] = {S_DQT, &tkn_append},
	[S_DQT][CH_LOG] = {S_DQT, &tkn_append},
	[S_DQT][CH_EQU] = {S_DQT, &tkn_append},
	[S_DQT][CH_HSH] = {S_DQT, &tkn_append},
	[S_DQT][CH_WSP] = {S_DQT, &tkn_append},

	[S_HSH][CH_GEN] = {S_HSH, NULL},
	[S_HSH][CH_NLN] = {S_GEN, NULL},
	[S_HSH][CH_SCL] = {S_HSH, NULL},
	[S_HSH][CH_ESC] = {S_HSH, NULL},
	[S_HSH][CH_SQT] = {S_HSH, NULL},
	[S_HSH][CH_DQT] = {S_HSH, NULL},
	[S_HSH][CH_EXP] = {S_HSH, NULL},
	[S_HSH][CH_IOR] = {S_HSH, NULL},
	[S_HSH][CH_LOG] = {S_HSH, NULL},
	[S_HSH][CH_EQU] = {S_HSH, NULL},
	[S_HSH][CH_HSH] = {S_HSH, NULL},
	[S_HSH][CH_WSP] = {S_HSH, NULL}
};

enum e_signal	get_signal(char c)
{
	if (c == '\\')
		return (CH_ESC);
	else if (c == '\'')
		return (CH_SQT);
	else if (c == '\"')
		return (CH_DQT);
	else if (c == '$')
		return (CH_EXP);
	else if (c == '<' || c == '>')
		return (CH_IOR);
	else if (c == '|' || c == '&')
		return (CH_LOG);
	else if (c == ' ' || c == '\t')
		return (CH_WSP);
	else if (c == '\n')
		return (CH_NLN);
	else if (c == ';')
		return (CH_SCL);
	else if (c == '#')
		return (CH_HSH);
	else if (c == '=')
		return (CH_EQU);
	else 
		return (CH_GEN);
}

static void		connect_tokens(t_token *prev, t_token *new)
{
	if (prev != NULL)
	{
		prev->next = new;
		tkn_complete(&prev, 0);
		new->prev = prev;
	}
	else
		new->prev = NULL;
}

/*
** Initiates new empty token with 'size' bytes allocated for data.
*/

t_token			*init_token(int size, t_token *prev)
{
	t_token	*tkn;

	if ((tkn = malloc(sizeof(t_token))) == NULL)
		return (NULL);
	connect_tokens(prev, tkn);
	if (size > 0)
	{
		if ((tkn->data = malloc(size + 1)) == NULL)
		{
			free((void *)tkn);
			report_error(ERR_MALLOC);
			return (NULL);
		}
		*(tkn->data) = '\0';
	}
	else
		tkn->data = NULL;
	tkn->pos = 0;
	tkn->complete = 0;
	tkn->type = -1;
	tkn->next = NULL;
	return (tkn);
}

static int		iterate(char *input, t_token **lst, enum e_state *st)
{
	t_token			*head;
	t_token			*token;
	enum e_signal	sig;
	t_lex_func		do_action;

	head = 0;
	token = *lst;
	while (*input)
	{
		sig = get_signal(*input);
		if ((do_action = g_fsm_table[*st][sig].func) != 0)
			if (do_action(&token, &input) < 0)
			{
				tknlst_free(head);
				ft_dprintf(2, "%s: error while parsing input\n",
														SHELL_NAME);
				return (-1);
			}
		head = (!head && token) ? token : head;
		*st = g_fsm_table[*st][sig].state;
		input++;
	}
	*lst = head;
	if (token && token->type == -1)
		token->type = T_WORD;
	return (0);
}

static char *get_type(enum e_tkn_type type)
{
	switch (type)
	{
		case T_WORD: 	return "WORD"; break;
		case T_IO_NUM: 	return " I/O"; break;
		case T_ASSIGN: 	return "ASSI"; break;
		case T_AND: 	return " AND"; break;
		case T_OR:		return "  OR"; break;
		case T_AMP:		return " AMP"; break;
		case T_PIPE:	return "PIPE"; break;
		case T_NEWLINE: return " N/L"; break;
		case T_SEMI:	return "SEMI"; break;
		default:		return "!N/A";
	}
}

static void	debug_tknlist(t_token *lst)
{
	while (lst)
	{
		ft_printf("%s: %s\n", get_type(lst->type), lst->data);
		lst = lst->next;
	}
}


t_token			*tokenize(char *input)
{
	t_token			*token;
	enum e_state	st;

	if (!input)
		return (0);
ft_printf("DEBUG: start tokenizing ----------------------------\n");
	token = 0;
	st = S_GEN;
	if (iterate(input, &token, &st) == -1)
	{
		ft_printf("debug: NULL returned.\n"); // for debug
		exit(0);                              // for debug
		return (0);
	}
	if (st == S_SQT || st == S_DQT)
	{
		ft_dprintf(2, "%s: parsing error - unmatched quotes found\n", SHELL_NAME);
		tknlst_free(token);
		return (0);
	}
	debug_tknlist(token); // for debug
	//exit(0);              // for debug
	ft_printf("========== END LEXER ===============\n");
	return (token);
}

void			tknlst_free(t_token *lst)
{
	t_token	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free((void *)(lst->data));
		free((void *)lst);
		lst = tmp;
	}
}
