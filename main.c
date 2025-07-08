/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikkayma <mikkayma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 11:33:37 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/08 18:44:18 by mikkayma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	error_msg(char *msg, t_cub *cub, int is_free)
{
	ft_putendl_fd("Error", 2);
	ft_putendl_fd(msg, 2);
	if (is_free >= 3)
		free_map(cub->map.map);
	if (is_free >= 2)
		free_texture(cub->texture);
	if (is_free >= 1)
		free(cub);
	exit(EXIT_FAILURE);
}

int	check_extension(char *file, char *ext)
{
	char	*res;

	res = ft_strrchr(file, '.');
	if (!res || ft_strncmp(res, ext, 4) != 0 || ft_strlen(file) <= 4)
		return (1);
	return (0);
}

void	init_cub(t_cub *cub)
{
	cub->texture.north = NULL;
	cub->texture.east = NULL;
	cub->texture.west = NULL;
	cub->texture.south = NULL;
	cub->map_index = 0;
	cub->len_of_file = 0;
	cub->is_player = 0;
	cub->fc.ceiling_c.r = -1;
	cub->fc.ceiling_c.g = -1;
	cub->fc.ceiling_c.b = -1;
	cub->fc.floor_c.r = -1;
	cub->fc.floor_c.g = -1;
	cub->fc.floor_c.b = -1;
	cub->player.dirx = 0;
	cub->player.diry = 0;
	cub->plane_x = 0;
	cub->plane_y = 0;
	cub->tex_data.bits_per_pixel = 0;
	cub->tex_data.endian = 0;
	cub->tex_data.size_line = 0;
	cub->tex_data.tex_height = 0;
	cub->tex_data.tex_width = 0;
	cub->tex_data.texture_data = 0;
	cub->map.map = NULL;
	init_movement_state(cub);
}

int	main(int argc, char **argv)
{
	t_cub	*cub;

	if (argc != 2)
		error_msg("too many arguments", NULL, 0);
	if (check_extension(argv[1], ".cub") == 1)
		error_msg("wrong map extansion", NULL, 0);
	cub = malloc(sizeof(t_cub));
	init_cub(cub);
	open_file(cub, argv[1]);
	if (BONUS)
		init_door(cub);
	cub->mlx.mlx = mlx_init();
	cub->mlx.win = mlx_new_window(cub->mlx.mlx, WIDTH, HEIGHT, "Cub3D");
	render_picture(cub);
	if (BONUS)
	{
		render_door(cub);
		cub->minimap.mini_image = mlx_new_image(cub->mlx.mlx, MINIMAP_WIDTH, MINIMAP_HEIGHT);
		cub->minimap.data = mlx_get_data_addr(cub->minimap.mini_image, &cub->minimap.bits_per_pixel, &cub->minimap.size_line, &cub->minimap.endian);
	}
	render_map(cub);
	setup_hooks(cub);
	mlx_loop(cub->mlx.mlx);
}
