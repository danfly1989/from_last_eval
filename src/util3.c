/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 11:13:28 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/28 11:13:35 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_skip_quote(char *str, int i)
{
	char	quote;

	quote = str[i++];
	while (str[i] && str[i] != quote)
		i++;
	if (str[i])
		i++;
	return (i);
}

int	is_special_char(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == ';' || c == '&');
}

char	*ft_get_var_value(t_va *list, const char *name)
{
	size_t	n;

	n = ft_strlen(name);
	while (list)
	{
		if (list->name && ft_strlen(list->name) == n && ft_strncmp(list->name,
				name, n) == 0)
			return (list->value);
		list = list->next;
	}
	return (NULL);
}

/*Simply replaces a \n character with the NULL terminator
Useful for stopping the appearance of strange filenames
after a final redirect*/
char	*trim_newline(char *s)
{
	size_t	len;

	len = ft_strlen(s);
	if (len > 0 && s[len - 1] == '\n')
		s[len - 1] = '\0';
	return (s);
}

/*Value is one when quote found. Else if compares chars
directly in the condition to see if the same character is
repeated and sets to 0 (outside quote). If not outside quote
by the end, quotes were clearly uneven. Appears as syntax error
because full on shell prompt continuation was just no required*/
int	ft_validate_quotes(const char *str)
{
	int		i;
	char	in_quote;

	i = 0;
	in_quote = 0;
	while (str[i])
	{
		if (!in_quote && (str[i] == '\'' || str[i] == '"'))
		{
			in_quote = str[i];
		}
		else if (in_quote && str[i] == in_quote)
		{
			in_quote = 0;
		}
		i++;
	}
	if (in_quote != 0)
	{
		write(2, "minishell: syntax error: unmatched quote\n", 41);
		return (0);
	}
	return (1);
}
