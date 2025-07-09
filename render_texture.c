/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:25:06 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/09 15:22:03 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

/* 
 * Window buffer'ına pixel yerleştiren fonksiyon
 * Manuel pixel çizimi için kullanılır
 */
void	my_mlx_pixel_put(t_cub *cub, int x, int y, int color)
{
	char	*dst;

	if (BONUS && color == 0x000000)
		return;
	// Koordinatların ekran sınırları içinde olup olmadığını kontrol et
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = cub->mlx.win_data.texture_data;  // Window buffer'ına erişim
	// Pixel'in buffer'daki adresini hesapla (y * satır genişliği + x * pixel boyutu)
	dst += (y * cub->mlx.win_data.size_line + x * (cub->mlx.win_data.bits_per_pixel / 8));
	// Renk değerini buffer'a yaz
	*(unsigned int *)dst = color;
}

/*
Tüm texture'ları yükler ve hazırlar
- 4 yön için XPM dosyalarını yükler (North, South, East, West)
- Her texture için data pointer'ı alır
- Ana window image'ını oluşturur
*/
void	render_picture(t_cub *cub)
{
	// Her yön için texture dosyasını yükle
	cub->north.image = mlx_xpm_file_to_image(cub->mlx.mlx, cub->texture.north, &cub->north.tex_width, &cub->north.tex_height);
	if (!cub->north.image)
		error_msg("Texture is incorrect: North", cub, 1);
	cub->south.image = mlx_xpm_file_to_image(cub->mlx.mlx, cub->texture.south, &cub->south.tex_width, &cub->south.tex_height);
	if (!cub->south.image)
		error_msg("Texture is incorrect: south", cub, 1);
	cub->east.image = mlx_xpm_file_to_image(cub->mlx.mlx, cub->texture.east, &cub->east.tex_width, &cub->east.tex_height);
	if (!cub->east.image)
		error_msg("Texture is incorrect: east", cub, 1);
	cub->west.image = mlx_xpm_file_to_image(cub->mlx.mlx, cub->texture.west, &cub->west.tex_width, &cub->west.tex_height);
	if (!cub->west.image)
		error_msg("Texture is incorrect: West", cub, 1);
	cub->south.texture_data = mlx_get_data_addr(cub->south.image, &cub->south.bits_per_pixel , &cub->south.size_line, &cub->south.endian);
	cub->north.texture_data = mlx_get_data_addr(cub->north.image, &cub->north.bits_per_pixel, &cub->north.size_line, &cub->north.endian);
	cub->east.texture_data = mlx_get_data_addr(cub->east.image, &cub->east.bits_per_pixel, &cub->east.size_line, &cub->east.endian);
	cub->west.texture_data = mlx_get_data_addr(cub->west.image, &cub->west.bits_per_pixel, &cub->west.size_line, &cub->west.endian);
	// Window image oluştur
	cub->mlx.win_data.image = mlx_new_image(cub->mlx.mlx, WIDTH, HEIGHT);
	cub->mlx.win_data.texture_data = mlx_get_data_addr(cub->mlx.win_data.image, &cub->mlx.win_data.bits_per_pixel, &cub->mlx.win_data.size_line, &cub->mlx.win_data.endian);
}

/*
Arkaplan renklerini boyar
- Tüm ekranı teker teker dolaşır
- Üst yarı = gökyüzü rengi (ceiling)
- Alt yarı = zemin rengi (floor)
- Pixel hesaplama: x = i % WIDTH, y = i / WIDTH
*/
void	painting_sky_and_ground(t_cub *cub)
{
	int	total_pixels;
	int	i;
	int	x;
	int	y;

	i = 0;
	total_pixels = WIDTH * HEIGHT;
	while (i < total_pixels)
	{
		x = i % WIDTH;
		y = i / WIDTH;
		if (y < HEIGHT / 2)
			my_mlx_pixel_put(cub, x, y, cub->fc.ceiling_c.colour);
		else
			my_mlx_pixel_put(cub, x, y, cub->fc.floor_c.colour);
		i++;
	}
}

/*
Hangi texture'ın kullanılacağını belirler
- side = 0: X tarafı duvaru (East/West)
- side = 1: Y tarafı duvaru (North/South)
- Işın yönüne göre doğru texture'ı seçer
*/
void	select_texture(t_cub *cub, t_render *render)
{
	if (BONUS && render->is_door == 1)
		return;
	if (render->side == 0 && render->ray_dir_x > 0)
		render->selected_texture = &cub->east;
	else if (render->side == 0 && render->ray_dir_x < 0)
		render->selected_texture = &cub->west;
	else if (render->side == 1 && render->ray_dir_y > 0)
		render->selected_texture = &cub->south;
	else
		render->selected_texture = &cub->north;
	
}

/*
Seçilen texture'ı duvara çizer
- wall_x hesaplama: Duvarın hangi noktasına çarptığını bulur
- tex_x hesaplama: Texture'ın X koordinatını belirler
- Yansıtma kontrolü: Bazı durumlarda texture'ı ters çevirir
- step hesaplama: Texture'ın Y ekseninde nasıl ölçekleneceği
- Pixel döngüsü: Her Y koordinatı için:
	- Texture Y koordinatını hesaplar
	- Texture'dan renk alır
	- Ekrana pixel çizer
*/
void	draw_texture(t_cub *cub, t_render *render, int x)
{
	double	wall_x;
	int		y;
	int		tex_y;
	int		tex_x;
	int		color;
	double	step;
	double	tex_pos;

   // ✅ ÖNCE arka plan çiz, SONRA kapı texture'ını çiz
	if (BONUS && render->is_door)
		draw_background_for_door(cub, render, x);
	
	if (render->side == 0)
		wall_x = cub->player.posy + render->perp_wall_dist * render->ray_dir_y;
	else
		wall_x = cub->player.posx + render->perp_wall_dist * render->ray_dir_x;
	wall_x -= floor(wall_x);
	
	tex_x = (int)(wall_x * (double)render->selected_texture->tex_width);
	if (render->side == 0 && render->ray_dir_x < 0)
		tex_x = render->selected_texture->tex_width - tex_x - 1;
	if (render->side == 1 && render->ray_dir_y > 0)
		tex_x = render->selected_texture->tex_width - tex_x - 1;
	
	step = 1.0 * render->selected_texture->tex_height / render->line_height;
	tex_pos = (render->draw_start - HEIGHT / 2 + render->line_height / 2) * step;
	y = render->draw_start;
	
	while (y < render->draw_end)
	{
		tex_y = (int)tex_pos & (render->selected_texture->tex_height - 1);
		tex_pos += step;
		color = *(unsigned int *)(render->selected_texture->texture_data
				+ (tex_y * render->selected_texture->size_line + tex_x
					* (render->selected_texture->bits_per_pixel / 8)));
		
		// ✅ Kapı için SADECE şeffaf pikselleri atla
		if (BONUS && render->is_door && color == 0x000000)
		{
			y++;
			continue; // Şeffaf pixel'i atla, arka plan zaten çizildi
		}
		
		my_mlx_pixel_put(cub, x, y, color);
		y++;
	}
}
