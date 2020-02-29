/* 1853790 庄镇华 数强 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <math.h>
#include <time.h>
#include "../common/cmd_hdc_tools.h"
using namespace std;
static const double PI = 3.14159;

static void snowflake_partial(double x1, double y1, double x2, double y2,double length,int thickness,int RGB_value)
{
	if (sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)) < length)
		hdc_line(int(x1), int(y1), int(x2), int(y2), thickness, RGB_value);
	else
	{
		double x3, y3, x4, y4, x5, y5;
		double radius, angle;
		x3 = x1 + (x2 - x1) / 3;  
		y3 = y1 + (y2 - y1) / 3;
		x5 = x2 - (x2 - x1) / 3;
		y5 = y2 - (y2 - y1) / 3;
		snowflake_partial(x1, y1, x3, y3, length, thickness, RGB_value);
		snowflake_partial(x5, y5, x2, y2,length,thickness,RGB_value);
		radius = sqrt((x5 - x3)*(x5 - x3) + (y5 - y3)*(y5 - y3));
		angle = atan((y5 - y3) / (x5 - x3));
		if ((x5 - x3) < 0)
			angle += PI;
		x4 = x3 + cos(angle + PI / 3)*radius;
		y4 = y3 + sin(angle + PI / 3)*radius;
		snowflake_partial(x3, y3, x4, y4, length, thickness, RGB_value);
		snowflake_partial(x4, y4, x5, y5, length, thickness, RGB_value);
	}
}

void hdc_draw_Mathematical_curve(double x1, double y1, double x2, double y2, double x3, double y3,int length,int thickness,const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	snowflake_partial(x1, y1, x2, y2, length, thickness, RGB_value);
	snowflake_partial(x2, y2, x3, y3, length, thickness, RGB_value);
	snowflake_partial(x3, y3, x1, y1, length, thickness, RGB_value);
}