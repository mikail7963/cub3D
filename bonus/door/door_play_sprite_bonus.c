/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_play_sprite_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:00:35 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/29 13:58:30 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D_bonus.h"

static void	start_door_sprite(t_cub *cub, int door_index)
{
	if (cub->doors_manager.door[door_index].is_opening == 1)
		cub->doors_manager.door[door_index].door_frame++;
	else if (cub->doors_manager.door[door_index].is_closing == 1)
		cub->doors_manager.door[door_index].door_frame--;
	cub->doors_manager.door[door_index].time = get_time_ms();
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

void	play_door_sprite(t_cub *cub, t_render *render)
{
	int	door_index;

	door_index = find_true_door(cub, render->map_x, render->map_y);
	if (cub->doors_manager.door[door_index].is_opening == 1
		|| cub->doors_manager.door[door_index].is_closing == 1)
	{
		if (get_time_ms() - cub->doors_manager.door[door_index].time >= 100)
			start_door_sprite(cub, door_index);
	}
	if (cub->doors_manager.door[door_index].is_closing == 1)
	{
		render->selected_texture = &cub->doors_manager.frames
		[cub->doors_manager.door[door_index].door_frame];
	}
	else if (cub->doors_manager.door[door_index].is_opening == 1)
		render->selected_texture = &cub->doors_manager.frames
		[cub->doors_manager.door[door_index].door_frame];
	else if (cub->doors_manager.door[door_index].is_open == 0)
		render->selected_texture = &cub->doors_manager.frames[0];
	else if (cub->doors_manager.door[door_index].is_open == 1)
		render->selected_texture = &cub->doors_manager.frames[7];
}
