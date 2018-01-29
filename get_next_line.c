/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrudyk <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/28 18:20:45 by mrudyk            #+#    #+#             */
/*   Updated: 2018/01/09 16:39:29 by mrudyk           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

static	char		**ft_cheсk_fd(t_list **begin_list, const int fd)
{
	t_list			*tmp;

	tmp = *begin_list;
	while (tmp)
	{
		if (tmp->content_size == (size_t)fd)
			return ((char**)&tmp->content);
		tmp = tmp->next;
	}
	ft_lstadd(begin_list, ft_lstnew("", (size_t)fd));
	return ((char**)&(*begin_list)->content);
}

static	int			m_malloc(char **line, int j, char *buff, int len)
{
	int				i;
	char			*new;

	i = 0;
	if (*line)
	{
		i = ft_strlen(*line);
		new = (char *)ft_memalloc(sizeof(char) * (j + i + 1));
		new = ft_strncpy(new, *line, i);
		ft_memdel((void **)line);
		*line = new;
	}
	else
	{
		ft_memdel((void **)line);
		*line = (char *)ft_memalloc(sizeof(char) * (j + 1));
	}
	if (len != -1)
		ft_strncpy((*line) + i, buff, len);
	else
		ft_strcpy((*line) + i, buff);
	return (OK);
}

static	int			arr_str(char **line, char **buff_str)
{
	int				i;
	int				len;

	i = 0;
	if (!(**buff_str))
	{
		ft_memdel((void **)buff_str);
		return (END);
	}
	while ((*buff_str)[i] != '\0' && (*buff_str)[i] != '\n')
		i++;
	m_malloc(line, i, *buff_str, i);
	len = ft_strlen(*buff_str) - i;
	if ((*buff_str)[i] != O && ft_memmove(*buff_str, (*buff_str) + i + 1, len))
	{
		i = -1;
		while ((*buff_str)[(++i) + len])
			(*buff_str)[i + len] = 0;
		return (OK);
	}
	else
	{
		ft_memdel((void **)buff_str);
		return (END);
	}
}

static int			ft_read(int fd, char *buff, char **buff_str, char **line)
{
	int				c;
	int				i;

	while ((c = read(fd, buff, BUFF_SIZE) > 0) && c != -1)
	{
		i = 0;
		while (buff[i] != '\0' && buff[i] != '\n')
			i++;
		m_malloc(line, i, buff, i);
		if (buff[i] == N && m_malloc(buff_str, BUFF_SIZE - i, buff + i + 1, -1))
			break ;
		ft_bzero(buff, BUFF_SIZE + 1);
	}
	if (c < 0)
		return (ERROR);
	if (!*line && c == 0)
		return (END);
	return (OK);
}

int					get_next_line(const int fd, char **line)
{
	static	t_list	*list = NULL;
	char			**buff_str;
	char			buff[BUFF_SIZE + 1];

	if (fd == -1 && !line)
		return (ERROR);
	if ((read(fd, NULL, 0)) == -1)
		return (ERROR);
	*line = NULL;
	ft_bzero(buff, BUFF_SIZE + 1);
	buff_str = ft_cheсk_fd(&list, fd);
	if (!*buff_str)
		return (END);
	if (**buff_str && arr_str(line, buff_str))
		return (OK);
	return (ft_read(fd, buff, buff_str, line));
}
