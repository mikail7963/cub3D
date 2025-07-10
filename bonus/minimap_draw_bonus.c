/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_draw_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:56:59 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/10 19:01:13 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

int	color_selection(t_cub *cub, int x, int y)
{
	int	color;

	color = 0;
	if (cub->map.map[y][x] == '1')
		color = 0xE0E0E0;
	else if (cub->map.map[y][x] == '0' || cub->map.map[y][x] == 'N'
				|| cub->map.map[y][x] == 'S' || cub->map.map[y][x] == 'E'
				|| cub->map.map[y][x] == 'W')
		color = 0x606060;
	else if (cub->map.map[y][x] == 'D')
		color = 0x8B4513;
	else
		color = 0x303030;
	return (color);
}

void	draw_rectangle(t_cub *cub, int start_x, int start_y, int width, int height, int color)
{
	int	x;
	int	y;

	y = start_y;
	while (y < start_y + height)
	{
		x = start_x;
		while (x < start_x + width)
		{
			my_mlx_pixel_put_for_minimap(cub, x, y, color);
			x++;
		}
		y++;
	}
}

// Oyuncuyu minimap üzerindeki gerçek pozisyonunda çiz
void	draw_player_in_minimap(t_cub *cub, float dx, float dy, int player_size)
{
	int			player_x;
	int			player_y;
	int			px;
	int			py;
	int			i;

	player_x = cub->minimap.offset_x + cub->minimap.player_map_px;
	player_y = cub->minimap.offset_y + cub->minimap.player_map_py;
	player_size = cub->minimap.cell_size / 2;
	if (player_size < 3)
		player_size = 3;
	if (player_size > 8)
		player_size = 8;
	// Basit bir çizgi çizimi
	draw_rectangle(cub, player_x - player_size / 2, player_y - player_size / 2,
		player_size, player_size, 0xFF0000);
	dx = cub->player.dirx;
	dy = cub->player.diry;
	i = 0;
	while (i < cub->minimap.cell_size + 4)
	{
		px = player_x + (int)(dx * i);
		py = player_y + (int)(dy * i);
		my_mlx_pixel_put_for_minimap(cub, px, py, 0x00FF00);
		i++;
	}	
}

//haritayı çiz
void	draw_minimap(t_cub *cub)
{
	int	mini_x;
	int	mini_y;
	int	x;
	int	y;
	int	color;

	color = 0;
	y = 0;
	while (y < cub->map.map_height && cub->map.map[y])
	{
		x = 0;
		while (x < (int)ft_strlen(cub->map.map[y]))
		{
			if (cub->map.map[y][x] && cub->map.map[y][x] != ' ')
			{
				color = color_selection(cub, x, y);
				mini_x = cub->minimap.offset_x + (x * cub->minimap.cell_size);
				mini_y = cub->minimap.offset_y + (y * cub->minimap.cell_size);
				draw_rectangle(cub, mini_x, mini_y, cub->minimap.cell_size,
					cub->minimap.cell_size, color);
			}
			x++;
		}
		y++;
	}
}
