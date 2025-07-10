/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:24:08 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/10 12:37:33 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	is_empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

int	is_texture_or_color_line(char *line)
{
	if (ft_strnstr(line, "NO ", 3) || ft_strnstr(line, "SO ", 3)
		|| ft_strnstr(line, "WE ", 3) || ft_strnstr(line, "EA ", 3))
		return (1);
	if (ft_strnstr(line, "F ", 2) || ft_strnstr(line, "C ", 2))
		return (1);
	return (0);
}

int	is_map_line(char *line)
{
	if (ft_strchr(line, '1') && !is_texture_or_color_line(line))
		return (1);
	return (0);
}

int	unknown_line_check(t_cub *cub, char *line)
{
	if (is_empty_line(line))
		return (0);
	if (is_texture_or_color_line(line))
		return (0);
	if (is_map_line(line))
		return (1);
	free(line);
	error_msg("Invalid line", cub, 2);
	return (0);
}
