/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 14:21:53 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/09 13:48:38 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

void	free_door_sprite(t_cub *cub)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		mlx_destroy_image(cub->mlx.mlx, cub->doors_manager.frames[i].image);
		i++;
	}
}

int	find_true_door(t_cub *cub, int x, int y)
{
	int i;

	i = 0;
	while (i < cub->doors_manager.door_len)
	{
		if (cub->doors_manager.door[i].door_x == x && cub->doors_manager.door[i].door_y == y)
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

    check_x = (int)(cub->player.posx + cub->player.dirx * 1.5);
    check_y = (int)(cub->player.posy + cub->player.diry * 1.5);
    
    if (check_y < 0 || check_x < 0 || check_x >= (int)ft_strlen(cub->map.map[check_y]))
        return ;
    
    if (cub->map.map[check_y][check_x] == 'D')
    {
        i = find_true_door(cub, check_x, check_y);
        
        // ✅ Kapı kapalıysa aç
        if (cub->doors_manager.door[i].is_open == 0 && cub->doors_manager.door[i].is_opening == 0)
        {
            cub->doors_manager.door[i].is_opening = 1;
            cub->doors_manager.door[i].is_closing = 0;
            cub->doors_manager.door[i].time = get_time_ms();
            cub->doors_manager.door[i].door_frame = 1; // ✅ Frame 1'den başla
        }
        // ✅ Kapı açıksa kapat
        else if (cub->doors_manager.door[i].is_open == 1)
        {
            cub->doors_manager.door[i].is_closing = 1;
            cub->doors_manager.door[i].is_opening = 0;
            cub->doors_manager.door[i].door_frame = 7;
        }
    }	
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
	cub->doors_manager.door_len = door_lenght(cub);
	if (cub->doors_manager.door_len == 0)
		return ;
	cub->doors_manager.door = malloc(sizeof(t_door) * cub->doors_manager.door_len + 1);
	y = 0;
	while (cub->map.map[y])
	{
		x = 0;
		while (cub->map.map[y][x])
		{
			if (cub->map.map[y][x] == 'D')	// kapıyı bulduğunda o kapının koordinatlarını (x, y) diziye kaydeder
			{
				cub->doors_manager.door[i].door_x = x;
				cub->doors_manager.door[i].door_y = y;
				cub->doors_manager.door[i].is_open = 0;	// Başlangıçta Kapalı
				cub->doors_manager.door[i].is_opening = 0;
				cub->doors_manager.door[i].is_closing = 0;
				cub->doors_manager.door[i].door_frame = 0;
				i++;
			}
			x++;
		}
		y++;
	}
}

void doors_get_data_addres(t_tex_data *frames)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		frames[i].texture_data = mlx_get_data_addr(frames[i].image, &frames[i].bits_per_pixel , &frames[i].size_line, &frames[i].endian);
		i++;
	}
	
}

void	render_door(t_cub *cub)
{
	int			i;
	char		*door_files[8];
	t_tex_data	*frames;

	if (cub->doors_manager.door_len == 0)
		return;
	door_files[0] = "textures/door/Door0.xpm";
	door_files[1] = "textures/door/Door1.xpm";
	door_files[2] = "textures/door/Door2.xpm";
	door_files[3] = "textures/door/Door3.xpm";
	door_files[4] = "textures/door/Door4.xpm";
	door_files[5] = "textures/door/Door5.xpm";
	door_files[6] = "textures/door/Door6.xpm";
	door_files[7] = "textures/door/Door7.xpm";
	frames = cub->doors_manager.frames;
	i = 0;
	while (i < 8)
	{
		frames[i].image = mlx_xpm_file_to_image(cub->mlx.mlx, door_files[i],
			&frames[i].tex_width, &frames[i].tex_height);
			i++;
	}
	i = 0;
	while (i < 8)
	{
		if (!frames[i].image)
			error_msg("hatalı image", cub, 3);
		i++;
	}
	doors_get_data_addres(cub->doors_manager.frames);
}
