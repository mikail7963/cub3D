/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atursun <atursun@student.42istanbul.com.tr +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:36:29 by atursun           #+#    #+#             */
/*   Updated: 2025/07/03 16:48:51 by atursun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"



void	read_texture(t_cub *cub, int fd)
{
	char	*line;
	int		i;

	i = 0;
	line = get_next_line(fd);
	int j = 0;
	while (line != NULL)
	{
		while (line[j] == ' ' || line[j] == '\t')
			j++;
		line[ft_strlen(line) - 1] = '\0';
		if (line[j] == 'N' && line[j + 1] == 'O' && ++i)
			cub->texture.north = ft_strtrim(line, "NO ");
		else if (line[j] == 'S' && line[j + 1] == 'O' && ++i)
			cub->texture.south = ft_strtrim(line, "SO ");
		else if (line[j] == 'W' && line[j + 1] == 'E' && ++i)
			cub->texture.west = ft_strtrim(line, "WE ");
		else if (line[j] == 'E' && line[j + 1] == 'A' && ++i)
			cub->texture.east = ft_strtrim(line, "EA ");
		free(line);
		line = get_next_line(fd);
		cub->map_index++;
		j = 0;
	}
	if (i > 4)
		error_msg("Some of the texture packs is too", cub, 2);
	if (!(cub->texture.north) || !(cub->texture.south)
		|| !(cub->texture.west) || !(cub->texture.east))
		error_msg("Some of the texture packs is missing", cub, 2);
	close(fd);
}

void	check_is_there_texture_file(t_cub *cub)
{
	int	fd1;
	int	fd2;
	int	fd3;
	int	fd4;

	fd1 = open(cub->texture.east, O_RDONLY);
	fd2 = open(cub->texture.north, O_RDONLY);
	fd3 = open(cub->texture.south, O_RDONLY);
	fd4 = open(cub->texture.west, O_RDONLY);
	if (fd1 == -1 || fd2 == -1 || fd3 == -1 || fd4 == -1)
	{
		ft_putendl_fd("wrong texture path", 1);
		exit(EXIT_FAILURE);
	}
	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);
}

int	check_texture(t_cub *cub)
{
	if (check_extension(cub->texture.east, ".xpm"))
		error_msg("wall file extensions are not valid", cub, 3);
	if (check_extension(cub->texture.north, ".xpm"))
		error_msg("wall file extensions are not valid", cub, 3);
	if (check_extension(cub->texture.west, ".xpm"))
		error_msg("wall file extensions are not valid", cub, 3);
	if (check_extension(cub->texture.south, ".xpm"))
		error_msg("wall file extensions are not valid", cub, 3);
	check_is_there_texture_file(cub);
	return (0);
}
