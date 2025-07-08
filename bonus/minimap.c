/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:47:02 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/08 18:47:04 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

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

void    minimap(t_cub *cub)
{
    int color;
    int x;
    int y;
    float scale_factor;
    int map_width = 0;
    int map_height = 0;

    // Harita boyutlarını hesapla
    while (cub->map.map[map_height])
    {
        x = 0;
        while (cub->map.map[map_height][x])
            x++;
        if (x > map_width)
            map_width = x;
        map_height++;
    }

    // Ölçeklendirme faktörünü hesapla (minimap'in ana haritaya oranı)
    scale_factor = 1.2; // Haritanın %15'i kadar büyüklükte olacak

    y = 0;
    while (cub->map.map[y])
    {
        x = 0;
        while (cub->map.map[y][x])
        {
            if (cub->map.map[y][x] == '1')
                color = 0xe9e1df;
            else if (cub->map.map[y][x] == '0')
                color = 0x000000;
            else
                color = 0x000000;
            
            // Küçültülmüş koordinatları hesapla
            int mini_x = (int)(x * MINIMAP_SCALE * scale_factor);
            int mini_y = (int)(y * MINIMAP_SCALE * scale_factor);
            int mini_size = (int)(MINIMAP_SCALE * scale_factor);
            
            draw_rectangle(cub, mini_x, mini_y, mini_size, mini_size, color);
            x++;
        }
        y++;
    }

    // Oyuncuyu ölçeklendirilmiş şekilde çiz
    int player_x = (int)(cub->player.posx * MINIMAP_SCALE * scale_factor);
    int player_y = (int)(cub->player.posy * MINIMAP_SCALE * scale_factor);
    int player_size = (int)(MINIMAP_SCALE * scale_factor / 2);
    
    draw_rectangle(cub, player_x, player_y, player_size, player_size, 0xec1313);
    mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->minimap.mini_image, 10, 10);
}
