#include "cub3D.h"

int is_valid_position(t_cub *cub, double x, double y)
{
	int map_x = (int)x;
	int map_y = (int)y;
	
	// Check boundaries
	if (map_x < 0 || map_y < 0)
		return (0);
	// pozisyonun map icinde olup olmadigini kontrol etme
	if (!cub->map.map[map_y] || !cub->map.map[map_y][map_x])
		return (0);
	
	// pozisyon duvar degilse
	if (cub->map.map[map_y][map_x] == '1')
		return (0);
	
	return (1);
}

// bu formul zaten
void rotate_player(t_cub *cub, double angle)
{
	double old_dirx;
	double old_planex;
	
	old_planex = cub->plane_x;
	old_dirx = cub->player.dirx;
	cub->player.dirx = cub->player.dirx * cos(angle) - cub->player.diry * sin(angle);
	cub->player.diry = old_dirx * sin(angle) + cub->player.diry * cos(angle);
	
	cub->plane_x = cub->plane_x * cos(angle) - cub->plane_y * sin(angle);
	cub->plane_y = old_planex * sin(angle) + cub->plane_y * cos(angle);
}

// mikkayma foksiyonu 4 e ayirisin her bir hareketi bir fonksion yap
void move_player(t_cub *cub)
{
	double new_x;
	double new_y;
	
	// ileri hareket etme
	if (cub->move_forward)
	{
		new_x = cub->player.posx + cub->player.dirx * MOVE_SPEED;
		new_y = cub->player.posy + cub->player.diry * MOVE_SPEED;
		if (is_valid_position(cub, new_x, cub->player.posy))
			cub->player.posx = new_x;
		if (is_valid_position(cub, cub->player.posx, new_y))
			cub->player.posy = new_y;
	}
	
	// geri hareket etme
	if (cub->move_backward)
	{
		new_x = cub->player.posx - cub->player.dirx * MOVE_SPEED;
		new_y = cub->player.posy - cub->player.diry * MOVE_SPEED;
		if (is_valid_position(cub, new_x, cub->player.posy))
			cub->player.posx = new_x;
		if (is_valid_position(cub, cub->player.posx, new_y))
			cub->player.posy = new_y;
	}
	
	// sola hareket etme
	if (cub->move_left)
	{
		new_x = cub->player.posx + cub->player.diry * MOVE_SPEED;
		new_y = cub->player.posy - cub->player.dirx * MOVE_SPEED;
		if (is_valid_position(cub, new_x, cub->player.posy))
			cub->player.posx = new_x;
		if (is_valid_position(cub, cub->player.posx, new_y))
			cub->player.posy = new_y;
	}
	
	// sag hareket etme
	if (cub->move_right)
	{
		new_x = cub->player.posx - cub->player.diry * MOVE_SPEED;
		new_y = cub->player.posy + cub->player.dirx * MOVE_SPEED;
		if (is_valid_position(cub, new_x, cub->player.posy))
			cub->player.posx = new_x;
		if (is_valid_position(cub, cub->player.posx, new_y))
			cub->player.posy = new_y;
	}
	
	// Rotation - dondurme
	if (cub->rotate_left)
		rotate_player(cub, -ROT_SPEED);
	if (cub->rotate_right)
		rotate_player(cub, ROT_SPEED);
}
