/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:55:10 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/02 18:02:59 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	check_map_line_start_end(char *line, t_cub *cub)
{
    int	i;
    int	in_block;
    int	block_start;

    i = 0;
    in_block = 0;
    while (line[i])
    {
        if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
        {
            if (!in_block)
            {
                block_start = i;
                in_block = 1;
                if (line[i] != '1')
                    error_msg("Map block does not start with '1'", cub, 3);
            }
        }
        else if (in_block)  // Blok bitti
        {
            if (line[i - 1] != '1')  // Blok '1' ile bitmeli
                error_msg("Map block does not end with '1'", cub, 3);
            in_block = 0;
        }
        i++;
    }
    // Son karakter kontrolü
    if (in_block && i > 0 && line[i - 1] != '1' && line[i - 1] != '\n')
        error_msg("Map block does not end with '1'", cub, 3);
}

int	get_num_lines(char **map)
{
	int	count;

	count = 0;
	while (map[count] != NULL)
		count++;
	return (count);
}

void	check_map(t_cub *cub)
{
	int	i;
	int	j;
	int	num_lines;

	i = 0;
	num_lines = get_num_lines(cub->map.map);
	while (cub->map.map[i])
	{
		j = 0;
		if (i == 0 )
		{
			while (cub->map.map[i][j])
			{
				if (cub->map.map[i][j] != '1' && cub->map.map[i][j] != '\n' &&
					cub->map.map[i][j] != ' ' && cub->map.map[i][j] != '\t')
					{
						ft_putchar_fd(cub->map.map[i][j], 1);
						error_msg("Map line does not start and end with '1", cub, 3);
					}
				j++;
			}
		}
		else
			check_map_line_start_end(cub->map.map[i], cub);
		i++;
	}
}
