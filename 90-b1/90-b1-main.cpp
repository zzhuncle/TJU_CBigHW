/* 1853790 ׯ�� ��ǿ */
#include"90-b1.h"
#include "../common/cmd_gmw_tools.h"
#include "../common/cmd_console_tools.h"
using namespace std;
const int N = 100;
const int COLS = 60;
const int LINES = 20;
int pattern(int(*p1)[9], int(*p2)[9], char option)
{
	srand((unsigned int)(time(0)));

	CONSOLE_GRAPHICS_INFO CGI; //����һ��CGI����  //����������ʵ��
	CONSOLE_GRAPHICS_INFO *pCGI = &CGI;

	int (*pf0)(int(*p1)[9], int(*p2)[9], CONSOLE_GRAPHICS_INFO *pCGI,const int mood);
	pf0 = caidan;
	Fun_Call fc[] = {
		{'A',pf0}, {'\0',NULL}
	};
	if (option == 'Q')
		return 0;
	for (int i = 0; fc[i].option; i++) {
		if (fc[i].option == option) {
			if (fc[i].pf0 != NULL)
				fc[i].pf0(p1,p2, pCGI,option - 'A' + 1);
		}
	}
	return 0;
}
int main()
{
	while (1)
	{
		setconsoleborder(COLS, LINES, COLS, 5 * LINES);//���ô���
		const char *p[] = { "A.cmdͼ�ν���������",
						"Q.�˳�",
			            NULL};
		const char *choice = "AQ";
		int p1[9][9] = { 0 };
		int p2[9][9] = { 0 };
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				p1[i][j] = 0;
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				p2[i][j] = int('0');
		//�ظ���ʾ5��"Hello"
		showstr(5, 5, "90-b1*", COLOR_HYELLOW, COLOR_HBLUE, 5);
		//�ָ���ʼ��ɫ
		setcolor();
		cout << endl;
		char option = menu(p, choice);
		pattern(p1, p2, option);
		if (option == 'Q')
			return 0;
		setcolor(COLOR_BLACK, COLOR_WHITE);
		wait_for_str("end", "��С�������������End����...");
		system("cls");
	}
}
