/* 1853790 庄镇华 数强 */
#define _CRT_SECURE_NO_WARNINGS
#include<iomanip>
#include <iostream>
#include <climits>
#include <conio.h>
#include "cmd_console_tools.h"
#include "cmd_gmw_tools.h"
using namespace std;

/* --------------------------------------------------
		此处可以给出需要的静态全局变量（尽可能少，最好没有）、静态全局只读变量/宏定义（个数不限）等
   -------------------------------------------------- */

/* --------------------------------------------------
   此处可以给出需要的内部辅助工具函数
   1、函数名不限，建议为 gmw_inner_*
   2、个数不限
   3、必须是static函数，确保只在本源文件中使用
 -------------------------------------------------- */

//检查表符
static void gmw_inner_check_string(char *change,const char * pstring)
{
	if (pstring == NULL || strlen(pstring) == 0)
		strcpy(change, "  ");
	if (strlen(pstring) >= 3)
		strncpy(change, pstring, 2);
	if (strlen(pstring) == 1) {
		strcpy(change,pstring);
		strcpy(change, " ");
	}
	else
		strcpy(change,pstring);
}

//定长输出
static int gmw_inner_output(const char *s, int l, const bool is_full = true)
{
	int i;
	if (s == NULL)
		return 0;
	for (i = 0; i < l && s[i]; i++) {
		if (s[i] < 0) {
			if (i + 1 < l) {
				cout << s[i] << s[i+1];
				i = i + 1;
				continue;
			}
			else
				break;
		}
		else
			cout << s[i];
	}
	if (is_full)
		for (; i < l; i++)
			cout << " ";
	return 0;
}

//改变内值
static void gmw_inner_change_value(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines
		+ (pCGI->CFI.block_high * pCGI->row_num + pCGI->CFI.separator * (pCGI->row_num - 1))
		+ pCGI->top_status_line + pCGI->lower_status_line + pCGI->draw_frame_with_row_no + 6;
	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols
		+ (pCGI->CFI.block_width * pCGI->col_num + pCGI->CFI.separator * (pCGI->col_num - 1) * 2)
		+ 2 * pCGI->draw_frame_with_col_no + 5;
	pCGI->start_x = pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2;
	pCGI->start_y = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no;
	pCGI->SLI.top_start_x = (pCGI->start_x - 2 * pCGI->draw_frame_with_col_no > 0 ? pCGI->start_x - 2 * pCGI->draw_frame_with_col_no : 0);
	pCGI->SLI.top_start_y = (pCGI->start_y - pCGI->draw_frame_with_row_no - 1 > 0 ? pCGI->start_y - pCGI->draw_frame_with_row_no - 1 : 0);
	pCGI->SLI.lower_start_x = pCGI->SLI.top_start_x;
	pCGI->SLI.lower_start_y = pCGI->start_y + (pCGI->CFI.block_high * pCGI->row_num +
		pCGI->CFI.separator * (pCGI->row_num - 1)) + 2;
	if (pCGI->SLI.lower_start_y < 0)
		pCGI->SLI.lower_start_y = 0;
}

//定点画图
static int gmw_inner_piant(const CONSOLE_GRAPHICS_INFO *const pCGI, const int x, const int y,
	const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
	int fgcolor, bgcolor;
	for (int i = 0; bdi[i].value != BDI_VALUE_END; i++) {
		if (bdi[i].value == bdi_value) {
			fgcolor = (bdi[i].fgcolor == -1 ? pCGI->CFI.fgcolor : bdi[i].fgcolor);
			bgcolor = (bdi[i].bgcolor == -1 ? pCGI->CFI.bgcolor : bdi[i].bgcolor);
			setcolor(bgcolor, fgcolor);
			//有内容
			if (bdi_value != BDI_VALUE_BLANK) {
				//绘制外框
				if (pCGI->CBI.block_border){
				gotoxy(x, y);
				cout << (pCGI->CBI.block_border ? pCGI->CBI.top_left : "  ");
				//getchar();
				for (int i = 0; i < pCGI->CFI.block_width / 2 - 2; i++)
					cout << (pCGI->CBI.block_border ? pCGI->CBI.h_normal : "  ");
				cout << (pCGI->CBI.block_border ? pCGI->CBI.top_right : "  ");
				//getchar();

				for (int i = 1; i < pCGI->CFI.block_high - 1; i++) {
					gotoxy(x, y + i);
					cout << (pCGI->CBI.block_border ? pCGI->CBI.v_normal : "  ");
					//getchar();
					for (int j = 0; j < pCGI->CFI.block_width / 2 - 2; j++)
						cout << "  ";
					cout << (pCGI->CBI.block_border ? pCGI->CBI.v_normal : "  ");
					//getchar();
				}
				gotoxy(x, y + pCGI->CFI.block_high - 1);
				cout << (pCGI->CBI.block_border ? pCGI->CBI.lower_left : "  ");
				//getchar();
				for (int i = 0; i < pCGI->CFI.block_width / 2 - 2; i++)
					cout << (pCGI->CBI.block_border ? pCGI->CBI.h_normal : "  ");
				cout << (pCGI->CBI.block_border ? pCGI->CBI.lower_right : "  ");
				//getchar();
				}
				else 
					for (int i = 0; i < pCGI->CFI.block_high; i++) {
						gotoxy(x, y + i);
						for (int j = 0; j < pCGI->CFI.block_width / 2; j++)
							cout << "  ";
					}
				//绘制内容
				//内容是纯数字
				int nl = 1, n;
				gotoxy(x + 2 * pCGI->CBI.block_border +
					(pCGI->CFI.block_width - 4 * pCGI->CBI.block_border - nl) / 2,
					y + pCGI->CBI.block_border +
					(pCGI->CFI.block_high - 2 * pCGI->CBI.block_border) / 2);
				if (bdi[i].content == NULL) {
					//求出数字的位数
					n = bdi[i].value;
					while (n /= 10)
						nl++;
					cout << bdi[i].value;
				}
				//内容是图案
				else {
					nl = (int)(strlen(bdi[i].content));
					cout << bdi[i].content;
				}
			}
			//无内容
			else {
				for (int i = 0; i < pCGI->CFI.block_high; i++) {
					gotoxy(x, y + i);
					for (int j = 0; j < pCGI->CFI.block_width / 2; j++)
						cout << "  ";
				}
			}
		}
	}
	return 0;
}

//移动一次
static void gmw_inner_move(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no,
	const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction)
{
	int x, y;
	x = pCGI->start_x + col_no * 2 * pCGI->CFI.separator + 2 + col_no * pCGI->CFI.block_width;
	y = pCGI->start_y + row_no * pCGI->CFI.separator + 1 + row_no * pCGI->CFI.block_high;
	if (direction == LEFT_TO_RIGHT) {
		for (int i = 1; i <= pCGI->CFI.block_width; i++) {
			gmw_inner_piant(pCGI, x + i, y, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_high; j++) {
				gotoxy(x + i - 1, y + j);
				cout << " ";
				//getchar();
			}
			Sleep(pCGI->delay_of_block_moved);
		}
		if (pCGI->CFI.separator) {
			gmw_inner_piant(pCGI, x + pCGI->CFI.block_width + 2, y, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_high; j++) {
				gotoxy(x + pCGI->CFI.block_width + 2 - 2, y + j);
				cout << pCGI->CFI.v_normal;
				Sleep(pCGI->delay_of_block_moved);
				//getchar();
			}
		}
	}
	if (direction == RIGHT_TO_LEFT) {
		for (int i = 1; i <= pCGI->CFI.block_width; i++) {
			gmw_inner_piant(pCGI, x - i, y, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_high; j++) {
				gotoxy(x - i + pCGI->CFI.block_width, y + j);
				cout << " ";
				//getchar();
			}
			Sleep(pCGI->delay_of_block_moved);
		}
		if (pCGI->CFI.separator) {
			gmw_inner_piant(pCGI, x - pCGI->CFI.block_width - 2, y, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_high; j++) {
				gotoxy(x - 2, y + j);
				cout << pCGI->CFI.v_normal;
				Sleep(pCGI->delay_of_block_moved);
				//getchar();
			}
		}
	}
	if (direction == UP_TO_DOWN) {
		for (int i = 1; i <= pCGI->CFI.block_high; i++) {
			gmw_inner_piant(pCGI, x, y + i, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_width; j++) {
				gotoxy(x + j, y + i - 1);
				cout << " ";
				//getchar();
			}
			Sleep(pCGI->delay_of_block_moved);
		}
		if (pCGI->CFI.separator) {
			gmw_inner_piant(pCGI, x, y + pCGI->CFI.block_high + 1, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_width; j += 2) {
				gotoxy(x + j, y + pCGI->CFI.block_high + 1 - 1);
				cout << pCGI->CFI.h_normal;
				Sleep(pCGI->delay_of_block_moved);
				//getchar();
			}
		}
	}
	if (direction == DOWN_TO_UP) {
		for (int i = 1; i <= pCGI->CFI.block_high; i++) {
			gmw_inner_piant(pCGI, x, y - i, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_width; j++) {
				gotoxy(x + j, y - i + pCGI->CFI.block_high);
				cout << " ";
				//getchar();
			}
			Sleep(pCGI->delay_of_block_moved);
		}
		if (pCGI->CFI.separator) {
			gmw_inner_piant(pCGI, x, y - pCGI->CFI.block_high - 1, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_width; j += 2) {
				gotoxy(x + j, y - 1);
				cout << pCGI->CFI.h_normal;
				Sleep(pCGI->delay_of_block_moved);
				//getchar();
			}
		}
	}
}

/* -----------------------------------------------
		实现下面给出的函数（函数声明不准动）
   ----------------------------------------------- */

   /***************************************************************************
	 函数名称：
	 功    能：设置游戏主框架的行列数
	 输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			   const int row						：行数(错误则为0，不设上限，人为保证正确性)
			   const int col						：列数(错误则为0，不设上限，人为保证正确性)
	 返 回 值：
	 说    明：1、指消除类游戏的矩形区域的行列值
			   2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
   ***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	pCGI->row_num = (row > 0 ? row : 0);
	pCGI->col_num = (col > 0 ? col : 0);
	gmw_inner_change_value(pCGI);
	return 0; //此句可根据需要修改····
}

/***************************************************************************
  函数名称：
  功    能：设置整个窗口（含游戏区、附加区在内的整个cmd窗口）的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int bg_color					：前景色（缺省COLOR_BLACK）
		   const int fg_color					：背景色（缺省COLOR_WHITE）
		   const bool cascade					：是否级联（缺省为true-级联）
  返 回 值：
  说    明：1、cascade = true时
				同步修改游戏主区域的颜色
				同步修改上下状态栏的正常文本的背景色和前景色，醒目文本的背景色（前景色不变）
			2、不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15
					前景色背景色的值一致导致无法看到内容
					前景色正好是状态栏醒目前景色，导致无法看到醒目提示
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	setcolor(bgcolor, fgcolor);
	if (cascade) {
		pCGI->CFI.bgcolor = bgcolor;
		pCGI->CFI.fgcolor = fgcolor;
		pCGI->SLI.lower_normal_bgcolor = bgcolor;
		pCGI->SLI.lower_normal_fgcolor = fgcolor;
		pCGI->SLI.top_normal_bgcolor = bgcolor;
		pCGI->SLI.top_normal_fgcolor = fgcolor;
		pCGI->SLI.lower_catchy_bgcolor = bgcolor;
		pCGI->SLI.top_catchy_bgcolor = bgcolor;
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置窗口的字体
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *fontname					：字体名称（只能是"Terminal"和"新宋体"两种，错误则返回-1，不改变字体）
		   const int fs_high					：字体高度（缺省及错误为16，不设其它限制，人为保证）
		   const int fs_width					：字体高度（缺省及错误为8，不设其它限制，人为保证）
  返 回 值：
  说    明：1、与cmd_console_tools中的setfontsize相似，目前只支持“点阵字体”和“新宋体”
			2、若设置其它字体则直接返回，保持原字体设置不变
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	if (strcmp(fontname, "Terminal") && strcmp(fontname, "新宋体"))
		return -1;
	strcpy((pCGI->CFT).font_type, fontname);
	pCGI->CFT.font_size_high = fs_high;
	pCGI->CFT.font_size_width = fs_width;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置延时
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type						：延时的类型（目前为3种）
		   const int delay_ms					：以ms为单位的延时
			   画边框的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   画色块的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   色块移动的延时：BLOCK_MOVED_DELAY_MS ~ 不设上限，人为保证正确（ <BLOCK_MOVED_DELAY_MS 则置 BLOCK_MOVED_DELAY_MS）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	if (type == DELAY_OF_DRAW_FRAME)
		pCGI->delay_of_draw_frame = (delay_ms > 0 ? delay_ms : 0);
	if (type == DELAY_OF_DRAW_BLOCK)
		pCGI->delay_of_draw_block = (delay_ms > 0 ? delay_ms : 0);
	if (type == DELAY_OF_BLOCK_MOVED)
		pCGI->delay_of_block_moved = (delay_ms > BLOCK_MOVED_DELAY_MS ? delay_ms : BLOCK_MOVED_DELAY_MS);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  输入参数：设置游戏主框架结构之外需要保留的额外区域
  功    能：CONSOLE_GRAPHICS_INFO *const pCGI	：
		   const int up_lines					：上部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int down_lines				：下部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int left_cols					：左边额外的列（缺省及错误为0，不设上限，人为保证）
		   const int right_cols				：右边额外的列（缺省及错误为0，不设上限，人为保证）
  返 回 值：
  说    明：额外行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理

***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	pCGI->extern_left_cols = left_cols;
	pCGI->extern_right_cols = right_cols;
	pCGI->extern_up_lines = up_lines;
	pCGI->extern_down_lines = down_lines;
	gmw_inner_change_value(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/*type.1:═║╔╗╚╝╠╣╩╦╬
	type.2:─│┌┐└┘├┤┴┬┼
	type.3:═│╒╕╘╛╞╡╧╤╪
	type.4:─║╓╖╙╜╟╢╨╥╫*/

	switch (type)
	{
	case 2:
	{
		strcpy(pCGI->CFI.h_normal, "─");
		strcpy(pCGI->CFI.v_normal, "│");
		strcpy(pCGI->CFI.top_left, "┌");
		strcpy(pCGI->CFI.top_right, "┐");
		strcpy(pCGI->CFI.lower_left, "└");
		strcpy(pCGI->CFI.lower_right, "┘");
		strcpy(pCGI->CFI.v_left_separator, "├");
		strcpy(pCGI->CFI.v_right_separator, "┤");
		strcpy(pCGI->CFI.h_lower_separator, "┴");
		strcpy(pCGI->CFI.h_top_separator, "┬");
		strcpy(pCGI->CFI.mid_separator, "┼");
		break;
	}
	case 3:
	{
		strcpy(pCGI->CFI.h_normal, "═");
		strcpy(pCGI->CFI.v_normal, "│");
		strcpy(pCGI->CFI.top_left, "╒");
		strcpy(pCGI->CFI.top_right, "╕");
		strcpy(pCGI->CFI.lower_left, "╘");
		strcpy(pCGI->CFI.lower_right, "╛");
		strcpy(pCGI->CFI.v_left_separator, "╞");
		strcpy(pCGI->CFI.v_right_separator, "╡");
		strcpy(pCGI->CFI.h_lower_separator, "╧");
		strcpy(pCGI->CFI.h_top_separator, "╤");
		strcpy(pCGI->CFI.mid_separator, "╪");
		break;
	}
	case 4:
	{
		strcpy(pCGI->CFI.h_normal, "─");
		strcpy(pCGI->CFI.v_normal, "║");
		strcpy(pCGI->CFI.top_left, "╓");
		strcpy(pCGI->CFI.top_right, "╖");
		strcpy(pCGI->CFI.lower_left, "╙");
		strcpy(pCGI->CFI.lower_right, "╜");
		strcpy(pCGI->CFI.v_left_separator, "╟");
		strcpy(pCGI->CFI.v_right_separator, "╢");
		strcpy(pCGI->CFI.h_lower_separator, "╨");
		strcpy(pCGI->CFI.h_top_separator, "╥");
		strcpy(pCGI->CFI.mid_separator, "╫");
		break;
	}
	default:
	{
		strcpy(pCGI->CFI.h_normal, "═");
		strcpy(pCGI->CFI.v_normal, "║");
		strcpy(pCGI->CFI.top_left, "╔");
		strcpy(pCGI->CFI.top_right, "╗");
		strcpy(pCGI->CFI.lower_left, "╚");
		strcpy(pCGI->CFI.lower_right, "╝");
		strcpy(pCGI->CFI.v_left_separator, "╠");
		strcpy(pCGI->CFI.v_right_separator, "╣");
		strcpy(pCGI->CFI.h_lower_separator, "╩");
		strcpy(pCGI->CFI.h_top_separator, "╦");
		strcpy(pCGI->CFI.mid_separator, "╬");
		break;
	}//缺省参数并且参数不对统一全双线
	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const char *...						：共11种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right,
	const char *lower_right, const char *h_normal, const char *v_normal, const char *h_top_separator,
	const char *h_lower_separator, const char *v_left_separator, const char *v_right_separator, const char *mid_separator)
{
	gmw_inner_check_string(pCGI->CFI.h_normal, h_normal);
	gmw_inner_check_string(pCGI->CFI.v_normal, v_normal);
	gmw_inner_check_string(pCGI->CFI.top_left, top_left);
	gmw_inner_check_string(pCGI->CFI.top_right, top_right);
	gmw_inner_check_string(pCGI->CFI.lower_left, lower_left);
	gmw_inner_check_string(pCGI->CFI.lower_right, lower_right);
	gmw_inner_check_string(pCGI->CFI.v_left_separator, v_left_separator);
	gmw_inner_check_string(pCGI->CFI.v_right_separator, v_right_separator);
	gmw_inner_check_string(pCGI->CFI.h_lower_separator, h_lower_separator);
	gmw_inner_check_string(pCGI->CFI.h_top_separator, h_top_separator);
	gmw_inner_check_string(pCGI->CFI.mid_separator, mid_separator);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的色块数量大小、是否需要分隔线等
  输入参数：输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int block_width						：宽度（错误及缺省2，因为约定表格线为中文制表符，如果给出奇数，要+1）
			const int block_high						：高度（错误及缺省1）
			const bool separator						：是否需要分隔线（缺省为true，需要分隔线）
  返 回 值：
  说    明：框架大小/是否需要分隔线等的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	pCGI->CFI.block_high = block_high;
	pCGI->CFI.block_width = (block_width % 2 == 0 ? block_width : (block_width + 1));
	pCGI->CFI.separator = separator;
	gmw_inner_change_value(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int bg_color					：背景色（缺省 -1表示用窗口背景色）
			const int fg_color					：前景色（缺省 -1表示用窗口前景色）
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	pCGI->CFI.fgcolor = (fgcolor == -1 ? pCGI->area_fgcolor : fgcolor);
	pCGI->CFI.bgcolor = (bgcolor == -1 ? pCGI->area_bgcolor : bgcolor);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	switch (type)
	{
	case 2:
	{
		strcpy(pCGI->CBI.h_normal, "─");
		strcpy(pCGI->CBI.v_normal, "│");
		strcpy(pCGI->CBI.top_left, "┌");
		strcpy(pCGI->CBI.top_right, "┐");
		strcpy(pCGI->CBI.lower_left, "└");
		strcpy(pCGI->CBI.lower_right, "┘");
		break;
	}
	case 3:
	{
		strcpy(pCGI->CBI.h_normal, "═");
		strcpy(pCGI->CBI.v_normal, "│");
		strcpy(pCGI->CBI.top_left, "╒");
		strcpy(pCGI->CBI.top_right, "╕");
		strcpy(pCGI->CBI.lower_left, "╘");
		strcpy(pCGI->CBI.lower_right, "╛");
		break;
	}
	case 4:
	{
		strcpy(pCGI->CBI.h_normal, "─");
		strcpy(pCGI->CBI.v_normal, "║");
		strcpy(pCGI->CBI.top_left, "╓");
		strcpy(pCGI->CBI.top_right, "╖");
		strcpy(pCGI->CBI.lower_left, "╙");
		strcpy(pCGI->CBI.lower_right, "╜");
		break;
	}
	default:
	{
		strcpy(pCGI->CBI.h_normal, "═");
		strcpy(pCGI->CBI.v_normal, "║");
		strcpy(pCGI->CBI.top_left, "╔");
		strcpy(pCGI->CBI.top_right, "╗");
		strcpy(pCGI->CBI.lower_left, "╚");
		strcpy(pCGI->CBI.lower_right, "╝");
		break;
	}
	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *...					：共6种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	gmw_inner_check_string(pCGI->CBI.h_normal, h_normal);
	gmw_inner_check_string(pCGI->CBI.v_normal, v_normal);
	gmw_inner_check_string(pCGI->CBI.top_left, top_left);
	gmw_inner_check_string(pCGI->CBI.top_right, top_right);
	gmw_inner_check_string(pCGI->CBI.lower_left, lower_left);
	gmw_inner_check_string(pCGI->CBI.lower_right, lower_right);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置每个游戏色块(彩球)是否需要小边框
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const bool on_off					：true - 需要 flase - 不需要（缺省false）
  返 回 值：
  说    明：边框约定为中文制表符，双线
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->CBI.block_border = on_off;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示上下状态栏
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const bool on_off					：true - 需要 flase - 不需要（缺省true）
  返 回 值：
  说    明：1、状态栏的相关约定如下：
			   1.1、上状态栏只能一行，在主区域最上方框线/列标的上面，为主区域的最开始一行（主区域的左上角坐标就是上状态栏的坐标）
			   1.2、下状态栏只能一行，在主区域最下方框线的下面
			   1.3、状态栏的宽度为主区域宽度，如果信息过长则截断
		   2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	if (type == TOP_STATUS_LINE) {
		pCGI->top_status_line = on_off;
		gmw_inner_change_value(pCGI);
	}
	if (type == LOWER_STATUS_LINE) {
		pCGI->lower_status_line = on_off;
		gmw_inner_change_value(pCGI);
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置上下状态栏的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const int normal_bgcolor			：正常文本背景色（缺省 -1表示使用窗口背景色）
			const int normal_fgcolor			：正常文本前景色（缺省 -1表示使用窗口前景色）
			const int catchy_bgcolor			：醒目文本背景色（缺省 -1表示使用窗口背景色）
			const int catchy_fgcolor			：醒目文本前景色（缺省 -1表示使用亮黄色）
  输入参数：
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor,
	const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	if (type == TOP_STATUS_LINE) {
		pCGI->SLI.top_normal_bgcolor = (normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor);
		pCGI->SLI.top_normal_fgcolor = (normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor);
		pCGI->SLI.top_catchy_bgcolor = (catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor);
		pCGI->SLI.top_catchy_fgcolor = (catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor);
	}
	if (type == LOWER_STATUS_LINE) {
		pCGI->SLI.lower_normal_bgcolor = (normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor);
		pCGI->SLI.lower_normal_fgcolor = (normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor);
		pCGI->SLI.lower_catchy_bgcolor = (catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor);
		pCGI->SLI.lower_catchy_fgcolor = (catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor);
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示行号
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、行号约定为字母A开始连续排列（如果超过26，则从a开始，超过52的统一为*，实际应用不可能）
			2、是否显示行号的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->draw_frame_with_row_no = on_off;
	gmw_inner_change_value(pCGI);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示列标
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、列标约定为数字0开始连续排列（数字0-99，超过99统一为**，实际应用不可能）
			2、是否显示列标的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->draw_frame_with_col_no = on_off;
	gmw_inner_change_value(pCGI);
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：打印 CONSOLE_GRAPHICS_INFO 结构体中的各成员值
  输入参数：
  返 回 值：
  说    明：1、仅供调试用，打印格式自定义
			2、本函数测试用例中未调用过，可以不实现
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：将 CONSOLE_GRAPHICS_INFO 结构体用缺省值进行初始化
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI：整体结构指针
		   const int row					：游戏区域色块行数（缺省10）
		   const int col					：游戏区域色块列数（缺省10）
		   const int bgcolor				：整个窗口背景色（缺省 COLOR_BLACK）
		   const int fgcolor				：整个窗口背景色（缺省 COLOR_WHITE）
  返 回 值：
  说    明：窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	gmw_set_rowcol(pCGI, row, col);
	//全局背景设置
	gmw_set_color(pCGI, bgcolor, fgcolor, true);
	//字体设置
	gmw_set_font(pCGI, "Terminal", 16, 8);
	//延时设置
	pCGI->delay_of_draw_frame = 0;
	pCGI->delay_of_draw_block = 0;
	pCGI->delay_of_block_moved = 3;
	//额外的空间设置
	gmw_set_ext_rowcol(pCGI);
	//状态栏设置
	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE, true);
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE, true);
	//色块高宽设置//游戏区域分割线设置
	gmw_set_frame_style(pCGI);
	//状态栏醒目文字前景设置
	pCGI->SLI.lower_catchy_fgcolor = 14;
	pCGI->SLI.top_catchy_fgcolor = 14;
	//行列标号设置
	gmw_set_rowno_switch(pCGI, false);
	gmw_set_colno_switch(pCGI, false);
	//555
	gmw_set_frame_default_linetype(pCGI, 1);
	//色块边框设置
	gmw_set_block_border_switch(pCGI, false);
	//555
	gmw_set_block_default_linetype(pCGI, 1);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：画主游戏框架
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：具体可参考demo的效果
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	//"═║╔╗╚╝╠╣╩╦╬"
	char string[60] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	//设置信息
	setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
	setconsoleborder(pCGI->cols, pCGI->lines, pCGI->cols, pCGI->lines);
	setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	//列号
	if (pCGI->draw_frame_with_col_no == true) {
		int middle = pCGI->CFI.block_width + pCGI->CFI.separator * 2, m;
		m = (middle > 4 ? middle / 2 : 2);
		for (int i = 0; i < pCGI->col_num; i++) {
			gotoxy(pCGI->start_x + i * middle + m, pCGI->start_y - 1);
			if (i < 99)
				cout << i;
			else
				cout << "**";
		}
	}
	//行标
	setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	if (pCGI->draw_frame_with_row_no) {
		int middle = pCGI->CFI.block_high + pCGI->CFI.separator;
		int m = (middle == 1 ? 1 : middle / 2);
		for (int i = 0; i < pCGI->row_num; i++) {
			gotoxy(pCGI->start_x - 1, pCGI->start_y + m + i * middle);
			cout << (i < 52 ? string[i] : '*');
		}
	}
	//第一行色块的第一行
	setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
	gotoxy(pCGI->start_x, pCGI->start_y);
	cout << pCGI->CFI.top_left;//"╔"
	for (int i = 0; i < pCGI->col_num; i++) {
		for (int j = 0; j < pCGI->CFI.block_width / 2; j++) {
			cout << pCGI->CFI.h_normal;//"═"
		}
		if (i < pCGI->col_num - 1 && pCGI->CFI.separator) {
			cout << pCGI->CFI.h_top_separator;//"╦"
		}
	}
	cout << pCGI->CFI.top_right;//"╗"
	Sleep(pCGI->delay_of_draw_frame);

	for (int i = 0; i < pCGI->row_num; i++) {

		//色块的非最后一行
		for (int j = 1; j <= pCGI->CFI.block_high; j++) {
			gotoxy(pCGI->start_x, pCGI->start_y + i * (pCGI->CFI.block_high + pCGI->CFI.separator) + j);
			cout << pCGI->CFI.v_normal;//"║"
			int k;
			for (k = 1; k < pCGI->col_num; k++) {
				for (int t = 0; t < pCGI->CFI.block_width / 2; t++)
					cout << "  ";
				if (pCGI->CFI.separator) {
					gotoxy(pCGI->start_x + k * (pCGI->CFI.block_width + pCGI->CFI.separator * 2),
						pCGI->start_y + i * (pCGI->CFI.block_high + pCGI->CFI.separator) + j);
					cout << pCGI->CFI.v_normal;//"║"
				}
			}
			for (int t = 0; t < pCGI->CFI.block_width / 2; t++)
				cout << "  ";
			gotoxy(pCGI->start_x + k * (pCGI->CFI.block_width + pCGI->CFI.separator * 2) + 2 * (!pCGI->CFI.separator),
				pCGI->start_y + i * (pCGI->CFI.block_high + pCGI->CFI.separator) + j);
			cout << pCGI->CFI.v_normal;//"║"
		}
		Sleep(pCGI->delay_of_draw_frame);

		//色块的最后一行
		gotoxy(pCGI->start_x, pCGI->start_y + i * (pCGI->CFI.block_high + pCGI->CFI.separator) + pCGI->CFI.block_high + 1);
		if (i < pCGI->row_num - 1) {
			cout << pCGI->CFI.v_left_separator;//"╠"
			for (int k = 1; k <= pCGI->col_num; k++) {
				if (pCGI->CFI.separator) {
					for (int t = 0; t < pCGI->CFI.block_width / 2; t++) {
						cout << pCGI->CFI.h_normal;//"═"
					}
					if (k < pCGI->col_num) {
						cout << pCGI->CFI.mid_separator;//"╬"
					}
				}
			}
			gotoxy(pCGI->start_x + pCGI->col_num*(pCGI->CFI.block_width + 2 * pCGI->CFI.separator), pCGI->start_y + i * (pCGI->CFI.block_high + pCGI->CFI.separator) + pCGI->CFI.block_high + 1);
			if (pCGI->CFI.separator) {
				cout << pCGI->CFI.v_right_separator;//"╣"
			}
			Sleep(pCGI->delay_of_draw_frame);
		}
	}

	//最后一个色块的最后一行
	gotoxy(pCGI->start_x, pCGI->start_y + (pCGI->row_num - 1) * (pCGI->CFI.block_high + pCGI->CFI.separator) + pCGI->CFI.block_high + 1);
	cout << pCGI->CFI.lower_left;//"╚"
	for (int i = 0; i < pCGI->col_num; i++) {
		for (int j = 0; j < pCGI->CFI.block_width / 2; j++) {
			cout << pCGI->CFI.h_normal;//"═"
		}
		if (i < pCGI->col_num - 1 && pCGI->CFI.separator)
			cout << pCGI->CFI.h_lower_separator;//"╩"
	}
	cout << pCGI->CFI.lower_right;//"╝"

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：在状态栏上显示信息
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type							：指定是上/下状态栏
		   const char *msg						：正常信息
		   const char *catchy_msg					：需要特别标注的信息（在正常信息前显示）
  返 回 值：
  说    明：1、输出宽度限定为主框架的宽度（含行号列标位置），超出则截去
			2、如果最后一个字符是某汉字的前半个，会导致后面乱码，要处理
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg)
{
	int len = (catchy_msg == NULL ? 0 : strlen(catchy_msg));
	if ((type == TOP_STATUS_LINE) && pCGI->top_status_line)
	{
		gotoxy(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y);
		setcolor(pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor);
		gmw_inner_output(catchy_msg, pCGI->cols - pCGI->extern_left_cols - pCGI->extern_right_cols,false);
		setcolor(pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor);
		gmw_inner_output(msg, pCGI->cols - pCGI->extern_left_cols - pCGI->extern_right_cols - len);
	}
	else if ((type == LOWER_STATUS_LINE) && pCGI->lower_status_line)
	{
		gotoxy(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y);
		setcolor(pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor);
		gmw_inner_output(catchy_msg, pCGI->cols - pCGI->extern_left_cols - pCGI->extern_right_cols,false);
		setcolor(pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);
		gmw_inner_output(msg, pCGI->cols - pCGI->extern_left_cols - pCGI->extern_right_cols - len);
	}
	return 0; //此句可根据需要修改*/
}

/***************************************************************************
  函数名称：
  功    能：显示某一个色块(内容为字符串，坐标为row/col)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
			2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
	int x, y;
	x = pCGI->start_x + col_no * 2 * pCGI->CFI.separator + 2 + col_no * pCGI->CFI.block_width;
	y = pCGI->start_y + row_no * pCGI->CFI.separator + 1 + row_no * pCGI->CFI.block_high;
	gmw_inner_piant(pCGI,x,y,bdi_value,bdi);
	Sleep(pCGI->delay_of_draw_block);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：移动某一个色块
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const int blank_bdi_value				：移动过程中用于动画效果显示时用于表示空白的值
		   （一般为0，此处做为参数代入，是考虑到可能出现的特殊情况）
		   const BLOCK_DISPLAY_INFO *const bdi		：存放显示值/空白值对应的显示信息的结构体数组
		   const int direction						：移动方向，一共四种，具体见cmd_gmw_tools.h
		   const int distance						：移动距离（从1开始，人为保证正确性，程序不检查）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
	int i = 0;
	if (direction == LEFT_TO_RIGHT) 
		for (; i < distance; i++) 
			gmw_inner_move(pCGI, row_no, col_no + i, bdi_value, blank_bdi_value, bdi, direction);
	if (direction == RIGHT_TO_LEFT)
		for (; i < distance; i++) 
		gmw_inner_move(pCGI, row_no, col_no - i, bdi_value, blank_bdi_value, bdi, direction);
	if (direction == UP_TO_DOWN) 
		for (; i < distance; i++)
			gmw_inner_move(pCGI, row_no + i, col_no, bdi_value, blank_bdi_value, bdi, direction);
	if (direction == DOWN_TO_UP) 
		for (; i < distance; i++)
			gmw_inner_move(pCGI, row_no - i, col_no, bdi_value, blank_bdi_value, bdi, direction);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：读键盘或鼠标
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   int &MAction							：如果返回 CCT_MOUSE_EVENT，则此值有效，为 MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK 两者之一
													   如果返回 CCT_KEYBOARD_EVENT，则此值无效
		   int &MRow								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的行号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &MCol								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的列号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode1							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第一个）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode2							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第二个，如果是单键码，则为0）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   const bool update_lower_status_line		：鼠标移动时，是否要在本函数中显示"[当前光标] *行*列"的信息（true=显示，false=不显示，缺省为true）
  返 回 值：函数返回约定
			1、如果是鼠标移动，不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] *行*列"
		   2、如果是按下鼠标左键，且当前鼠标指针停留在主游戏区域的*行*列上，则返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow 为行号，MCol 为列标
								  且当前鼠标指针停留在非法区域（非游戏区域，游戏区域中的分隔线），则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
		   3、如果是按下鼠标右键，则不判断鼠标指针停留区域是否合法，直接返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow、MCol不可信
		   4、如果按下键盘上的某键（含双键码按键），则直接返回 CCT_KEYBOARD_EVENT，KeyCode1/KeyCode2中为对应的键码值
 说    明：通过调用 cmd_console_tools.cpp 中的 read_keyboard_and_mouse 函数实现
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	enable_mouse();
	int X = 0, Y = 0,x=-1,y=-1;
	int ret, maction;
	int keycode1, keycode2;
	KeyCode1 = 0;
	KeyCode2 = 0;
	while (1) {
		MRow = -1;
		MCol = -1;
		ret = read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		x = (X - 2 - pCGI->start_x) % (2 * pCGI->CFI.separator + pCGI->CFI.block_width);
		y = (Y - 1 - pCGI->start_y) % (pCGI->CFI.separator + pCGI->CFI.block_high);
		if (ret == CCT_KEYBOARD_EVENT) {
			KeyCode1 = keycode1;
			KeyCode2 = keycode2;
			disable_mouse();
			return ret; 
		}
		if (ret == CCT_MOUSE_EVENT) {
			MAction = maction;
			if (x >= 0 && x < pCGI->CFI.block_width && y >= 0 && y < pCGI->CFI.block_high) {
				MCol = (X - 2 - pCGI->start_x) / (2 * pCGI->CFI.separator + pCGI->CFI.block_width);
				MRow = (Y - 1 - pCGI->start_y) / (pCGI->CFI.separator + pCGI->CFI.block_high);
			}
			if (update_lower_status_line) {
				char temp[256];
				if (MRow >= 0 && MRow < pCGI->row_num && MCol >= 0 && MCol < pCGI->col_num)
					sprintf(temp, "[当前光标] %c行%d列", char('A' + (Y - 1 - pCGI->start_y) / (pCGI->CFI.separator + pCGI->CFI.block_high)),
					(X - 2 - pCGI->start_x) / (2 * pCGI->CFI.separator + pCGI->CFI.block_width));
				else
					sprintf(temp, "[当前光标] 位置非法");
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
			}
			//gotoxy(0, 0);
			//cout << setw(3) << MRow << "   " << setw(3) << MCol<<"   "<< pCGI->row_num<<"   "<< pCGI->col_num;
			//gotoxy(0, 1);
			//cout << setw(3) << x << "   " << setw(3) << y<< "   "<<pCGI->CFI.block_width<<"   "<< pCGI->CFI.block_high;
			if ((maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_RIGHT_BUTTON_CLICK) &&
				(MRow >= 0 && MRow < pCGI->row_num && MCol >= 0 && MCol < pCGI->col_num)){
				disable_mouse();
				return ret;
			}
		}
	}
}
