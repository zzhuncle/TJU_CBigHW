/* 1853790 庄镇华 数强 */
#define _CRT_SECURE_NO_WARNINGS
#include "90-b1.h"
#include "../common/cmd_gmw_tools.h"
#include "../common/cmd_console_tools.h"
/***************************************************************************
等待回车键
***************************************************************************/
void wait_for_enter(const char *prompt = "")
{
	cout << endl << prompt;
	while (_getch() != '\r')
		;
}
/*==============================================================================
随机生成彩球
==============================================================================*/
void buildchess(int(*p1)[9], int lines, int cols)//n_chess是需要下的棋子数 
{
	int flag = 1;
	while (flag)
	{
		int n = 0;
		int num[9] = { 0 };
		for (int i = 0; i < lines; i++)
			for (int j = 0; j < cols; j++)
			{
				p1[i][j] = rand() % 9 + 1;
				num[p1[i][j] - 1]++;
			}
		for (int i = 0; i < 9; i++)
			if (num[i] == 0)
				n++;
		if (n == 0)
			flag = 0;
	}
}
/*==============================================================================
判断两两之间是否可以交换
==============================================================================*/
int is_exchange(int(*p1)[9], int(*p2)[9], int x, int y, int x_, int y_, int n_r, int n_c, const int mood)
{
	tj_swap(p1[x][y], p1[x_][y_]);
	int i = 0; /* 临时计数 */
	int sum[5] = { 0 };
	/* 检查竖行 */
	for (i = x; i < n_r; i++)
	{
		if (p1[x][y] != p1[i][y])
			break;
		sum[1]++;
	}
	for (i = x; i >= 0; i--)
	{
		if (p1[x][y] != p1[i][y])
			break;
		sum[1]++;
	}
	/* 检查横行 */
	for (i = y; i < n_c; i++)
	{
		if (p1[x][y] != p1[x][i])
			break;
		sum[2]++;
	}
	for (i = y; i >= 0; i--)
	{
		if (p1[x][y] != p1[x][i])
			break;
		sum[2]++;
	}
	/* 检查竖行 */
	for (i = x_; i < n_r; i++)
	{
		if (p1[x_][y_] != p1[i][y_])
			break;
		sum[3]++;
	}
	for (i = x_; i >= 0; i--)
	{
		if (p1[x_][y_] != p1[i][y_])
			break;
		sum[3]++;
	}
	/* 检查横行 */
	for (i = y_; i < n_c; i++)
	{
		if (p1[x_][y_] != p1[x_][i])
			break;
		sum[4]++;
	}
	for (i = y_; i >= 0; i--)
	{
		if (p1[x_][y_] != p1[x_][i])
			break;
		sum[4]++;
	}
	tj_swap(p1[x][y], p1[x_][y_]);
	if ((sum[1] > 3) || (sum[2] > 3) || sum[3] > 3 || sum[4] > 3)
		return 1;
	return 0;
}
/*==============================================================================
判断初始可消除项
==============================================================================*/
int delchess(int(*p1)[9], int(*p2)[9], int n_r, int n_c, const int mood)
{
	for (int ii = 0; ii < n_r; ii++)
		for (int jj = 0; jj < n_c; jj++)
			p2[ii][jj] = int('0');
	int sum_total = 0;//用于双三连情况的计数
	for (int x = 0; x < n_r; x++)
		for (int y = 0; y < n_c; y++)
			if (p2[x][y] != int('*'))
			{
				int i = 0; /* 临时计数 */
				int sum[3] = { 0 };
				/* 检查竖行 */
				for (i = x; i < n_r; i++)
				{
					if (p1[x][y] != p1[i][y])
						break;
					sum[1]++;
				}
				for (i = x; i >= 0; i--)
				{
					if (p1[x][y] != p1[i][y])
						break;
					sum[1]++;
				}
				if (sum[1] > 3)
				{
					for (i = x + 1; i < n_r; i++)
					{
						if (p1[x][y] != p1[i][y])
							break;
						p2[i][y] = int('*');
					}
					for (i = x - 1; i >= 0; i--)
					{
						if (p1[x][y] != p1[i][y])
							break;
						p2[i][y] = int('*');
					}
				}
				/* 检查横行 */
				for (i = y; i < n_c; i++)
				{
					if (p1[x][y] != p1[x][i])
						break;
					sum[2]++;
				}
				for (i = y; i >= 0; i--)
				{
					if (p1[x][y] != p1[x][i])
						break;
					sum[2]++;
				}
				if (sum[2] > 3)
				{
					for (i = y + 1; i < n_c; i++)
					{
						if (p1[x][y] != p1[x][i])
							break;
						p2[x][i] = int('*');
					}
					for (i = y - 1; i >= 0; i--) {
						if (p1[x][y] != p1[x][i])
							break;
						p2[x][i] = int('*');
					}
				}
				if ((sum[1] > 3) || (sum[2] > 3))
				{
					p2[x][y] = int('*');
					for (int k = 1; k < 3; k++)
					{
						if (sum[k] > 3)
							sum_total += sum[k] - 1;
					}
				}
			}
	return sum_total;
}
/*==============================================================================
下落除0
==============================================================================*/
void delzero(const BLOCK_DISPLAY_INFO bdi_normal[], CONSOLE_GRAPHICS_INFO *pCGI, int(*p1)[9], int(*p2)[9], int n_r, int n_c, const int mood)
{
	for (int j = 0; j < n_c; j++)
	{
		int empty = 0;//空0的个数
		for (int i = n_r - 1; i >= 0; i--)
		{
			int index = 0;//要下落的距离
			if (p2[i][j] != int('*'))
			{
				for (int k = i; k < n_r; k++)
					if (p2[k][j] == int('*'))
						index++;
				p1[i + index][j] = p1[i][j];
				if (mood == 7)
					gmw_move_block(pCGI, i, j, p1[i][j], 0, bdi_normal, UP_TO_DOWN, index);
			}
			else
				empty++;
		}
		for (int i = n_r - 1; i >= 0; i--)
			p2[i][j] = int('0');
		for (int i = 0; i < empty; i++)
		{
			p1[i][j] = 0;
			p2[i][j] = int('*');
		}
	}
}
/*==============================================================================
新值填充
==============================================================================*/
void fill(const BLOCK_DISPLAY_INFO bdi_normal[], CONSOLE_GRAPHICS_INFO *pCGI, int(*p1)[9], int(*p2)[9], int n_r, int n_c, const int mood)
{
	for (int i = 0; i < n_r; i++)
		for (int j = 0; j < n_c; j++)
			if (p1[i][j] == 0)
			{
				p1[i][j] = rand() % 9 + 1;
				if (mood == 7)
					gmw_draw_block(pCGI, i, j, p1[i][j], bdi_normal);
			}
}
/*==============================================================================
数组形式的消除提示
==============================================================================*/
int mark_remove(int(*p1)[9], int(*p2)[9], int n_r, int n_c, const int mood)
{
	for (int ii = 0; ii < n_r; ii++)
		for (int jj = 0; jj < n_c; jj++)
			p2[ii][jj] = int('0');
	for (int i = 0; i < n_r; i++)
		for (int j = 0; j < n_c; j++)
		{
			if (j - 1 >= 0)//左交换
				if (is_exchange(p1, p2, i, j, i, j - 1, n_r, n_c))
				{
					p2[i][j - 1] = int('*');
					p2[i][j] = int('*');
				}
			if (j + 1 < n_c)//右交换
				if (is_exchange(p1, p2, i, j, i, j + 1, n_r, n_c))
				{
					p2[i][j + 1] = int('*');
					p2[i][j] = int('*');
				}
			if (i + 1 < n_r)//下交换
				if (is_exchange(p1, p2, i + 1, j, i, j, n_r, n_c))
				{
					p2[i + 1][j] = int('*');
					p2[i][j] = int('*');
				}
			if (i - 1 >= 0)//上交换
				if (is_exchange(p1, p2, i - 1, j, i, j, n_r, n_c))
				{
					p2[i - 1][j] = int('*');
					p2[i][j] = int('*');
				}
		}
	int ball_num = 0;
	for (int i = 0; i < n_r; i++)
		for (int j = 0; j < n_c; j++)
			if (p2[i][j] == int('*'))
				ball_num++;
	return ball_num;
}
/*==============================================================================
实现伪图形消除彩球
==============================================================================*/
void console_delchess(const BLOCK_DISPLAY_INFO bdi_exploded[], const BLOCK_DISPLAY_INFO bdi_normal[], CONSOLE_GRAPHICS_INFO *pCGI, int(*p1)[9], int(*p2)[9], int n_r, int n_c)
{
	for (int i = 0; i < n_r; i++)
		for (int j = 0; j < n_c; j++)
			if (p2[i][j] == '*')
			{
				for (int k = 0; k < 3; k++) {
					gmw_draw_block(pCGI, i, j, p1[i][j], bdi_exploded);
					Sleep(50);
					gmw_draw_block(pCGI, i, j, 0, bdi_normal); //0是空白
					Sleep(50);
				}
			}
	return;
}
/*==============================================================================
实现伪图形显示消除提示
==============================================================================*/
void console_mark_remove(const BLOCK_DISPLAY_INFO bdi_prompt[], const BLOCK_DISPLAY_INFO bdi_normal[], CONSOLE_GRAPHICS_INFO *pCGI, int(*p1)[9], int(*p2)[9], int n_r, int n_c)
{
	for (int i = 0; i < n_r; i++)
		for (int j = 0; j < n_c; j++)
		{
			if (p2[i][j] == int('*'))
				gmw_draw_block(pCGI, i, j, p1[i][j], bdi_prompt);
			else
				gmw_draw_block(pCGI, i, j, p1[i][j], bdi_normal);
		}
}
/*==============================================================================
实现伪图形交换彩球
==============================================================================*/
void console_swap(const BLOCK_DISPLAY_INFO bdi_normal[], CONSOLE_GRAPHICS_INFO *pCGI, int(*p1)[9], int x_qi, int y_qi, int xx, int yy)
{
	if (xx == x_qi && yy == y_qi - 1)//left
	{
		gmw_move_block(pCGI, x_qi, y_qi, p1[x_qi][y_qi], 0, bdi_normal, RIGHT_TO_LEFT, 1);
		gmw_draw_block(pCGI, x_qi, y_qi, p1[xx][yy], bdi_normal);
	}
	if (xx == x_qi && yy == y_qi + 1)//right
	{
		gmw_move_block(pCGI, x_qi, y_qi, p1[x_qi][y_qi], 0, bdi_normal, LEFT_TO_RIGHT, 1);
		gmw_draw_block(pCGI, x_qi, y_qi, p1[xx][yy], bdi_normal);
	}
	if (xx == x_qi - 1 && yy == y_qi)//over
	{
		gmw_move_block(pCGI, x_qi, y_qi, p1[x_qi][y_qi], 0, bdi_normal, DOWN_TO_UP, 1);
		gmw_draw_block(pCGI, x_qi, y_qi, p1[xx][yy], bdi_normal);
	}
	if (xx == x_qi + 1 && yy == y_qi)//below
	{
		gmw_move_block(pCGI, x_qi, y_qi, p1[x_qi][y_qi], 0, bdi_normal, UP_TO_DOWN, 1);
		gmw_draw_block(pCGI, x_qi, y_qi, p1[xx][yy], bdi_normal);
	}
}
/*=============================================================================
主菜单项
==============================================================================*/
int caidan(int(*p1)[9], int(*p2)[9], CONSOLE_GRAPHICS_INFO *pCGI, const int mood)
{
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, 0, 0, "  "},  //0不显示，用空格填充即可
		{1, 1, 0, "♀"},
		{2, 2, 0, "♀"},
		{3, 3, 0, "♀"},
		{4, 4, 0, "♀"},
		{5, 5, 0, "♀"},
		{6, 6, 0, "♀"},
		{7, 7, 0, "♀"},
		{8, 8, 0, "♀"},
		{9, 9, 0, "♀"},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
	};
	/* 定义1-9的数字用何种形式显示在界面上（选中状态） */
	const BLOCK_DISPLAY_INFO bdi_selected[] = {
	{BDI_VALUE_BLANK, 0, 0, "  "},  //0不显示，用空格填充即可
		{1, 1, 0, "♂"},
		{2, 2, 0, "♂"},
		{3, 3, 0, "♂"},
		{4, 4, 0, "♂"},
		{5, 5, 0, "♂"},
		{6, 6, 0, "♂"},
		{7, 7, 0, "♂"},
		{8, 8, 0, "♂"},
		{9, 9, 0, "♂"},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
	};
	/* 定义1-9的数字用何种形式显示在界面上（可消除提示状态） */
	const BLOCK_DISPLAY_INFO bdi_prompt[] = {
	{BDI_VALUE_BLANK, 0, 0, "  "},
		{1, 1, 0, "◎"},
		{2, 2, 0, "◎"},
		{3, 3, 0, "◎"},
		{4, 4, 0, "◎"},
		{5, 5, 0, "◎"},
		{6, 6, 0, "◎"},
		{7, 7, 0, "◎"},
		{8, 8, 0, "◎"},
		{9, 9, 0, "◎"},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
	};
	/* 定义1-9的数字用何种形式显示在界面上（爆炸/消除状态） */
	const BLOCK_DISPLAY_INFO bdi_exploded[] = {
		{BDI_VALUE_BLANK, 0, 0, "  "},
		{1, 1, 0, "¤"},
		{2, 2, 0, "¤"},
		{3, 3, 0, "¤"},
		{4, 4, 0, "¤"},
		{5, 5, 0, "¤"},
		{6, 6, 0, "¤"},
		{7, 7, 0, "¤"},
		{8, 8, 0, "¤"},
		{9, 9, 0, "¤"},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
	};

	system("cls");
	gotoxy(0, 0);
	int n_r, n_c;
	char a1[] = "请输入行数";
	n_r = get_int(a1, 5, 9);
	char a2[] = "请输入列数";
	n_c = get_int(a2, 5, 9);//参数输入

	buildchess(p1, n_r, n_c);
	drawarray(p1, p2, n_r, n_c, 11);//mood1-1代表90-b1无需找到可消除项的数组输出
	wait_for_key("按回车键显示图形...");
	cls();

	//初始化
	gmw_init(pCGI, n_r, n_c, COLOR_BLACK, COLOR_WHITE);

	//更改
	gmw_set_rowno_switch(pCGI, true);
	gmw_set_colno_switch(pCGI, true);
	setcursor(CURSOR_INVISIBLE);//光标设置
	gmw_set_font(pCGI, "新宋体", 28, 28);//字体设置
	gmw_set_ext_rowcol(pCGI, 0, 0, 0, 0);//额外的空间设置
	gmw_set_frame_default_linetype(pCGI, 4);//设置框架风格
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, 20);//移动延时设置

	/* 显示框架 */
	gmw_draw_frame(pCGI);

	/* 上状态栏显示内容 */
	char temp[256];
	temp[0] = '\0';
	sprintf(temp, "屏幕:%d行%d列(当前分数: 0 右键退出)", pCGI->lines, pCGI->cols);
	gmw_status_line(pCGI, TOP_STATUS_LINE, temp);

	/* 下状态栏显示内容 */
	gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前光标] 01行0A列");

	/* 将内部数组展现到屏幕上 */
	for (int i = 0; i < n_r; i++)
		for (int j = 0; j < n_c; j++) {
			gmw_draw_block(pCGI, i, j, p1[i][j], bdi_normal);
		}

	int grade = 0;
	int base_num[5] = { 12,8,5,3,1 };
	while (delchess(p1, p2, n_r, n_c))
	{
		for (int i = 0; i < n_r; i++)
			for (int j = 0; j < n_c; j++)
				if (p2[i][j] == int('*'))
					gmw_draw_block(pCGI, i, j, p1[i][j], bdi_prompt);
		console_delchess(bdi_exploded, bdi_normal, pCGI, p1, p2, n_r, n_c);
		delzero(bdi_normal, pCGI, p1, p2, n_r, n_c, 7);
		fill(bdi_normal, pCGI, p1, p2, n_r, n_c, 7);
	}
	mark_remove(p1, p2, n_r, n_c);
	console_mark_remove(bdi_prompt, bdi_normal, pCGI, p1, p2, n_r, n_c);
	//消除初始可消除项

	int mrow = 0, mcol = 0;
	int ret, maction;
	int keycode1, keycode2;
	int loop = 1;
	int flag[9][9] = { 0 };
	while (loop == 1)
	{
		if (!mark_remove(p1, p2, n_r, n_c))
			loop = 0;
		ret = gmw_read_keyboard_and_mouse(pCGI, maction, mrow, mcol, keycode1, keycode2);
		if (ret == CCT_MOUSE_EVENT)
		{

			switch (maction)
			{
			case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
			{
				int is_flagaround = 1;
				int xx = mrow, yy = mcol;
				if ((flag[xx][yy - 1] == 1 && is_exchange(p1, p2, xx, yy, xx, yy - 1, n_r, n_c))
					|| (flag[xx][yy + 1] == 1 && is_exchange(p1, p2, xx, yy, xx, yy + 1, n_r, n_c))
					|| (flag[xx - 1][yy] == 1 && is_exchange(p1, p2, xx, yy, xx - 1, yy, n_r, n_c))
					|| (flag[xx + 1][yy] == 1 && is_exchange(p1, p2, xx, yy, xx + 1, yy, n_r, n_c)))
					is_flagaround = 0;
				if (p2[xx][yy] == int('*') && is_flagaround)
				{
					for (int i = 0; i < n_r; i++)
						for (int j = 0; j < n_c; j++)
							if (flag[i][j] == 1)
							{
								flag[i][j] = 0;
								if (p2[i][j] != int('*'))
									gmw_draw_block(pCGI, i, j, p1[i][j], bdi_normal);
								else
									gmw_draw_block(pCGI, i, j, p1[i][j], bdi_prompt);
							}
					gmw_draw_block(pCGI, xx, yy, p1[xx][yy], bdi_selected);
					flag[xx][yy] = 1;
					setcolor();//重新设定起始彩球
				}
				else
				{
					int x_qi = -1, y_qi = -1;
					for (int i = 0; i < n_r; i++)
						for (int j = 0; j < n_c; j++)
							if (flag[i][j] == 1)
							{
								x_qi = i;
								y_qi = j;
							}//寻找起始彩球的坐标
					if (p2[xx][yy] != int('*'))
						continue;
					console_swap(bdi_normal, pCGI, p1, x_qi, y_qi, xx, yy);
					tj_swap(p1[xx][yy], p1[x_qi][y_qi]);
					while (int single_grade = delchess(p1, p2, n_r, n_c))
					{
						for (int i = 0; i < n_r; i++)
							for (int j = 0; j < n_c; j++)
								if (p2[i][j] == int('*'))
									gmw_draw_block(pCGI, i, j, p1[i][j], bdi_prompt);
						console_delchess(bdi_exploded, bdi_normal, pCGI, p1, p2, n_r, n_c);
						grade += single_grade * base_num[n_r - 5];
						temp[0] = '\0';
						sprintf(temp, "屏幕:%d行%d列(当前分数: %d 右键退出)", pCGI->lines, pCGI->cols, grade);
						gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
						delzero(bdi_normal, pCGI, p1, p2, n_r, n_c, 7);
						fill(bdi_normal, pCGI, p1, p2, n_r, n_c, 7);
					}
					if (!mark_remove(p1, p2, n_r, n_c))
						loop = 0;
					console_mark_remove(bdi_prompt, bdi_normal, pCGI, p1, p2, n_r, n_c);
				}
			}
			break;
			case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
			case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:	//双击右键
			{
				loop = 0;
				break;
			}
			}//end of switch(maction)
		}//end of 鼠标事件
	}//end of while(loop)
	temp[0] = '\0';
	sprintf(temp, "游戏结束啦,再玩一局吧~~~");
	gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
	return 0;
}