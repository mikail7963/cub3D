/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atursun <atursun@student.42istanbul.com.tr +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:52:05 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/04 17:00:30 by atursun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H 

# include "libft/libft.h"
# include <fcntl.h>
# include "mlx/mlx.h"
# include <math.h>

# define WIDTH 800
# define HEIGHT 600
# define MOVE_SPEED 0.02
# define ROT_SPEED 0.02

# define KEY_W 119
# define KEY_A 97 
# define KEY_S 115
# define KEY_D 100
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_ESC 65307

typedef struct s_texture
{
	char	*north;
	char	*south;
	char	*west;
	char	*east;
}	t_texture;

typedef struct s_rgb
{
	int				r;
	int				g;
	int				b;
	unsigned int	colour;
}	t_rgb;

typedef struct s_fc
{
	t_rgb	floor_c;
	t_rgb	ceiling_c;
}	t_fc;

typedef struct s_map
{
	char	**map;
}	t_map;

typedef struct s_player
{
	double	dirx;
	double	diry;
	double	posx;
	double	posy;
}	t_player;

typedef struct s_tex_data
{
	void	*image;
	char	*texture_data;
	int		bits_per_pixel;
	int		size_line;
	int		endian;
	int		tex_width;
	int		tex_height;
}	t_tex_data;

typedef struct s_mlx
{
	void		*mlx;
	void		*win;
	void		*tex_image;
	t_tex_data	win_data;
}	t_mlx;

typedef struct s_render
{
    double rayDirX;
    double rayDirY;
    int mapX;
    int mapY;
    double deltaDistX;
    double deltaDistY;
    double sideDistX;
    double sideDistY;
    int stepX;
    int stepY;
    int hit;
    int side;
    double perpWallDist;
    int lineHeight;
    int drawStart;
    int drawEnd;
    t_tex_data *selected_texture;
}	t_render;

typedef struct s_cub
{
	int			len_of_file;
	double		plane_x;
	double		plane_y;
	int			is_player;
	int			map_index;
	t_texture	texture;
	t_map		map;
	t_fc		fc;
	t_player	player;
	t_mlx		mlx;
	t_tex_data	tex_data;
	t_tex_data	north;
	t_tex_data	west;
	t_tex_data	south;
	t_tex_data	east;
	int			move_forward;
	int			move_backward;
	int			move_left;
	int			move_right;
	int			rotate_left;
	int			rotate_right;
}	t_cub;

void	check_map_around_wall(t_cub *cub);
void	validate_map_line(char *line, t_cub *cub);
void	set_coor_and_pos(t_cub *cub, char *line, int i);
int		map_reel_lenght(char *file, t_cub *cub);
int		skip_whitespaces(char *line, int i);
int		len_of_double_tmp(char **double_ptr);
int		is_valid_position(t_cub *cub, double x, double y);
void	read_texture(t_cub *cub, int fd, int i, int j);
int		check_texture(t_cub *cub, int i);
void	open_file(t_cub *cub, char *file);
int		check_extension(char *file, char *ext);
void	check_map(t_cub *cub);
void	render_map(t_cub *cub);
void	read_fc_rgb(t_cub *cub, int fd);
void	error_msg(char *msg, t_cub *cub, int is_free);
void	free_texture(t_texture texture);
void	free_map(t_cub *cub);
int		game_loop(t_cub *cub);
void	init_movement_state(t_cub *cub);
void	setup_hooks(t_cub *cub);
void	move_player(t_cub *cub);
int		handle_keypress(int key, t_cub *cub);
int		handle_keyrelease(int key, t_cub *cub);
int		handle_close(t_cub *cub);
void	render_picture(t_cub *cub);
void	rotate_player(t_cub *cub, double angle);

#endif