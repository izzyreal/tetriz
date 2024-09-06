#include <stdio.h>
#include <stdint.h>

const uint8_t WIDTH = 80;
const uint8_t HEIGHT = 40;

char canvas[HEIGHT][WIDTH];

void clear_canvas()
{
	for (uint8_t y = 0; y < HEIGHT; y++)
	{
		for (uint8_t x = 0; x < WIDTH; x++)
		{
			canvas[y][x] = ' ';
		}
	}
}

void draw_border()
{
	for (uint8_t y = 0; y < HEIGHT; y++)
	{
		for (uint8_t x = 0; x < WIDTH; x++)
		{
			if (y > 0 && y < (HEIGHT - 1) && x != 0 && x != WIDTH - 1)
			{
			    continue;
			}
			
			canvas[y][x] = '#';
		}
	}
}

void draw_canvas_to_screen()
{
	for (uint8_t y = 0; y < HEIGHT; y++)
	{
		for (uint8_t x = 0; x < WIDTH; x++)
		{
		    printf("%c", canvas[y][x]);
		}
		printf("\n");
	}
}

int main()
{
	clear_canvas();
	draw_border();
	draw_canvas_to_screen();
}
