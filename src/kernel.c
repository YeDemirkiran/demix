#include <stdint.h>

volatile uint16_t	*vga = (uint16_t *)0xB8000;

void	print(const char *str)
{
	uint16_t	pos;

	pos = 0;
	while (str[pos] != '\0')
	{
		vga[pos] = (uint16_t)str[pos] | 0x0F00;
		pos++;
	}
}

void	kernel_main(void)
{
	print("DemiX");

	while (1)
	{
		__asm__ volatile ("hlt");
	}
}
