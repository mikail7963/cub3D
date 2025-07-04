#include "cub3D.h"

/* 
 * Window buffer'ına pixel yerleştiren fonksiyon
 * Manuel pixel çizimi için kullanılır
 */
void my_mlx_pixel_put(t_cub *cub, int x, int y, int color)
{
	// Koordinatların ekran sınırları içinde olup olmadığını kontrol et
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return;

	char *dst;
	dst = cub->mlx.win_data.texture_data;  // Window buffer'ına erişim
	// Pixel'in buffer'daki adresini hesapla (y * satır genişliği + x * pixel boyutu)
	dst += (y * cub->mlx.win_data.size_line + x * (cub->mlx.win_data.bits_per_pixel / 8));
	// Renk değerini buffer'a yaz
	*(unsigned int *)dst = color;
}

/*
 * Texture dosyasını yükleyen fonksiyon
 * XPM formatındaki texture'ı memory'e yükler
 */
void render_picture(t_cub *cub)
{
    // Her yön için texture dosyasını yükle
    cub->north.image = mlx_xpm_file_to_image(cub->mlx.mlx, cub->texture.north, &cub->north.tex_width, &cub->north.tex_height);
	if (!cub->north.image)
	{
    	error_msg("Texture yüklenemedi: North", cub, 1);
	}

    cub->north.texture_data = mlx_get_data_addr(cub->north.image, &cub->north.bits_per_pixel, &cub->north.size_line, &cub->north.endian);

    cub->south.image= mlx_xpm_file_to_image(cub->mlx.mlx, cub->texture.south, &cub->south.tex_width, &cub->south.tex_height);
	if (!cub->south.image)
    	error_msg("Texture yüklenemedi: south", cub, 1);

    cub->south.texture_data= mlx_get_data_addr(cub->south.image, &cub->south.bits_per_pixel , &cub->south.size_line, &cub->south.endian);

    cub->east.image= mlx_xpm_file_to_image(cub->mlx.mlx, cub->texture.east, &cub->east.tex_width, &cub->east.tex_height);
    if (!cub->east.image)
	{
    	error_msg("Texture yüklenemedi: east", cub, 1);
	}

	cub->east.texture_data= mlx_get_data_addr(cub->east.image, &cub->east.bits_per_pixel, &cub->east.size_line, &cub->east.endian);

    
	cub->west.image= mlx_xpm_file_to_image(cub->mlx.mlx, cub->texture.west, &cub->west.tex_width, &cub->west.tex_height);
	
    if (!cub->west.image)
	{
    	error_msg("Texture yüklenemedi: West", cub, 1);
	}

	cub->west.texture_data= mlx_get_data_addr(cub->west.image, &cub->west.bits_per_pixel, &cub->west.size_line, &cub->west.endian);

    // Window image oluştur
    
	cub->mlx.win_data.image = mlx_new_image(cub->mlx.mlx, WIDTH, HEIGHT);
	cub->mlx.win_data.texture_data= mlx_get_data_addr(cub->mlx.win_data.image, &cub->mlx.win_data.bits_per_pixel, &cub->mlx.win_data.size_line, &cub->mlx.win_data.endian);
}

void	painting_sky_and_ground(t_cub *cub)
{
	int	total_pixels;
	int	i;
	int x;
	int y;

    i = 0;
	total_pixels = WIDTH * HEIGHT;
    while (i < total_pixels)
    {
        x = i % WIDTH;
        y = i / WIDTH;
        if (y < HEIGHT / 2)
            my_mlx_pixel_put(cub, x, y, cub->fc.ceiling_c.colour);
        else
            my_mlx_pixel_put(cub, x, y, cub->fc.floor_c.colour);
        i++;
    }
}

// ===================== RAY CASTING SETUP =====================
void setup_ray(t_cub *cub, t_render *render, int x)
{
    render->rayDirX = cub->player.dirx + cub->plane_x * (2 * x / (double)WIDTH - 1);
    render->rayDirY = cub->player.diry + cub->plane_y * (2 * x / (double)WIDTH - 1);
    render->mapX = (int)cub->player.posx;
    render->mapY = (int)cub->player.posy;
    render->deltaDistX = (render->rayDirX != 0) ? fabs(1 / render->rayDirX) : 1e30;
    render->deltaDistY = (render->rayDirY != 0) ? fabs(1 / render->rayDirY) : 1e30;
    render->hit = 0;
}

// ===================== STEP AND SIDE DISTANCE =====================
void calculate_step_and_side_dist(t_cub *cub, t_render *render)
{
    if (render->rayDirX < 0)
    {
        render->stepX = -1;
        render->sideDistX = (cub->player.posx - render->mapX) * render->deltaDistX;
    }
    else
    {
        render->stepX = 1;
        render->sideDistX = (render->mapX + 1.0 - cub->player.posx) * render->deltaDistX;
    }
    if (render->rayDirY < 0)
    {
        render->stepY = -1;
        render->sideDistY = (cub->player.posy - render->mapY) * render->deltaDistY;
    }
    else
    {
        render->stepY = 1;
        render->sideDistY = (render->mapY + 1.0 - cub->player.posy) * render->deltaDistY;
    }
}

// ===================== DDA ALGORITHM =====================
void perform_dda(t_cub *cub, t_render *render)
{
    while (render->hit == 0)
    {
        if (render->sideDistX < render->sideDistY)
        {
            render->sideDistX += render->deltaDistX;
            render->mapX += render->stepX;
            render->side = 0;
        }
        else
        {
            render->sideDistY += render->deltaDistY;
            render->mapY += render->stepY;
            render->side = 1;
        }
        if (cub->map.map[render->mapY][render->mapX] == '1')
            render->hit = 1;
    }
}

// ===================== WALL DISTANCE AND HEIGHT =====================
void calculate_wall_distance_and_height(t_cub *cub, t_render *render)
{
    if (render->side == 0)
        render->perpWallDist = (render->mapX - cub->player.posx + (1 - render->stepX) / 2) / render->rayDirX;
    else
        render->perpWallDist = (render->mapY - cub->player.posy + (1 - render->stepY) / 2) / render->rayDirY;

    render->lineHeight = (int)(HEIGHT / render->perpWallDist);
    render->drawStart = -render->lineHeight / 2 + HEIGHT / 2;
    render->drawEnd = render->lineHeight / 2 + HEIGHT / 2;

    if (render->drawStart < 0)
        render->drawStart = 0;
    if (render->drawEnd >= HEIGHT)
        render->drawEnd = HEIGHT - 1;
}

// ===================== TEXTURE SELECTION =====================
void select_texture(t_cub *cub, t_render *render)
{
    if (render->side == 0 && render->rayDirX > 0)
        render->selected_texture = &cub->east;
    else if (render->side == 0 && render->rayDirX < 0)
        render->selected_texture = &cub->west;
    else if (render->side == 1 && render->rayDirY > 0)
        render->selected_texture = &cub->south;
    else
        render->selected_texture = &cub->north;
}

// ===================== TEXTURE DRAWING =====================
void draw_texture(t_cub *cub, t_render *render, int x)
{
    double wallX = (render->side == 0) ? cub->player.posy + render->perpWallDist * render->rayDirY
                                       : cub->player.posx + render->perpWallDist * render->rayDirX;
    wallX -= floor(wallX);

    int tex_x = (int)(wallX * (double)render->selected_texture->tex_width);
    if (render->side == 0 && render->rayDirX < 0)
        tex_x = render->selected_texture->tex_width - tex_x - 1;
    if (render->side == 1 && render->rayDirY > 0)
        tex_x = render->selected_texture->tex_width - tex_x - 1;

    double step = 1.0 * render->selected_texture->tex_height / render->lineHeight;
    double tex_pos = (render->drawStart - HEIGHT / 2 + render->lineHeight / 2) * step;

    int y = render->drawStart;
    while (y < render->drawEnd)
    {
        int tex_y = (int)tex_pos & (render->selected_texture->tex_height - 1);
        tex_pos += step;

        int color = *(unsigned int *)(render->selected_texture->texture_data +
                                      (tex_y * render->selected_texture->size_line + tex_x * (render->selected_texture->bits_per_pixel / 8)));
        my_mlx_pixel_put(cub, x, y, color);
        y++;
    }
}

// ===================== MAIN RENDER FUNCTION =====================
void render_map(t_cub *cub)
{
    int x = 0;
    t_render render;

    painting_sky_and_ground(cub);

    while (x < WIDTH)
    {
        setup_ray(cub, &render, x);
        calculate_step_and_side_dist(cub, &render);
        perform_dda(cub, &render);
        calculate_wall_distance_and_height(cub, &render);
        select_texture(cub, &render);
        draw_texture(cub, &render, x);
        x++;
    }
	// ===================== EKRANA GÖSTER =====================
	// Tamamlanan image'ı window'a yerleştir
	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->mlx.win_data.image, 0, 0);

}
