/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:21:53 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/07 18:15:29 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

int	find_true_door(t_cub *cub, int x, int y)
{
	int i;

	i = 0;
	while (i < cub->door_len)
	{
		if (cub->door[i].door_x == x && cub->door[i].door_y == y)
			return (i);
		i++;
	}
	return (-1);
}

/*
Oyuncunun baktığı yönde kapı olup olmadığını kontrol eder ve varsa kapıyı açar/kapatır.
*/
void	handle_door(t_cub *cub)
{
	int i;
	int check_x;
	int	check_y;

	i = 0;		// Bu, oyuncunun önündeki yaklaşık 1.5 birim mesafedeki noktayı verir
	check_x = (int)(cub->player.posx + cub->player.dirx * 1.5);
	check_y = (int)(cub->player.posy + cub->player.diry * 1.5);
	if (check_y < 0 || check_x < 0 || check_x >= (int)ft_strlen(cub->map.map[check_y]))
	{
		i++;
		return ;
	}
	if (cub->map.map[check_y][check_x] == 'D')
	{
		i = find_true_door(cub, check_x, check_y);
		ft_putendl_fd("kapı var",1);
		if (cub->door[i].is_open == 0)
		{
			ft_putendl_fd("kapı açıldı",1);
			cub->door[i].is_open = 1;
		}
		else
		{
			ft_putendl_fd("kapı kapandı",1);
			cub->door[i].is_open = 0;
		}
		return ;
	}	
	ft_putendl_fd("kapı yok",1);
}

int	door_lenght(t_cub *cub)
{
	int x;	
	int y;
	int door_len;

	y = 0;
	door_len = 0;
	while (cub->map.map[y])
	{
		x = 0;
		while (cub->map.map[y][x])
		{
			if (cub->map.map[y][x] == 'D')
				door_len++;
			x++;
		}
		y++;
	}
	return (door_len);
}

void init_door(t_cub *cub)
{
	int x;	
	int y;
	int	i;

	i = 0;
	cub->door_len = door_lenght(cub);
	cub->door = malloc(sizeof(t_door) * cub->door_len + 1);
	y = 0;
	while (cub->map.map[y])
	{
		x = 0;
		while (cub->map.map[y][x])
		{
			if (cub->map.map[y][x] == 'D')	// kapıyı bulduğunda o kapının koordinatlarını (x, y) diziye kaydeder
			{
				cub->door[i].door_x = x;
				cub->door[i].door_y = y;
				cub->door[i].is_open = 0;	// Başlangıçta Kapalı
				i++;
			}
			x++;
		}
		y++;
	}
}
