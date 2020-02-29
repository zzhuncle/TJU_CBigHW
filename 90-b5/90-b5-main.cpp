/* 1853790 庄镇华 数强 */
#define _CRT_SECURE_NO_WARNINGS
#define ai    0
#define human 1
#include<iostream>
#include <iomanip>
#include <conio.h>
#include<cstring>
#include "90-b5.h"
#include "../common/cmd_net_tools.h"
#include "../common/md5.h"
#include "../common/cmd_gmw_tools.h"
using namespace std;
int console_human_game_progress(cmd_tcp_socket &client, CONSOLE_GRAPHICS_INFO * pCGI, const BLOCK_DISPLAY_INFO bdi[],
	int plane[][Size], int score);
int console_ai_game_progress(cmd_tcp_socket &client, CONSOLE_GRAPHICS_INFO * pCGI, const BLOCK_DISPLAY_INFO bdi[],
	int plane[][Size], int score, int*** Methods, int*** Store);
/***************************************************************************
  函数名称：
  功    能：检查学号是否都是数字
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int check_num(const char str[])
{
	for (int i = 0; i < (int)strlen(str); i++) {
		if (str[i] - '9' > 0 || str[i] - '0' < 0) {
			return -1;
		}
	}
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：将学号+MD5(password)再用security_str按位异或后形成要发送的“报道”数据
  输入参数：char *reg_str				：异或后需要发送的串（返回）
			const char *stu_no			：学号
			const char *stu_password	：交作业系统中的口令（明码），无论口令多长，转换为MD5后为32字节的hex串
			const char *security_str	：从Server端收到的异或加密串
  返 回 值：
  说    明：以学号 : 1859999 , 密码 : Password，异或串 : f272a9b7422e0e1ddec6c4b1abe758cadefc658c2 为例
			1、Password     ： => MD5 => “dc647eb65e6711e155375218212b3964”
			2、认证串(原始) ：1859999+dc647eb65e6711e155375218212b3964
			3、认证串(原始)和异或串进行按位异或
				   1859999+dc647eb65e6711e155375218212b3964
				   f272a9b7422ee1ddec6c4b1abe758cadefc658c2
			   进行按位异或，结果不保证一定是图形ASCII字符，所以再转为hex
			   认证串(Hex发送) ：570a020b58005b1c50510451525406525006005405535450575004020d51505c5757515406015506
			   解释：第1个字节 1和f，即 0x31^0x66 = 0x57
					 第2个字节 8和2，即 0x38^0x32 = 0x0a
					 ...(略)
***************************************************************************/
int cmd_tcp_socket::make_register_string(char *send_regstr, const char *stu_no, const char *stu_password, const char *security_str, const char *secure_crt)
{
	/* 需要自己实现 */
	char dst[33], origin[41], finall[41];
	char hex[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
	MD5(dst, stu_password);
	strcpy(origin, stu_no);
	strcat(origin, secure_crt);
	strcat(origin, dst);
	for (int i = 0; i < 40; i++)
		finall[i] = origin[i] ^ security_str[i];
	for (int i = 0; i < 40; i++)
	{
		send_regstr[2 * i] = hex[(int)(finall[i]) / 16];
		send_regstr[2 * i + 1] = hex[(int)(finall[i]) % 16];
	}
	send_regstr[80] = '\0';
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：实现图形化界面
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void console_manual(const char *num, const char *psw, const int mood)
{
	CONSOLE_GRAPHICS_INFO CGI;
	CONSOLE_GRAPHICS_INFO *pCGI = &CGI;
	const BLOCK_DISPLAY_INFO bdi[] = {
		{BDI_VALUE_BLANK, 7, 0, "  "},  //0不显示，用空格填充即可
		{1, 7, 1, "◎"},
		{4, 7, 4, "〇"},
		{-2, 7, 0, "ⅹ"},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
	};

	//初始化
	cls();
	gmw_init(pCGI, Size, Size, COLOR_WHITE, COLOR_GREEN);
	gmw_set_rowno_switch(pCGI, true);
	gmw_set_colno_switch(pCGI, true);
	setcursor(CURSOR_INVISIBLE);//光标设置
	gmw_set_font(pCGI, "新宋体", 28, 28);//字体设置
	gmw_set_ext_rowcol(pCGI, 2, 6, 0, 0);//额外的空间设置
	gmw_set_frame_default_linetype(pCGI, 4);//设置框架风格
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, 20);//移动延时设置
	const int sleep_ms = 5000;
	cmd_tcp_socket client;
	client.set_debug_switch(false);

	int*** Methods = NULL;// 用于记录全部的合法方案 
	int*** Store = NULL;// Store[cnt][i][1/2/3] 表示第cnt种方案中第i架飞机的位置信息 

	if (mood == ai) {
		gotoxy(0, 0);
		cout << "初始化中...";
		Init3d(&Methods, &Store);
		int plane[Size][Size] = { 0 };
		int left = 3;                                                       // 剩余飞机数量 
		int X[4] = { 0 }, Y[4] = { 0 }, dir[4] = { 0 }, cnt = 0;            // cnt表示合法的方案数
		FindAllSolutions(0, 0, left, cnt, plane, Methods, Store, X, Y, dir);// 统计所有方案数
	}

	while (1) {
		if (mood == human) {
			int score = 100;
			int plane[Size][Size] = { 0 };

			/* 显示框架 */
			gmw_draw_frame(pCGI);

			/* 上状态栏显示内容以及游戏说明 */
			char temp[256];
			temp[0] = '\0';
			gotoxy(0, 0);
			cout << "左键单坐标,右键双坐标" << endl;
			cout << "R键重新连接 Q键退出";
			output_top_info(pCGI, score);

			// 下状态栏显示内容 
			gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前光标]01行0A列");
			bool first = true;

			while (1) {
				if (!first) { //出任何错误，延时5秒重连（不包括第一次）
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "与服务器的连接断开!");
					Sleep(sleep_ms);
				}
				else
					first = false;
				if (client.connect() < 0) {
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "连接服务器失败!");
					continue;//return -1;
				}
				string s1;
				if (client.get_security_string(s1) < 0) {
					client.close();
					continue;// return -2;
				}
				char reg_str[81];
				client.make_register_string(reg_str, num, psw, s1.c_str());
				client.send_register_string(reg_str);

				/* 进入游戏交互环节
				   1、必须收到Server的StartGame，才能收发后续
				   2、收到Server的GameOver则返回0，游戏结束
				   3、其它错误均返回-1（报文信息不正确等错误），重连，再次重复	*/
					if (console_human_game_progress(client, pCGI, bdi, plane, score) < 0) {
						client.close();
						continue;
					}
					else
						break;
			}
		}

		if (mood == ai) {
			for (int ii = 0; ii < 52; ii++) {
				int score = 100;
				int plane[Size][Size] = { 0 };

				/* 显示框架 */
				gmw_draw_frame(pCGI);

				/* 上状态栏显示内容以及游戏说明 */
				char temp[256];
				temp[0] = '\0';
				gotoxy(0, 0);
				cout << "左键单坐标,右键双坐标" << endl;
				cout << "R键重新连接 Q键退出";
				output_top_info(pCGI, score);

				// 下状态栏显示内容 
				gmw_status_line(pCGI, LOWER_STATUS_LINE, "[当前光标]01行0A列");
				bool first = true;
				if (!first) { //出任何错误，延时5秒重连（不包括第一次）
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "与服务器的连接断开!");
					Sleep(sleep_ms);
				}
				else
					first = false;
				if (client.connect() < 0) {
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "连接服务器失败!");
					continue;//return -1;
				}
				string s1;
				if (client.get_security_string(s1) < 0) {
					client.close();
					continue;// return -2;
				}
				char reg_str[81];
				char secure_crt[2];
				if (ii < 26)
					secure_crt[0] = 'A' + ii;
				else
					secure_crt[0] = 'a' + ii - 26;
				secure_crt[1] = '\0';

				client.make_register_string(reg_str, num, psw, s1.c_str(), secure_crt);
				client.send_register_string(reg_str);

				/* 进入游戏交互环节
				   1、必须收到Server的StartGame，才能收发后续
				   2、收到Server的GameOver则返回0，游戏结束
				   3、其它错误均返回-1（报文信息不正确等错误），重连，再次重复	*/
				if (console_ai_game_progress(client, pCGI, bdi, plane, score, Methods, Store) < 0) {
					client.close();
					continue;
				}
			}
		}

		char sel;
		while (1) {
			gotoxy(0, 1);
			cout << "R键重新连接 Q键退出";
			sel = getchar();
			if (sel == 'Q' || sel == 'R' || sel == 'q' || sel == 'r')
				break;
		}
		if (sel == 'Q' || sel == 'q')
			break;
		else
			continue;
	}

	client.close();
	gmw_status_line(pCGI, LOWER_STATUS_LINE, "游戏结束");

	if (mood == ai)//别忘了释放，否则会丢内存
		free3d(&Methods, &Store);
	return;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：argv[1] : "-auto"/"-manual"，表示自动/人工游戏
			argv[2] : 学号
			argv[3] : 密码
  返 回 值：
  说    明：
***************************************************************************/
int main(int argc, char **argv)
{
	if (argc != 4) {
		cout << "Usage : " << argv[0] << " -auto|-manual  stu_no  stu_pwd" << endl;
		return -1;
	}
	/* 其余合理性检查未做，需要自己补
	   1、argv[1]除 -auto -manual 外均错误
	   2、学号7位
	   3、学号7位数字
	   4、*/
	if (strcmp(argv[1], "-auto") != 0 && strcmp(argv[1], "-manual") != 0) {
		cout << "Usage : " << argv[0] << " -auto|-manual  stu_no  stu_pwd" << endl;
		return -1;
	}
	if (strlen(argv[2]) != 7) {
		cout << "学号不是7位" << endl;
		return -1;
	}
	if (check_num(argv[2]) == -1) {
		cout << "学号含非数字符号" << endl;
		return -1;
	}

	int mood = (strcmp(argv[1], "-auto") == 0 ? 0 : 1);
	console_manual(argv[2], argv[3], mood);
	return 0;
}
