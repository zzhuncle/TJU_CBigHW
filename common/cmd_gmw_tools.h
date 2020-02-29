/* 1853790 ׯ�� ��ǿ */
#pragma once
#include <iostream>
#include"cmd_console_tools.h"
using namespace std;

/* ����״̬����Ϣ */
#define TOP_STATUS_LINE			0	//��״̬����λ�ù̶�����Ϸ�������������Ϸ���������б꣬�����б�֮�ϣ�
#define LOWER_STATUS_LINE		1   //��״̬����λ�ù̶�����Ϸ�������������·�

/* ��������״̬����Ϣ�Ľṹ�� */
typedef struct _status_line_info_ {
	/* �����涨����ǿ�ͨ�����ú����ı��ֵ����׼�޸ĳ�Ա���� */

	/* �Ƿ�������״̬�� */
	bool is_top_status_line;
	bool is_lower_status_line;

	/* ��״̬����λ�� */
	int top_start_x;
	int top_start_y;

	/* ��״̬��������Ϣ��ɫ */
	int top_normal_bgcolor;
	int top_normal_fgcolor;

	/* ��״̬����Ŀ��Ϣ��ɫ */
	int top_catchy_bgcolor;
	int top_catchy_fgcolor;

	/* ��״̬����λ�� */
	int lower_start_x;
	int lower_start_y;

	/* ��״̬��������Ϣ��ɫ */
	int lower_normal_bgcolor;
	int lower_normal_fgcolor;

	/* ��״̬����Ŀ��Ϣ��ɫ */
	int lower_catchy_bgcolor;
	int lower_catchy_fgcolor;

	/* �����������������Ϊ��Ҫ��ֵ����Щֵ����ͨ�����ú���ֱ�����ã��������ݸ�����Ϣ���״̬���Ŀ�ȣ����ڿ�������ʾ״̬����Ϣʱ��Ҫ����ָ����ȣ��� */


} STATUS_LINE_INFO;

/* �洢��Ϸ����ܱ߿���Ϣ���ַ�����ĳ��ȣ���β�㣩
   ע�����ַ����������ָ���ԭ����Ϊ�˽������Դ������ļ��ж� */
#define CFI_LEN		3

   /* ����������Ϸ���������Ľṹ�� */
typedef struct _console_frame_info_ {
	/* �����涨����ǿ�ͨ�����ú����ı��ֵ����׼�޸ĳ�Ա���� */

	/* ������Ϸ���������ĸ����Ԫ�ص���״
	�X �T �h �T �[
	�U          �U
	�c �T �� �T �f
	�U          �U
	�^ �T �k �T �a */
	char top_left[CFI_LEN];			// "�X","��"
	char lower_left[CFI_LEN];		// "�^","��"
	char top_right[CFI_LEN];			// "�[","��"
	char lower_right[CFI_LEN];		// "�a","��"
	char h_normal[CFI_LEN];			// "�T"	//Horizontal,"��"
	char v_normal[CFI_LEN];			// "�U"	//Vertical,"��"
	char h_top_separator[CFI_LEN];	// �j","��"
	char h_lower_separator[CFI_LEN];	// �m","��"
	char v_left_separator[CFI_LEN];	// "�d","��"
	char v_right_separator[CFI_LEN];	// "�g","��"
	char mid_separator[CFI_LEN];		// "�p","��"

	/* ��Ϸ���������ı���ɫ��ǰ��ɫ */
	int bgcolor;
	int fgcolor;

	/* ÿ��ɫ��Ŀ�Ⱥ͸߶�(ע�⣺��ȱ�����2�ı���) */
	int block_width;
	int block_high;

	/* ɫ��֮���Ƿ���Ҫ�ָ��� */
	bool separator;

	/* �����������������Ϊ��Ҫ��ֵ����Щֵ����ͨ�����ú���ֱ�����á� */

} CONSOLE_FRAME_INFO;

/* �洢ɫ��߿���Ϣ���ַ�����ĳ��ȣ���β�㣩
   ע�����ַ����������ָ���ԭ����Ϊ�˽������Դ������ļ��ж� */
#define CBI_LEN					3

   /* ��������ɫ����Ϣ�Ľṹ�� */
typedef struct _console_block_info_ {
	/* �����涨����ǿ�ͨ�����ú����ı��ֵ����׼�޸ĳ�Ա���� */

	/* ����С�߿�ĸ����Ԫ�ص���״
	�X �T �[
	�U    �U
	�^ �T �a */
	char top_left[CBI_LEN];			// "�X"
	char lower_left[CBI_LEN];		// "�^"
	char top_right[CBI_LEN];			// "�["
	char lower_right[CBI_LEN];		// "�a"
	char h_normal[CBI_LEN];			// "�T"	//Horizontal
	char v_normal[CBI_LEN];			// "�U"	//Vertical

	bool block_border;				//ɫ���Ƿ���Ҫ�߿�

	/* �����������������Ϊ��Ҫ��ֵ����Щֵ����ͨ�����ú���ֱ�����á� */

} CONSOLE_BLOCK_INFO;

/* ���� BLOCK_DISPLAY_INFO �����б�ʾ �հ׵�����ֵ/����������ֵ */
#define BDI_VALUE_BLANK		0		//����հ׵�����ֵ
#define BDI_VALUE_END		-999		//�������������ֵ

/* ����ɫ����ʾʱ���ڲ������intֵ���������ʾͼ�εĶ�Ӧ��ϵ
   ע���˽ṹ�������ֵ�ڲ��������ж��壬������Ϸ�Ĳ�ͬ����ͬ */
typedef struct _block_display_info_ {
	const int   value;	//Ҫ��ʾ���ڲ�����ֵ
	const int   bgcolor;	//-1��ʾ����Ϸ���򱳾�ɫ
	const int   fgcolor;	//-1��ʾ����Ϸ����ǰ��ɫ
	const char *content;	//�ڲ�����ֵ��Ӧ��ͼ�Σ����ΪNULL����ֱ����ʾ�ڲ������ֵ��
	_block_display_info_() :value(-1), bgcolor(-1), fgcolor(-1), content(NULL) {}
	_block_display_info_(const int v,const int g,const int f,const char *c) :value(v), bgcolor(g), fgcolor(f), content(c) {}
} BLOCK_DISPLAY_INFO;

/* ������������/�ֺ���Ϣ�Ľṹ�� */
#define		FONTNAME_LEN	12	//�������Ƶ���󳤶ȣ���β�㣩
typedef struct _console_font_type_ {
	char font_type[FONTNAME_LEN];
	int  font_size_width;
	int  font_size_high;
} CONSOLE_FONT_TYPE;

/* ������ʱ������ */
#define DELAY_OF_DRAW_FRAME		0	//����Ϸ�����ʱ����ʱ
#define DELAY_OF_DRAW_BLOCK		1	//��ɫ��ʱ����ʱ
#define DELAY_OF_BLOCK_MOVED		2	//ɫ���ƶ�ʱ����ʱ

#define BLOCK_MOVED_DELAY_MS		15	//ɫ���ƶ���ȱʡ��ʱ����λms����������ȱʡΪ0��

/* ����������������ȫ����Ϣ�Ľṹ�� */
typedef struct _console_graphics_info_ {
	/* �����涨����ǿ�ͨ�����ú����ı��ֵ����׼�޸ĳ�Ա���� */

	/* �������Ϣ */
	CONSOLE_FRAME_INFO CFI;

	/* ɫ����Ϣ */
	CONSOLE_BLOCK_INFO CBI;

	/* ״̬����Ϣ */
	STATUS_LINE_INFO SLI;

	/* ������Ϣ */
	CONSOLE_FONT_TYPE CFT;

	/* ����ͼ�ν��������������Ҫ�Ķ��������� */
	int extern_up_lines;
	int extern_down_lines;
	int extern_left_cols;
	int extern_right_cols;

	/* ��Ϸ��������������ɫ��������� */
	int row_num;
	int col_num;

	/* ����ͼ�ν��棨������ܡ�����״̬�����к��бꡢ�������У��ı���ɫ��ǰ��ɫ */
	int area_bgcolor;
	int area_fgcolor;

	/* �Ƿ���Ҫ����״̬��
	   - ע��true-��Ҫ false-����Ҫ
	   -    �������Ϊfalse����ʹ������״̬����Ϣ��ʾ������Ҳ����ʾ���ݣ� */
	bool top_status_line;
	bool lower_status_line;

	/* ��ӡ�����ʱ�Ƿ���Ҫ�к��б� */
	bool draw_frame_with_row_no;
	bool draw_frame_with_col_no;

	/* ��ʱʱ������ */
	int delay_of_draw_frame;		//������ǵ���ʱ
	int delay_of_draw_block;		//����Ϸ�����ʱ
	int delay_of_block_moved;	//��Ϸ���ƶ�����ʱ

	/* �����������������Ϊ��Ҫ��ֵ����Щֵ����ͨ�����ú���ֱ�����á� */
	/* ����ͼ�ν�������������Ĳο�������ʼλ��(���Ͻǣ�
	   - ע����Ϸ����ܣ������а���m��n�е�ɫ�������⣬������״̬����0/1�У�/��״̬����0/1�У�/�к���ʾ��0/2�У�/�б���ʾ����0/1�У�
	   -     ������ֵ������ͬ������£�start_x/start_y��ֵ�ǲ�ͬ�� */
	int start_x;
	int start_y;

	/* cmd���ڵĴ�С */
	int lines;		//Ϊ�˸��������뷨��ʾ�м����н�������ʾ��Ϣ���ռ䣬Ҫ���ڼ���õ��Ľ�������ϣ��϶���ռ�+��״̬��+�б���ʾ+������+��״̬����+ 4��1�������뷨��ʾ��+3Ԥ�����У�
	int cols;

	/* ������Լ���Ҫ����Ŀ */

} CONSOLE_GRAPHICS_INFO;

/* �˵�ѡ��Ľṹ�� */
struct Fun_Call {
	char option;
	int(*pf0)(int(*p1)[9], int(*p2)[9], CONSOLE_GRAPHICS_INFO *pCGI,const int mood);
};

/* ����ɫ��������ƶ����� */
#define DOWN_TO_UP		0
#define UP_TO_DOWN		1
#define RIGHT_TO_LEFT	2
#define LEFT_TO_RIGHT	3

/* �����������ڵ�ĳЩ���� */
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col);
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor = COLOR_BLACK, const int fgcolor = COLOR_WHITE, const bool cascade = true);
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname = "Terminal", const int fs_high = 16, const int fs_width = 8);
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms);
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines = 0, const int down_lines = 0, const int left_cols = 0, const int right_cols = 0);

/* ����������Ϸ����ܵĲ��� */
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type);
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left = "�X", const char *lower_left = "�^", const char *top_right = "�[",
	const char *lower_right = "�a", const char *h_normal = "�T", const char *v_normal = "�U", const char *h_top_separator = "�j",
	const char *h_lower_separator = "�m", const char *v_left_separator = "�d", const char *v_right_separator = "�g", const char *mid_separator = "�p");
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width = 2, const int block_high = 1, const bool separator = true);
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor = -1, const int fgcolor = -1);

/* ����ÿ��ɫ��Ĳ��� */
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int def_type);
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left = "�X", const char *lower_left = "�^", const char *top_right = "�[",
	const char *lower_right = "�a", const char *h_normal = "�T", const char *v_normal = "�U");
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off = false);

/* �����Ƿ���Ҫ����״̬������ɫ���ֿ�����������Ŀ����color�����ڸı�������Ļ��ɫ����Ҫ�������� */
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off = true);
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor = -1, const int normal_fgcolor = -1, const int catchy_bgcolor = -1, const int catchy_fgcolor = -1);

/* �����Ƿ���Ҫ�к��б� */
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off = false);
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off = false);

/* ��ȱʡֵ��һ��ȫ�ֳ�ʼ�� */
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row = 10, const int col = 10, const int bgcolor = COLOR_BLACK, const int fgcolor = COLOR_WHITE);
/* ������ - ��ӡ CONSOLE_GRAPHICS_INFO �ṹ���еĸ���Աֵ */
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI);

/* ����Ϸ����� */
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI);
/* ������״̬������ʾ��Ϣ */
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg = NULL);
/* ��һ��ɫ�� */
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi);
/* �ƶ�һ��ɫ�� */
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance);
/* �����̺���� */
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line = true);

void output_top_info(CONSOLE_GRAPHICS_INFO * pCGI, const int score);
int paint_whole__plane(const int row1, const int col1, const int row2, const int col2, CONSOLE_GRAPHICS_INFO * pCGI,
	const BLOCK_DISPLAY_INFO bdi[], int plane[][10], int mood);
