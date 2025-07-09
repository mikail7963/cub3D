/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:16:26 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/03 19:16:26 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	game_loop(t_cub *cub)
{
	move_player(cub);
	render_map(cub);
	if (BONUS)
		minimap(cub);
	return (0);
}

void	init_movement_state(t_cub *cub)
{
	cub->move_forward = 0;
	cub->move_backward = 0;
	cub->move_left = 0;
	cub->move_right = 0;
	cub->rotate_left = 0;
	cub->rotate_right = 0;
	cub->mouse_rotate_left = 0;
	cub->mouse_rotate_right = 0;
}


/*
mlx_hook: Belirli olaylara (event) karşılık bir callback (geri çağırma) fonksiyonu tanımlar.
mlx_loop_hook: Her frame'de (her döngüde) sürekli çalışan bir fonksiyon tanımlar.
*/
void	setup_hooks(t_cub *cub)
{
	mlx_hook(cub->mlx.win, 2, 1L << 0, handle_keypress, cub);
	mlx_hook(cub->mlx.win, 3, 1L << 1, handle_keyrelease, cub);
	if (BONUS)
	{
		mlx_hook(cub->mlx.win, 6, 1L << 6, handle_mouse_move, cub);
	}

	mlx_hook(cub->mlx.win, 33, 0L, handle_close, cub);
	mlx_loop_hook(cub->mlx.mlx, game_loop, cub);
}