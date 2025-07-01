/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atursun <atursun@student.42istanbul.com.tr +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 14:35:00 by atursun           #+#    #+#             */
/*   Updated: 2025/07/01 15:22:46 by atursun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void validate_map_line(char *line)
{
    int     has_content;
    int     i;    

    i = 0;
    has_content = 0;;
    while (line[i])
    {
    
        if (!(line[i] == 'S' || line[i] == 'N' || line[i] == 'E' || line[i] == 'W'
            || line[i] == '0' || line[i] == '1' || line[i] == '\n' || line[i] == '\t' || line[i] == ' '))
            error_msg("Error: unknown character");
        i++;
    }
    if ((line[0] == ' ' || line[0] == '\t' || line[0] == '\n') && !ft_strchr(line, '1'))
            has_content = 1;
    if (has_content == 1)
        error_msg("Error: Empty or whitespace-only line in map");
}

void set_coor_and_pos(t_cub *cub, char *line, int i)
{
    int j;
    
    if (ft_strchr(line, 'N') || ft_strchr(line, 'S')
        || ft_strchr(line, 'W') || ft_strchr(line, 'E'))
    {
        j = 0;
        cub->player.posy = (double)i + 0.5; // double sil ve + 0.5 kaldır farkı gör.
        while (line[j])
        {
            if (line[j] == 'N' || line[j] == 'S'
                || line[j] == 'W' || line[j] == 'E')
                {
                    cub->is_player = 1;
                    cub->player.posx = (double)j + 0.5;
                }
            j++;
        }
        if (ft_strchr(line, 'N'))
            cub->player.diry = -1;
        if (ft_strchr(line, 'S'))
            cub->player.diry = 1;
        if (ft_strchr(line, 'E'))
            cub->player.dirx = 1;
        if (ft_strchr(line, 'W'))
            cub->player.dirx = -1;
    }
}

void read_map(t_cub *cub, int fd)
{
    int     i;
    char    *tmp;
    
    tmp = get_next_line(fd);
    while (tmp)
    {
        if (!ft_strchr(tmp, '1'))
        {
            free(tmp);
            tmp = get_next_line(fd);
            continue;
        }
        break ;
    }
    i = 0;
    cub->map.map = malloc(sizeof(char *) * (cub->len_of_file - cub->map_index + 3));
    while (tmp != NULL && tmp[0] != '\0')
    {
        validate_map_line(tmp);
        set_coor_and_pos(cub, tmp, i);
        if (ft_strchr(tmp, '1'))
            cub->map.map[i++] = ft_strdup(tmp);
        free(tmp);
        tmp = get_next_line(fd);
    }
    if (cub->is_player != 1)
        error_msg("Error: There are Multiplayer Or palyer number is 0");
    cub->map.map[i] = NULL;
    close(fd);
}

void read_file(t_cub *cub, char *file)
{
    int fd;
    int fd2;

    fd = open(file, O_RDONLY);
	read_texture(cub, fd);
    fd2 = open(file, O_RDONLY);
	read_fc_rgb(cub, fd2);
	read_map(cub, fd);
}

void open_file(t_cub *cub, char *file)
{
	int     counter;
    int     fd;
    char    *line;

    counter = 0;
	fd = open(file, O_RDONLY);
	if (fd == -1)
		return ;
	line = get_next_line(fd);
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
