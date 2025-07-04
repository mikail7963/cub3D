/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atursun <atursun@student.42istanbul.com.tr +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:55:10 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/04 16:11:01 by atursun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	check_map_line_start_end(char *line, t_cub *cub)
{
	int	i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i] != '1')
		error_msg("Map block does not start with '1'", cub, 3);
	i = ft_strlen(line) - 1;
	while (i >= 0 && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n'))
		i--;
	if (i >= 0 && line[i] != '1')
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

void	check_walls(t_cub *cub)
{
	int	i;
	int	j;

	i = 0;
	while (cub->map.map[i])
	{
		j = 0;
		if (i == 0)
		{
			while (cub->map.map[i][j])
			{
				if (cub->map.map[i][j] != '1' && cub->map.map[i][j] != '\n' &&
					cub->map.map[i][j] != ' ' && cub->map.map[i][j] != '\t')
				{
					error_msg("There is no '1 in the first or \
						last line of the map", cub, 3);
				}
				j++;
			}
		}
		else
			check_map_line_start_end(cub->map.map[i], cub);
		i++;
	}
}

void	check_map(t_cub *cub)
{
	int	i;
	int	num_lines;

	i = 0;
	num_lines = get_num_lines(cub->map.map);
	while (cub->map.map[num_lines - 1][i])
	{
		if (cub->map.map[num_lines - 1][i] != '1'
			&& cub->map.map[num_lines - 1][i] != '\n'
			&& cub->map.map[num_lines - 1][i] != ' '
			&& cub->map.map[num_lines - 1][i] != '\t')
			error_msg("Map line does not start and end with '1", cub, 3);
		i++;
	}
	check_walls(cub);
}

#include <stdio.h>

// void	check_map_around_wall(t_cub *cub)
// {
// 	int		i;
// 	int		g;
// 	size_t	len;

// 	i = 0;
// 	g = 0;
// 	while (cub->map.map[i])
// 	{
// 		len = ft_strlen(cub->map.map[i]);
// 		if (len == ft_strlen(cub->map.map[i + 1]))
// 		{
// 			i++;
// 			continue;
// 		}
// 		if (len > ft_strlen(cub->map.map[i + 1]))
// 		{
// 			g = len - ft_strlen(cub->map.map[i + 1]);
// 			printf("g: %d\n",g);
// 			while (g >= 0)
// 			{
// 				if (cub->map.map[i][len - g] != '1')
// 				{
// 					printf("line %s\n",cub->map.map[i]);
// 					printf("len %lu index %c\n", len , cub->map.map[i][len]);
// 					error_msg("Map around not wall", cub, 3);
// 				}
// 				g--;
// 			}
// 		}
// 		else
// 		{
// 			g = len - ft_strlen(cub->map.map[i + 1]);
// 			printf("g: %d\n",g);
// 			while (g > 0)
// 			{
// 				if (cub->map.map[i + 1][ft_strlen(cub->map.map[i + 1])] != '1')
// 				{
// 					printf("line %s\n",cub->map.map[i]);
// 					printf("len %lu index %c\n", len - 1 - g, cub->map.map[i][len - 1 -g]);
// 					error_msg("Map around not walll", cub, 3);
// 				}				
// 				g--;
// 			}
// 		}
// 		i++;
// 	}
// }