/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atursun <atursun@student.42istanbul.com.tr +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:36:29 by atursun           #+#    #+#             */
/*   Updated: 2025/07/01 15:22:28 by atursun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void read_texture(t_cub *cub, int fd)
{
	char	*line;
	int		i;
	
	i = 0;
	line = get_next_line(fd);
	while (line != NULL && i != 4)
	{
		line[ft_strlen(line) - 1] = '\0';
		if (line[0] == 'N' && line[1] == 'O' && ++i)
			cub->texture.north = ft_strtrim(line, "NO ");
		else if (line[0] == 'S' && line[1] == 'O' && ++i)
			cub->texture.south = ft_strtrim(line, "SO ");
		else if (line[0] == 'W' && line[1] == 'E' && ++i)
			cub->texture.west = ft_strtrim(line, "WE ");
		else if (line[0] == 'E' && line[1] == 'A' && ++i)
			cub->texture.east = ft_strtrim(line, "EA ");
		free(line);
		line = get_next_line(fd);
		cub->map_index++;
	}
	if (!(cub->texture.north) || !(cub->texture.south) || !(cub->texture.west) || !(cub->texture.east))
		error_msg("Error: Some of the texture packs is missing");
	close(fd);
}

void	check_is_there_texture_file(t_cub *cub)
{
	int fd1;
	int fd2;
	int fd3;
	int fd4;

	fd1 = open(cub->texture.east, O_RDONLY);
	fd2 = open(cub->texture.north, O_RDONLY);
	fd3 = open(cub->texture.south, O_RDONLY);
	fd4 = open(cub->texture.west, O_RDONLY);

	if (fd1 == -1 || fd2 == -1 || fd3 == -1 || fd4 == -1)
	{
    	ft_putendl_fd("Error: wrong texture path", 1);
    	exit(EXIT_FAILURE);
    }
	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);
}


int check_texture(t_cub *cub)
{
	if (check_extension(cub->texture.east, ".xpm"))
		return (1);
	if (check_extension(cub->texture.north, ".xpm"))
		return (1);
	if (check_extension(cub->texture.west, ".xpm"))
		return (1);
	if (check_extension(cub->texture.south, ".xpm"))
		return (1);
	check_is_there_texture_file(cub);
	return (0);
}
