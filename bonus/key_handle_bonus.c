/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handle_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:41:34 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/07 14:14:08 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

int	handle_mouse_press(int button, int x, int y, t_cub *cub)
{
	(void)x;
	(void)y;
	if (button == MOUSE_LEFT)
		cub->mouse_rotate_left = 1;
	if (button == MOUSE_RIGHT)
		cub->mouse_rotate_right = 1;
	return (0);
}

int	handle_mouse_release(int button, int x, int y, t_cub *cub)
{
	(void)x;
	(void)y;
	if (button == MOUSE_LEFT)
		cub->mouse_rotate_left = 0;
	if (button == MOUSE_RIGHT)
		cub->mouse_rotate_right = 0;
	return (0);
}
