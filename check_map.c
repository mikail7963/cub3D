
#include "cub3D.h"

void check_map_line_start_end(char *line, int line_num)
{
	(void)line_num;
    int start = 0;
    int end = ft_strlen(line) - 1;

    while (line[start] == ' ' || line[start] == '\t')
        start++;
    while (end >= 0 && (line[end] == ' ' || line[end] == '\t' || line[end] == '\n'))
        end--;

    if (line[start] != '1' || line[end] != '1')
        error_msg("Error: Map line does not start and end with '1");
}

int get_num_lines(char **map)
{
    int count = 0;

    while (map[count] != NULL)
        count++;
    return count;
}

void check_map(t_cub *cub)
{
    int i = 0;
    int j;
    int num_lines = get_num_lines(cub->map.map);

    while (cub->map.map[i])
    {
        j = 0;
        if (i == 0 || i == num_lines - 1)
        {
            while (cub->map.map[i][j])
            {
                if (cub->map.map[i][j] != '1' && cub->map.map[i][j] != '\n' &&
                    cub->map.map[i][j] != ' ' && cub->map.map[i][j] != '\t')
                    error_msg("Error: Map line does not start and end with '1");
                j++;
            }
        }
        else
            check_map_line_start_end(cub->map.map[i], i);
        i++;
    }
}
