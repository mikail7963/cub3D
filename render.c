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

/*
 * Ana render fonksiyonu - Raycasting ile 3D sahne oluşturur
 * 1. Texture'ları yükler
 * 2. Her pixel sütunu için ray cast yapar
 * 3. Duvarları tespit eder ve texture ile renklendirir
 */
void render_map(t_cub *cub)
{
	// Texture dosyasını yükle
	int x;
	int y;

	x = 0;
	y = 0;

	// if (!(cub->move_backward || cub->move_forward || cub->move_left || cub->move_right || cub->rotate_left || cub->rotate_right))
	// 	return;
	int total_pixels = WIDTH * HEIGHT;
    int i = 0;
    while (i < total_pixels)
    {
        int x = i % WIDTH;
        int y = i / WIDTH;
        if (y < HEIGHT / 2)
            my_mlx_pixel_put(cub, x, y, cub->fc.ceiling_c.colour);
        else
            my_mlx_pixel_put(cub, x, y, cub->fc.floor_c.colour);
        i++;
    }
	// Ekranın her x koordinatı için ray cast yap
	while (x < WIDTH)
	{
		// ===================== RAY CASTING SETUP =====================
		// Kamera düzlemi ve ışın yönü hesaplamaları
		double cameraX = 2 * x / (double)WIDTH - 1; // -1 ile 1 arasında normalize et
		double rayDirX = cub->player.dirx + cub->plane_x * cameraX; // Işın X yönü
		double rayDirY = cub->player.diry + cub->plane_y * cameraX; // Işın Y yönü

		// Oyuncunun bulunduğu harita karesi
		int mapX = (int)cub->player.posx;
		int mapY = (int)cub->player.posy;

		// ===================== DDA ALGORİTMASI =====================
		// Her adımda ne kadar uzaklık artacağını hesapla
		double deltaDistX; 
		double deltaDistY;
		if (rayDirX != 0)
			deltaDistX = fabs(1 / rayDirX);
		else
		{
			deltaDistX =  1e30;
		}
		if (rayDirY != 0)
		deltaDistY = fabs(1 / rayDirY);
		else
		{
			deltaDistY = 1e30;
		}

		// Adım yönü ve ilk sınıra uzaklık hesaplaması
		int stepX, stepY;
		double sideDistX, sideDistY;

		// X yönünde adım hesaplaması
		if (rayDirX < 0) 
		{
			stepX = -1; // Sol tarafa git
			sideDistX = (cub->player.posx - mapX) * deltaDistX; // Mevcut kareden sol sınıra uzaklık
		} else 
		{
			stepX = 1; // Sağ tarafa git
			sideDistX = (mapX + 1.0 - cub->player.posx) * deltaDistX; // Mevcut kareden sağ sınıra uzaklık
		}

		// Y yönünde adım hesaplaması
		if (rayDirY < 0) {
			stepY = -1; // Yukarı git
			sideDistY = (cub->player.posy - mapY) * deltaDistY; // Mevcut kareden üst sınıra uzaklık
		} else {
			stepY = 1; // Aşağı git
			sideDistY = (mapY + 1.0 - cub->player.posy) * deltaDistY; // Mevcut kareden alt sınıra uzaklık
		}

		// ===================== DUVAR ARAMA DÖNGÜSÜ =====================
		// Duvar bulana kadar adım at
		int hit = 0; // Duvar bulundu mu?
		int side; // Hangi taraftan vuruldu (0=X taraf, 1=Y taraf)
		
		while (hit == 0)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX; // X yönünde bir adım daha
				mapX += stepX; // Grid pozisyonunu güncelle
				side = 0; // X tarafından çarpma
			} 
			else 
			{
				sideDistY += deltaDistY; // Y yönünde bir adım daha
				mapY += stepY; // Grid pozisyonunu güncelle
				side = 1; // Y tarafından çarpma
			}
			// Duvar var mı kontrol et
			if (cub->map.map[mapY][mapX] == '1') 
				hit = 1;
		}

		// ===================== DUVAR UZAKLIĞI HESAPLAMASI =====================
		// Perspektif düzeltmesi için duvara olan dik uzaklığı hesapla
		double perpWallDist;
		if (side == 0)
			perpWallDist = (mapX - cub->player.posx + (1 - stepX) / 2) / rayDirX;
		else
			perpWallDist = (mapY - cub->player.posy + (1 - stepY) / 2) / rayDirY;

		// ===================== DUVAR YÜKSEKLİĞİ HESAPLAMASI =====================
		// Uzaklığa göre ekrandaki duvar yüksekliğini hesapla
		int lineHeight = (int)(HEIGHT / perpWallDist);
		int drawStart = -lineHeight / 2 + HEIGHT / 2; // Duvarın üst kenarı
		int drawEnd = lineHeight / 2 + HEIGHT / 2; // Duvarın alt kenarı
		
		// Ekran sınırları içinde tut
		if (drawStart < 0) 
			drawStart = 0;
		if (drawEnd >= HEIGHT) 
			drawEnd = HEIGHT - 1;

		// ===================== TEXTURE MAPPING =====================
		// Duvardaki texture koordinatlarını hesapla
		double wallX; // Duvarın hangi noktasında çarpıldığını hesapla
		if (side == 0)
			wallX = cub->player.posy + perpWallDist * rayDirY;
		else
			wallX = cub->player.posx + perpWallDist * rayDirX;
		wallX -= floor(wallX); // Sadece ondalık kısmı al (0-1 arası)

		// Texture'daki X koordinatını hesapla
		
		t_tex_data *selected_texture;
		if (side == 0 && rayDirX > 0)
			selected_texture = &cub->east; // Doğu yönü
		else if (side == 0 && rayDirX < 0)
			selected_texture = &cub->west; // Batı yönü
		else if (side == 1 && rayDirY > 0)
			selected_texture = &cub->south; // Güney yönü
		else
			selected_texture = &cub->north; // Kuzey yönü
		// Texture'ın yönünü düzelt (ters görünmeyi engelle)
		int tex_x = (int)(wallX * (double)selected_texture->tex_width);
		if (side == 0 && rayDirX < 0) 
			tex_x = selected_texture->tex_width - tex_x - 1;
		if (side == 1 && rayDirY > 0) 
			tex_x = selected_texture->tex_width - tex_x - 1;

		// Texture'ın Y koordinatı için adım miktarını hesapla
		double step = 1.0 * selected_texture->tex_height / lineHeight;
		double tex_pos = (drawStart - HEIGHT / 2 + lineHeight / 2) * step;

		// ===================== TEXTURE SEÇİMİ =====================
        // Duvarın hangi yöne çarptığını kontrol et ve doğru texture'ı seç
		if (!selected_texture || !selected_texture->texture_data)
		{
			error_msg("Geçersiz texture seçimi!", cub, 1);
			return;
		}
			
		// ===================== TEXTURE ÇİZİMİ =====================
		y = drawStart;
		while (y < drawEnd)
		{
			// Texture'ın Y koordinatını hesapla
			int tex_y = (int)tex_pos & (selected_texture->tex_height - 1);
			tex_pos += step; // Bir sonraki pixel için Y koordinatını artır
			
			// Texture'dan renk değerini al
            int color = *(unsigned int *)(selected_texture->texture_data + 
                (tex_y * selected_texture->size_line + tex_x * (selected_texture->bits_per_pixel / 8)));
			
			// Pixel'i ekrana çiz
			my_mlx_pixel_put(cub, x, y, color);
			y++;
		}
		x++;
	}
	// ===================== EKRANA GÖSTER =====================
	// Tamamlanan image'ı window'a yerleştir
	mlx_put_image_to_window(cub->mlx.mlx, cub->mlx.win, cub->mlx.win_data.image, 0, 0);

}
