/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 11:33:41 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/08 19:24:15 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_BONUS_H
# define CUB3D_BONUS_H
# include "../cub3D.h"
#define MINIMAP_SCALE 10
#define MINIMAP_WIDTH 170
#define MINIMAP_HEIGHT 150

void	init_door(t_cub *cub);
void	handle_door(t_cub *cub);
int		find_true_door(t_cub *cub, int x, int y);
void	render_door(t_cub *cub);
int		handle_mouse_move(int x, int y, t_cub *cub);
void	continue_ray_after_door(t_cub *cub, t_render *render);
void	draw_wall_texture(t_cub *cub, t_render *render, int x);
void	draw_background_for_door(t_cub *cub, t_render *render, int x);
int		check_wall_collisions(t_cub *cub, double x, double y);
void	play_door_sprite(t_cub *cub, t_render *render);
long	get_time_ms(void);
void    minimap(t_cub *cub);
void	free_door_sprite(t_cub *cub);

#endif