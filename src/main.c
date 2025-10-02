/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 20:22:41 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/13 20:22:49 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*This helper was necessary to make space for norminette
as main was already too large to fit the logic for quote
logic. The heavy work here is done by the validate quotes
function*/
static int	quote_check(char *line)
{
	if (!ft_validate_quotes(line))
	{
		g_last_exit_status = 2;
		free(line);
		return (1);
	}
	return (0);
}

/*The quote logic recommended by Sandy was added toward the end
Space was made by using the comma trick to clean up and return
exit status on the same line*/
int	main(int argc, char *argv[], char *env[])
{
	char	*line;
	t_dat	data;

	data = ft_duplicate_input_args(argc, argv, env);
	ft_set_main_signals();
	while (1)
	{
		line = readline("dandav>");
		if (line == NULL)
		{
			if (ft_is_interactive())
				write(1, "exit\n", 5);
			break ;
		}
		if (quote_check(line))
			continue ;
		if (line && *line && !ft_strisspace(line) && ft_strcmp(line, "<<"))
			add_history(line);
		if (line && *line && !ft_strisspace(line))
			ft_check_var_assign_and_expand_line(&data, line);
		if (line)
			free(line);
		line = NULL;
	}
	return (ft_cleanup_data(&data), (g_last_exit_status));
}
