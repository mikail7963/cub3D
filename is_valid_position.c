/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_valid_position.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atursun <atursun@student.42istanbul.com.tr +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 13:32:22 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/10 12:48:26 by atursun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int	is_valid_position(t_cub *cub, double x, double y)
{
	int	map_x;
	int	map_y;
	int	map_height;
	int	map_width;

	map_x = (int)x;
	map_y = (int)y;
	
	// Harita boyutlarını hesapla
	map_height = 0;
	while (cub->map.map[map_height])
		map_height++;
	
	if (map_y >= map_height || map_y < 0)
		return (0);
		
	map_width = ft_strlen(cub->map.map[map_y]);
	
	if (map_x < 0 || map_x >= map_width)
		return (0);
	
	// Pozisyonun harita içinde geçerli bir hücre olup olmadığını kontrol eder.
	if (!cub->map.map[map_y] || !cub->map.map[map_y][map_x])
		return (0);
	
	// Sadece dış duvarları kontrol et (harita kenarlarındaki '1' karakterleri)
	// Üst kenar
	if (map_y == 0 && cub->map.map[map_y][map_x] == '1')
		return (0);
	// Alt kenar
	if (map_y == map_height - 1 && cub->map.map[map_y][map_x] == '1')
		return (0);
	// Sol kenar
	if (map_x == 0 && cub->map.map[map_y][map_x] == '1')
		return (0);
	// Sağ kenar
	if (map_x == map_width - 1 && cub->map.map[map_y][map_x] == '1')
		return (0);
	
	return (1);
}
