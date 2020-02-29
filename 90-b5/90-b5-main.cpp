/* 1853790 ׯ�� ��ǿ */
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
  �������ƣ�
  ��    �ܣ����ѧ���Ƿ�������
  ���������
  �� �� ֵ��
  ˵    ����
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
  �������ƣ�
  ��    �ܣ���ѧ��+MD5(password)����security_str��λ�����γ�Ҫ���͵ġ�����������
  ���������char *reg_str				��������Ҫ���͵Ĵ������أ�
			const char *stu_no			��ѧ��
			const char *stu_password	������ҵϵͳ�еĿ�����룩�����ۿ���೤��ת��ΪMD5��Ϊ32�ֽڵ�hex��
			const char *security_str	����Server���յ��������ܴ�
  �� �� ֵ��
  ˵    ������ѧ�� : 1859999 , ���� : Password����� : f272a9b7422e0e1ddec6c4b1abe758cadefc658c2 Ϊ��
			1��Password     �� => MD5 => ��dc647eb65e6711e155375218212b3964��
			2����֤��(ԭʼ) ��1859999+dc647eb65e6711e155375218212b3964
			3����֤��(ԭʼ)����򴮽��а�λ���
				   1859999+dc647eb65e6711e155375218212b3964
				   f272a9b7422ee1ddec6c4b1abe758cadefc658c2
			   ���а�λ��򣬽������֤һ����ͼ��ASCII�ַ���������תΪhex
			   ��֤��(Hex����) ��570a020b58005b1c50510451525406525006005405535450575004020d51505c5757515406015506
			   ���ͣ���1���ֽ� 1��f���� 0x31^0x66 = 0x57
					 ��2���ֽ� 8��2���� 0x38^0x32 = 0x0a
					 ...(��)
***************************************************************************/
int cmd_tcp_socket::make_register_string(char *send_regstr, const char *stu_no, const char *stu_password, const char *security_str, const char *secure_crt)
{
	/* ��Ҫ�Լ�ʵ�� */
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
  �������ƣ�
  ��    �ܣ�ʵ��ͼ�λ�����
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void console_manual(const char *num, const char *psw, const int mood)
{
	CONSOLE_GRAPHICS_INFO CGI;
	CONSOLE_GRAPHICS_INFO *pCGI = &CGI;
	const BLOCK_DISPLAY_INFO bdi[] = {
		{BDI_VALUE_BLANK, 7, 0, "  "},  //0����ʾ���ÿո���伴��
		{1, 7, 1, "��"},
		{4, 7, 4, "��"},
		{-2, 7, 0, "��"},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
	};

	//��ʼ��
	cls();
	gmw_init(pCGI, Size, Size, COLOR_WHITE, COLOR_GREEN);
	gmw_set_rowno_switch(pCGI, true);
	gmw_set_colno_switch(pCGI, true);
	setcursor(CURSOR_INVISIBLE);//�������
	gmw_set_font(pCGI, "������", 28, 28);//��������
	gmw_set_ext_rowcol(pCGI, 2, 6, 0, 0);//����Ŀռ�����
	gmw_set_frame_default_linetype(pCGI, 4);//���ÿ�ܷ��
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, 20);//�ƶ���ʱ����
	const int sleep_ms = 5000;
	cmd_tcp_socket client;
	client.set_debug_switch(false);

	int*** Methods = NULL;// ���ڼ�¼ȫ���ĺϷ����� 
	int*** Store = NULL;// Store[cnt][i][1/2/3] ��ʾ��cnt�ַ����е�i�ܷɻ���λ����Ϣ 

	if (mood == ai) {
		gotoxy(0, 0);
		cout << "��ʼ����...";
		Init3d(&Methods, &Store);
		int plane[Size][Size] = { 0 };
		int left = 3;                                                       // ʣ��ɻ����� 
		int X[4] = { 0 }, Y[4] = { 0 }, dir[4] = { 0 }, cnt = 0;            // cnt��ʾ�Ϸ��ķ�����
		FindAllSolutions(0, 0, left, cnt, plane, Methods, Store, X, Y, dir);// ͳ�����з�����
	}

	while (1) {
		if (mood == human) {
			int score = 100;
			int plane[Size][Size] = { 0 };

			/* ��ʾ��� */
			gmw_draw_frame(pCGI);

			/* ��״̬����ʾ�����Լ���Ϸ˵�� */
			char temp[256];
			temp[0] = '\0';
			gotoxy(0, 0);
			cout << "���������,�Ҽ�˫����" << endl;
			cout << "R���������� Q���˳�";
			output_top_info(pCGI, score);

			// ��״̬����ʾ���� 
			gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���]01��0A��");
			bool first = true;

			while (1) {
				if (!first) { //���κδ�����ʱ5����������������һ�Σ�
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "������������ӶϿ�!");
					Sleep(sleep_ms);
				}
				else
					first = false;
				if (client.connect() < 0) {
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "���ӷ�����ʧ��!");
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

				/* ������Ϸ��������
				   1�������յ�Server��StartGame�������շ�����
				   2���յ�Server��GameOver�򷵻�0����Ϸ����
				   3���������������-1��������Ϣ����ȷ�ȴ��󣩣��������ٴ��ظ�	*/
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

				/* ��ʾ��� */
				gmw_draw_frame(pCGI);

				/* ��״̬����ʾ�����Լ���Ϸ˵�� */
				char temp[256];
				temp[0] = '\0';
				gotoxy(0, 0);
				cout << "���������,�Ҽ�˫����" << endl;
				cout << "R���������� Q���˳�";
				output_top_info(pCGI, score);

				// ��״̬����ʾ���� 
				gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���]01��0A��");
				bool first = true;
				if (!first) { //���κδ�����ʱ5����������������һ�Σ�
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "������������ӶϿ�!");
					Sleep(sleep_ms);
				}
				else
					first = false;
				if (client.connect() < 0) {
					gmw_status_line(pCGI, LOWER_STATUS_LINE, "���ӷ�����ʧ��!");
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

				/* ������Ϸ��������
				   1�������յ�Server��StartGame�������շ�����
				   2���յ�Server��GameOver�򷵻�0����Ϸ����
				   3���������������-1��������Ϣ����ȷ�ȴ��󣩣��������ٴ��ظ�	*/
				if (console_ai_game_progress(client, pCGI, bdi, plane, score, Methods, Store) < 0) {
					client.close();
					continue;
				}
			}
		}

		char sel;
		while (1) {
			gotoxy(0, 1);
			cout << "R���������� Q���˳�";
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
	gmw_status_line(pCGI, LOWER_STATUS_LINE, "��Ϸ����");

	if (mood == ai)//�������ͷţ�����ᶪ�ڴ�
		free3d(&Methods, &Store);
	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������argv[1] : "-auto"/"-manual"����ʾ�Զ�/�˹���Ϸ
			argv[2] : ѧ��
			argv[3] : ����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int main(int argc, char **argv)
{
	if (argc != 4) {
		cout << "Usage : " << argv[0] << " -auto|-manual  stu_no  stu_pwd" << endl;
		return -1;
	}
	/* ��������Լ��δ������Ҫ�Լ���
	   1��argv[1]�� -auto -manual �������
	   2��ѧ��7λ
	   3��ѧ��7λ����
	   4��*/
	if (strcmp(argv[1], "-auto") != 0 && strcmp(argv[1], "-manual") != 0) {
		cout << "Usage : " << argv[0] << " -auto|-manual  stu_no  stu_pwd" << endl;
		return -1;
	}
	if (strlen(argv[2]) != 7) {
		cout << "ѧ�Ų���7λ" << endl;
		return -1;
	}
	if (check_num(argv[2]) == -1) {
		cout << "ѧ�ź������ַ���" << endl;
		return -1;
	}

	int mood = (strcmp(argv[1], "-auto") == 0 ? 0 : 1);
	console_manual(argv[2], argv[3], mood);
	return 0;
}
