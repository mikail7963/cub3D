/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:10:14 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/09 15:22:03 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

/*
Her ekran sütunu için ışın (ray) oluşturur
- Kamera düzlemindeki her piksel için ışın yönünü hesaplar
- Oyuncunun bakış yönü + kamera düzlemi = ışın yönü
- Başlangıç harita koordinatlarını (map_x, map_y) belirler
- delta_dist_x/Y: ışının bir harita karesini geçmek için gereken mesafe
*/
void	setup_ray(t_cub *cub, t_render *render, int x)
{
	render->ray_dir_x = cub->player.dirx + cub->plane_x * (2 * x / (double)WIDTH - 1);
	render->ray_dir_y = cub->player.diry + cub->plane_y * (2 * x / (double)WIDTH - 1);
	render->map_x = (int)cub->player.posx;
	render->map_y = (int)cub->player.posy;
	if (render->ray_dir_x != 0)
		render->delta_dist_x = fabs(1 / render->ray_dir_x);
	else
		render->delta_dist_x = 1e30;
	if (render->ray_dir_y != 0)
		render->delta_dist_y = fabs(1 / render->ray_dir_y);
	else
		render->delta_dist_y = 1e30;
	render->hit = 0;
}

/*
DDA algoritması için adım yönünü ve mesafeleri hesaplar
- step_x/Y: Işının hangi yönde ilerleyeceği (-1 veya +1)
- side_dist_x/Y: Bir sonraki harita çizgisine olan mesafe
- Pozitif yön: sağ/aşağı, Negatif yön: sol/yukar
*/
void	calculate_step_and_side_dist(t_cub *cub, t_render *render)
{
	if (render->ray_dir_x < 0)
	{
		render->step_x = -1;
		render->side_dist_x = (cub->player.posx - render->map_x) * render->delta_dist_x;
	}
	else
	{
		render->step_x = 1;
		render->side_dist_x = (render->map_x + 1.0 - cub->player.posx) * render->delta_dist_x;
	}
	if (render->ray_dir_y < 0)
	{
		render->step_y = -1;
		render->side_dist_y = (cub->player.posy - render->map_y) * render->delta_dist_y;
	}
	else
	{
		render->step_y = 1;
		render->side_dist_y = (render->map_y + 1.0 - cub->player.posy) * render->delta_dist_y;
	}
}

/*
Digital Differential Analyzer (DDA) - duvarı bulana kadar ışını ilerletir
- Her adımda X veya Y yönünde ilerler (hangisi daha yakınsa)
- Harita üzerinde '1' (duvar) bulana kadar devam eder
- side: hangi taraftan duvara çarptığını belirler (0=X tarafı, 1=Y tarafı)
*/
void	perform_dda(t_cub *cub, t_render *render)
{
	while (render->hit == 0)
	{
		if (render->side_dist_x < render->side_dist_y)
		{
			render->side_dist_x += render->delta_dist_x;
			render->map_x += render->step_x;
			render->side = 0;
		}
		else
		{
			render->side_dist_y += render->delta_dist_y;
			render->map_y += render->step_y;
			render->side = 1;
		}
		if (cub->map.map[render->map_y][render->map_x] == '1')
		{
			render->hit = 1;
			render->is_door = 0;	
		}
		if (BONUS && cub->map.map[render->map_y][render->map_x] == 'D')
		{
			play_door_sprite(cub,render);
			render->is_door = 1;
			render->hit = 1;
		}
	}
}

/*
Duvar mesafesini ve ekrandaki yüksekliğini hesaplar
- perp_wall_dist: Balıkgözü etkisini önlemek için perpendicular mesafe
- line_height: Duvarin ekrandaki pixel yüksekliği
- draw_start/End: Duvarın çizileceği Y koordinatları
*/
void	calculate_wall_distance_and_height(t_cub *cub, t_render *render)
{
	if (render->side == 0)
		render->perp_wall_dist = (render->map_x - cub->player.posx + (1 - render->step_x) / 2) / render->ray_dir_x;
	else
		render->perp_wall_dist = (render->map_y - cub->player.posy + (1 - render->step_y) / 2) / render->ray_dir_y;
	render->line_height = (int)(HEIGHT / render->perp_wall_dist);
	render->draw_start = -render->line_height / 2 + HEIGHT / 2;
	render->draw_end = render->line_height / 2 + HEIGHT / 2;
	if (render->draw_start < 0)
		render->draw_start = 0;
	if (render->draw_end >= HEIGHT)
		render->draw_end = HEIGHT - 1;
}

/*
Tüm render sürecini yönetir
- Gökyüzü ve zemin boyar
- Her ekran sütunu için (WIDTH boyunca):
	- Işın oluştur
	- DDA ile duvar bul
	- Mesafe ve yükseklik hesapla
	- Texture seç ve çiz
- Sonucu ekrana gösterir
*/
void	render_map(t_cub *cub)
{
	t_render	render;
	int			x;

	x = 0;
	painting_sky_and_ground(cub);
	while (x < WIDTH)
	{
		render.is_door = 0;
		setup_ray(cub, &render, x);
		calculate_step_and_side_dist(cub, &render);
		perform_dda(cub, &render);
		calculate_wall_distance_and_height(cub, &render);
		select_texture(cub, &render);
		draw_texture(cub, &render, x);
		x++;
	}
	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->mlx.win_data.image, 0, 0);
}
