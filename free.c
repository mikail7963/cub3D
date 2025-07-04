/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atursun <atursun@student.42istanbul.com.tr +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:33:28 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/04 11:53:50 by atursun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	free_texture(t_texture texture)
{
	if (texture.north)
		free(texture.north);
	if (texture.east)
		free(texture.east);
	if (texture.south)
		free(texture.south);
	if (texture.west)
		free(texture.west);
}

void	free_map(t_cub *cub)
{
	int	i;

	i = 0;
	while (cub->map.map[i])
	{
		free(cub->map.map[i]);
		i++;
	}
	free(cub->map.map);
}
