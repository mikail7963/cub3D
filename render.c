/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:10:14 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/04 17:27:19 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

// ===================== RAY CASTING SETUP =====================
void	setup_ray(t_cub *cub, t_render *render, int x)
{
	render->rayDirX = cub->player.dirx + cub->plane_x * (2 * x / (double)WIDTH - 1);
	render->rayDirY = cub->player.diry + cub->plane_y * (2 * x / (double)WIDTH - 1);
	render->mapX = (int)cub->player.posx;
	render->mapY = (int)cub->player.posy;
	if (render->rayDirX != 0)
		render->deltaDistX = fabs(1 / render->rayDirX);
	else
		render->deltaDistX = 1e30;
	if (render->rayDirY != 0)
		render->deltaDistY = fabs(1 / render->rayDirY);
	else
		render->deltaDistY = 1e30;
	render->hit = 0;
}

// ===================== STEP AND SIDE DISTANCE =====================
void	calculate_step_and_side_dist(t_cub *cub, t_render *render)
{
	if (render->rayDirX < 0)
	{
		render->stepX = -1;
		render->sideDistX = (cub->player.posx - render->mapX) * render->deltaDistX;
	}
	else
	{
		render->stepX = 1;
		render->sideDistX = (render->mapX + 1.0 - cub->player.posx) * render->deltaDistX;
	}
	if (render->rayDirY < 0)
	{
		render->stepY = -1;
		render->sideDistY = (cub->player.posy - render->mapY) * render->deltaDistY;
	}
	else
	{
		render->stepY = 1;
		render->sideDistY = (render->mapY + 1.0 - cub->player.posy) * render->deltaDistY;
	}
}

// ===================== DDA ALGORITHM =====================
void	perform_dda(t_cub *cub, t_render *render)
{
	while (render->hit == 0)
	{
		if (render->sideDistX < render->sideDistY)
		{
			render->sideDistX += render->deltaDistX;
			render->mapX += render->stepX;
			render->side = 0;
		}
		else
		{
			render->sideDistY += render->deltaDistY;
			render->mapY += render->stepY;
			render->side = 1;
		}
		if (cub->map.map[render->mapY][render->mapX] == '1')
			render->hit = 1;
	}
}

// ===================== WALL DISTANCE AND HEIGHT =====================
void	calculate_wall_distance_and_height(t_cub *cub, t_render *render)
{
	if (render->side == 0)
		render->perpWallDist = (render->mapX - cub->player.posx + (1 - render->stepX) / 2) / render->rayDirX;
	else
		render->perpWallDist = (render->mapY - cub->player.posy + (1 - render->stepY) / 2) / render->rayDirY;
	render->lineHeight = (int)(HEIGHT / render->perpWallDist);
	render->drawStart = -render->lineHeight / 2 + HEIGHT / 2;
	render->drawEnd = render->lineHeight / 2 + HEIGHT / 2;
	if (render->drawStart < 0)
		render->drawStart = 0;
	if (render->drawEnd >= HEIGHT)
		render->drawEnd = HEIGHT - 1;
}

// ===================== MAIN RENDER FUNCTION =====================
void	render_map(t_cub *cub)
{
	t_render	render;
	int			x;

	x = 0;
	painting_sky_and_ground(cub);
	while (x < WIDTH)
	{
		setup_ray(cub, &render, x);
		calculate_step_and_side_dist(cub, &render);
		perform_dda(cub, &render);
		calculate_wall_distance_and_height(cub, &render);
		select_texture(cub, &render);
		draw_texture(cub, &render, x);
		x++;
	}
	// ===================== EKRANA GÖSTER =====================
	// Tamamlanan image'ı window'a yerleştir
	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->mlx.win_data.image, 0, 0);
}
