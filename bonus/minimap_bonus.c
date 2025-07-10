/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 17:11:11 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/10 18:58:28 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"

void	my_mlx_pixel_put_for_minimap(t_cub *cub, int x, int y, int color)
{
	char	*dst;

	// Koordinatların ekran sınırları içinde olup olmadığını kontrol et
	if (x < 0 || x >= MINIMAP_WIDTH || y < 0 || y >= MINIMAP_HEIGHT)
		return ;
	dst = cub->minimap.data;  // Window buffer'ına erişim
	// Pixel'in buffer'daki adresini hesapla (y * satır genişliği + x * pixel boyutu)
	dst += (y * cub->minimap.size_line + x * (cub->minimap.bits_per_pixel / 8));
	// Renk değerini buffer'a yaz
	*(unsigned int *)dst = color;
}

// Harita boyutlarını hesapla
void	calculate_map_height_and_with(t_cub *cub)
{
	int	current_width;

	current_width = 0;
	while (cub->map.map[cub->map.map_height])
	{
		current_width = ft_strlen(cub->map.map[cub->map.map_height]);
		if (current_width > cub->map.map_width)
			cub->map.map_width = current_width;
		cub->map.map_height++;
	}
}

// Haritayı kaydırmak için ofset hesapla
// Eğer harita minimap'ten küçükse, haritayı ortala ve kaydırmayı durdur
void	check_minimap(t_cub *cub)
{
	int	total_map_pixel_width;
	int	total_map_pixel_height;

	total_map_pixel_width = cub->map.map_width * cub->minimap.cell_size;
	total_map_pixel_height = cub->map.map_height * cub->minimap.cell_size;
	if (total_map_pixel_width < MINIMAP_WIDTH)
		cub->minimap.offset_x = (MINIMAP_WIDTH - total_map_pixel_width) / 2;
	else
	{
		// Harita kenarlara ulaştığında kaydırmayı sınırla
		if (cub->minimap.offset_x > 0)
			cub->minimap.offset_x = 0;
		if (cub->minimap.offset_x < MINIMAP_WIDTH - total_map_pixel_width)
			cub->minimap.offset_x = MINIMAP_WIDTH - total_map_pixel_width;
	}
	if (total_map_pixel_height < MINIMAP_HEIGHT)
		cub->minimap.offset_y = (MINIMAP_HEIGHT - total_map_pixel_height) / 2;
	else
	{
		if (cub->minimap.offset_y > 0)
			cub->minimap.offset_y = 0;
		if (cub->minimap.offset_y < MINIMAP_HEIGHT - total_map_pixel_height)
			cub->minimap.offset_y = MINIMAP_HEIGHT - total_map_pixel_height;
	}
}

void	draw_minimap_edge(t_cub *cub)
{
	// Minimap'i temizle
	draw_rectangle(cub, 0, 0, MINIMAP_WIDTH, MINIMAP_HEIGHT, 0x000000);
	// Minimap çerçevesi
	draw_rectangle(cub, 0, 0, MINIMAP_WIDTH, 1, 0xFFFFFF);
	draw_rectangle(cub, 0, MINIMAP_HEIGHT - 1, MINIMAP_WIDTH, 1, 0xFFFFFF);
	draw_rectangle(cub, 0, 0, 1, MINIMAP_HEIGHT, 0xFFFFFF);
	draw_rectangle(cub, MINIMAP_WIDTH - 1, 0, 1, MINIMAP_HEIGHT, 0xFFFFFF);
}

void	minimap(t_cub *cub)
{
	calculate_map_height_and_with(cub);
	// Oyuncunun harita üzerindeki pixel pozisyonu
	cub->minimap.player_map_px = cub->player.posx * cub->minimap.cell_size;
	cub->minimap.player_map_py = cub->player.posy * cub->minimap.cell_size;
	// Başlangıçta ofset, oyuncuyu merkeze alır
	cub->minimap.offset_x = (MINIMAP_WIDTH / 2) - cub->minimap.player_map_px;
	cub->minimap.offset_y = (MINIMAP_HEIGHT / 2) - cub->minimap.player_map_py;
	check_minimap(cub);
	draw_minimap_edge(cub);
	draw_minimap(cub);
	draw_player_in_minimap(cub, 0, 0, 0);
	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win,
		cub->minimap.mini_image, 10, 10);
}
