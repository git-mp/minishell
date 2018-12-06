/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocompl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpetruno <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/04 19:30:36 by mpetruno          #+#    #+#             */
/*   Updated: 2018/12/06 16:16:02 by mpetruno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		add_file(char *file, t_list **lst)
{
	t_list	*new;
	char	*data;

	new = malloc(sizeof(t_list));
	if (!new)
		return ;
	data = ft_strdup(file);
	if (!data)
	{
		free((void *)new);
		return ;
	}
	new->content = (void *)data;
	ft_lstins(lst, new, sort);
}

char			*convert_pattern(t_inp_buff *buff)
{
	long	*start;
	char	*patt;
	int		i;

	if (buff->data[buff->len - 1] == (long)' ' || buff->len == 0)
		return (0);
	i = buff->len - 1;
	while (buff->data[i] != (long)' ' && i > 0)
		i--;
	start = buff->data + i + 1;
	patt = utf_to_str(start, buff->len - i);
	return (patt);
}

static t_list	*get_files(t_inp_buff *buff, DIR *dstr)
{
	struct dirent	*dirp;
	char			dir[4100];
	char			*patt;
	t_list			*lst;
	int				i;

	getcwd(dir, 4100);
	patt = convert_pattern(buff);
	if ((dstr = opendir(dir)) == 0)
		return (0);
	lst = 0;
	while ((dirp = readdir(dstr)) != 0)
		if (patt == 0)
			add_file(dirp->d_name, &lst);
		else
		{
			i = 0;
			while (patt[i] && dirp->d_name[i] == patt[i])
				i++;
			if (i > 0 && dirp->d_name[i] && patt[i] == '\0')
				add_file(dirp->d_name + i, &lst);
		}
	closedir(dstr);
	free((void *)patt);
	return (lst);
}

static void		fill(t_inp_buff **buff, t_list *head)
{
	t_list	*lst;
	int		i;
	int		add;
	char	*tmp;
	char	sym;

	i = 0;
	add = 1;
	while (head && add)
	{
		lst = head;
		add = 1;
		sym = *(char *)((lst->content) + i);
		while (lst && add)
		{
			tmp = (char *)(lst->content);
			if (!tmp[i] || tmp[i] != sym)
				add = 0;
			lst = lst->next;
		}
		if (add)
			inp_insert(buff, (int)sym);
		i++;
	}
	return ;
}

void			auto_complete(t_inp_buff **buff)
{
	t_list	*match;
	t_list	*head;
	char	*str;
	DIR		*dstr;

	dstr = 0;
	match = get_files(*buff, dstr);
	head = match;
	while (match && ft_lstsize(head) > 1)
	{
		techo("\n");
		techo((char *)(match->content));
		match = match->next;
	}
	if (ft_lstsize(head) > 1)
	{
		techo("\n");
		show_prompt();
		if (*(str = utf_to_str((*buff)->data, (*buff)->len)))
			techo(str);
		free((void *)str);
	}
	fill(buff, head);
	ft_lstfree(&head);
}
