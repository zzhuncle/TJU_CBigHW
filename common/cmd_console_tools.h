/* 1853790 ׯ�� ��ǿ */
#pragma once
#include<iostream>
#include <Windows.h>
using namespace std;

/* ������ɫ���ú궨��ȡ�����֣�������䣩 */
#define COLOR_BLACK		0	//��
#define COLOR_BLUE		1	//��
#define COLOR_GREEN		2	//��
#define COLOR_CYAN		3	//��
#define COLOR_RED		4	//��
#define COLOR_PINK		5	//��
#define COLOR_YELLOW	6	//��
#define COLOR_WHITE		7	//��
#define COLOR_HBLACK	8	//����
#define COLOR_HBLUE		9	//����
#define COLOR_HGREEN	10	//����
#define COLOR_HCYAN		11	//����
#define COLOR_HRED		12	//����
#define COLOR_HPINK		13	//����
#define COLOR_HYELLOW	14	//����
#define COLOR_HWHITE	15	//����

/* ���������̲������� */
#define CCT_MOUSE_EVENT				0
#define CCT_KEYBOARD_EVENT			1

/* �����ĸ������ */
#define KB_ARROW_UP					72
#define KB_ARROW_DOWN				80
#define KB_ARROW_LEFT				75
#define KB_ARROW_RIGHT				77

/* �������Ĳ��� */
#define MOUSE_NO_ACTION						0x0000	//��������
#define MOUSE_ONLY_MOVED						0x0001	//����ƶ�
#define MOUSE_LEFT_BUTTON_CLICK				0x0002	//�������
#define MOUSE_LEFT_BUTTON_DOUBLE_CLICK		0x0004	//˫�����
#define MOUSE_RIGHT_BUTTON_CLICK				0x0008	//�����Ҽ�
#define MOUSE_RIGHT_BUTTON_DOUBLE_CLICK		0x0010	//˫���Ҽ�
#define MOUSE_LEFTRIGHT_BUTTON_CLICK			0x0020	//ͬʱ�������Ҽ�
#define MOUSE_WHEEL_CLICK					0x0040	//���ֱ�����
#define MOUSE_WHEEL_MOVED_UP					0x0080	//���������ƶ�
#define MOUSE_WHEEL_MOVED_DOWN				0x0100	//���������ƶ�

/* ���������̬ */
#define CURSOR_VISIBLE_FULL			0	//�����ʾ��ȫ��ɫ��
#define CURSOR_VISIBLE_HALF			1	//�����ʾ�����ɫ��
#define CURSOR_VISIBLE_NORMAL		2	//�����ʾ�����ߣ�ȱʡΪ�˷�ʽ��
#define CURSOR_INVISIBLE				3	//��겻��ʾ

/* cmd_console_tools�µĺ������� */

/* ��������ɫ���á�������á��ַ�/�ַ�����ʾ */
void cls      (void);
void setcolor (const int bg_color = COLOR_BLACK, const int fg_color = COLOR_WHITE);
void getcolor (int &bg_color, int &fg_color);
void gotoxy   (const int X, const int Y);
void getxy    (int &x, int &y);
void setcursor(const int options);
void showch   (const int X, const int Y, const char ch, const int bg_color = COLOR_BLACK, const int fg_color = COLOR_WHITE, const int rpt = 1);
void showstr  (const int X, const int Y, const char *str, const int bg_color = COLOR_BLACK, const int fg_color = COLOR_WHITE, int rpt = 1, int max_len=-1);
void showint  (const int X, const int Y, const int num, const int bg_color = COLOR_BLACK, const int fg_color = COLOR_WHITE, const int rpt = 1);

/* �봰�ڴ�С�йصĺ��� */
void setconsoleborder(int set_cols, int set_lines, int set_buffer_cols = -1, int set_buffer_lines = -1);
void getconsoleborder(int &cols, int &lines, int &buffer_cols, int &buffer_lines);

/* �б������йصĺ��� */
void getconsoletitle (char *title, int maxbuflen);
void setconsoletitle (const char *title);

/* ���������йصĺ��� */
void enable_mouse           (void);
void disable_mouse          (void);
int  read_keyboard_and_mouse(int &MX, int &MY, int &MAction, int &keycode1, int &keycode2);

/* �����弰�ֺ������йصĺ��� */
int  getfontinfo(void);
void setfontsize(const char *fontname, const int high, const int width = 0);

/* 90-b0��90-b1�Ĺ������� */
void clear_cin_buf(void);
void clear_input_line(int X, int Y, char *filled);
void drawarray(int(*p1)[9], int(*p2)[9], int lines, int cols, const int mood = 1);
void dispaly(struct model_info &model);
void draw_border(struct border_info &bor);
void draw_ball(struct ball_info &ball);
int move_ball(struct move_ball_info &move);
int get_int(const char *prompt, const int min_value, const int max_value, const int def_value = 0x80000000, int X = -1, int Y = -1);
void wait_for_str(const char *wait_str, const char *prompt = NULL, int X = -1, int Y = -1);
void wait_for_key(const char *prompt = NULL,
	const int X = -1,
	const int Y = -1,
	const int wait_key_value = '\r');
void tj_swap(int &a, int &b);


/* ״̬���Ľṹ�� */
struct model_info{
	int x;
	int y;
	int is_up;
	int n_r;
	int n_c;
};

/* �߿�Ľṹ�� */
struct border_info {
	int x;
	int y;
	int n_r;
	int n_c;
	int is_mark;
	int is_empty;
	int is_gapline;
};

/* С��Ľṹ�� */
struct ball_info {
	int mode;
	int front;
	int back;
	int n_r;
	int n_c;
	int is_mark;
	int is_gapline;
	int extra;//Ԥ��ͳ��ʱʹ��
	int lines;//�߿������
	int i;    //���������
	int move_mood; //���ĸ������ƶ�ʱ�������м����
};

/* �ƶ�����Ľṹ�� */
struct move_ball_info {
	int color;//С����ɫ
	int x;//С�������
	int y;//С��������
	int mood;//�ƶ�ģʽ��1�����£�2�ϣ�3�ң�4��
	int n;//�ƶ�����
	int is_mark;//�Ƿ��б߿���������Ƿ�ʵ��
};
