#include "cub3D.h"

int check_extension(char *file, char *ext)
{
	char *res;

	res = ft_strrchr(file, '.');
	if (!res || ft_strncmp(res, ext, 4) != 0 || ft_strlen(file) <= 4)
		return (1);
	return (0);
}

void read_texture(t_cub *cub, int fd)
{
	char *line = get_next_line(fd);
	int i = 0;
	while (line != NULL && i != 4)
	{
		if (line[0] == 'N' && line[1] == 'O')
		{
			cub->texture.north = ft_strtrim(line, "NO ");
			i++;
		}
		if (line[0] == 'S' && line[1] == 'O')
		{
			cub->texture.south = ft_strtrim(line, "SO ");
			i++;
		}
		if (line[0] == 'W' && line[1] == 'E')
		{
			cub->texture.west = ft_strtrim(line, "WE ");
			i++;
		}
		if (line[0] == 'E' && line[1] == 'A')
		{
			cub->texture.east = ft_strtrim(line, "EA ");
			i++;
		} 
		free(line);
		line = get_next_line(fd);
		cub->map_index++;
	}
}

char *skip_spaces(char *str)
{
    while (*str && (*str == ' ' || *str == '\t'))
        str++;
    return str;
}


void read_fc_rgb(t_cub *cub, int fd)
{
    char *line = get_next_line(fd);
    char *tmp;
    char **double_tmp;
	int i = 0;
    while (line != NULL && i != 2)
    {
		cub->map_index++;
        char *trimmed = skip_spaces(line);
        if (trimmed[0] == 'F' && trimmed[1] == ' ')
        {
            tmp = ft_strdup(trimmed + 2);
            double_tmp = ft_split(tmp, ',');
            if (double_tmp[0] && double_tmp[1] && double_tmp[2]) 
			{
                cub->fc.floor_c.r = ft_atoi(double_tmp[0]);
                cub->fc.floor_c.g = ft_atoi(double_tmp[1]);
                cub->fc.floor_c.b = ft_atoi(double_tmp[2]);
            }
            free(tmp);
            free(double_tmp[0]);
            free(double_tmp[1]);
            free(double_tmp[2]);
            free(double_tmp);
			i++;
        }
        else if (trimmed[0] == 'C' && trimmed[1] == ' ')
        {
            tmp = ft_strdup(trimmed + 2);
            double_tmp = ft_split(tmp, ',');
            if (double_tmp[0] && double_tmp[1] && double_tmp[2]) {
                cub->fc.ceiling_c.r = ft_atoi(double_tmp[0]);
                cub->fc.ceiling_c.g = ft_atoi(double_tmp[1]);
                cub->fc.ceiling_c.b = ft_atoi(double_tmp[2]);
				i++;
            }
            free(tmp);
            free(double_tmp[0]);
            free(double_tmp[1]);
            free(double_tmp[2]);
            free(double_tmp);
        }
        free(line);
        line = get_next_line(fd);
    }
}

void read_map(t_cub *cub, int fd)
{
	char *tmp;
	int		i;
	i = 0;
	int map_start_i = cub->len_of_file - cub->map_index;
	cub->map.map = malloc(sizeof(char *) * map_start_i);
	tmp = get_next_line(fd);
	while (tmp != NULL)
	{
		if (ft_strchr(tmp, 'N') || ft_strchr(tmp, 'S') ||
			ft_strchr(tmp, 'W') ||	ft_strchr(tmp, 'E'))
			cub->is_player = 1;
		cub->map.map[i++] = ft_strdup(tmp);
		free(tmp);
		tmp = get_next_line(fd);
	}
	close(fd);
}


void read_file(t_cub *cub, char *file)
{
    int fd = open(file, O_RDONLY);
	read_texture(cub, fd);
	read_fc_rgb(cub, fd);
	read_map(cub, fd);
}

void check_texture(t_cub *cub)
{
	if (check_extension(cub->texture.east, ".xpm"))
		return (1);
	if (check_extension(cub->texture.north, ".xpm"))
		return (1);
	if (check_extension(cub->texture.west, ".xpm"))
		return (1);
	if (check_extension(cub->texture.south, ".xpm"))
		return (1);

	int fd = open(cub->texture.east, O_RDONLY);
	if (fd != -1)
		return ;
	close(fd);
}

void open_file(t_cub *cub, char *file)
{
	int counter = 0;
	int fd = open(file, O_RDONLY);
	if (fd == -1)
		return ;
	
	char *line = get_next_line(fd);
	while (line != NULL)
	{
		counter++;
		free(line);
		line = get_next_line(fd);
	}
	cub->len_of_file = counter;
	close(fd);
	read_file(cub, file);
	check_texture(cub);
}

int main(int argc, char **argv)
{
	t_cub *cub = malloc(sizeof(t_cub));
	cub->map_index = 0;
	if (argc != 2 || check_extension(argv[1], ".cub") == 1)
		return (0);

	open_file(cub, argv[1]);
	
	void *ptr = mlx_init();
	void *win = mlx_new_window(ptr, 850, 850, "Cub3D");

	int x;
	int y;
	(void)win;
	mlx_loop(ptr);
}
