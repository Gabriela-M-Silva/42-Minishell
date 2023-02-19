/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_repl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsmereka <gsmereka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 23:09:03 by gsmereka          #+#    #+#             */
/*   Updated: 2023/02/19 17:57:58 by gsmereka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	get_user_input(t_data *data);
static int	is_valid(char *user_input);
static void	save_input_on_history(char *user_input);

void	init_repl(t_data *data)
{
	data->prompt = "HopeShell:$ ";
	init_repl_signals_handling(data);
	while (1)
	{
		get_user_input(data);
		if (!is_valid(data->user_input))
			break ;
		save_input_on_history(data->user_input);
		init_expander(data); //se o usuário mandou alguma var de ambiente
		init_lexer(data);
		init_parser(data);
		init_executor(data);
		free(data->user_input);
	}
}

static void	save_input_on_history(char *user_input)
{
	int	i;

	i = 0;
	while (user_input[i] == '\t' || user_input[i] == ' ')
		i++;
	if (user_input[i] != '\0')
		add_history(user_input);
}

static int	is_valid(char *user_input)
{
	if (!user_input)
	{
		ft_printf("exit\n");
		return (0);
	}
	else if (!ft_strncmp("exit", user_input, 4)) //cuidado com o exit. Se tiver um minishell dentro de outro, só deve sair do atual
		return (0);
	return (1);
}

static int	get_user_input(t_data *data)
{
	data->user_input = readline(data->prompt);
}
