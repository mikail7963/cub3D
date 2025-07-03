/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atursun <atursun@student.42istanbul.com.tr +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:35:00 by atursun           #+#    #+#             */
/*   Updated: 2025/07/03 16:39:59 by atursun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	is_player(char p)
{
	if (p == 'N' || p == 'S' || p == 'E' || p == 'W')
		return (1);
	return (0);
}

void	validate_map_line(char *line, t_cub *cub)
{
	int	has_content;
	int	i;

	i = 0;
	has_content = 0;
	while (line[i])
	{
		if (!(line[i] == '0' || line[i] == '1' || is_player(line[i])
		|| line[i] == '\n' || line[i] == '\t' || line[i] == ' '))
			error_msg(": unknown character", cub, 2);
		i++;
	}
	if ((line[0] == ' ' || line[0] == '\t' || line[0] == '\n') && !ft_strchr(line, '1'))
		has_content = 1;
	if (has_content == 1)
		error_msg(": Empty or whitespace-only line in map", cub, 2);
}

void	set_coor_and_pos(t_cub *cub, char *line, int i)
{
	int	j;

	if (ft_strchr(line, 'N') || ft_strchr(line, 'S')
		|| ft_strchr(line, 'W') || ft_strchr(line, 'E'))
	{
		j = 0;
		cub->player.posy = (double)i + 0.5; // double sil ve + 0.5 kaldır farkı gör.
		while (line[j])
		{
			if (is_player(line[j]))
			{
				cub->is_player++;
				cub->player.posx = (double)j + 0.5;
			}
			j++;
		}
		if (ft_strchr(line, 'N'))
		{
			cub->player.diry = -1;
			cub->plane_x = 0.66;
		}
		if (ft_strchr(line, 'S'))
		{
			cub->plane_x = -0.66;
			cub->player.diry = 1;
		}
		if (ft_strchr(line, 'E'))
		{
			cub->player.dirx = 1;
			cub->plane_y = 0.66;
		}
		if (ft_strchr(line, 'W'))
		{
			cub->player.dirx = -1;
			cub->plane_y = -0.66;
		}
	}
}

int	map_reel_lenght(char *file, t_cub *cub)
{
	int fd;
	char *line;
	int len;

	len = 0;
	fd = open(file, O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		if (ft_strchr(line, '1') && !ft_strchr(line, 'F') && !ft_strchr(line, 'C')
			 && !ft_strchr(line, 'S') && !ft_strchr(line, 'W') && !ft_strchr(line, 'N') && !ft_strchr(line, 'E'))
			len++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	if (len == 0)
		error_msg("there is not map", cub, 2);
	len++;
	return (len);
}

void	read_map(t_cub *cub, char *file)
{
	int		i;
	char	*tmp;
	int		fd;
	int		map_start;
	int		map_lengt;
	
	map_lengt = map_reel_lenght(file, cub);		
	map_start = 0;
	fd = open(file, O_RDONLY);
	tmp = get_next_line(fd);
	while (tmp)
	{
		if (ft_strchr(tmp, '1') && !ft_strchr(tmp, 'F') && !ft_strchr(tmp, 'C')
		&& !ft_strnstr(tmp, "SO", ft_strlen(tmp)) && !ft_strnstr(tmp, "WE", ft_strlen(tmp)) 
		&& !ft_strnstr(tmp, "NO", ft_strlen(tmp)) && !ft_strnstr(tmp, "EA", ft_strlen(tmp)))
		break ;
		map_start++;
		free(tmp);
		tmp = get_next_line(fd);
	}
	if (ft_strchr(tmp, 'W') || ft_strchr(tmp, 'E')|| ft_strchr(tmp, 'N')|| ft_strchr(tmp, 'S'))
		error_msg("player on the wall", cub, 2);
	i = 0;
	cub->map.map = malloc(sizeof(char *) * map_lengt + 1);
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
	while (tmp != NULL)
	{
		i = 0;
		while (tmp[i])
		{
			if (tmp[i] != '\0' && tmp[i] != ' ' && tmp[i] != '\t' && tmp[i] != '\n')
			error_msg("wrong map design", cub, 3);
			i++;
		}
		free(tmp);
		tmp = get_next_line(fd);
	}
	close(fd);
	if (cub->is_player != 1)
		error_msg("There are Multiplayer or player number is 0", cub, 3);
}

void	read_file(t_cub *cub, char *file)
{
	int	fd;
	int	fd2;

	fd = open(file, O_RDONLY);
	read_texture(cub, fd);
	fd2 = open(file, O_RDONLY);
	read_fc_rgb(cub, fd2);
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
		error_msg("file is empty", cub, 2);
	while (line != NULL)
	{
		counter++;
		free(line);
		line = get_next_line(fd);
	}
	cub->len_of_file = counter;
	close(fd);
	read_file(cub, file);
	check_texture(cub);
	check_map(cub);
}
