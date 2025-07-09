/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:35:00 by atursun           #+#    #+#             */
/*   Updated: 2025/07/09 13:40:00 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"
#include <stdio.h>

void	read_xpm_after_map(t_cub *cub, char *tmp, int fd)
{
	int	i;

	if (cub->is_player == 0)
		error_msg("Player is missing", cub, 3);
	while (tmp != NULL)
	{
		i = 0;
		while (tmp[i])
		{
			if (tmp[i] != '\0' && tmp[i] != ' '
				&& tmp[i] != '\t' && tmp[i] != '\n')
				{
					free(tmp);
					error_msg("Wrong map design", cub, 3);
				}
			i++;
		}
		free(tmp);
		tmp = get_next_line(fd);
	}
	close(fd);
}

char	*read_xpm_until_map(t_cub *cub, int fd)
{
	char	*tmp;
	int		map_start;

	map_start = 0;
	tmp = get_next_line(fd);
	while (tmp)
	{
		if (ft_strchr(tmp, '1')
			&& !ft_strchr(tmp, 'F') && !ft_strchr(tmp, 'C')
			&& !ft_strnstr(tmp, "SO", ft_strlen(tmp))
			&& !ft_strnstr(tmp, "WE", ft_strlen(tmp))
			&& !ft_strnstr(tmp, "NO", ft_strlen(tmp))
			&& !ft_strnstr(tmp, "EA", ft_strlen(tmp)))
			break ;
		map_start++;
		free(tmp);
		tmp = get_next_line(fd);
	}
	if (!tmp)
		error_msg("No map", cub, 2);
	if (ft_strchr(tmp, 'W') || ft_strchr(tmp, 'E')
		|| ft_strchr(tmp, 'N') || ft_strchr(tmp, 'S'))
		{
			free(tmp);
			error_msg("Player on the wall", cub, 2);
		}
	return (tmp);
}

void	read_map(t_cub *cub, char *file)
{
	char	*tmp;
	int		fd;
	int		i;
	int		map_lengt;

	i = 0;
	fd = open(file, O_RDONLY);
	tmp = read_xpm_until_map(cub, fd);
	map_lengt = map_reel_lenght(file, cub);
	cub->map.map = ft_calloc(map_lengt + 1, sizeof(char *));
	while (tmp != NULL && tmp[0] != '\0')
	{
		if (!ft_strchr(tmp, '1'))
			break ;
		validate_map_line(tmp, cub);
		set_coor_and_pos(cub, tmp, i);
		cub->map.map[i] = ft_strdup(tmp);
		free(tmp);
		tmp = NULL;
		tmp = get_next_line(fd);
		i++;
	}
	cub->map.map[i] = NULL;
	read_xpm_after_map(cub, tmp, fd);
}

void	read_file(t_cub *cub, char *file)
{
	int	fd;
	int	fd2;

	fd = open(file, O_RDONLY);
	read_texture(cub, fd, 0, 0);
	fd2 = open(file, O_RDONLY);
	read_fc_rgb(cub, fd2);
	close(fd2);
	read_map(cub, file);
}

void	open_file(t_cub *cub, char *file)
{
	int		counter;
	int		fd;
	char	*line;

	counter = 0;
	fd = open(file, O_RDONLY);
	if (fd == -1)
		error_msg("can not open file", cub, 2);
	line = get_next_line(fd);
	if (!line)
		error_msg("File is empty", cub, 2);
	while (line != NULL)
	{
		counter++;
		free(line);
		line = get_next_line(fd);
	}
	cub->len_of_file = counter;
	close(fd);
	read_file(cub, file);
	check_texture(cub, 0);
	check_map(cub);
}
