/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handle_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:41:34 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/08 18:09:14 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

int	handle_mouse_move(int x, int y, t_cub *cub)
{
	static int prev_x = WIDTH / 2; // Fare imlecinin bir önceki x koordinatını saklar
	double rotation_speed;

	(void)y; // Y eksenini görmezden geliyoruz
	rotation_speed = ROT_SPEED;
	if (x > prev_x) // Fare sağa hareket ederse
		rotate_player(cub, rotation_speed * (x - prev_x) / 13);
	else if (x < prev_x) // Fare sola hareket ederse,
		rotate_player(cub, -rotation_speed * (prev_x - x) / 13);
	prev_x = WIDTH / 2;
	mlx_mouse_move(cub->mlx.mlx, cub->mlx.win, WIDTH / 2, HEIGHT / 2);
	return (0);
}



