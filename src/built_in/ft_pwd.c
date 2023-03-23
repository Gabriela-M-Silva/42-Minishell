/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsmereka <gsmereka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 17:02:59 by gsmereka          #+#    #+#             */
/*   Updated: 2023/03/23 00:55:01 by gsmereka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static char	*get_pwd(int buffer_size);
static int	buffer_size_overflow(int buffer_size);

void	ft_pwd(char **args, t_data *data)
{
	char	*pwd;
	int		buffer_size;

	(void)args;
	buffer_size = 1024;
	pwd = get_pwd(buffer_size);
	if (!pwd)
	{
		data->error_msg = "path too long\n";
		write(2, data->error_msg, ft_strlen(data->error_msg));
		att_exit_status(1, data);
	}
	ft_printf("%s\n", pwd);
	free(pwd);
}

static char	*get_pwd(int buffer_size)
{
	char	*pwd;

	pwd = getcwd(NULL, buffer_size);
	if (!pwd)
	{
		if (!buffer_size_overflow(buffer_size))
			pwd = get_pwd(buffer_size * 2);
		else
			return (NULL);
	}
	return (pwd);
}

static int	buffer_size_overflow(int buffer_size)
{
	if (buffer_size > 0)
		return (0);
	return (1);
}
