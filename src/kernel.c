#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// Are we in Linux? Which means we are not using a cross-compiler.
#ifdef __linux___
# error "You are not using a cross-compiler. Come back when you use one."
#endif

#ifndef __i386__
# error "This kernel needs to be compiled for an i386-elf or a similar \
architecture"
#endif

typedef enum e_vga_color
{
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
}	t_vga_color;

#ifndef VGA_WIDTH
# define VGA_WIDTH 80
#endif

#ifndef VGA_HEIGHT
# define VGA_HEIGHT 25
#endif

#define VGA_MEMORY 0xB8000

typedef struct s_terminal
{
	size_t				row;
	size_t				column;
	uint8_t				color;
	volatile uint16_t	*buffer;
}	t_terminal;

size_t	ft_strlen(const char *str)
{
	size_t	len;

	if (str == NULL)
		return (0);
	len = 0;
	while (str[len] != '\0')
		len++;
	return (len);
}

static inline uint8_t	vga_color(t_vga_color fg, t_vga_color bg)
{
	return (fg | bg << 4);
}

static inline uint16_t	vga_char(unsigned char uc, uint8_t color)
{
	return ((uint16_t)uc | (uint16_t)color << 8);
}

void	terminal_set_color(t_terminal *terminal_addr, uint8_t color)
{
	if (terminal_addr == NULL)
		return ;
	terminal_addr->color = color;
}

void	terminal_putchar(t_terminal *terminal_addr, uint16_t c)
{
	size_t	index;

	if (terminal_addr == NULL)
		return ;
	index = terminal_addr->row * VGA_HEIGHT + terminal_addr->column;
	terminal_addr->buffer[index] = c;
	terminal_addr->column += 1;
	if (terminal_addr->column >= VGA_WIDTH)
	{
		terminal_addr->column = 0;
		terminal_addr->row += 1;
	}
	if (terminal_addr->row >= VGA_HEIGHT)
		terminal_addr->row = 0;
}

void	terminal_putstr(t_terminal *terminal_addr, const char *str)
{
	size_t		i;
	uint16_t	c;

	if (str == NULL || terminal_addr == NULL)
		return ;
	i = 0;
	while (str[i] != '\0')
	{
		c = vga_char((unsigned char)str[i], terminal_addr->color);
		terminal_putchar(terminal_addr, c);
		i++;
	}
}

void	clear_terminal(t_terminal *terminal_addr)
{
	size_t		x;
	size_t		y;
	size_t		index;
	uint16_t	color;

	if (terminal_addr == NULL)
		return ;
	y = 0;
	color = terminal_addr->color;
	while (y < VGA_HEIGHT)
	{
		x = 0;
		while (x < VGA_WIDTH)
		{
			index = y * VGA_WIDTH + x;
			terminal_addr->buffer[index] = vga_char(' ', color);
			x++;
		}
		y++;
	}
	terminal_addr->row = 0;
	terminal_addr->column = 0;
}

void	init_terminal(t_terminal *terminal_addr)
{
	if (terminal_addr == NULL)
		return ;
	terminal_addr->row = 0;
	terminal_addr->column = 0;
	terminal_addr->buffer = (uint16_t *)VGA_MEMORY;
	terminal_addr->color = vga_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	clear_terminal(terminal_addr);
}

void	kernel_main(void)
{
	t_terminal	terminal;

	init_terminal(&terminal);
	terminal_putstr(&terminal, "Anan baban");
}
