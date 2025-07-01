
#include "cub3D.h"


void    check_rgb_line(char *line)
{
    int i;
    int counter;

    i = 0;
    while (line[i] == ' ')
            i++;
    if (line[i] == 'F' || line[i] == 'C')
    {
        while (line[++i] == ' ')
            i++;
    }
    counter = 0;
    while (line[i])
    {
        if (line[i] == ',' && line[i + 1] != ',')
            counter++;
        if (line[i] != ',' && line[i] != '\n')
        {
            if (!ft_isdigit(line[i]))
                error_msg("Error: rgb is not valid");
        }
        i++;
    }
    if (++counter != 3)
        error_msg("Error: rgb num is not valid");
}

void rbg_line(t_rgb *rgb, char *line)
{
    char *tmp;
    char **double_tmp;

    tmp = ft_strdup(line + 2);
    double_tmp = ft_split(tmp, ',');
    check_rgb_line(line);
    if (double_tmp[0] && double_tmp[1] && double_tmp[2])
	{
        rgb->r = ft_atoi(double_tmp[0]);
        rgb->g = ft_atoi(double_tmp[1]);
        rgb->b = ft_atoi(double_tmp[2]);
        if (!(rgb->r >= 0 && rgb->r <= 255) || !(rgb->g >= 0 && rgb->g <= 255) ||
            !(rgb->b >= 0 && rgb->b <= 255))
        {
            ft_putendl_fd("Error: RGB range value must be between 0 - 255", 1);
            exit(EXIT_FAILURE);
        }
    }
    free(tmp);
    free(double_tmp[0]);
    free(double_tmp[1]);
    free(double_tmp[2]);
    free(double_tmp);
}

unsigned int rgb_to_colour(int r, int g, int b)
{
    return ((r << 16) | (g << 8) | b);
}

void read_fc_rgb(t_cub *cub, int fd)
{
    char    *line;
    int     i;
    
	i = 0;
    line = get_next_line(fd);
    while (line != NULL && i != 2)
    {
		cub->map_index++;
        if (line[0] == 'F' && line[1] == ' ')
        {
            rbg_line(&cub->fc.floor_c, line);
            i++;
        }
        if (line[0] == 'C' && line[1] == ' ')
        {
            rbg_line(&cub->fc.ceiling_c,line);
            i++;
        }
        if (line)
            free(line);
        line = get_next_line(fd);
    }
    cub->fc.ceiling_c.colour = rgb_to_colour(cub->fc.ceiling_c.r, cub->fc.ceiling_c.g, cub->fc.ceiling_c.b);
    cub->fc.floor_c.colour = rgb_to_colour(cub->fc.floor_c.r, cub->fc.floor_c.g, cub->fc.floor_c.b);
}
