#include "cub3D.h"

int handle_keypress(int key, t_cub *cub)
{
    if (key == KEY_ESC)
        handle_close(cub);
    if (key == KEY_W)
        cub->move_forward = 1;
    if (key == KEY_S)
        cub->move_backward = 1;
    if (key == KEY_A)
        cub->move_left = 1;
    if (key == KEY_D)
        cub->move_right = 1;
    if (key == KEY_LEFT)
        cub->rotate_left = 1;
    if (key == KEY_RIGHT)
        cub->rotate_right = 1;
    return (0);
}

int handle_keyrelease(int key, t_cub *cub)
{
    if (key == KEY_W)
        cub->move_forward = 0;
    if (key == KEY_S)
        cub->move_backward = 0;
    if (key == KEY_A)
        cub->move_left = 0;
    if (key == KEY_D)
        cub->move_right = 0;
    if (key == KEY_LEFT)
        cub->rotate_left = 0;
    if (key == KEY_RIGHT)
        cub->rotate_right = 0;
    return (0);
}

int handle_close(t_cub *cub)
{
    if (cub->mlx.win_image)
        mlx_destroy_image(cub->mlx.mlx, cub->mlx.win_image);
    if (cub->mlx.tex_image)
        mlx_destroy_image(cub->mlx.mlx, cub->mlx.tex_image);
    if (cub->mlx.win)
        mlx_destroy_window(cub->mlx.mlx, cub->mlx.win);
    if (cub->mlx.mlx)
        mlx_destroy_display(cub->mlx.mlx);
    exit(0);
}