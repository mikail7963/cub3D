/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 10:56:00 by atursun           #+#    #+#             */
/*   Updated: 2025/07/07 16:55:37 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

/*
Oyuncunun (x, y) konumunun harita üzerinde geçerli ve yürünebilir (çarpışma olmayan) bir pozisyon olup olmadığını kontrol eder

*/
int	is_valid_position(t_cub *cub, double x, double y)
{
	int	map_x;
	int	map_y;

	map_x = (int)x;
	map_y = (int)y;
	if (map_x < 0 || map_y < 0)		// Eksi koordinatlar geçersizdir. Harita dışına çıkılmış demektir.
		return (0);
	// Pozisyonun harita içinde geçerli bir hücre olup olmadığını kontrol eder.
	if (!cub->map.map[map_y] || !cub->map.map[map_y][map_x])
		return (0);
	if (cub->map.map[map_y][map_x] == '1')	// Haritada '1' bir duvarı temsil eder. Yani buraya girilemez. (Duvardan geçemezsin/duvar çarpışması)
	{		
		return (0);
	}
	if (BONUS && cub->map.map[map_y][map_x] == 'D' && cub->door[find_true_door(cub,map_x,map_y)].is_open == 0)
		return (0);
	return (1);
}

/* bu formul zaten
Oyuncunun yönünü ve görüş açısını döndürmek
*/
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
