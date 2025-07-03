/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:33:28 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/03 19:16:15 by mikkayma         ###   ########.fr       */
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
