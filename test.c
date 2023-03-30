#include "mlx.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 500
#define HEIGHT 500
#define ZOOM_FACTOR 1.0

typedef struct s_fractal
{
    int max_iter;
    double zoom;
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    double c_r;
    double c_i;
    void *mlx_ptr;
    void *win_ptr;
} t_fractal;

void set_default_fractal(t_fractal *fractal)
{
    fractal->max_iter = 50;
    fractal->zoom = 1.0;
    fractal->x_min = -2.0;
    fractal->x_max = 2.0;
    fractal->y_min = -2.0;
    fractal->y_max = 2.0;
    fractal->c_r = 0.285;
    fractal->c_i = 0.01;
}

int get_color(int iter, int MAX_ITER)
{
//   int palette[] = {0x000000, 0xFF0000, 0xFFFF00, 0x00FF00, 0x00FFFF, 0x0000FF};

   int color;
    double t = (double)iter / MAX_ITER;
    double r = 0.5 + 0.5 * sin(2 * M_PI * (0.5 * t + 0.0));
    double g = 0.5 + 0.5 * sin(2 * M_PI * (0.5 * t + 0.0));
    double b = 0.5 + 0.5 * sin(2 * M_PI * (0.5 * t + 0.0));
    int red = (int)(255.0 * r);
    int green = (int)(255.0 * g);
    int blue = (int)(255.0 * b);
  // color = palette[iter % 6];
  // return (palette[iter % 6]);
  return (red << 16) | (green << 8) | blue;
}

void draw_fractal(t_fractal *fractal, void *mlx_ptr, void *win_ptr)
{
    double x, y, x_temp, x0, y0;
    int color, i, j, iter;

    i = -1;
    while (++i < WIDTH)
    {
        j = -1;
        while (++j < HEIGHT)
        {
            x0 = fractal->x_min + (double)i / (double)WIDTH * (fractal->x_max - fractal->x_min);
            y0 = fractal->y_min + (double)j / (double)HEIGHT * (fractal->y_max - fractal->y_min);
            x = 0;
            y = 0;
            iter = 0;
            while (x * x + y * y <= 4 && iter < fractal->max_iter)
            {
                x_temp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = x_temp;
                iter++;
            }
            if (iter == fractal->max_iter)
                color = 0;
            else
                color = get_color(iter, fractal->max_iter);
            mlx_pixel_put(mlx_ptr, win_ptr, i, j, color);
        }
    }
}

void    draw_julia_set(t_fractal *fractal, void *mlx_ptr, void *win_ptr)
{
    double  x_temp, x, y;
    int     color, i, j, iter;

    i = -1;
    while (++i < WIDTH)
    {
        j = -1;
        while (++j < HEIGHT)
        {
            x = fractal->x_max + (double)i / (double)WIDTH * (fractal->x_min - fractal->x_max);
            y = fractal->y_min + (double)j / (double)HEIGHT * (fractal->y_max - fractal->y_min);
            iter = 0;
            while (iter < fractal->max_iter && x*x + y*y <= 4)
            {
                x_temp = x*x - y*y + fractal->c_r;
                y = 2 * x * y + fractal->c_i;
                x = x_temp;
                iter++;
            }
	    if (iter == fractal->max_iter)
		    color = 0x000000;
	    else
		    color = get_color(iter, fractal->max_iter);
            mlx_pixel_put(mlx_ptr, win_ptr, i, j, color);
        }
    }
}

/*int my_key_hook(int keycode, t_fractal *fractal)
{
	if (keycode == 65361)
	{
		fractal->x_min+=OFFSET_STEP;
		fractal->x_max+=OFFSET_STEP;
	}
	if (keycode == 65362)
	{
		fractal->y_min-=OFFSET_STEP;
		fractal->y_max-=OFFSET_STEP;
	}
	if (keycode == 65363)
	{
		fractal->x_min-=OFFSET_STEP;
		fractal->x_max-=OFFSET_STEP;
	}
	if (keycode == 65364)
	{
		fractal->y_min+=OFFSET_STEP;
		fractal->y_max+=OFFSET_STEP;
	}
	return(0);
}*/

int     main(void)
{
    void        *mlx_ptr;
    void        *win_ptr;
    t_fractal   fractal;

    set_default_fractal(&fractal);

    mlx_ptr = mlx_init();
    win_ptr = mlx_new_window(mlx_ptr, WIDTH, HEIGHT, "Fractale");

    fractal.mlx_ptr = mlx_ptr;
    fractal.win_ptr = win_ptr;

    draw_julia_set(&fractal, mlx_ptr, win_ptr);
    //my_key_hook(win_ptr, &my_key_hook, &fractal);


    mlx_loop(mlx_ptr);
    return (0);
}
