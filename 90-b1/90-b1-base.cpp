/* 1853790 ׯ�� ��ǿ */
#define _CRT_SECURE_NO_WARNINGS
#include "90-b1.h"
#include "../common/cmd_gmw_tools.h"
#include "../common/cmd_console_tools.h"
/***************************************************************************
�ȴ��س���
***************************************************************************/
void wait_for_enter(const char *prompt = "")
{
	cout << endl << prompt;
	while (_getch() != '\r')
		;
}
/*==============================================================================
������ɲ���
==============================================================================*/
void buildchess(int(*p1)[9], int lines, int cols)//n_chess����Ҫ�µ������� 
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
�ж�����֮���Ƿ���Խ���
==============================================================================*/
int is_exchange(int(*p1)[9], int(*p2)[9], int x, int y, int x_, int y_, int n_r, int n_c, const int mood)
{
	tj_swap(p1[x][y], p1[x_][y_]);
	int i = 0; /* ��ʱ���� */
	int sum[5] = { 0 };
	/* ������� */
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
	/* ������ */
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
	/* ������� */
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
	/* ������ */
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
�жϳ�ʼ��������
==============================================================================*/
int delchess(int(*p1)[9], int(*p2)[9], int n_r, int n_c, const int mood)
{
	for (int ii = 0; ii < n_r; ii++)
		for (int jj = 0; jj < n_c; jj++)
			p2[ii][jj] = int('0');
	int sum_total = 0;//����˫��������ļ���
	for (int x = 0; x < n_r; x++)
		for (int y = 0; y < n_c; y++)
			if (p2[x][y] != int('*'))
			{
				int i = 0; /* ��ʱ���� */
				int sum[3] = { 0 };
				/* ������� */
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
				/* ������ */
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
�����0
==============================================================================*/
void delzero(const BLOCK_DISPLAY_INFO bdi_normal[], CONSOLE_GRAPHICS_INFO *pCGI, int(*p1)[9], int(*p2)[9], int n_r, int n_c, const int mood)
{
	for (int j = 0; j < n_c; j++)
	{
		int empty = 0;//��0�ĸ���
		for (int i = n_r - 1; i >= 0; i--)
		{
			int index = 0;//Ҫ����ľ���
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
��ֵ���
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
������ʽ��������ʾ
==============================================================================*/
int mark_remove(int(*p1)[9], int(*p2)[9], int n_r, int n_c, const int mood)
{
	for (int ii = 0; ii < n_r; ii++)
		for (int jj = 0; jj < n_c; jj++)
			p2[ii][jj] = int('0');
	for (int i = 0; i < n_r; i++)
		for (int j = 0; j < n_c; j++)
		{
			if (j - 1 >= 0)//�󽻻�
				if (is_exchange(p1, p2, i, j, i, j - 1, n_r, n_c))
				{
					p2[i][j - 1] = int('*');
					p2[i][j] = int('*');
				}
			if (j + 1 < n_c)//�ҽ���
				if (is_exchange(p1, p2, i, j, i, j + 1, n_r, n_c))
				{
					p2[i][j + 1] = int('*');
					p2[i][j] = int('*');
				}
			if (i + 1 < n_r)//�½���
				if (is_exchange(p1, p2, i + 1, j, i, j, n_r, n_c))
				{
					p2[i + 1][j] = int('*');
					p2[i][j] = int('*');
				}
			if (i - 1 >= 0)//�Ͻ���
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
ʵ��αͼ����������
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
					gmw_draw_block(pCGI, i, j, 0, bdi_normal); //0�ǿհ�
					Sleep(50);
				}
			}
	return;
}
/*==============================================================================
ʵ��αͼ����ʾ������ʾ
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
ʵ��αͼ�ν�������
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
���˵���
==============================================================================*/
int caidan(int(*p1)[9], int(*p2)[9], CONSOLE_GRAPHICS_INFO *pCGI, const int mood)
{
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, 0, 0, "  "},  //0����ʾ���ÿո���伴��
		{1, 1, 0, "��"},
		{2, 2, 0, "��"},
		{3, 3, 0, "��"},
		{4, 4, 0, "��"},
		{5, 5, 0, "��"},
		{6, 6, 0, "��"},
		{7, 7, 0, "��"},
		{8, 8, 0, "��"},
		{9, 9, 0, "��"},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
	};
	/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ�ѡ��״̬�� */
	const BLOCK_DISPLAY_INFO bdi_selected[] = {
	{BDI_VALUE_BLANK, 0, 0, "  "},  //0����ʾ���ÿո���伴��
		{1, 1, 0, "��"},
		{2, 2, 0, "��"},
		{3, 3, 0, "��"},
		{4, 4, 0, "��"},
		{5, 5, 0, "��"},
		{6, 6, 0, "��"},
		{7, 7, 0, "��"},
		{8, 8, 0, "��"},
		{9, 9, 0, "��"},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
	};
	/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ���������ʾ״̬�� */
	const BLOCK_DISPLAY_INFO bdi_prompt[] = {
	{BDI_VALUE_BLANK, 0, 0, "  "},
		{1, 1, 0, "��"},
		{2, 2, 0, "��"},
		{3, 3, 0, "��"},
		{4, 4, 0, "��"},
		{5, 5, 0, "��"},
		{6, 6, 0, "��"},
		{7, 7, 0, "��"},
		{8, 8, 0, "��"},
		{9, 9, 0, "��"},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
	};
	/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ���ը/����״̬�� */
	const BLOCK_DISPLAY_INFO bdi_exploded[] = {
		{BDI_VALUE_BLANK, 0, 0, "  "},
		{1, 1, 0, "��"},
		{2, 2, 0, "��"},
		{3, 3, 0, "��"},
		{4, 4, 0, "��"},
		{5, 5, 0, "��"},
		{6, 6, 0, "��"},
		{7, 7, 0, "��"},
		{8, 8, 0, "��"},
		{9, 9, 0, "��"},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
	};

	system("cls");
	gotoxy(0, 0);
	int n_r, n_c;
	char a1[] = "����������";
	n_r = get_int(a1, 5, 9);
	char a2[] = "����������";
	n_c = get_int(a2, 5, 9);//��������

	buildchess(p1, n_r, n_c);
	drawarray(p1, p2, n_r, n_c, 11);//mood1-1����90-b1�����ҵ�����������������
	wait_for_key("���س�����ʾͼ��...");
	cls();

	//��ʼ��
	gmw_init(pCGI, n_r, n_c, COLOR_BLACK, COLOR_WHITE);

	//����
	gmw_set_rowno_switch(pCGI, true);
	gmw_set_colno_switch(pCGI, true);
	setcursor(CURSOR_INVISIBLE);//�������
	gmw_set_font(pCGI, "������", 28, 28);//��������
	gmw_set_ext_rowcol(pCGI, 0, 0, 0, 0);//����Ŀռ�����
	gmw_set_frame_default_linetype(pCGI, 4);//���ÿ�ܷ��
	gmw_set_delay(pCGI, DELAY_OF_BLOCK_MOVED, 20);//�ƶ���ʱ����

	/* ��ʾ��� */
	gmw_draw_frame(pCGI);

	/* ��״̬����ʾ���� */
	char temp[256];
	temp[0] = '\0';
	sprintf(temp, "��Ļ:%d��%d��(��ǰ����: 0 �Ҽ��˳�)", pCGI->lines, pCGI->cols);
	gmw_status_line(pCGI, TOP_STATUS_LINE, temp);

	/* ��״̬����ʾ���� */
	gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���] 01��0A��");

	/* ���ڲ�����չ�ֵ���Ļ�� */
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
	//������ʼ��������

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
			case MOUSE_LEFT_BUTTON_CLICK:			//�������
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
					setcolor();//�����趨��ʼ����
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
							}//Ѱ����ʼ���������
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
						sprintf(temp, "��Ļ:%d��%d��(��ǰ����: %d �Ҽ��˳�)", pCGI->lines, pCGI->cols, grade);
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
			case MOUSE_RIGHT_BUTTON_CLICK:			//�����Ҽ�
			case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:	//˫���Ҽ�
			{
				loop = 0;
				break;
			}
			}//end of switch(maction)
		}//end of ����¼�
	}//end of while(loop)
	temp[0] = '\0';
	sprintf(temp, "��Ϸ������,����һ�ְ�~~~");
	gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
	return 0;
}