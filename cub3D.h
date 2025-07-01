#ifndef CUB3D_H
#define CUB3D_H 


#include "libft/libft.h"
#include <fcntl.h>
#include "mlx/mlx.h"
#include <stdio.h>  // sil
#include <math.h>

#define WIDTH 800
#define HEIGHT 600

typedef struct s_texture
{
	char *north;
	char *south;
	char *west;
	char *east;
}   t_texture;

typedef struct s_rgb
{
	int r;
	int g;
	int b;
	unsigned int colour;
}   t_rgb;

typedef struct s_fc
{
	t_rgb floor_c;
	t_rgb ceiling_c;
}   t_fc;


typedef struct s_map
{
	char **map;

}   t_map;

typedef struct s_player 
{
	int dirx;
	int diry;
	double posx;
	double posy;
}   t_player;

typedef struct s_mlx
{
	void *mlx;
	void *win;
	void *tex_image;    // Texture için
	void *win_image;    // Window için
	char *win_data;     // Window buffer
	int win_bpp;
	int win_size_line;
	int win_endian;
}   t_mlx;

typedef struct s_tex_data
{
	char	*texture_data;
	int		bits_per_pixel;
	int		size_line;
	int		endian;
	int		tex_width;
	int		tex_height;
}	t_tex_data;


typedef struct s_cub
{
	int       len_of_file;
	double      planeX;
	double      planeY;
	int       is_player;
	int        map_index;
	t_texture texture;
	t_map      map;
	t_fc      fc;
	t_player   player;
	t_mlx       mlx;
	t_tex_data	tex_data;
}   t_cub;    

void read_texture(t_cub *cub, int fd);
int check_texture(t_cub *cub);
void open_file(t_cub *cub, char *file);
int check_extension(char *file, char *ext);
void	check_map(t_cub *cub);
void    render_map(t_cub *cub);
void	error_msg(char *msg);
void read_fc_rgb(t_cub *cub, int fd);


#endif