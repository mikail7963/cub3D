/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_render_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 13:27:38 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/29 14:03:56 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D_bonus.h"

static void render_vertical_texture(t_cub *cub, t_render *render, int x, int tex_x)
{
	double step;
	double tex_pos;
	int color;
	int tex_y;
	int y;

	(void)tex_x;
	color = 0;
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
		if (render->is_door && color == 0x0B0A0A)
		{
			t_render bg_render = *render;
			continue_ray_after_door(cub, &bg_render);
			select_texture(cub, &bg_render);
			if (bg_render.selected_texture)
			{
				if (y >= bg_render.draw_start && y < bg_render.draw_end)
				{
					double bg_step = 1.0 * bg_render.selected_texture->tex_height / bg_render.line_height;
					double bg_tx_pos = (y - HEIGHT / 2 + bg_render.line_height / 2) * bg_step;
					int bg_tex_y = (int)bg_tx_pos & (bg_render.selected_texture->tex_height - 1);
					int bg_tex_x;
					double wall_x;
					if (bg_render.side == 0)
						wall_x = cub->player.posy + bg_render.perp_wall_dist * bg_render.ray_dir_y;
					else
						wall_x = cub->player.posx + bg_render.perp_wall_dist * bg_render.ray_dir_x;
					wall_x -= floor(wall_x);
					bg_tex_x = (int)(wall_x * (double)bg_render.selected_texture->tex_width);
					if (bg_render.side == 0 && bg_render.ray_dir_x < 0)
						bg_tex_x = bg_render.selected_texture->tex_width - bg_tex_x - 1;
					if (bg_render.side == 1 && bg_render.ray_dir_y > 0)
						bg_tex_x = bg_render.selected_texture->tex_width - bg_tex_x - 1;
					int bg_color = *(unsigned int *)(bg_render.selected_texture->texture_data
						+ (bg_tex_y * bg_render.selected_texture->size_line + bg_tex_x
							* (bg_render.selected_texture->bits_per_pixel / 8)));
					if (bg_render.is_door && bg_color == 0x0B0A0A)
					{
						return (render_vertical_texture(cub, &bg_render, x , bg_tex_x));
						y++;
					}
					my_mlx_pixel_put(cub, x, y, bg_color);
				}
				else
				{
					if (y < HEIGHT / 2)
						my_mlx_pixel_put(cub, x, y, cub->fc.ceiling_c.colour);
					else
						my_mlx_pixel_put(cub, x, y, cub->fc.floor_c.colour);
				}
			}
			else
			{
				if (y < HEIGHT / 2)
					my_mlx_pixel_put(cub, x, y, cub->fc.ceiling_c.colour);
				else
					my_mlx_pixel_put(cub, x, y, cub->fc.floor_c.colour);
			}
			y++;
			continue ;
		}
		my_mlx_pixel_put(cub, x, y, color);
		y++;
	}
}

static void draw_wall_texture_for_door_back(t_cub *cub, t_render *render, int x)
{
	double wall_x;
	int tex_x;

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
	render_vertical_texture(cub, render, x, tex_x);
}

void continue_ray_after_door(t_cub *cub, t_render *render)
{
	render->hit = 0;
	while (render->hit == 0)
	{
		move_ray(render);
		if (render->map_y < 0 || render->map_x < 0 || !cub->map.map[render->map_y] || render->map_x >= (int)ft_strlen(cub->map.map[render->map_y]))
		{
			render->hit = 0;
			render->selected_texture = NULL;
			return ;
		}
		if (cub->map.map[render->map_y][render->map_x] == '1')
		{
			render->hit = 1;
			render->is_door = 0;
			break ;
		}
		if (cub->map.map[render->map_y][render->map_x] == 'D')
		{
			render->hit = 1;
			render->is_door = 1;
			play_door_sprite(cub, render);
		}
	}
	calculate_wall_distance_and_height(cub, render);
}

void	draw_background_for_door(t_cub *cub, t_render *render, int x)
{
	int			y;
	t_render	bg_render;
	t_door		true_door;

	true_door = cub->doors_manager.door[find_true_door(cub, render->map_x, render->map_y)];
	if (true_door.is_open == 0 && true_door.is_opening != 1
		&& true_door.is_closing != 1)
		return;
	bg_render = *render;
	continue_ray_after_door(cub, &bg_render);
	select_texture(cub, &bg_render);
	if (bg_render.selected_texture)
		draw_wall_texture_for_door_back(cub, &bg_render, x);
	else
	{
		y = render->draw_start;
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
