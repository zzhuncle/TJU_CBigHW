/* 1853790 庄镇华 数强 */
#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <cmath>
#include "90-b5.h"
#include "../common/cmd_net_tools.h"
#include "../common/md5.h"
#include "../common/cmd_gmw_tools.h"
#include <fstream>
const int Delay = 0;
const int PlaneShape[5][5] = {
		{0, 0, 4, 0, 0},
		{1, 1, 1, 1, 1},
		{0, 0, 1, 0, 0},
		{0, 1, 1, 1, 0},
		{0, 0, 0, 0, 0}
};

/***************************************************************************
  函数名称：
  功    能：动态申请初始化
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Init3d(int**** Methods, int**** Store)
{
	(*Methods) = new(nothrow) int**[N];
	for (int i = 0; i < N; i++) {
		(*Methods)[i] = new(nothrow) int*[Size];
		for (int j = 0; j < Size; j++) {
			(*Methods)[i][j] = new(nothrow) int[Size];
			for (int k = 0; k < Size; k++)
				(*Methods)[i][j][k] = 0;
		}
	}
	(*Store) = new(nothrow) int**[N];
	for (int i = 0; i < N; i++) {
		(*Store)[i] = new(nothrow) int*[4];
		for (int j = 0; j < 4; j++) {
			(*Store)[i][j] = new(nothrow) int[4];
			for (int k = 0; k < 4; k++)
				(*Store)[i][j][k] = 0;
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：三维数组动态申请的释放
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void free3d(int**** Methods, int**** Store)
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < Size; j++) {
			delete ((*Methods)[i][j]);
		}
		delete ((*Methods)[i]);
	}
	delete (*Methods);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < 4; j++) {
			delete ((*Store)[i][j]);
		}
		delete ((*Store)[i]);
	}
	delete (*Store);
}

/***************************************************************************
  函数名称：
  功    能：根据已有信息确定双坐标法的首尾坐标
  输入参数：cnt代表序号，i代表第几架飞机
  返 回 值：
  说    明：
***************************************************************************/
void FindOnly(int*** Store, int cnt, char &head_row, char &tail_row, int &head_col, int &tail_col, int i)
{
	if (Store[cnt][i][3] == 0) {
		head_row = Store[cnt][i][1] + 'A';
		head_col = Store[cnt][i][2] + 2;
		tail_row = Store[cnt][i][1] + 3 + 'A';
		tail_col = Store[cnt][i][2] + 2;
	}
	if (Store[cnt][i][3] == 1) {
		head_row = Store[cnt][i][1] + 2 + 'A';
		head_col = Store[cnt][i][2];
		tail_row = Store[cnt][i][1] + 2 + 'A';
		tail_col = Store[cnt][i][2] + 3;
	}
	if (Store[cnt][i][3] == 2) {
		head_row = Store[cnt][i][1] + 3 + 'A';
		head_col = Store[cnt][i][2] + 2;
		tail_row = Store[cnt][i][1] + 'A';
		tail_col = Store[cnt][i][2] + 2;
	}
	if (Store[cnt][i][3] == 3) {
		head_row = Store[cnt][i][1] + 2 + 'A';
		head_col = Store[cnt][i][2] + 3;
		tail_row = Store[cnt][i][1] + 2 + 'A';
		tail_col = Store[cnt][i][2];
	}
}

/***************************************************************************
  函数名称：
  功    能：检测棋盘上某个区域的状态
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int CheckInfo(int discovered[Size][Size], int plane[Size][Size], int x, int y) {
	if ((0 <= x && x < Size) && (0 <= y && y < Size)) {
		if (discovered[x][y])
			return plane[x][y];
		else
			return -1;//未查看过这个方格
	}
	else
		return -2;//在棋盘外
}

/***************************************************************************
  函数名称：
  功    能：得到旋转后的飞机模型
  输入参数：
  返 回 值：
  说    明：根据旋转角度分类
***************************************************************************/
int SwitchPlane(int direction, int x, int y)
{
	switch (direction) {
	case 0: //原来
		return PlaneShape[x][y];
	case 1: //旋转90°
		return PlaneShape[y][x];
	case 2: //旋转180
		if (3 - x >= 0)
			return PlaneShape[3 - x][y];
		else
			return 0;
	case 3: //旋转270
		if (3 - y >= 0)
			return PlaneShape[3 - y][x];
		else
			return 0;
	default:
		return 0;
	}
}

/***************************************************************************
  函数名称：
  功    能：检测该区域是否可以放置飞机
  输入参数：x，y 表示飞机所在的区域中，最左上角的位置(空地)
  返 回 值：
  说    明：
***************************************************************************/
bool CanSetPlane(int plane[Size][Size], int x, int y, int dir)
{
	dir %= 4; // dir=0,1,2,3 表示将飞机逆时针旋转 0°90，180，270 
	for (int i = 0; i <= 4; i++) {
		for (int j = 0; j <= 4; j++) {
			int xn = x + i, yn = y + j; // 当前点的坐标 
			if (SwitchPlane(dir, i, j) > 0)// 不是空地的地方才需要检验 
			{
				if (!(0 <= xn && xn <= Size - 1) || !(0 <= yn && yn <= Size - 1))
					return false; // 在棋盘外 
				if (plane[xn][yn] != 0)
					return false; // 覆盖其他飞机 
			}
		}
	}
	return true; // 可以放置飞机 
}

/***************************************************************************
  函数名称：
  功    能：放置飞机
  输入参数：
  返 回 值：
  说    明：放前一定要确定可以放置
***************************************************************************/
void SetPlane(int plane[Size][Size], int x, int y, int dir) {
	dir %= 4;
	for (int i = 0; i <= 4; i++) {
		for (int j = 0; j <= 4; j++) {
			int xn = x + i, yn = y + j;     // 当前点的坐标 
			if (SwitchPlane(dir, i, j) > 0) {    // 不是空地的地方才需要放置 
				plane[xn][yn] = SwitchPlane(dir, i, j);
			}
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：放置飞机的“析构函数”
  输入参数：
  返 回 值：
  说    明：清零错误放置的飞机
***************************************************************************/
void UnSetPlane(int plane[Size][Size], int x, int y, int dir) {
	dir %= 4;
	for (int i = 0; i <= 4; i++) {
		for (int j = 0; j <= 4; j++) {
			int xn = x + i, yn = y + j; // 当前点的坐标 
			if (SwitchPlane(dir, i, j) > 0) {
				plane[xn][yn] = 0;
			}
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：处理单坐标形式的得分及其他
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void ai_single_deal(const string spack, int &score, CONSOLE_GRAPHICS_INFO * pCGI, const BLOCK_DISPLAY_INFO bdi[], int plane[][Size],
	const int xn, const int yn)
{
	if (spack == "HitFail") {//所选坐标为空
		score -= 1;
		plane[xn][yn] = 0;
		gmw_draw_block(pCGI, xn, yn, -2, bdi);
	}
	if (spack == "HitBody") {//所选坐标为机身
		score += 1;
		plane[xn][yn] = 1;
		gmw_draw_block(pCGI, xn, yn, 1, bdi);
	}
	if (spack == "HitHead") {//所选坐标为机头
		score += 2;
		plane[xn][yn] = 4;
		gmw_draw_block(pCGI, xn, yn, 4, bdi);
	}
	if (spack == "HitRepeat")
		score -= 10;
	if (spack == "InvalidCoordinate")
		score -= 10;
}

/***************************************************************************
  函数名称：
  功    能：处理双坐标形式的得分及其他
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void ai_double_deal(const string spack, int &score, CONSOLE_GRAPHICS_INFO * pCGI, const BLOCK_DISPLAY_INFO bdi[], int plane[][Size],
	const char head_row, const char tail_row, const int head_col, const int tail_col, int*** Methods, const int n)
{
	if (spack == "HitFail") {//所选坐标为空
		score -= 3;
		gmw_draw_block(pCGI, int(head_row - 'A'), head_col, -2, bdi);
		gmw_draw_block(pCGI, int(tail_row - 'A'), tail_col, -2, bdi);
		Sleep(200);
		gmw_draw_block(pCGI, int(head_row - 'A'), head_col, plane[int(head_row - 'A')][head_col], bdi);
		gmw_draw_block(pCGI, int(tail_row - 'A'), tail_col, plane[int(head_row - 'A')][head_col], bdi);
	}
	if (spack == "HitPlane") {//所选坐标为飞机
		CalScore(int(head_row - 'A'), head_col, int(tail_row - 'A'), tail_col, plane, score);
		paint_whole__plane(int(head_row - 'A'), head_col, int(tail_row - 'A'), tail_col, pCGI, bdi, plane, 3);
	}
	if (spack == "HitRepeat")
		score -= 10;
	if (spack == "InvalidCoordinate")
		score -= 10;
}

/***************************************************************************
  函数名称：
  功    能：递归找到所有的方案并且存储
  输入参数：
  返 回 值：
  说    明：分支结束的条件有两个，一个是已经遍历完所有的棋盘方格(遍历方式:先列后行)，
			另一个是已经找到一种解决方案
***************************************************************************/
void FindAllSolutions(int x, int y, int &left, int &cnt, int plane[Size][Size], int*** Methods, int*** Store, int X[4], int Y[4], int dir[4])
{
	if (left == 0) {
		cnt++;
		for (int i = 0; i < Size; i++)
			memcpy(Methods[cnt][i], plane[i], Size * sizeof(int));
		for (int i = 1; i <= 3; i++) { // 储存三架飞机的坐标 
			Store[cnt][i][1] = X[i];
			Store[cnt][i][2] = Y[i];
			Store[cnt][i][3] = dir[i];
		}
		return;
	}
	if (x == 7)
		return;

	// 当前位置放飞机 
	for (int i = 0; i <= 3; i++)
	{
		if (CanSetPlane(plane, x, y, i))
		{
			SetPlane(plane, x, y, i);
			// 储存当前方案 
			X[left] = x;//最左上角的横坐标
			Y[left] = y;//最左上角的纵坐标
			dir[left] = i;//飞机的方向
			left--;
			if (y == 7)
				FindAllSolutions(x + 1, 0, left, cnt, plane, Methods, Store, X, Y, dir);
			else
				FindAllSolutions(x, y + 1, left, cnt, plane, Methods, Store, X, Y, dir);
			//清零，确保遍历所有的方向
			left++;
			UnSetPlane(plane, x, y, i);
		}
	}

	// 当前位置不放飞机 
	if (y == 7)
		FindAllSolutions(x + 1, 0, left, cnt, plane, Methods, Store, X, Y, dir);
	else
		FindAllSolutions(x, y + 1, left, cnt, plane, Methods, Store, X, Y, dir);
}

/***************************************************************************
  函数名称：
  功    能：每一次点击最优方格用以确定三架飞机的分布
  输入参数：
  返 回 值：
  说    明：由于已经知道所有可能方案的分布，因此只需要每次点开一个方格进行排除，就可以得到最后唯一的方案
			为了较快的排除以及较高的得分率，每次都点开最优的方格，这个方格的得分概率最接近0.94
***************************************************************************/
int GetPosition(int& Xn, int& Yn, int &last_x, int &last_y, int discovered[Size][Size], int plane[Size][Size], int*** Methods, bool Unavailable[N])
{
	int avai_cnt = 0; // 计算当前可能成为答案的方案数 
	int last_method_cnt = 0; // 记录当前可行方案中的最后一种
	// 用于处理当前可行方案只剩下一种的情况 
	if (last_x != 0 && last_y != 0) { // 如果不是第一次查询 
		int tmp = CheckInfo(discovered, plane, last_x, last_y); // 得到上一次查询的位置的值 
		if (tmp != -1) {
			for (int i = 1; i <= M; i++) {
				if (Methods[i][last_x][last_y] != tmp)
					Unavailable[i] = true;
				if (!Unavailable[i]) { // 可行方案 
					avai_cnt++;
					last_method_cnt = i;
				}
			}
		}
	}
	else
		avai_cnt = M; // 第一次查询

	if (avai_cnt == 0) {
		cout << "所有情况没找全,出错了！！！";//应该不可能，不过写上以防万一
		exit(0);
	}

	if (avai_cnt != 1) { // 方案未锁定 
		int position[Size][Size] = { 0 }; // 记录分数 
		int total[Size][Size] = { 0 };    // 记录总数
		for (int i = 1; i <= M; i++) {
			if (!Unavailable[i]) {
				for (int x = 0; x < Size; x++) {
					for (int y = 0; y < Size; y++) {
						if (Methods[i][x][y] != 4) {
							position[x][y] += (Methods[i][x][y] != 0);
							total[x][y]++;
						}
						else {
							position[x][y] += 2;
							total[x][y] += 2;
						}
					}
				}
			}
		}
		double cur_rate = 0;
		int cur_x = 0, cur_y = 0; // 当前寻找到的最优查询位置 
		for (int x = 0; x < Size; x++) {
			for (int y = 0; y < Size; y++) {
				if (CheckInfo(discovered, plane, x, y) == -1) { /// 这个位置还未被查询过 
					double rate = (double)position[x][y] / total[x][y];
					if (fabs(rate - 0.94) < fabs(cur_rate - 0.94)) {
						cur_rate = rate;
						cur_x = x;
						cur_y = y;
					}
					if (fabs(rate - 1) < 1e-6) {//如果可以直接确定，当然最好
						cur_x = x;
						cur_y = y;
						break;
					}
				}
			}
		}
		last_x = cur_x;
		Xn = last_x;
		last_y = cur_y;
		Yn = last_y;
		return N;
	}

	else
		return last_method_cnt;
	return -1;
}

/***************************************************************************
  函数名称：
  功    能：智能版游戏过程的实现
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int console_ai_game_progress(cmd_tcp_socket &client, CONSOLE_GRAPHICS_INFO * pCGI, const BLOCK_DISPLAY_INFO bdi[],
	int plane[][Size], int score, int*** Methods, int*** Store)
{
	int discovered[Size][Size] = { 0 };
	for (int i = 0; i < Size; i++)
		for (int j = 0; j < Size; j++)
			plane[i][j] = 0;

	int last_x = 0, last_y = 0; // 记录上一次查询的位置 
	int Xn = 0, Yn = 0;
	bool Unavailable[N] = { false }; // 判断某种方案 当前是否还可能成为答案

	char row, head_row, tail_row;
	int col, head_col = 0, tail_col = 0;
	bool recv_startgame = false;

	string spack;
	if (client.get_gameprogress_string(spack) < 0)
		return -1;
	if (spack == "StartGame")
		recv_startgame = true;
	if (!recv_startgame)
		return -1;
	if (spack == "GameOver")
		return 0;

	while (1) {
		if (spack == "StartGame")
			recv_startgame = true;
		if (!recv_startgame)
			return -1;
		if (spack == "GameOver")
			return 0;

		while (1) {
			int n = GetPosition(Xn, Yn, last_x, last_y, discovered, plane, Methods, Unavailable);
			if (n == N) { //未确定唯一的方案
				row = Xn + 'A', col = Yn;
				client.send_coordinate(row, col);
				discovered[Xn][Yn] = 1;//表示此方块已经翻开过，不能再翻，否则扣分严重
				if (client.get_gameprogress_string(spack) < 0)
					return -1;
				gotoxy(20, 1);
				cout << spack << "    ";
				ai_single_deal(spack, score, pCGI, bdi, plane, Xn, Yn);
				if (spack == "StartGame")
					recv_startgame = true;
				if (!recv_startgame)
					return -1;
				Sleep(Delay);
				output_top_info(pCGI, score);//实时更新得分
			}
			else { //找到唯一方案，可以直接双坐标法确定
				for (int i = 1; i <= 3; i++) {
					FindOnly(Store, n, head_row, tail_row, head_col, tail_col, i);
					//先用单坐标把没点过的机头点了，以获取最高得分
					for (int j = 0; j <= 4; j++) {
						for (int k = 0; k <= 4; k++) {
							int xn = Store[n][i][1] + j, yn = Store[n][i][2] + k; // 当前点的坐标 
							if (SwitchPlane(Store[n][i][3], j, k) > 0 && discovered[xn][yn] == 0 &&
								(xn == int(head_row - 'A') && yn == head_col))
							{
								row = xn + 'A', col = yn;
								client.send_coordinate(row, col);

								discovered[xn][yn] = 1;//表示此方块已经翻开过，不能再翻，否则扣分严重

								if (client.get_gameprogress_string(spack) < 0)
									return -1;
								gotoxy(20, 1);
								cout << spack << "    ";
								ai_single_deal(spack, score, pCGI, bdi, plane, xn, yn);
								if (spack == "StartGame")
									recv_startgame = true;
								if (!recv_startgame)
									return -1;
								Sleep(Delay);
								output_top_info(pCGI, score);//实时更新得分
							}
						}
					}
					//再用双坐标进行绝杀
					client.send_plane_coordinates(head_row, head_col, tail_row, tail_col);
					if (client.get_gameprogress_string(spack) < 0)
						return -1;
					gotoxy(20, 1);
					cout << spack << "    ";
					ai_double_deal(spack, score, pCGI, bdi, plane, head_row, tail_row, head_col, tail_col, Methods, n);
					if (spack == "StartGame")
						recv_startgame = true;
					if (!recv_startgame)
						return -1;
					if (spack == "GameOver") {//游戏结束也别忘了更新得分与界面
						CalScore(int(head_row - 'A'), head_col, int(tail_row - 'A'), tail_col, plane, score);
						paint_whole__plane(int(head_row - 'A'), head_col, int(tail_row - 'A'), tail_col, pCGI, bdi, plane, 3);
						output_top_info(pCGI, score);
						gotoxy(0, 2);
						cout << "本次GameID:" << client.get_gameid() << "  本次得分:" << client.get_score();
						return 0;
					}
					Sleep(Delay);
					output_top_info(pCGI, score);
				}
			}
		}
	}
}