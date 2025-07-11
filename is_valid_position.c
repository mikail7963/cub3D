/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_position.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:15:04 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/11 12:43:15 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	is_valid_position(t_cub *cub, double x, double y)
{
	double	radius;
	double	dx;
	double	dy;
	int		check_x;
	int		check_y;

	radius = 0.2;
	dx = -radius;
	while (dx <= radius)
	{
		dy = -radius;
		while (dy <= radius)
		{
			check_x = (int)(x + dx);
			check_y = (int)(y + dy);
			if (cub->map.map[check_y] && cub->map.map[check_y][check_x] == '1')
				return (0);
			dy += 0.1;
		}
		dx += 0.1;
	}	
	return (1);
}
