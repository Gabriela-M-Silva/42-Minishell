/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsmereka <gsmereka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 17:01:55 by gsmereka          #+#    #+#             */
/*   Updated: 2023/02/28 23:30:26 by gsmereka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void	change_dir_envp(t_data *data);
static int	dir_exist(char *dir);
static char	*get_pwd(int buffer_size);
static int	buffer_size_overflow(int buffer_size);

void	ft_cd(char **args, t_data *data)
{
	int		dir_changed;

	if (args[1] && args[2])
	{
		data->error_msg = "bash: cd: too many arguments";
		write(2, data->error_msg, ft_strlen(data->error_msg));
		data->exit_status = 1;
		return ;
	}
	if (!args[1] || (ft_strlen(args[1]) == 1 && args[1][0] == '~'))
	{
		ft_printf("go home\n");
		data->exit_status = 0;
		return ;
	}
	if (!dir_exist(args[1])) // verifica se o diretório existe
		return ;
	dir_changed = chdir(args[1]); // tenta alterar o diretório atual
	if (dir_changed != -1)
	{
		// change_oldpwd
		change_dir_envp(data); // se alterou, altera a variavel de ambiente no dicionario.
		att_virtual_envp(data); // Tambem atualiza a virtual_envp
	}
}

static void	change_dir_envp(t_data *data)
{
	t_env	*pwd;
	int		buffer_size;

	pwd = data->dict_envp;
	while (pwd && ft_strncmp(pwd->key, "PWD", 4) != 0) // busca a variavel de ambiente PWD
	{
		pwd = pwd->next;
	}
	if (pwd)
	{
		if (pwd->value)
			free(pwd->value);
		buffer_size = 1024;
		pwd->value = get_pwd(buffer_size); // atualiza o endereço do diretorio atual na variavel de ambiente.
		if (!pwd->value)
			exit_error(12, "Fail at alloc pwd value at cd", data);
	}
}

static int	dir_exist(char *dir) //falta verificar permissões
{
	DIR	*path;

	path = opendir(dir); // tenta abrir o diretorio
	if (path) // se abrir, o diretório existe e tem acesso.
	{
		closedir(path);
		return (1);
	}
	return (0);
}

static char	*get_pwd(int buffer_size)
{
	char	*pwd;

	pwd = getcwd(NULL, buffer_size);
	if (!pwd)
	{
		if (!buffer_size_overflow(buffer_size))
			pwd = get_pwd(buffer_size * 2);
	}
	return (pwd);
}

static int	buffer_size_overflow(int buffer_size)
{
	if (buffer_size > 0)
		return (0);
	return (1);
}
