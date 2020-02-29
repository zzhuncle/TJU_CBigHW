/* 1853790 庄镇华 数强 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "90-b5.h"
#include "../common/cmd_net_tools.h"
#include "../common/md5.h"
#include "../common/cmd_gmw_tools.h"
/***************************************************************************
  函数名称：
  功    能：处理单坐标形式的得分及其他
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void human_single_deal(const string spack, int &score, CONSOLE_GRAPHICS_INFO * pCGI, const BLOCK_DISPLAY_INFO bdi[], int plane[][Size],
	const char mrow1, const int mcol1)
{
	if (spack == "HitFail") {//所选坐标为空
		score -= 1;
		plane[mrow1][mcol1] = -2;
		gmw_draw_block(pCGI, mrow1, mcol1, -2, bdi);
	}
	if (spack == "HitBody") {//所选坐标为机身
		score += 1;
		plane[mrow1][mcol1] = 1;
		gmw_draw_block(pCGI, mrow1, mcol1, 1, bdi);
	}
	if (spack == "HitHead") {//所选坐标为机头
		score += 2;
		plane[mrow1][mcol1] = 4;
		gmw_draw_block(pCGI, mrow1, mcol1, 4, bdi);
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
void human_double_deal(const string spack, int &score, CONSOLE_GRAPHICS_INFO * pCGI, const BLOCK_DISPLAY_INFO bdi[], int plane[][Size],
	const char mrow1, const char mrow2, const int mcol1, const int mcol2)
{
	if (spack == "HitFail") {//所选坐标为空
		score -= 3;
		gmw_draw_block(pCGI, mrow1, mcol1, -2, bdi);
		gmw_draw_block(pCGI, mrow2, mcol2, -2, bdi);
		Sleep(200);
		gmw_draw_block(pCGI, mrow1, mcol1, plane[mrow1][mcol1], bdi);
		gmw_draw_block(pCGI, mrow2, mcol2, plane[mrow2][mcol2], bdi);
	}
	if (spack == "HitPlane") {//所选坐标为飞机
		CalScore(mrow1, mcol1, mrow2, mcol2, plane, score);
		paint_whole__plane(mrow1, mcol1, mrow2, mcol2, pCGI, bdi, plane, 3);
	}
	if (spack == "HitRepeat")
		score -= 10;
	if (spack == "InvalidCoordinate")
		score -= 10;

}

/***************************************************************************
  函数名称：
  功    能：人工版游戏过程的实现
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int console_human_game_progress(cmd_tcp_socket &client, CONSOLE_GRAPHICS_INFO * pCGI, const BLOCK_DISPLAY_INFO bdi[], int plane[][Size], int score)
{
	char row, head_row, tail_row;
	int col, head_col, tail_col;
	bool recv_startgame = false;

	int loop = 1;
	int maction, mrow1, mcol1, mrow2, mcol2;
	int keycode1, keycode2;
	int ret;

	string spack;
	if (client.get_gameprogress_string(spack) < 0) {
		return -1;
	}
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
			ret = gmw_read_keyboard_and_mouse(pCGI, maction, mrow1, mcol1, keycode1, keycode2);
			if (ret == CCT_MOUSE_EVENT) {
				if (maction == MOUSE_RIGHT_BUTTON_CLICK) {
					gmw_draw_block(pCGI, mrow1, mcol1, 4, bdi);
					while (1) {
						ret = gmw_read_keyboard_and_mouse(pCGI, maction, mrow2, mcol2, keycode1, keycode2);
						if (ret == CCT_MOUSE_EVENT) {//此时为双坐标方式
							head_row = mrow1 + 'A', tail_row = mrow2 + 'A', head_col = mcol1, tail_col = mcol2;
							client.send_plane_coordinates(head_row, head_col, tail_row, tail_col);
							if (client.get_gameprogress_string(spack) < 0) {
								return -1;
							}
							gotoxy(20, 1);
							cout << spack << "    ";
							human_double_deal(spack, score, pCGI, bdi, plane, mrow1, mrow2, mcol1, mcol2);
							if (spack == "StartGame")
								recv_startgame = true;
							if (!recv_startgame)
								return -1;
							if (spack == "GameOver") {//游戏结束也别忘了更新得分与界面
								gotoxy(0, 2);
								cout << "本次GameID:" << client.get_gameid() << "  本次得分:" << client.get_score();
								CalScore(mrow1, mcol1, mrow2, mcol2, plane, score);
								paint_whole__plane(mrow1, mcol1, mrow2, mcol2, pCGI, bdi, plane, 3);
								return 0;
							}
							output_top_info(pCGI, score);
							break;
						}
					}
				}
				else {//此时为单坐标方式
					row = mrow1 + 'A', col = mcol1;
					client.send_coordinate(row, col);
					if (client.get_gameprogress_string(spack) < 0) {
						return -1;
					}
					gotoxy(20, 1);
					cout << spack << "    ";
					human_single_deal(spack, score, pCGI, bdi, plane, mrow1, mcol1);
					if (spack == "StartGame")
						recv_startgame = true;
					if (!recv_startgame)
						return -1;
					output_top_info(pCGI, score);
				}
			}
		}
	}
}

