/* 1853790 ׯ�� ��ǿ */
#include"90-b0.h"
#include "../common/cmd_gmw_tools.h"
#include "../common/cmd_console_tools.h"
using namespace std;
const int width = 80;
const int length = 25;
const int N = 100;
int pattern(char option)
{
	srand((unsigned int)(time(0)));
	int  chesscolor[9][9] = { 0 }; /* �������̲�����ɫ */
	int  chessboard[9][9] = { 0 }; /* ��¼���߹���· */

	CONSOLE_GRAPHICS_INFO ColorLinez_CGI; //����һ��CGI����  //����������ʵ��
	CONSOLE_GRAPHICS_INFO *pCGI = &ColorLinez_CGI;

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			chessboard[i][j] = int('0');
	int grade = 0; /* ���� */
	int(*pf0)(int(*p1)[9], int(*p2)[9], CONSOLE_GRAPHICS_INFO *pCGI, const int mood);
	pf0 = caidan;
	Fun_Call fc[] = {
		{'0',pf0}, {'1',pf0},{'\0',NULL}
	};
	if (option == '0')
		return 0;
	for (int i = 1; fc[i].option; i++) {
		if (fc[i].option == option) {
			if (fc[i].pf0 != NULL)
				fc[i].pf0(chesscolor, chessboard, pCGI, option - '0');
		}
	}
	return 0;
}
int main()
{
	while (1) {
		setfontsize("Terminal", 16, 8);
		const char *p[] = {"1.cmdͼ�ν���������",
						"0.�˳�",NULL };
		const char *choice = "01";
		setconsoleborder(width, length);
		showstr(10, 10, "This is 90-b0\n\n");
		char option = menu(p, choice);
		pattern(option);
		if (option == '0')
			return 0;
		setcolor(COLOR_BLACK, COLOR_WHITE);
		cout << endl<<endl;
		wait_for_str("end", "��С�������������End����...");
		system("cls");
		setfontsize("Terminal", 16, 8);
	}
}