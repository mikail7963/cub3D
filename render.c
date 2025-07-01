#include "cub3D.h"

void my_mlx_pixel_put(t_cub *cub, int x, int y, int color)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;

    char *dst;
    dst = cub->mlx.win_data;  // Window buffer kullan
    dst += (y * cub->mlx.win_size_line + x * (cub->mlx.win_bpp / 8));
    *(unsigned int *)dst = color;
}

void render_picture(t_cub *cub)
{
    cub->mlx.tex_image = mlx_xpm_file_to_image(cub->mlx.mlx, cub->texture.north, &cub->tex_data.tex_width, &cub->tex_data.tex_height);
    cub->tex_data.texture_data = mlx_get_data_addr(cub->mlx.tex_image, &cub->tex_data.bits_per_pixel, &cub->tex_data.size_line, &cub->tex_data.endian);
}

void render_map(t_cub *cub)
{
    render_picture(cub);
    
    // Window için image buffer oluştur
    cub->mlx.win_image = mlx_new_image(cub->mlx.mlx, WIDTH, HEIGHT);
    cub->mlx.win_data = mlx_get_data_addr(cub->mlx.win_image, 
        &cub->mlx.win_bpp, &cub->mlx.win_size_line, &cub->mlx.win_endian);
    
    for (int x = 0; x < WIDTH; x++) 
    {
        // Kamera düzlemi ve ışın yönü hesaplamaları
        double cameraX = 2 * x / (double)WIDTH - 1;
        double rayDirX = cub->player.dirx + cub->planeX * cameraX;
        double rayDirY = cub->player.diry + cub->planeY * cameraX;

        int mapX = (int)cub->player.posx;
        int mapY = (int)cub->player.posy;

        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);

        int stepX, stepY;
        double sideDistX, sideDistY;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (cub->player.posx - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - cub->player.posx) * deltaDistX;
        }

        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (cub->player.posy - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - cub->player.posy) * deltaDistY;
        }

        int hit = 0;
        int side;
        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (cub->map.map[mapY][mapX] == '1') hit = 1;
        }

        double perpWallDist;
        if (side == 0)
            perpWallDist = (mapX - cub->player.posx + (1 - stepX) / 2) / rayDirX;
        else
            perpWallDist = (mapY - cub->player.posy + (1 - stepY) / 2) / rayDirY;

        int lineHeight = (int)(HEIGHT / perpWallDist);
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0) 
            drawStart = 0;
        if (drawEnd >= HEIGHT) 
            drawEnd = HEIGHT - 1;

        double wallX;
        if (side == 0)
            wallX = cub->player.posy + perpWallDist * rayDirY;
        else
            wallX = cub->player.posx + perpWallDist * rayDirX;
        wallX -= floor(wallX);

        int tex_x = (int)(wallX * (double)cub->tex_data.tex_width);
        
        // X eksenini düzelt - bu satırları değiştir:
        if (side == 0 && rayDirX < 0) tex_x = cub->tex_data.tex_width - tex_x - 1;
        if (side == 1 && rayDirY > 0) tex_x = cub->tex_data.tex_width - tex_x - 1;

        double step = 1.0 * cub->tex_data.tex_height / lineHeight;
        double tex_pos = (drawStart - HEIGHT / 2 + lineHeight / 2) * step;

        for (int y = drawStart; y < drawEnd; y++)
        {
            int tex_y = (int)tex_pos & (cub->tex_data.tex_height - 1);
            tex_pos += step;
            int color = *(unsigned int *)(cub->tex_data.texture_data + (tex_y * cub->tex_data.size_line + tex_x * (cub->tex_data.bits_per_pixel / 8)));
            my_mlx_pixel_put(cub, x, y, color);
        }
    }
    // Window image'ı göster
    mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->mlx.win_image, 0, 0);
}
