/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atursun <atursun@student.42istanbul.com.tr +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 13:37:13 by mikkayma          #+#    #+#             */
/*   Updated: 2025/07/03 15:57:39 by atursun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	check_rgb_line(char *line, t_cub *cub)
{
	int	i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (line[i] == 'F' || line[i] == 'C')
	{
		i++;
		while (line[i] == ' ' || line[i] == '\t')
			i++;
	}
	while (line[i])
	{
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		if (line[i] == ',')
		{
			i++;
			while (line[i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '\n')
				error_msg("rgb is not valid", cub, 2);
		}
		if (line[i] != ',' && line[i] != '\n')
		{
			if (!ft_isdigit(line[i]))
				error_msg("rgb is not valid", cub, 2);
		}
		i++;
	}
}

int len_of_double_tmp(char **double_ptr)
{
	int i;
	
	i = 0;
	while (double_ptr[i])
		i++;
	return (i);
}

void	rbg_line(t_rgb *rgb, char *line, t_cub *cub)
{
	char	*tmp;
	char	**double_tmp;

	check_rgb_line(line, cub);
	tmp = ft_strdup(line + 2);
	double_tmp = ft_split(tmp, ',');
	if (len_of_double_tmp(double_tmp) != 3)
		error_msg("rgb num is not valid1", cub, 2);
	if (double_tmp[0] && double_tmp[1] && double_tmp[2])
	{
		rgb->r = ft_atoi(double_tmp[0]);
		rgb->g = ft_atoi(double_tmp[1]);
		rgb->b = ft_atoi(double_tmp[2]);
		free(tmp);
		free(double_tmp[0]);
		free(double_tmp[1]);
		free(double_tmp[2]);
		free(double_tmp);
		if (!(rgb->r >= 0 && rgb->r <= 255) || !(rgb->g >= 0 && rgb->g <= 255)
			|| !(rgb->b >= 0 && rgb->b <= 255))
		{
			error_msg("RGB range value must be between 0 - 255", cub, 2);
			exit(EXIT_FAILURE);
		}
	}
}

unsigned int	rgb_to_colour(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}

void	read_fc_rgb(t_cub *cub, int fd)
{
	char	*line;
	int		i;
	int		j;

	i = 0;
	line = get_next_line(fd);
	while (line != NULL && i != 2)
	{
		j = 0;
		while (line[j] == ' ' || line[j] == '\t')
			j++;
		cub->map_index++;
		if (line[j] == 'F' && line[j + 1] == ' ')
		{
			rbg_line(&cub->fc.floor_c, line, cub);
			i++;
		}
		if (line[j] == 'C' && line[j + 1] == ' ')
		{
			rbg_line(&cub->fc.ceiling_c,line, cub);
			i++;
		}
		if (line)
			free(line);
		line = get_next_line(fd);
	}
	if (i != 2)
		error_msg("missing floor or ceiling color", cub, 2);
	cub->fc.ceiling_c.colour = rgb_to_colour(cub->fc.ceiling_c.r, cub->fc.ceiling_c.g, cub->fc.ceiling_c.b);
	cub->fc.floor_c.colour = rgb_to_colour(cub->fc.floor_c.r, cub->fc.floor_c.g, cub->fc.floor_c.b);
	close(fd);
}
