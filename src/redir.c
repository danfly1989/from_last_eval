/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 20:02:33 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/13 20:02:39 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*All functions in this file are formerly part of a much larger
apply_redirect function, which was split further down. All functions here
are helpers for processing redirection tokens, which is in turn a helper
 Each one returns a value (0) on failure
and 1 on success as they are used in if conditional within that
function*/

/*these are essentially two separate paths to the positive return
((we need. if (*last_in_fd !=-1) close(*last_in_fd)));
*last_in_fd = open(filename,O_RDONLY);
  While its still -1 and the first redirect of its type,
	there's nothing to close. Skip it. But assign it a value with open.
	If it is not the first one,
	then do close and assign the new one with open.*/
int	handle_infile(char **tokens, int *i, int *last_in_fd)
{
	char	*filename;

	filename = trim_newline(tokens[*i + 1]);
	if (*last_in_fd != -1)
		close(*last_in_fd);
	*last_in_fd = open(filename, O_RDONLY);
	if (*last_in_fd < 0)
	{
		perror(filename);
		return (0);
	}
	*i += 2;
	return (1);
}

/*The addition of the filename variable was needed here as it was
previous indiscriminately adding the newline character to the final
filename in a chain*/
int	handle_outfile_trunc(char **tokens, int *i, int *last_out_fd)
{
	char	*filename;

	filename = trim_newline(tokens[*i + 1]);
	if (*last_out_fd != -1)
		close(*last_out_fd);
	*last_out_fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (*last_out_fd < 0)
	{
		perror(filename);
		return (0);
	}
	*i += 2;
	return (1);
}

int	handle_outfile_append(char **tokens, int *i, int *last_out_fd)
{
	char	*filename;

	filename = trim_newline(tokens[*i + 1]);
	if (*last_out_fd != -1)
		close(*last_out_fd);
	*last_out_fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (*last_out_fd < 0)
	{
		perror(filename);
		return (0);
	}
	*i += 2;
	return (1);
}

/*It is important for this to return 1 in the case of
no heredoc delimeter so that apply_redirect can
continue on as normal, due to it being the first call
therein. Heredoc remains more linked to the old logic
through the call to the original handle_heredoc
which in turn calls heredoc child. Details in heredoc.c*/
int	handle_heredoc_redirect(char *heredoc_delim)
{
	int	fd;

	if (!heredoc_delim)
		return (1);
	fd = handle_heredoc(heredoc_delim);
	if (fd == -1)
		return (0);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}
