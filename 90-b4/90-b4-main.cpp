/* 1853790 庄镇华 数强 */
#include"90-b4.h"
#include "../common/cmd_gmw_tools.h"
#include "../common/cmd_console_tools.h"
using namespace std;

struct TopGrade {
	int row;
	int col;
	int grade;
};
/*==============================================================================
更新输出上状态栏
==============================================================================*/
void output_top_info(CONSOLE_GRAPHICS_INFO *pCGI,const int ideal, const int score, const int top_grade, const int sec)
{
	/* 上状态栏显示内容 */
	char temp[256];
	if (top_grade>=score)
	    sprintf(temp, "目标:%d 分数:%d 最高:%d 时间:%d",ideal,score,top_grade,sec);
	else
		sprintf(temp, "目标:%d 分数:%d 最高:%d 时间:%d", ideal, score,score, sec);
	gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
}
/*==============================================================================
更新最高分文件
==============================================================================*/
void write_file(TopGrade topgrade[5][7], const int row, const int col, const int top_grade)
{
	ofstream fwsaving("./Highscore.2048", ios::out);
	topgrade[row - 4][col - 4].grade = top_grade;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 7; j++) {
			fwsaving << topgrade[i][j].row << " ";
			fwsaving << topgrade[i][j].col << " ";
			fwsaving << topgrade[i][j].grade << endl;
		}
	fwsaving.close();
}
/*==============================================================================
读取最高分文件
==============================================================================*/
int read_file(TopGrade topgrade[5][7],const int row,const int col)
{
	//读取最高分保存文件,没有则创建空白文件
	ifstream  fsaving("./Highscore.2048", ios::in);
	if (fsaving.is_open() == 0) {
		fsaving.close();
		write_file(topgrade, row, col, 0);
	}
	fsaving.close();

	ifstream  Fsaving("./Highscore.2048", ios::in);
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 7; j++) {
			Fsaving >> topgrade[i][j].row;
			Fsaving >> topgrade[i][j].col;
			Fsaving >> topgrade[i][j].grade;
		}
	Fsaving.close();
	return topgrade[row-4][col-4].grade;
}
/*==============================================================================
简化版菜单输出
==============================================================================*/
static void menu(int &n_r, int &n_c, int &ideal_score, int &delay)
{
	cls();
	int score[10] = { 512,1024,2048,4096,8192,16384,32768,65536 };
	CONSOLE_GRAPHICS_INFO CGI;
	CONSOLE_GRAPHICS_INFO *pCGI = &CGI;
	//初始化
	gmw_init(pCGI, 16, 28, COLOR_BLACK, COLOR_WHITE);
	//更改
	gmw_set_font(pCGI, "新宋体", 20, 20);//字体设置
	gmw_set_frame_default_linetype(pCGI, 3);//设置框架风格
	gmw_set_frame_style(pCGI, 1, 1, false);//设置没有分割线
	gmw_set_frame_color(pCGI, 15, 0);//主框架的背景与前景颜色
	//显示框架
	gmw_draw_frame(pCGI);
	gotoxy(pCGI->start_x + 2, pCGI->start_y + 2);
	cout << "游戏2048参数设置：";

	gotoxy(pCGI->start_x + 2, pCGI->start_y + 3);
	char a1[] = "请输入行数";
	n_r = get_int(a1, 4, 8);

	gotoxy(pCGI->start_x + 2, pCGI->start_y + 4);
	char a2[] = "请输入列数";
	n_c = get_int(a2, 4, 10);//参数输入

	int order = 0;
	if (n_r*n_c <= 16)
		order = 0;
	else if (n_r*n_c <= 25)
		order = 1;
	else if (n_r*n_c <= 36)
		order = 2;
	else if (n_r*n_c <= 49)
		order = 3;
	else
		order = 4;

	while (1) {
		gotoxy(pCGI->start_x + 2, pCGI->start_y + 5);
		cout << "请输入目标分数(" << score[order] << "/" << score[order + 1] << "/" << score[order + 2] << "/" << score[order + 3] << "):";
		cin >> ideal_score;
		if (ideal_score == score[order] || ideal_score == score[order + 1] || ideal_score == score[order + 2] || ideal_score == score[order + 3])
			break;
	}

	gotoxy(pCGI->start_x + 2, pCGI->start_y + 6);
	char a3[] = "请输入动画延时";
	delay = get_int(a3, 0, 5);
}
/*==============================================================================
初始化
==============================================================================*/
void Init(CONSOLE_GRAPHICS_INFO *pCGI, const BLOCK_DISPLAY_INFO bdi_normal[], int p[8][10], int n_r, int n_c,int ideal, const int top_grade)
{
	cls();
	int x, y, cnt = 0;

	while (1) {
		x = rand() % n_r;
		y = rand() % n_c;
		if (p[x][y] == 2)
			continue;
		p[x][y] = 2;
		cnt++;
		if (cnt == 2)
			break;
	}
	//初始化
	gmw_init(pCGI, n_r, n_c, COLOR_BLACK, COLOR_WHITE);
	//更改
	gmw_set_font(pCGI, "新宋体", 20, 20);//字体设置
	gmw_set_frame_default_linetype(pCGI, 1);//设置框架风格
	gmw_set_frame_style(pCGI, 6, 3, true);//设置没有分割线
	gmw_set_frame_color(pCGI, 15, 0);//主框架的背景与前景颜色
	gmw_set_rowcol(pCGI, n_r, n_c);//重设游戏主区域行列
	gmw_draw_frame(pCGI);//显示框架
	gmw_set_block_border_switch(pCGI, true);//色块需要边框

	/* 上状态栏显示内容 */
	char temp[256];
	sprintf(temp, "目标:%d 分数:0 最高:%d 时间:0", ideal,top_grade);
	gmw_status_line(pCGI, TOP_STATUS_LINE, temp);

	/* 下状态栏显示内容 */
	gmw_status_line(pCGI, LOWER_STATUS_LINE,"(R:重玩 Q:退出)");

	for (int i = 0; i < n_r; i++)
		for (int j = 0; j < n_c; j++)
			gmw_draw_block(pCGI, i, j, p[i][j], bdi_normal);
}
/*==============================================================================
游戏是否结束
==============================================================================*/
int Gameover(const int p[8][10], const int n_r, const int n_c, const int ideal)
{
	//判断是否存在可以继续合并的数字，如有则游戏继续；判断是否有空位，如有则游戏继续return 0；
	//判断是否存在2048，如有则游戏结束return 2；
	//不存在2048，但是同行同列中没有能够合并的数字，并且没有空位置,游戏结束return 1；
	for (int i = 0; i < n_r; i++)
		for (int j = 0; j < n_c; j++)
			if (p[i][j] == ideal)
				return 2;
	for (int i = 0; i < n_r; i++)
		for (int j = 0; j < n_c; j++)
			if (p[i][j] == 0)
				return 0;
	for (int i = 0; i < n_r; i++)
		for (int j = 0; j < n_c; j++) {
			if (j + 1 < n_c && p[i][j] == p[i][j + 1])
				return 1;
			if (j + 1 < n_r && p[j][i] == p[j + 1][i])
				return 1;
		}
	return 0;
}
/*==============================================================================
结束界面
==============================================================================*/
int Gameoveroutput(int is_win)
{
	cls();
	CONSOLE_GRAPHICS_INFO CGI;
	CONSOLE_GRAPHICS_INFO *pCGI = &CGI;
	//初始化
	gmw_init(pCGI, 5, 8, COLOR_BLACK, COLOR_WHITE);
	//更改
	gmw_set_font(pCGI, "新宋体", 20, 20);//字体设置
	gmw_set_frame_default_linetype(pCGI, 1);//设置框架风格
	gmw_set_frame_style(pCGI, 1, 1, false);//设置没有分割线
	gmw_set_frame_color(pCGI, COLOR_RED, COLOR_WHITE);//主框架的背景与前景颜色
	//显示框架
	gmw_draw_frame(pCGI);
	gotoxy(pCGI->start_x + 6, pCGI->start_y + 2);
	if (is_win)
		cout << "游戏成功";
	else
	    cout << "游戏失败";

	gotoxy(pCGI->start_x + 6, pCGI->start_y + 3);
	cout << "R.再玩";

	gotoxy(pCGI->start_x + 6, pCGI->start_y + 4);
	cout << "Q.退出";

	char keyboard;
	while (1) {
		if (keyboard = _getch()) {
			if (keyboard == 'q' || keyboard == 'Q')//退出键 
				return 0;
			if (keyboard == 'r' || keyboard == 'R')
				return 2;
		}
	}
}
/*==============================================================================
生成一个元素
==============================================================================*/
void Generate(CONSOLE_GRAPHICS_INFO *pCGI, const BLOCK_DISPLAY_INFO bdi_normal[], int p[8][10], int n_r, int n_c)
{
	int x, y, temp, num;
	while (1) {
		x = rand() % n_r;
		y = rand() % n_c;
		temp = rand() % 4;//2和4出现概率不同
		if (temp == 1)
			num = 4;
		else
			num = 2;
		if (p[x][y] == 0) {
			p[x][y] = num;
			gmw_draw_block(pCGI, x, y, p[x][y], bdi_normal);
			break;
		}
	}
}
/*==============================================================================
向上移动处理
==============================================================================*/
int up(CONSOLE_GRAPHICS_INFO *pCGI, const BLOCK_DISPLAY_INFO bdi_normal[], int p[8][10], const int n_r, const int n_c, const int ideal, 
	int &score, const int sec,const int top_grade)
{
	int is_step = 0;
	for (int col = 0; col < n_c; col++)
	{
		for (int line = 1; line < n_r;line++)
			if (p[line][col] != 0) {
				int distance = 0, i = line - 1;
				for (; i >= 0 && p[i][col] == 0; i--)
					distance++;
				if (distance == 0)
					continue;
				gmw_move_block(pCGI, line, col, p[line][col], 0, bdi_normal, DOWN_TO_UP, distance);
				is_step = 1;
				p[i + 1][col] = p[line][col];
				p[line][col] = 0;
			}
		for (int line = 0;  line < n_r; line++)
			if (line - 1 >= 0 && p[line - 1][col] != 0 && p[line][col] == p[line - 1][col])
			{
				gmw_move_block(pCGI, line, col, p[line][col], 0, bdi_normal, DOWN_TO_UP, 1);
				is_step = 1;
				p[line - 1][col] *= 2;
				score += p[line - 1][col];

				/* 上状态栏显示内容 */
				output_top_info(pCGI, ideal, score, top_grade,sec);

				gmw_draw_block(pCGI, line - 1, col, p[line - 1][col], bdi_normal);
				p[line][col] = 0;
				int i;
				for (i = line; p[i + 1][col] != 0 && i + 1 < n_r; i++) {
					p[i][col] = p[i + 1][col];
					gmw_move_block(pCGI, i + 1, col, p[i + 1][col], 0, bdi_normal, DOWN_TO_UP, 1);
				}
				p[i][col] = 0;
				gmw_draw_block(pCGI, line - 1, col, p[line - 1][col], bdi_normal);
			}
	}
	return is_step;
}
/*==============================================================================
向下移动处理
==============================================================================*/
int down(CONSOLE_GRAPHICS_INFO *pCGI, const BLOCK_DISPLAY_INFO bdi_normal[], int p[8][10], const int n_r, const int n_c, const int ideal,
	int &score, const int sec, const int top_grade)
{
	int is_step = 0;
	for (int col = 0; col < n_c; col++)
	{
		for (int line = n_r - 1; line >= 0; line--)
			if (p[line][col] != 0) {
				int distance = 0, i = line + 1;
				for (; i < n_r && p[i][col] == 0; i++)
					distance++;
				if (distance == 0)
					continue;
				gmw_move_block(pCGI, line, col, p[line][col], 0, bdi_normal, UP_TO_DOWN, distance);
				is_step = 1;
				p[i - 1][col] = p[line][col];
				p[line][col] = 0;
			}
		for (int line = n_r - 1; line >= 0; line--)
			if (line + 1 < n_r && p[line + 1][col] != 0 && p[line][col] == p[line + 1][col])
			{
				gmw_move_block(pCGI, line, col, p[line][col], 0, bdi_normal, UP_TO_DOWN, 1);
				is_step = 1;
				p[line + 1][col] *= 2;
				score += p[line + 1][col];

				/* 上状态栏显示内容 */
				output_top_info(pCGI, ideal, score, top_grade, sec);

				gmw_draw_block(pCGI, line + 1, col, p[line + 1][col], bdi_normal);
				p[line][col] = 0;
				int i;
				for (i = line; p[i - 1][col] != 0 && i - 1 >= 0; i--) {
					p[i][col] = p[i - 1][col];
					gmw_move_block(pCGI, i - 1, col, p[i - 1][col], 0, bdi_normal, UP_TO_DOWN, 1);
				}
				p[i][col] = 0;
				gmw_draw_block(pCGI, line + 1, col, p[line + 1][col], bdi_normal);
			}
	}
	return is_step;
}
/*==============================================================================
向左移动处理
==============================================================================*/
int left(CONSOLE_GRAPHICS_INFO *pCGI, const BLOCK_DISPLAY_INFO bdi_normal[], int p[8][10], const int n_r, const int n_c, const int ideal,
	int &score, const int sec, const int top_grade)
{
	int is_step = 0;
	for (int line = 0; line < n_r; line++)
	{
		for (int col = 1; col < n_c; col++)
			if (p[line][col] != 0) {
				int distance = 0, i = col - 1;
				for (; i >= 0 && p[line][i] == 0; i--)
					distance++;
				if (distance == 0)
					continue;
				gmw_move_block(pCGI, line, col, p[line][col], 0, bdi_normal, RIGHT_TO_LEFT, distance);
				is_step = 1;
				p[line][i + 1] = p[line][col];
				p[line][col] = 0;
			}
		for (int col = 0; col < n_c; col++)
			if (col - 1 >= 0 && p[line][col - 1] != 0 && p[line][col] == p[line][col - 1])
			{
				gmw_move_block(pCGI, line, col, p[line][col], 0, bdi_normal, RIGHT_TO_LEFT, 1);
				is_step = 1;
				p[line][col - 1] *= 2;
				score += p[line][col - 1];

				/* 上状态栏显示内容 */
				output_top_info(pCGI, ideal, score, top_grade, sec);

				gmw_draw_block(pCGI, line, col-1, p[line][col-1], bdi_normal);
				p[line][col] = 0;
				int i;
				for (i = col; p[line][i+1] != 0 && i + 1 < n_c; i++) {
					p[line][i] = p[line][i+1];
					gmw_move_block(pCGI, line, i+1, p[line][i+1], 0, bdi_normal, RIGHT_TO_LEFT, 1);
				}
				p[line][i] = 0;
				gmw_draw_block(pCGI, line, col-1, p[line][col-1], bdi_normal);
			}
	}
	return is_step;
}
/*==============================================================================
向右移动处理
==============================================================================*/
int right(CONSOLE_GRAPHICS_INFO *pCGI, const BLOCK_DISPLAY_INFO bdi_normal[], int p[8][10], const int n_r, const int n_c, const int ideal,
	int &score, const int sec, const int top_grade)
{
	int is_step = 0;
	for (int line = 0; line < n_r; line++)
	{
		for (int col = n_c - 1; col >= 0; col--)
			if (p[line][col] != 0) {
				int distance = 0, i = col + 1;
				for (; i < n_c && p[line][i] == 0; i++)
					distance++;
				if (distance == 0)
					continue;
				gmw_move_block(pCGI, line, col, p[line][col], 0, bdi_normal, LEFT_TO_RIGHT, distance);
				is_step = 1;
				p[line][i-1] = p[line][col];
				p[line][col] = 0;
			}
		for (int col = n_c - 1; col >= 0; col--)
			if (col + 1 < n_c && p[line][col+1] != 0 && p[line][col] == p[line][col+1])
			{
				gmw_move_block(pCGI, line, col, p[line][col], 0, bdi_normal, LEFT_TO_RIGHT, 1);
				is_step = 1;
				p[line][col+1] *= 2;
				score += p[line][col+1];

				/* 上状态栏显示内容 */
				output_top_info(pCGI, ideal, score, top_grade, sec);

				gmw_draw_block(pCGI, line, col+1, p[line][col+1], bdi_normal);
				p[line][col] = 0;
				int i;
				for (i = col; p[line][i-1] != 0 && i - 1 >= 0; i--) {
					p[line][i] = p[line][i-1];
					gmw_move_block(pCGI, line,i-1, p[line][i-1], 0, bdi_normal, LEFT_TO_RIGHT, 1);
				}
				p[line][i] = 0;
				gmw_draw_block(pCGI, line, col+1, p[line][col+1], bdi_normal);
			}
	}
	return is_step;
}
/*==============================================================================
移动元素的实现
==============================================================================*/
int Movement(CONSOLE_GRAPHICS_INFO *pCGI, const BLOCK_DISPLAY_INFO bdi_normal[], int p[8][10], const int n_r, const int n_c,const int ideal,
	int &score,const int sec, const int top_grade)
{
	/* 上状态栏显示内容 */
	output_top_info(pCGI, ideal, score, top_grade, sec);

	char keyboard;
	int is_step;
	while (1) {
		if (keyboard = _getch()) {
			if (keyboard == 'q' || keyboard == 'Q')//退出键 
				return 0;
			if (keyboard == 'r' || keyboard == 'R')
				return 2;
			keyboard = _getch();
			switch (keyboard) {
			case 72:
				is_step = up(pCGI, bdi_normal, p, n_r, n_c, ideal, score, sec, top_grade);
				return is_step + 3;
				break;
			case 80:
				is_step = down(pCGI, bdi_normal, p, n_r, n_c, ideal, score, sec, top_grade);
				return is_step + 3;
				break;
			case 75:
				is_step = left(pCGI, bdi_normal, p, n_r, n_c, ideal, score, sec, top_grade);
				return is_step + 3;
				break;
			case 77:
				is_step = right(pCGI, bdi_normal, p, n_r, n_c, ideal, score, sec, top_grade);
				return is_step + 3;
				break;
			}
		}
	}
}
int main()
{
	setcolor();
	srand((unsigned int)(time(0)));
	int n_r, n_c, ideal_score, delay;
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
		{2, COLOR_HYELLOW, COLOR_BLACK, NULL},
		{4, COLOR_HRED, COLOR_BLACK, NULL},
		{8, COLOR_GREEN, COLOR_BLACK, NULL},
		{16, COLOR_RED, COLOR_BLACK, NULL},
		{32, COLOR_HGREEN, COLOR_BLACK, NULL},
		{64, COLOR_HYELLOW, COLOR_BLACK, NULL},
		{128, COLOR_HRED, COLOR_BLACK, NULL},
		{256, COLOR_HWHITE, COLOR_BLACK, NULL},
		{512, COLOR_HBLACK, COLOR_BLACK, NULL},
		{1024, COLOR_HPINK, COLOR_BLACK, NULL},
		{2048, COLOR_WHITE, COLOR_BLACK, NULL},
		{4096, COLOR_YELLOW, COLOR_BLACK, NULL},
		{8192, COLOR_PINK, COLOR_BLACK, NULL},
		{16384, COLOR_HBLUE, COLOR_BLACK, NULL},
		{32768, COLOR_HCYAN, COLOR_BLACK, NULL},
		{65536, COLOR_HGREEN, COLOR_BLACK, NULL},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
	};

	while (1) {
		int score = 0;
		int chess[8][10] = { 0 };
		menu(n_r, n_c, ideal_score, delay);
		CONSOLE_GRAPHICS_INFO CGI; //声明一个CGI变量  //用于完整版实现
		CONSOLE_GRAPHICS_INFO *pCGI = &CGI;

		//创建结构体用以存储最高分信息
		TopGrade topgrade[5][7];
		for (int i = 0; i < 5 ; i++) 
		    for (int j = 0;j < 7; j++) {
				topgrade[i][j].row = 4 + i;
				topgrade[i][j].col = 4 + j;
				topgrade[i][j].grade = 0;
		}

		int top_grade = read_file(topgrade,n_r,n_c);//读取最高分信息
		Init(pCGI, bdi_normal, chess, n_r, n_c,ideal_score,top_grade);                         //游戏初始化
		int time_delay[6] = { 1,4,9,16,25,60};
		gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED,time_delay[delay]);
		clock_t start, end;//用于计时
		start = clock();
		while (1) {

			if (int is_win = Gameover(chess, n_r, n_c, ideal_score)) {
				int choice = Gameoveroutput(is_win - 1);
				if (choice == 2) {
					if (score > top_grade)
						write_file(topgrade, n_r, n_c, score);
					break;
				}
				if (choice == 0) {
					if (score > top_grade)
						write_file(topgrade, n_r, n_c, score);
					exit(0);
				}
			}

			end = clock();
			int sec = (int)((double)(end - start) / CLOCKS_PER_SEC);

			int choice = Movement(pCGI,bdi_normal,chess,n_r,n_c,ideal_score,score,sec,top_grade);      //移动操作

			if (choice == 2) {
				if (score > top_grade)
					write_file(topgrade, n_r, n_c, score);
				break;
			}
			if (choice == 0) {
				if (score > top_grade)
					write_file(topgrade, n_r, n_c, score);
				exit(0);
			}

			//生成之前也需要检验是否可以继续生成，否则会陷入死循环
			if (int is_win = Gameover(chess, n_r, n_c, ideal_score)) {
				int choice = Gameoveroutput(is_win - 1);
				if (choice == 2) {
					if (score > top_grade)
						write_file(topgrade, n_r, n_c, score);
					break;
				}
				if (choice == 0) {
					if (score > top_grade)
						write_file(topgrade, n_r, n_c, score);
					exit(0);
				}
			}
			if (choice == 4)
			    Generate(pCGI, bdi_normal, chess, n_r, n_c);       //随机生成一个元素
		}

	}
	return 0;
}
