/* 1853790 庄镇华 数强 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>

const int COLS = 60;
const int LINES = 50;

/* 允许添加需要的头文件 */
#include <conio.h>  
#include "cmd_console_tools.h"
#include "cmd_hdc_tools.h"
using namespace std;

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();

/* 已定义的静态全局变量 */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;

/* 允许适度使用静态全局变量，但仅限static，即外部不可见 */
static const int N = 10;
/* 此处允许添加自定义函数，但仅限static，即外部不可见 */
/***************************************************************************
  交换两数的值
***************************************************************************/
static void change(int &a, int &b)
{
	int t;
	t = a;
	a = b;
	b = t;
}

/***************************************************************************
  还原坐标变换的原始坐标
***************************************************************************/
static void origin_xy(double &x0, double &y0, double x, double y, int angle, int p_x, int p_y)
{
	x0 = sin(angle*PI / 180)*x + cos(angle*PI / 180)*y + p_x;
	y0 = sin(angle*PI / 180)*y - cos(angle*PI / 180)*x + p_y;
}

/***************************************************************************
  将三数顺序排列
***************************************************************************/
static void order(int xx[], int yy[])
{
	int k, tempx, tempy;
	for (int i = 0; i < 3; i++) {
		k = i;
		for (int j = i + 1; j < 3; j++)
			if (xx[j] < xx[k])
				k = j;
		tempx = xx[k];
		tempy = yy[k];
		xx[k] = xx[i];
		yy[k] = yy[i];
		xx[i] = tempx;
		yy[i] = tempy;
	}
}

/***************************************************************************
  求直线某点的y值//直线斜率必须存在
***************************************************************************/
static double solve_y(int x, int x1, int y1, int x2, int y2)
{
	double y;
	if (x1 == x2)
		return double(y1);
	y = (double(y1) - y2) / (x1 - x2)*(x - x1) + y1;
	return y;
}

/***************************************************************************
  求两数最小值
***************************************************************************/
static double _min(double x, double y)
{
	return (x < y ? x : y);
}

/***************************************************************************
  求两数最大值
***************************************************************************/
static double _max(double x, double y)
{
	return (x > y ? x : y);
}


/* 下面给出了几个基本函数的完整实现，不要改动 */
/***************************************************************************
  函数名称：
  功    能：初始化
  输入参数：const int bgcolor：背景色
			const int fgcolor：前景色
			const int width  ：屏幕宽度（点阵）
			const int high   ：屏幕高度（点阵）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* 先释放，防止不release而再次init（hdc_release可重入） */
	hdc_release();

	/* 窗口init后，用一个静态全局量记录，后续hdc_cls()会用到 */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);
	setcursor(CURSOR_INVISIBLE);

	setconsoleborder(width, high, width, high);
	setcolor(bgcolor, fgcolor);
	setfontsize("新宋体", 28);//设置字体

	cls();
}

/***************************************************************************
  函数名称：
  功    能：释放画图资源
  输入参数：
  返 回 值：
  说    明：可重入
***************************************************************************/
void hdc_release()
{
	if (hdc) {
		ReleaseDC(hWnd, hdc);
		hdc = NULL;
		setcursor(CURSOR_VISIBLE_NORMAL);
	}
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB值
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB三色，值0-255
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  函数名称：
  功    能：清除屏幕上现有的图形
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_cls()
{
	/* 发现一定要换一种颜色初始化才能清除像素点，找到更简便方法的同学可以通知我 */
	hdc_init(_BgColor_, (_FgColor_ + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* 部分机器上运行demo时，会出现hdc_cls()后第一根线有缺线的情况，加延时即可
	   如果部分机器运行还有问题，调高此延时值 */
	Sleep(30);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
			const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
			const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画一个指定粗细的点(用画实心圆来模拟)
  输入参数：const int x			：x坐标，左上角为(0,0)
			const int y			：y坐标，左上角为(0,0)
			const int thickness	：点的粗细，下限位1，上限不限(如过大会导致不完全填充)（有缺省值）
			const int RGB_value	：点的颜色（有缺省值）
  返 回 值：
  说    明：改进运行效率
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限
	/* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */

	const int tn_end = (tn <= 1) ? 1 : tn / 2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* 用画圆的方式模拟粗点 */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle++) {
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* 放开此注释，可以看到一个粗点用了几个像素点组成 */
	gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	给出下列函数的实现，函数名及参数表不准改动
	1、不需要调用系统的其他有关图形操作的函数
	2、下列函数的实现都基于hdc_point函数的组合
	3、想一想，是不是所有函数都需要给出独立的实现过程？应该先实现哪些函数？
	4、填充是实现中最复杂的部分
	5、系统的角度与函数参数中的角度含义相差180°
   ------------------------------------------------------------------------- */
   /***************************************************************************
	 函数名称：
	 功    能：画线段
	 输入参数：const int x1		：起点的x
			   const int y1		：起点的y
			   const int x2		：终点的x
			   const int y2		：终点的y
			   const int thickness	：线段的粗细（有缺省值）
			   const int RGB_value	：线段的颜色（有缺省值）
	 返 回 值：
	 说    明：
   ***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	double tn = thickness / 2.0;
	if (tn < 1)
		tn = 1;		//处理下限
	int angle;
	int old_xx1 = INT_MAX, old_yy1 = INT_MAX, old_xx2 = INT_MAX, old_yy2 = INT_MAX, xx1, yy1, xx2, yy2;
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	/* 用画圆的方式模拟粗点 */
	for (angle = 0; angle <= 360; angle++) {
		/* 注意，系统的坐标轴，0°在圆的最下方 */
		xx1 = x1 + (int)(tn * sin(angle * PI / 180));
		xx2 = x2 + (int)(tn * sin(angle * PI / 180));
		yy1 = y1 - (int)(tn * cos(angle * PI / 180));
		yy2 = y2 - (int)(tn * cos(angle * PI / 180));
		/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
		if (xx1 != old_xx1 || yy1 != old_yy1 || xx2 != old_xx2 || yy2 != old_yy2) {
			old_xx1 = xx1;
			old_yy1 = yy1;
			old_xx2 = xx2;
			old_yy2 = yy2;
			hdc_base_line(xx1, yy1, xx2, yy2);
		}
	}
	return;
}

/***************************************************************************
  函数名称：
  功    能：给出三点的坐标，画一个三角形
  输入参数：const int x1		：第1个点的x
			const int y1		：第1个点的y
			const int x2		：第2个点的x
			const int y2		：第2个点的y
			const int x3		：第3个点的x
			const int y3		：第3个点的y
			bool filled			：是否需要填充（有缺省值）
			const int thickness	：边的粗细（有缺省值）
			const int RGB_value	：颜色（有缺省值）
  返 回 值：
  说    明：不判断三点是否共线，如果共线，划出一条直线即可
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	int x;
	int xx[3] = { x1,x2,x3 }, yy[3] = { y1,y2,y3 };
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x1, y1, x3, y3, thickness, RGB_value);
	hdc_line(x3, y3, x2, y2, thickness, RGB_value);
	if (filled) {
		order(xx, yy);
		for (x = xx[0]; x <= xx[1];) {
			hdc_base_line(x, (int)_min(solve_y(x, xx[0], yy[0], xx[1], yy[1]), solve_y(x, xx[0], yy[0], xx[2], yy[2])), x, (int)_max(solve_y(x, xx[0], yy[0], xx[1], yy[1]), solve_y(x, xx[2], yy[2], xx[0], yy[0])));
			x += 1;
		}
		for (x = xx[1] + 1; x <= xx[2];) {
			hdc_line(x, (int)_min(solve_y(x, xx[2], yy[2], xx[1], yy[1]), solve_y(x, xx[0], yy[0], xx[2], yy[2])), x, (int)_max(solve_y(x, xx[2], yy[2], xx[1], yy[1]), solve_y(x, xx[0], yy[0], xx[2], yy[2])));
			x += 1;
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：给出左上角坐标及宽度、高度，画出一个长方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int width				：宽度
			const int high				：高度
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）
			bool filled					：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_rectangle(const int left_up_x, const int left_up_y, const int width, const int high, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	if (!filled) {
		hdc_line(left_up_x, left_up_y, left_up_x + (int)(width*cos(rotation_angles * PI / 180)),
			left_up_y + (int)(width*sin(rotation_angles * PI / 180)), thickness, RGB_value);
		hdc_line(left_up_x, left_up_y, left_up_x - (int)(high * sin(rotation_angles * PI / 180)),
			left_up_y + (int)(high * cos(rotation_angles * PI / 180)), thickness, RGB_value);
		hdc_line(left_up_x - (int)(high * sin(rotation_angles * PI / 180)), left_up_y + (int)(high * cos(rotation_angles * PI / 180)),
			left_up_x + (int)(width * cos(rotation_angles * PI / 180)) - (int)(high * sin(rotation_angles * PI / 180)),
			left_up_y + (int)(width * sin(rotation_angles * PI / 180)) + (int)(high * cos(rotation_angles * PI / 180)), thickness, RGB_value);
		hdc_line(left_up_x + (int)(width * cos(rotation_angles * PI / 180)), left_up_y + (int)(width * sin(rotation_angles * PI / 180)),
			left_up_x - (int)(high * sin(rotation_angles * PI / 180)) + (int)(width * cos(rotation_angles * PI / 180)),
			left_up_y + (int)(high * cos(rotation_angles * PI / 180)) + (int)(width * sin(rotation_angles * PI / 180)), thickness, RGB_value);
	}
	if (filled) {
		int old_x = INT_MAX, old_y = INT_MAX;
		double x, y;
		for (x = left_up_x, y = left_up_y; fabs(x - (left_up_x - high * sin(rotation_angles * PI / 180))) >= 1
			|| fabs(y - (left_up_y + high * cos(rotation_angles * PI / 180))) >= 1;) {
			x -= sin(rotation_angles * PI / 180);
			y += cos(rotation_angles * PI / 180);
			if (int(x) != old_x || int(y) != old_y) {
				old_x = int(x);
				old_y = int(y);
				hdc_line(int(x), int(y), int(x) + (int)(width*cos(rotation_angles * PI / 180)),
					int(y) + (int)(width*sin(rotation_angles * PI / 180)), thickness, RGB_value);
			}
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：给出左上角坐标及边长，画出一个长方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int length			：边长
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一段圆弧
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
	double tn = thickness / 2.0;
	if (tn < 1)
		tn = 1;
	double angle;
	int level, old_x = INT_MAX, old_y = INT_MAX;
	int x1, y1;
	double _angle_begin = angle_begin, _angle_end = angle_end, dangle = tn / radius * 180 / PI;
	if (thickness == 46)
		_angle_begin -= dangle;
	while (_angle_begin <= 0.1)
		_angle_begin += 360;
	while (_angle_end > 720)
		_angle_end -= 360;
	while (_angle_begin >= _angle_end)
		_angle_end += 360;
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	for (level = int(radius - tn); level <= int(radius + tn); level++) {
		for (angle = _angle_begin; fabs(angle - _angle_end) > 1e-2; angle += 0.005) {
			x1 = point_x + (int)(level * cos(angle * PI / 180 - PI / 2));
			y1 = point_y + (int)(level * sin(angle * PI / 180 - PI / 2));
			if (x1 != old_x || y1 != old_y) {
				old_x = x1;
				old_y = y1;
				hdc_base_point(x1, y1);
			}
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个扇形
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  输入参数：

  返 回 值：
  说    明：当起始/结束角度差值为360的倍数时，不画两条边
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	int _angle_begin = angle_begin, _angle_end = angle_end;
	while (_angle_begin < 0)
		_angle_begin += 360;
	while (_angle_end > 720)
		_angle_end -= 360;
	while (_angle_begin >= _angle_end)
		_angle_end += 360;//使begin和end最多相差360°
	int dangle = _angle_end - _angle_begin;
	hdc_arc(point_x, point_y, radius, angle_begin, angle_end, thickness, RGB_value);
	if (fabs(_angle_end - (_angle_begin + 360.0)) > 0.1) {
		hdc_line(point_x, point_y, point_x + int(radius*sin(_angle_begin*PI / 180)), point_y - int(radius*cos(_angle_begin*PI / 180)), thickness, RGB_value);
		hdc_line(point_x, point_y, point_x + int(radius*sin(_angle_end*PI / 180)), point_y - int(radius*cos(_angle_end*PI / 180)), thickness, RGB_value);
	}
	int  y;
	if (filled) {
		if (dangle <= 90) {
			for (y = 0; y <= int(radius*sin(dangle*PI / 180)); y++) {
				double x0, y0, x1, y1;
				origin_xy(x0, y0, sqrt(radius*radius - y * y), double(y), _angle_begin, point_x, point_y);
				double xx = 0;
				if (dangle != 90)
					xx = y / tan(dangle*PI / 180);
				origin_xy(x1, y1, xx, double(y), _angle_begin, point_x, point_y);
				hdc_line((int)x0, (int)y0, (int)x1, (int)y1, 3, RGB_value);
			}
		}
		if (dangle > 90 && dangle <= 180) {
			hdc_sector(point_x, point_y, radius, _angle_begin, _angle_begin + 90, filled, thickness, RGB_value);
			hdc_sector(point_x, point_y, radius, _angle_begin + 90, _angle_end, filled, thickness, RGB_value);
		}
		if (dangle > 180 && dangle <= 270) {
			hdc_sector(point_x, point_y, radius, _angle_begin, _angle_begin + 90, filled, thickness, RGB_value);
			hdc_sector(point_x, point_y, radius, _angle_begin + 90, _angle_begin + 180, filled, thickness, RGB_value);
			hdc_sector(point_x, point_y, radius, _angle_begin + 180, _angle_end, filled, thickness, RGB_value);
		}
		if (dangle > 270 && dangle <= 360) {
			hdc_sector(point_x, point_y, radius, _angle_begin, _angle_begin + 90, filled, thickness, RGB_value);
			hdc_sector(point_x, point_y, radius, _angle_begin + 90, _angle_begin + 180, filled, thickness, RGB_value);
			hdc_sector(point_x, point_y, radius, _angle_begin + 180, _angle_begin + 270, filled, thickness, RGB_value);
			hdc_sector(point_x, point_y, radius, _angle_begin + 270, _angle_end, filled, thickness, RGB_value);
		}
	}
}
/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个圆
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	hdc_sector(point_x, point_y, radius, 0, 0, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个椭圆
  输入参数：const int point_x			：圆心的x
			const int point_y			：圆心的y
			const int radius_a			：平行于X轴的半径
			const int radius_b			：平行于Y轴的半径
			const int rotation_angles	：以圆心为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	int _rotation_angles = rotation_angles;
	while (_rotation_angles < 0)
		_rotation_angles += 360;
	while (_rotation_angles > 360)
		_rotation_angles -= 360;
	double angle = 0, x0, y0, x1, y1;
	double level, x, y;
	int old_x = INT_MAX, old_y = INT_MAX;
	if (!filled) {
		for (; fabs(angle - 360) >= 0.01; angle += 0.1) {
			x1 = radius_a * cos(angle*PI / 180);
			y1 = radius_b * sin(angle*PI / 180);
			for (level = 0; fabs(level - thickness) > 0.01; level++) {
				x1 = (radius_a + level) * cos(angle*PI / 180);
				y1 = (radius_b + level) * sin(angle*PI / 180);
				origin_xy(x0, y0, x1, y1, _rotation_angles + 90, point_x, point_y);
				if (RGB_value != INVALID_RGB)
					hdc_set_pencolor(RGB_value);
				if (int(x0) != old_x || int(y0) != old_y) {
					old_x = int(x0);
					old_y = int(y0);
					hdc_base_point(int(x0), int(y0));
				}
			}
		}
	}
	if (filled) {
		for (y = -radius_b; fabs(y - radius_b) > 0.01; y++) {
			x = sqrt(radius_a*radius_a - double(radius_a)*radius_a / (radius_b*radius_b)*y*y);
			double xxl, xxr, yyl, yyr;
			origin_xy(xxl, yyl, -x, y, _rotation_angles + 90, point_x, point_y);
			origin_xy(xxr, yyr, x, y, _rotation_angles + 90, point_x, point_y);
			hdc_line((int)xxl, (int)yyl, (int)xxr, (int)yyr, 4, RGB_value);
		}
	}
}
