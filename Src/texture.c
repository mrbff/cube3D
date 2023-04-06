/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbozzi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 16:31:00 by mbozzi            #+#    #+#             */
/*   Updated: 2023/04/06 19:26:36 by mbozzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cube3D.h"

char	*ext_path(char *line, int flag)
{
	int		i;
	int		x;
	char	*ret;

	i = 0;
	x = 0;
	if (flag == 0)
		while (line[i] && line[i] != '.')
			i++;
	else if (flag == 1)
		while (line[i] && line[i] <= '0' && line[i] >= '9')
			i++;
	x = i;
	while (line[i] && line[i] != '\n')
		i++;
	ret = ft_calloc(sizeof(char), i - x + 1);
	i = 0;
	while (line[x] && line[x] != '\n')
	{
		ret[i] = line[x];
		x++;
		i++;
	}
	return (ret);
}

int	get_color(char *line)
{
	char	**color;
	char	*trim;
	int		ret;
	int		i;

	trim = ext_path(line, 1);
	ft_printf("%s\n", trim);
	color = ft_split(trim, ',');
	ret = ft_color_converter(ft_atoi(color[0]),
			ft_atoi(color[1]), ft_atoi(color[2]));
	i = -1;
	while (color[++i])
		free(color[i]);
	free(color);
	free(trim);
	return (ret);
}

char	*get_path(char *line)
{
	if (!ft_strncmp(line, "NO ", 3))
		return (ext_path(line, 0));
	else if (!ft_strncmp(line, "SO ", 3))
		return (ext_path(line, 0));
	else if (!ft_strncmp(line, "WE ", 3))
		return (ext_path(line, 0));
	else if (!ft_strncmp(line, "EA ", 3))
		return (ext_path(line, 0));
	return (NULL);
}

void	texture_path(t_cube *cb)
{
	int		fd;
	char	*line;
	int		i;

	i = 0;
	fd = open(cb->path, O_RDONLY);
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (ft_strncmp(line, "\n", 2) && i < 4)
		{
			cb->tex_path[i] = get_path(line);
			i++;
		}
		if (!ft_strncmp(line, "F ", 2))
			cb->floor = get_color(line);
		else if (!ft_strncmp(line, "C ", 2))
			cb->ceil = get_color(line);
		free(line);
		line = get_next_line(fd);
	}
	printf ("%d %d\n", cb->floor, cb->ceil);
	close(fd);
}

void	texture_init(t_cube *cb)
{
	int	x;
	int	y;
	int	i;

	i = -1;
	x = IMG_WID;
	y = IMG_HGT;
	cb->tex_path = ft_calloc(sizeof(char *), 5);
	texture_path(cb);
	cb->tex = ft_calloc(sizeof(t_img *), 5);
	while (++i < 4)
		cb->tex[i] = ft_calloc(sizeof(t_img), 1);
	i = -1;
	while (++i < 4)
	{
		cb->tex[i]->mlx_img = mlx_xpm_file_to_image(cb->mlx, cb->tex_path[i],
				&x, &y);
		cb->tex[i]->data = mlx_get_data_addr(cb->tex[i]->mlx_img,
				&cb->tex[i]->bits_per_pixel,
				&cb->tex[i]->line_length,
				&cb->tex[i]->endian);
		free(cb->tex_path[i]);
	}
	free(cb->tex_path);
}