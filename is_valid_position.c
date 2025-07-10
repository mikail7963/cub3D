/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_position.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:15:04 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/10 14:15:14 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	is_valid_position(t_cub *cub, double x, double y)
{
	int	map_x;
	int	map_y;
	int	map_height;
	int	map_width;

	map_x = (int)x;
	map_y = (int)y;
	map_height = 0;
	while (cub->map.map[map_height])
		map_height++;
	if (map_y >= map_height || map_y < 0)
		return (0);
	map_width = ft_strlen(cub->map.map[map_y]);
	if (map_x < 0 || map_x >= map_width)
		return (0);
	if (!cub->map.map[map_y] || !cub->map.map[map_y][map_x])
		return (0);
	if ((map_y == 0 && cub->map.map[map_y][map_x] == '1')	// Üst kenar
		|| (map_y == map_height - 1 && cub->map.map[map_y][map_x] == '1')	// Alt kenar
		|| (map_x == 0 && cub->map.map[map_y][map_x] == '1')		// Sol kenar
		|| (map_x == map_width - 1 && cub->map.map[map_y][map_x] == '1'))	// Sağ kenar
		return (0);
	return (1);
}
