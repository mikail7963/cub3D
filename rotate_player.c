/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 10:56:00 by atursun           #+#    #+#             */
/*   Updated: 2025/07/11 12:46:32 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	rotate_player(t_cub *cub, double angle)
{
	double	old_dirx;
	double	old_planex;

	old_planex = cub->plane_x;
	old_dirx = cub->player.dirx;
	cub->player.dirx = \
		cub->player.dirx * cos(angle) - cub->player.diry * sin(angle);
	cub->player.diry = old_dirx * sin(angle) + cub->player.diry * cos(angle);
	cub->plane_x = cub->plane_x * cos(angle) - cub->plane_y * sin(angle);
	cub->plane_y = old_planex * sin(angle) + cub->plane_y * cos(angle);
}
