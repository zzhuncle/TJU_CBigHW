/* 1853790 庄镇华 数强 */
#pragma once
#include <iostream>
#include"cmd_console_tools.h"
using namespace std;

/* 定义状态栏信息 */
#define TOP_STATUS_LINE			0	//上状态栏，位置固定在游戏主框架区域的最上方（如果有列标，则在列标之上）
#define LOWER_STATUS_LINE		1   //上状态栏，位置固定在游戏主框架区域的最下方

/* 定义描述状态栏信息的结构体 */
typedef struct _status_line_info_ {
	/* 【下面定义的是可通过设置函数改变的值，不准修改成员名】 */

	/* 是否有上下状态栏 */
	bool is_top_status_line;
	bool is_lower_status_line;

	/* 上状态栏的位置 */
	int top_start_x;
	int top_start_y;

	/* 上状态栏正常信息配色 */
	int top_normal_bgcolor;
	int top_normal_fgcolor;

	/* 上状态栏醒目信息配色 */
	int top_catchy_bgcolor;
	int top_catchy_fgcolor;

	/* 下状态栏的位置 */
	int lower_start_x;
	int lower_start_y;

	/* 上状态栏正常信息配色 */
	int lower_normal_bgcolor;
	int lower_normal_fgcolor;

	/* 上状态栏醒目信息配色 */
	int lower_catchy_bgcolor;
	int lower_catchy_fgcolor;

	/* 【下面允许添加你认为需要的值，这些值不能通过设置函数直接设置（例：根据各种信息算出状态栏的宽度，用于控制在显示状态栏信息时不要超过指定宽度）】 */


} STATUS_LINE_INFO;

/* 存储游戏主框架边框信息的字符数组的长度（含尾零）
   注：用字符数组而不用指针的原因是为了将来可以从配置文件中读 */
#define CFI_LEN		3

   /* 定义描述游戏主框架区域的结构体 */
typedef struct _console_frame_info_ {
	/* 【下面定义的是可通过设置函数改变的值，不准修改成员名】 */

	/* 定义游戏主框架区域的各组成元素的形状
	╔ ═ ╤ ═ ╗
	║          ║
	╟ ═ ┼ ═ ╢
	║          ║
	╚ ═ ╧ ═ ╝ */
	char top_left[CFI_LEN];			// "╔","┏"
	char lower_left[CFI_LEN];		// "╚","┗"
	char top_right[CFI_LEN];			// "╗","┓"
	char lower_right[CFI_LEN];		// "╝","┛"
	char h_normal[CFI_LEN];			// "═"	//Horizontal,"━"
	char v_normal[CFI_LEN];			// "║"	//Vertical,"┃"
	char h_top_separator[CFI_LEN];	// ╦","┳"
	char h_lower_separator[CFI_LEN];	// ╩","┻"
	char v_left_separator[CFI_LEN];	// "╠","┣"
	char v_right_separator[CFI_LEN];	// "╣","┫"
	char mid_separator[CFI_LEN];		// "╬","╋"

	/* 游戏主框架区域的背景色和前景色 */
	int bgcolor;
	int fgcolor;

	/* 每个色块的宽度和高度(注意：宽度必须是2的倍数) */
	int block_width;
	int block_high;

	/* 色块之间是否需要分隔线 */
	bool separator;

	/* 【下面允许添加你认为需要的值，这些值不能通过设置函数直接设置】 */

} CONSOLE_FRAME_INFO;

/* 存储色狂边框信息的字符数组的长度（含尾零）
   注：用字符数组而不用指针的原因是为了将来可以从配置文件中读 */
#define CBI_LEN					3

   /* 定义描述色块信息的结构体 */
typedef struct _console_block_info_ {
	/* 【下面定义的是可通过设置函数改变的值，不准修改成员名】 */

	/* 定义小边框的各组成元素的形状
	╔ ═ ╗
	║    ║
	╚ ═ ╝ */
	char top_left[CBI_LEN];			// "╔"
	char lower_left[CBI_LEN];		// "╚"
	char top_right[CBI_LEN];			// "╗"
	char lower_right[CBI_LEN];		// "╝"
	char h_normal[CBI_LEN];			// "═"	//Horizontal
	char v_normal[CBI_LEN];			// "║"	//Vertical

	bool block_border;				//色块是否需要边框

	/* 【下面允许添加你认为需要的值，这些值不能通过设置函数直接设置】 */

} CONSOLE_BLOCK_INFO;

/* 定义 BLOCK_DISPLAY_INFO 数组中表示 空白的特殊值/结束的特殊值 */
#define BDI_VALUE_BLANK		0		//代表空白的特殊值
#define BDI_VALUE_END		-999		//代表结束的特殊值

/* 定义色块显示时，内部数组的int值与界面上显示图形的对应关系
   注：此结构体数组的值在测试用例中定义，随着游戏的不同而不同 */
typedef struct _block_display_info_ {
	const int   value;	//要显示的内部数组值
	const int   bgcolor;	//-1表示用游戏区域背景色
	const int   fgcolor;	//-1表示用游戏区域前景色
	const char *content;	//内部数组值对应的图形（如果为NULL，则直接显示内部数组的值）
	_block_display_info_() :value(-1), bgcolor(-1), fgcolor(-1), content(NULL) {}
	_block_display_info_(const int v,const int g,const int f,const char *c) :value(v), bgcolor(g), fgcolor(f), content(c) {}
} BLOCK_DISPLAY_INFO;

/* 定义描述字体/字号信息的结构体 */
#define		FONTNAME_LEN	12	//字体名称的最大长度（含尾零）
typedef struct _console_font_type_ {
	char font_type[FONTNAME_LEN];
	int  font_size_width;
	int  font_size_high;
} CONSOLE_FONT_TYPE;

/* 定义延时的种类 */
#define DELAY_OF_DRAW_FRAME		0	//画游戏主框架时的延时
#define DELAY_OF_DRAW_BLOCK		1	//画色块时的延时
#define DELAY_OF_BLOCK_MOVED		2	//色块移动时的延时

#define BLOCK_MOVED_DELAY_MS		15	//色块移动的缺省延时，单位ms（其余两个缺省为0）

/* 定义描述整个窗口全部信息的结构体 */
typedef struct _console_graphics_info_ {
	/* 【下面定义的是可通过设置函数改变的值，不准修改成员名】 */

	/* 主框架信息 */
	CONSOLE_FRAME_INFO CFI;

	/* 色块信息 */
	CONSOLE_BLOCK_INFO CBI;

	/* 状态栏信息 */
	STATUS_LINE_INFO SLI;

	/* 字体信息 */
	CONSOLE_FONT_TYPE CFT;

	/* 整个图形界面的上下左右需要的额外行列数 */
	int extern_up_lines;
	int extern_down_lines;
	int extern_left_cols;
	int extern_right_cols;

	/* 游戏主框架区域包含的色块的行列数 */
	int row_num;
	int col_num;

	/* 整个图形界面（含主框架、上下状态栏、行号列标、附加行列）的背景色和前景色 */
	int area_bgcolor;
	int area_fgcolor;

	/* 是否需要上下状态栏
	   - 注：true-需要 false-不需要
	   -    如果设置为false，则即使调用了状态新信息显示函数，也不显示内容） */
	bool top_status_line;
	bool lower_status_line;

	/* 打印主框架时是否需要行号列标 */
	bool draw_frame_with_row_no;
	bool draw_frame_with_col_no;

	/* 延时时间设置 */
	int delay_of_draw_frame;		//画外框是的延时
	int delay_of_draw_block;		//画游戏块的延时
	int delay_of_block_moved;	//游戏块移动的延时

	/* 【下面允许添加你认为需要的值，这些值不能通过设置函数直接设置】 */
	/* 整个图形界面的主框架区域的参考坐标起始位置(左上角）
	   - 注：游戏主框架，除了有包含m行n列的色块的外框外，还有上状态栏（0/1行）/下状态栏（0/1行）/行号显示（0/2列）/列标显示区域（0/1行）
	   -     在上述值各不相同的情况下，start_x/start_y的值是不同的 */
	int start_x;
	int start_y;

	/* cmd窗口的大小 */
	int lines;		//为了给中文输入法提示行及运行结束的提示信息留空间，要求在计算得到的结果基础上（上额外空间+上状态栏+列标显示+主区域+下状态栏）+ 4（1中文输入法提示行+3预留空行）
	int cols;

	/* 可添加自己需要的项目 */

} CONSOLE_GRAPHICS_INFO;

/* 菜单选项的结构体 */
struct Fun_Call {
	char option;
	int(*pf0)(int(*p1)[9], int(*p2)[9], CONSOLE_GRAPHICS_INFO *pCGI,const int mood);
};

/* 定义色块的四种移动方向 */
#define DOWN_TO_UP		0
#define UP_TO_DOWN		1
#define RIGHT_TO_LEFT	2
#define LEFT_TO_RIGHT	3

/* 设置整个窗口的某些参数 */
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col);
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor = COLOR_BLACK, const int fgcolor = COLOR_WHITE, const bool cascade = true);
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname = "Terminal", const int fs_high = 16, const int fs_width = 8);
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms);
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines = 0, const int down_lines = 0, const int left_cols = 0, const int right_cols = 0);

/* 设置整个游戏主框架的参数 */
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type);
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left = "╔", const char *lower_left = "╚", const char *top_right = "╗",
	const char *lower_right = "╝", const char *h_normal = "═", const char *v_normal = "║", const char *h_top_separator = "╦",
	const char *h_lower_separator = "╩", const char *v_left_separator = "╠", const char *v_right_separator = "╣", const char *mid_separator = "╬");
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width = 2, const int block_high = 1, const bool separator = true);
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor = -1, const int fgcolor = -1);

/* 设置每个色块的参数 */
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int def_type);
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left = "╔", const char *lower_left = "╚", const char *top_right = "╗",
	const char *lower_right = "╝", const char *h_normal = "═", const char *v_normal = "║");
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off = false);

/* 设置是否需要上下状态栏及颜色，分开两个函数的目的是color可能在改变整个屏幕颜色是需要级联调用 */
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off = true);
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor = -1, const int normal_fgcolor = -1, const int catchy_bgcolor = -1, const int catchy_fgcolor = -1);

/* 设置是否需要行号列标 */
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off = false);
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off = false);

/* 用缺省值做一个全局初始化 */
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row = 10, const int col = 10, const int bgcolor = COLOR_BLACK, const int fgcolor = COLOR_WHITE);
/* 调试用 - 打印 CONSOLE_GRAPHICS_INFO 结构体中的各成员值 */
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI);

/* 画游戏主框架 */
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI);
/* 在上下状态栏上显示信息 */
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg = NULL);
/* 画一个色块 */
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi);
/* 移动一个色块 */
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance);
/* 读键盘和鼠标 */
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line = true);

void output_top_info(CONSOLE_GRAPHICS_INFO * pCGI, const int score);
int paint_whole__plane(const int row1, const int col1, const int row2, const int col2, CONSOLE_GRAPHICS_INFO * pCGI,
	const BLOCK_DISPLAY_INFO bdi[], int plane[][10], int mood);
