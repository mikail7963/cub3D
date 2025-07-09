/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_render_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 13:27:38 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/09 15:22:03 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D_bonus.h"
#include <sys/time.h>

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	play_door_sprite(t_cub *cub, t_render *render)
{
	int	door_index;

	door_index = find_true_door(cub, render->map_x, render->map_y);	
	// ✅ Animasyon kontrolü
	if (cub->doors_manager.door[door_index].is_opening == 1 || cub->doors_manager.door[door_index].is_closing == 1)
	{
		if (get_time_ms() - cub->doors_manager.door[door_index].time >= 100)
		{
			if (cub->doors_manager.door[door_index].is_opening == 1)
				cub->doors_manager.door[door_index].door_frame++;
			else if (cub->doors_manager.door[door_index].is_closing == 1)
				cub->doors_manager.door[door_index].door_frame--;
			cub->doors_manager.door[door_index].time = get_time_ms();
			
			// ✅ Animasyon tamamlandı
			if (cub->doors_manager.door[door_index].door_frame >= 7)
			{
				cub->doors_manager.door[door_index].is_opening = 0;
				cub->doors_manager.door[door_index].is_open = 1;
				cub->doors_manager.door[door_index].door_frame = 7;
			}
			else if (cub->doors_manager.door[door_index].door_frame <= 1)
			{
				cub->doors_manager.door[door_index].is_closing = 0;
				cub->doors_manager.door[door_index].is_open = 0;
				cub->doors_manager.door[door_index].door_frame = 0;
			}
		}
	}
	
	// ✅ Frame'i kapı durumuna göre belirle
	if (cub->doors_manager.door[door_index].is_closing == 1)
	{
		render->selected_texture = &cub->doors_manager.frames[cub->doors_manager.door[door_index].door_frame];
	}
	else if (cub->doors_manager.door[door_index].is_opening == 1)
		render->selected_texture = &cub->doors_manager.frames[cub->doors_manager.door[door_index].door_frame];
	else if (cub->doors_manager.door[door_index].is_open == 0)
		render->selected_texture = &cub->doors_manager.frames[0];
	else if (cub->doors_manager.door[door_index].is_open == 1)
		render->selected_texture = &cub->doors_manager.frames[7];
}

void	draw_wall_texture(t_cub *cub, t_render *render, int x)
{
	double	wall_x;
	int		y;
	int		tex_y;
	int		tex_x;
	int		color;
	double	step;
	double	tex_pos;

	// Wall_x hesaplama
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
		my_mlx_pixel_put(cub, x, y, color);
		y++;
	}
}
void	draw_background_for_door(t_cub *cub, t_render *render, int x)
{
	t_render	bg_render;
	
	// Aynı ray'i kopyala
	bg_render = *render;
	
	// Kapıdan sonraki duvarı bul
	continue_ray_after_door(cub, &bg_render);
	
 	// Arka duvarın texture'ını seç
		select_texture(cub, &bg_render);

		// Arka duvarı çiz
	if (bg_render.selected_texture)
		draw_wall_texture(cub, &bg_render, x);
	else
	{
		int y = render->draw_start;  
		while (y < render->draw_end)
		{
			if (y < HEIGHT / 2)
				my_mlx_pixel_put(cub, x, y, cub->fc.ceiling_c.colour);
			else
				my_mlx_pixel_put(cub, x, y, cub->fc.floor_c.colour);
			y++;
		}
	}
}

void	continue_ray_after_door(t_cub *cub, t_render *render)
{
	// Ray'i kapıdan sonra devam ettir
	render->hit = 0;
	
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
		
		// Map sınırlarını kontrol et
		if (render->map_y < 0 || render->map_x < 0 || 
			!cub->map.map[render->map_y] || 
			render->map_x >= (int)ft_strlen(cub->map.map[render->map_y]))
		{
			render->hit = 0;
			render->selected_texture = NULL;
			return;
		}
		
		// Sadece duvarları ara, kapıları atla
		if (cub->map.map[render->map_y][render->map_x] == '1')
		{
			render->hit = 1;
			render->is_door = 0;
			break;
		}
		// Kapıları atla
		if (cub->map.map[render->map_y][render->map_x] == 'D')
			continue;
		
	}
	// Arka duvar mesafesini hesapla
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
