NAME = cub3D
NAME_BONUS = cub3D_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
MLX_FLAGS = -lm -lbsd -lmlx -lXext -lX11

LIBFT_PATH = libs/libft/
MLX_PATH = libs/mlx/
LIBFT = $(LIBFT_PATH)libft.a
MLX = $(MLX_PATH)libmlx.a

SRC = main.c read_file.c read_texture.c render.c rgb.c check_map.c free.c \
	key_handle.c movement.c game_loop.c rotate_player.c read_file_utils.c render_texture.c flood_fill.c

BONUS_SRC = bonus/key_handle_bonus.c bonus/door_bonus.c bonus/door_render_bonus.c bonus/wall_collisions.c bonus/minimap.c

OBJ = $(SRC:.c=.o)
OBJ_BONUS = $(SRC:.c=_bonus.o)
OBJ_BONUS_FILE = $(BONUS_SRC:.c=_bonus.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT_PATH) -lft -L$(MLX_PATH) $(MLX_FLAGS) -o $(NAME)

$(NAME_BONUS): $(OBJ_BONUS) $(OBJ_BONUS_FILE) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) -D BONUS=1 $(OBJ_BONUS) $(OBJ_BONUS_FILE) -L$(LIBFT_PATH) -lft -L$(MLX_PATH) $(MLX_FLAGS) -o $(NAME_BONUS)

$(MLX):
	@make -C $(MLX_PATH)

$(LIBFT):
	@make -C $(LIBFT_PATH)

bonus:  $(NAME_BONUS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%_bonus.o: %.c
	$(CC) $(CFLAGS) -DBONUS=1 -c $< -o $@

clean:
	@make clean -C $(LIBFT_PATH)
	rm -rf $(OBJ)
	rm -rf $(OBJ_BONUS)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(NAME_BONUS)
	@make fclean -C $(LIBFT_PATH)

re: fclean all

re_bonus: fclean bonus

.PHONY: all clean fclean re bonus
