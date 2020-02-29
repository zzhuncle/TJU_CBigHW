/* 1853790 ׯ�� ��ǿ */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>

const int COLS = 60;
const int LINES = 50;

/* ���������Ҫ��ͷ�ļ� */
#include <conio.h>  
#include "cmd_console_tools.h"
#include "cmd_hdc_tools.h"
using namespace std;

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();

/* �Ѷ���ľ�̬ȫ�ֱ��� */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;

/* �����ʶ�ʹ�þ�̬ȫ�ֱ�����������static�����ⲿ���ɼ� */
static const int N = 10;
/* �˴���������Զ��庯����������static�����ⲿ���ɼ� */
/***************************************************************************
  ����������ֵ
***************************************************************************/
static void change(int &a, int &b)
{
	int t;
	t = a;
	a = b;
	b = t;
}

/***************************************************************************
  ��ԭ����任��ԭʼ����
***************************************************************************/
static void origin_xy(double &x0, double &y0, double x, double y, int angle, int p_x, int p_y)
{
	x0 = sin(angle*PI / 180)*x + cos(angle*PI / 180)*y + p_x;
	y0 = sin(angle*PI / 180)*y - cos(angle*PI / 180)*x + p_y;
}

/***************************************************************************
  ������˳������
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
  ��ֱ��ĳ���yֵ//ֱ��б�ʱ������
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
  ��������Сֵ
***************************************************************************/
static double _min(double x, double y)
{
	return (x < y ? x : y);
}

/***************************************************************************
  ���������ֵ
***************************************************************************/
static double _max(double x, double y)
{
	return (x > y ? x : y);
}


/* ��������˼�����������������ʵ�֣���Ҫ�Ķ� */
/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʼ��
  ���������const int bgcolor������ɫ
			const int fgcolor��ǰ��ɫ
			const int width  ����Ļ��ȣ�����
			const int high   ����Ļ�߶ȣ�����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* ���ͷţ���ֹ��release���ٴ�init��hdc_release�����룩 */
	hdc_release();

	/* ����init����һ����̬ȫ������¼������hdc_cls()���õ� */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);
	setcursor(CURSOR_INVISIBLE);

	setconsoleborder(width, high, width, high);
	setcolor(bgcolor, fgcolor);
	setfontsize("������", 28);//��������

	cls();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ͷŻ�ͼ��Դ
  ���������
  �� �� ֵ��
  ˵    ����������
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
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGBֵ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGB��ɫ��ֵ0-255
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ļ�����е�ͼ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_cls()
{
	/* ����һ��Ҫ��һ����ɫ��ʼ������������ص㣬�ҵ�����㷽����ͬѧ����֪ͨ�� */
	hdc_init(_BgColor_, (_FgColor_ + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* ���ֻ���������demoʱ�������hdc_cls()���һ������ȱ�ߵ����������ʱ����
	   ������ֻ������л������⣬���ߴ���ʱֵ */
	Sleep(30);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ����һ�����ص�
  ���������const int x��x���꣬���Ͻ�Ϊ(0,0)
			const int y��y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨�������ڵ��ô�ָ��
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ����һ�����ص�
  ���������const int x��x���꣬���Ͻ�Ϊ(0,0)
			const int y��y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨�������ڵ��ô�ָ��
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ��һ��ָ����ϸ�ĵ�(�û�ʵ��Բ��ģ��)
  ���������const int x			��x���꣬���Ͻ�Ϊ(0,0)
			const int y			��y���꣬���Ͻ�Ϊ(0,0)
			const int thickness	����Ĵ�ϸ������λ1�����޲���(�����ᵼ�²���ȫ���)����ȱʡֵ��
			const int RGB_value	�������ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �����Ľ�����Ч��
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������
	/* ������ֱ�������ޣ����ǵ����޳���һ����С�ǣ�������Բ����λ��δʵ�� */

	const int tn_end = (tn <= 1) ? 1 : tn / 2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* �û�Բ�ķ�ʽģ��ֵ� */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle++) {
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* �ſ���ע�ͣ����Կ���һ���ֵ����˼������ص���� */
	gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	�������к�����ʵ�֣���������������׼�Ķ�
	1������Ҫ����ϵͳ�������й�ͼ�β����ĺ���
	2�����к�����ʵ�ֶ�����hdc_point���������
	3����һ�룬�ǲ������к�������Ҫ����������ʵ�ֹ��̣�Ӧ����ʵ����Щ������
	4�������ʵ������ӵĲ���
	5��ϵͳ�ĽǶ��뺯�������еĽǶȺ������180��
   ------------------------------------------------------------------------- */
   /***************************************************************************
	 �������ƣ�
	 ��    �ܣ����߶�
	 ���������const int x1		������x
			   const int y1		������y
			   const int x2		���յ��x
			   const int y2		���յ��y
			   const int thickness	���߶εĴ�ϸ����ȱʡֵ��
			   const int RGB_value	���߶ε���ɫ����ȱʡֵ��
	 �� �� ֵ��
	 ˵    ����
   ***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	double tn = thickness / 2.0;
	if (tn < 1)
		tn = 1;		//��������
	int angle;
	int old_xx1 = INT_MAX, old_yy1 = INT_MAX, old_xx2 = INT_MAX, old_yy2 = INT_MAX, xx1, yy1, xx2, yy2;
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
	/* �û�Բ�ķ�ʽģ��ֵ� */
	for (angle = 0; angle <= 360; angle++) {
		/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
		xx1 = x1 + (int)(tn * sin(angle * PI / 180));
		xx2 = x2 + (int)(tn * sin(angle * PI / 180));
		yy1 = y1 - (int)(tn * cos(angle * PI / 180));
		yy2 = y2 - (int)(tn * cos(angle * PI / 180));
		/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
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
  �������ƣ�
  ��    �ܣ�������������꣬��һ��������
  ���������const int x1		����1�����x
			const int y1		����1�����y
			const int x2		����2�����x
			const int y2		����2�����y
			const int x3		����3�����x
			const int y3		����3�����y
			bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness	���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value	����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �������ж������Ƿ��ߣ�������ߣ�����һ��ֱ�߼���
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
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰��ȡ��߶ȣ�����һ��������
  ���������const int left_up_x			�����Ͻ�x
			const int left_up_y			�����Ͻ�y
			const int width				�����
			const int high				���߶�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ����
			bool filled					���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰�߳�������һ��������
  ���������const int left_up_x			�����Ͻ�x
			const int left_up_y			�����Ͻ�y
			const int length			���߳�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ��
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ������
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  ���������

  �� �� ֵ��
  ˵    ��������ʼ/�����ǶȲ�ֵΪ360�ı���ʱ������������
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	int _angle_begin = angle_begin, _angle_end = angle_end;
	while (_angle_begin < 0)
		_angle_begin += 360;
	while (_angle_end > 720)
		_angle_end -= 360;
	while (_angle_begin >= _angle_end)
		_angle_end += 360;//ʹbegin��end������360��
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
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	hdc_sector(point_x, point_y, radius, 0, 0, filled, thickness, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ����Բ
  ���������const int point_x			��Բ�ĵ�x
			const int point_y			��Բ�ĵ�y
			const int radius_a			��ƽ����X��İ뾶
			const int radius_b			��ƽ����Y��İ뾶
			const int rotation_angles	����Բ��Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			����ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
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
