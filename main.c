#include "cub3D.h"

void	error_msg(char *msg)
{
	ft_putendl_fd(msg, 1);
	exit(EXIT_FAILURE);
}

int check_extension(char *file, char *ext)
{
	char *res;

	res = ft_strrchr(file, '.');
	if (!res || ft_strncmp(res, ext, 4) != 0 || ft_strlen(file) <= 4)
		return (1);
	return (0);
}

void print_map(t_cub *cub)
{
	ft_putendl_fd(cub->texture.north, 1);
	ft_putendl_fd(cub->texture.east, 1);
	ft_putendl_fd(cub->texture.south, 1);
	ft_putendl_fd(cub->texture.west, 1);

	printf("%d %d %d\n", cub->fc.ceiling_c.r, cub->fc.ceiling_c.g, cub->fc.ceiling_c.b);
	printf("%d %d %d\n", cub->fc.floor_c.r, cub->fc.floor_c.g, cub->fc.floor_c.b);

	int i = 0;
	while (cub->map.map[i])
		ft_putstr_fd(cub->map.map[i++],1);
}

void	init_cub(t_cub *cub, char *file)
{
	(void)file;
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
	cub->planeX = 0.66;
	cub->planeY = 0;
}

int main(int argc, char **argv)
{
	t_cub *cub;
	
	if (argc != 2 || check_extension(argv[1], ".cub") == 1)
		return (0);
	cub = malloc(sizeof(t_cub));
	init_cub(cub, argv[1]);
	open_file(cub, argv[1]);
	print_map(cub);

	cub->mlx.mlx = mlx_init();
	cub->mlx.win = mlx_new_window(cub->mlx.mlx, WIDTH, HEIGHT, "Cub3D");
	render_map(cub);
	mlx_loop(cub->mlx.mlx);
}
