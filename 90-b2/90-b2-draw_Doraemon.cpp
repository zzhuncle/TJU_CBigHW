/* 1853790 庄镇华 数强 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <math.h>
#include <time.h>
#include "../common/cmd_hdc_tools.h"
using namespace std;
static const double PI = 3.14159;

const int PCOLOR_LINE = RGB(0, 0, 0);
const int PCOLOR_BLANK = RGB(255, 255, 255);

const int PCOLOR_HEAD_FACE_FILLED = PCOLOR_BLANK;
const int PCOLOR_HEAD_WEIBO_FILLED = RGB(231,0,16);
const int PCOLOR_HEAD_FILLED = RGB(0,160,222);

const int PCOLOR_NOSE_FILLED = RGB(231,0,16);

const int PCOLOR_BODY = RGB(192,192,192);
const int PCOLOR_BODY_FILLED = RGB(0, 160, 222);


const int PCOLOR_BELL_FILLED = RGB(255,210,0);

const int PCOLOR_HANDS = RGB(0, 160, 222);

const int PCOLOR_MOUTH = PCOLOR_LINE;

static int base_x = 400;
static int base_y = 200;

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static void nose()
{
	hdc_circle(base_x, base_y+2, 18, 0, 8, PCOLOR_LINE);
	hdc_circle(base_x, base_y+2, 18, 1, 3, PCOLOR_NOSE_FILLED);
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static void head()
{
	hdc_ellipse(base_x, base_y, 136, 132, 0, 0, 5, PCOLOR_LINE);//头部
	hdc_ellipse(base_x,base_y, 136, 132, 0, 1, 1, PCOLOR_HEAD_FILLED);//头部填充
	hdc_ellipse(base_x, base_y+40, 90,100, 0,0, 6, PCOLOR_LINE);
	hdc_ellipse(base_x, base_y + 40,100, 90, 0, 0, 6, PCOLOR_LINE);
	hdc_ellipse(base_x, base_y + 40, 100, 90, 0, 1, 1, PCOLOR_HEAD_FACE_FILLED);
	hdc_ellipse(base_x, base_y + 40, 90, 100, 0, 1, 1, PCOLOR_HEAD_FACE_FILLED);//脸部
}
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static void weibo()
{
	hdc_rectangle(base_x-100, base_y+100, 196, 20, 0, 0, 8, PCOLOR_LINE);
	hdc_rectangle(base_x - 100, base_y + 100,196, 20, 0, 1, 1, PCOLOR_HEAD_WEIBO_FILLED);//围巾
}
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static void bell()
{
	hdc_circle(base_x, base_y+123, 10, 0, 8, PCOLOR_LINE);
	hdc_circle(base_x, base_y + 123, 10, 1, 1, PCOLOR_BELL_FILLED);
	hdc_point(base_x, base_y + 126, 9, PCOLOR_LINE);
	hdc_line(base_x, base_y + 126, base_x, base_y+133, 4, PCOLOR_LINE);
}
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static void eyes()
{
	hdc_ellipse(base_x-28, base_y-45, 24, 34, 0, 0, 5, PCOLOR_LINE);//眼睛
	hdc_ellipse(base_x - 28, base_y - 45, 24, 34, 0, 1, 1, PCOLOR_BLANK);//眼睛填充
	hdc_ellipse(base_x + 28, base_y - 45, 24, 34, 0, 0, 5, PCOLOR_LINE);//眼睛
	hdc_ellipse(base_x + 28, base_y - 45, 24, 34, 0, 1, 1, PCOLOR_BLANK);//眼睛填充
	hdc_circle(base_x-26, base_y-37, 14, 1, 1, PCOLOR_LINE);
	hdc_point(base_x-18,base_y-34, 5, PCOLOR_BLANK);
	hdc_arc(base_x+26,base_y-32, 14, -70, 70, 4, PCOLOR_LINE);
}
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static void beard()
{
	hdc_line(base_x+30, base_y+22, base_x+86, base_y+1, 4, PCOLOR_LINE);
	hdc_line(base_x+30,base_y+30,base_x+90,base_y+30,4,PCOLOR_LINE);
	hdc_line(base_x+30, base_y+38, base_x+80, base_y+51, 4, PCOLOR_LINE);
	hdc_line(base_x-30, base_y+23, base_x-86, base_y+1, 4, PCOLOR_LINE);
	hdc_line(base_x-30, base_y+30, base_x-90, base_y+30, 4, PCOLOR_LINE);
	hdc_line(base_x-30, base_y+38, base_x-80, base_y+51, 4, PCOLOR_LINE);
}
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static void mouth()
{
	hdc_line(base_x,base_y+25,base_x,base_y+90,4,PCOLOR_MOUTH);
	hdc_arc(base_x, base_y - 40, 290 - 160, 142, 218, 3, PCOLOR_MOUTH);
}
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static void body()
{
	hdc_rectangle(base_x-92, base_y+120, 200, 170, 0, 1, 1, PCOLOR_BODY_FILLED);
	hdc_sector(base_x-92, base_y+290, 170, -3, 0, 1, 1, PCOLOR_BODY_FILLED);
	hdc_sector(base_x+108, base_y+290, 170, 0, 7, 1, 1, PCOLOR_BODY_FILLED);
	hdc_line(base_x-92, base_y+290, base_x-100, base_y+170, 4, PCOLOR_LINE);
	hdc_line(base_x+108, base_y+290, int(base_x+245 - sin(38 * PI / 180)*(140 + 50))-2, int(base_y+5 + cos(38 * PI / 180)*(140 + 50)),
		4, PCOLOR_LINE);
	hdc_ellipse(base_x+8, base_y+290, 40, 35, 0, 0, 5, PCOLOR_LINE);
	hdc_ellipse(base_x+8, base_y+290, 40,35, 0,1, 1, PCOLOR_BODY);
	hdc_sector(base_x+8, base_y+290, 50, 90, 270, 1, 4, PCOLOR_BODY);
}
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static void hands()
{
	hdc_arc(base_x+245,base_y+5,140,218,240,100, PCOLOR_HANDS);
	hdc_line(int(base_x+245 - sin(38 * PI / 180)*(140 - 50)), int(base_y+5 + cos(38 * PI / 180)*(140 - 50)),
		int(base_x+245 - sin(38 * PI / 180)*(140 + 50)), int(base_y+5 + cos(38 * PI / 180)*(140 + 50)),
		4, PCOLOR_LINE);
	hdc_line(int(base_x + 245 - sin(60 * PI / 180)*(140 - 50)), int(base_y+5 + cos(60 * PI / 180)*(140 - 50)),
		int(base_x+245 - sin(60 * PI / 180)*(140 + 50)), int(base_y+5 + cos(60 * PI / 180)*(140 + 50)),
		4, PCOLOR_LINE);
	hdc_ellipse(base_x+198, base_y+52, 28, 28, 0, 0, 5, PCOLOR_LINE);
	hdc_ellipse(base_x + 198, base_y + 52, 28, 28, 0, 1, 1, PCOLOR_BLANK);
	hdc_rectangle(base_x-100,base_y+120, 48, 90, 42, 0, 6, PCOLOR_LINE);
	hdc_rectangle(base_x-100, base_y+120, 48, 90, 42, 1, 1, PCOLOR_HANDS);
	hdc_ellipse(base_x-148, base_y+205, 28, 28, 0, 0, 5, PCOLOR_LINE);
	hdc_ellipse(base_x - 148, base_y + 205, 28, 28, 0, 1, 1, PCOLOR_BLANK);
}
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static void foot()
{
	hdc_ellipse(base_x-72, base_y+302, 48, 12, 0, 0, 6, PCOLOR_LINE);
	hdc_ellipse(base_x - 72, base_y + 302, 48, 12, 0, 1, 1, PCOLOR_BLANK);
	hdc_ellipse(base_x+85, base_y+302, 48, 12, 0, 0, 6, PCOLOR_LINE);
	hdc_ellipse(base_x + 85, base_y + 302, 48, 12, 0, 1, 1, PCOLOR_BLANK);
}
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static void pocket()
{
	hdc_sector(base_x, base_y+168, 75, 50, 310, 0, 10, PCOLOR_LINE);
	hdc_sector(base_x, base_y+168, 75, 50, 310, 1, 1, PCOLOR_BLANK);
	hdc_sector(base_x, base_y+120, 57, 90, 270, 1, 2, PCOLOR_BLANK);
	hdc_sector(base_x, base_y+166, 66, 90, 270, 0, 4, PCOLOR_LINE);
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int hdc_draw_Doraemon(const int middle_head_x, const int middle_head_y)
{
	base_x = middle_head_x;
	base_y = middle_head_y;
	head();
	weibo();
	eyes();
	nose();
	mouth();
	beard();
	hands();
	body();
	foot();
	pocket();
	weibo();
	bell();
	return 0;
}