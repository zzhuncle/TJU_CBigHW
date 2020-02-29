/* 1853790 ׯ�� ��ǿ */
#define _CRT_SECURE_NO_WARNINGS
#include"90-b0.h"
/*==============================================================================
��������Ƿ�����(ʣ�������3��)�Լ��Ƿ��ѿ� ������1,����Ҳ���շ���2,���򷵻�0
==============================================================================*/
int boardfull(int(*p)[9], int n_r, int n_c)
{
	int sum = 0;
	int i, t; /* ��ʱ���� */
	for (i = 0; i < 9; i++)
		for (t = 0; t < 9; t++)
			if (p[i][t])
				sum++;
	if (sum > n_r * n_c - 3)
		return 1;
	else if (sum == 0)
		return 0;
	else
		return 2;

}
/*==============================================================================
������ɲ���
==============================================================================*/
void buildchess(const BLOCK_DISPLAY_INFO bdi_normal[],CONSOLE_GRAPHICS_INFO *pCGI, int(*p1)[9], int(*p2)[9], int *p3, int lines,
	int cols, const int n_chess, const int mood)//n_chess����Ҫ�µ������� 
{
	int x, y, num = 0;
	if (mood == 0)
	{
		while (num < n_chess)
		{
			do
			{
				x = rand() % cols;
				y = rand() % lines;
			} while (p1[y][x] != 0);
			p1[y][x] = rand() % 7 + 1;
			num++;
		}
	}
	if ((mood == 2) || (mood == 3))
	{
		while (num < n_chess)
		{
			do
			{
				x = rand() % cols;
				y = rand() % lines;
			} while (p1[y][x] != 0);
			do
			{
				p1[y][x] = rand() % 7 + 1;
			} while (p1[y][x] != p3[num]);
			if (mood == 3)
			{
				gmw_draw_block(pCGI, y, x,p3[num], bdi_normal);
			}
			num++;
		}
	}
}
/*==============================================================================
����ڵ�,�ڵ���÷���1,�����÷���0
==============================================================================*/
int check_pos_valid(int(*p1)[9], int(*p2)[9], int x, int y, int n_r, int n_c)
{
	//�ڵ��Ƿ���߽�
	if (x < 0 || x >= n_r || y < 0 || y >= n_c)
		return 0;
	//��ǰ�ڵ��Ƿ����·
	if (0 != p1[x][y])
		return 0;
	//��ǰ�ڵ��Ƿ��Ѿ��߹�
	if (int('*') == p2[x][y])
		return 0;
	return 1;
}
/*==============================================================================
�ж�λ�ڵ�x��,y�еĲ����Ƿ����ƶ�����to_x��,to_y��,���Է���1,���򷵻�0.
==============================================================================*/
int canmove(int &begin, int(*p1)[9], int(*p2)[9], int x, int y, int to_x, int to_y, int n_r, int n_c)
{
	begin++;
	if (begin >= 1000000) {
		p2[x][y] = int('0');
		return 0;
	}
	if (check_pos_valid(p1, p2, x, y, n_r, n_c))
	{
		if (x == to_x && y == to_y)
		{
			p2[x][y] = int('*');
			return 1;
		}
		p2[x][y] = int('*');
		if (canmove(begin, p1, p2, x, y - 1, to_x, to_y, n_r, n_c))//��
			return 1;
		if (canmove(begin, p1, p2, x - 1, y, to_x, to_y, n_r, n_c))//��
			return 1;
		if (canmove(begin, p1, p2, x, y + 1, to_x, to_y, n_r, n_c))//��
			return 1;
		if (canmove(begin, p1, p2, x + 1, y, to_x, to_y, n_r, n_c))//��
			return 1;
		p2[x][y] = int('0');
		return 0;
	}
	return 0;
}
/*==============================================================================
�ж�����������Ƿ������ȥ����,��������ȥ����������ĸ���(n>=5),���򷵻�0.
==============================================================================*/
int delchess(const BLOCK_DISPLAY_INFO bdi_normal[], CONSOLE_GRAPHICS_INFO *pCGI,int(*p1)[9], int(*p2)[9], int x, int y, int n_r, int n_c, int mood)
{
	int i, j; /* ��ʱ���� */
	int sum[5] = { 0 };
	int sum_total = 0;//����˫��������ļ���
	/* �������� */
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
	if (sum[1] > 5)
	{
		for (i = x + 1; i < n_r; i++)
		{
			if (p1[x][y] != p1[i][y])
				break;
			p1[i][y] = 0;
			if (mood == 7)
			{
				gmw_draw_block(pCGI, i, y, BDI_VALUE_BLANK, bdi_normal);
			}
		}
		for (i = x - 1; i >= 0; i--)
		{
			if (p1[x][y] != p1[i][y])
				break;
			p1[i][y] = 0;
			if (mood == 7)
			{
				gmw_draw_block(pCGI, i, y, BDI_VALUE_BLANK, bdi_normal);
			}
		}
	}
	/* �������� */
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
	if (sum[2] > 5)
	{
		for (i = y + 1; i < n_c; i++)
		{
			if (p1[x][y] != p1[x][i])
				break;
			p1[x][i] = 0;
			if (mood == 7)
			{
				gmw_draw_block(pCGI, x, i, BDI_VALUE_BLANK, bdi_normal);
			}
		}
		for (i = y - 1; i >= 0; i--) {
			if (p1[x][y] != p1[x][i])
				break;
			p1[x][i] = 0;
			if (mood == 7)
			{
				gmw_draw_block(pCGI, x, i, BDI_VALUE_BLANK, bdi_normal);
			}
		}
	}
	/* ����б��( \) */
	for (i = x, j = y; (i < n_r) && (j < n_c); i++, j++)
	{
		if (p1[x][y] != p1[i][j])
			break;
		sum[3]++;
	}
	for (i = x, j = y; (i >= 0) && (j >= 0); i--, j--)
	{
		if (p1[x][y] != p1[i][j])
			break;
		sum[3]++;
	}
	if (sum[3] > 5)
	{
		for (i = x + 1, j = y + 1; (i < n_r) && (j < n_c); i++, j++)
		{
			if (p1[x][y] != p1[i][j])
				break;
			p1[i][j] = 0;
			if (mood == 7)
			{
				gmw_draw_block(pCGI, i, j, BDI_VALUE_BLANK, bdi_normal);
			}
		}
		for (i = x - 1, j = y - 1; (i >= 0) && (j >= 0); i--, j--)
		{
			if (p1[x][y] != p1[i][j])
				break;
			p1[i][j] = 0;
			if (mood == 7)
			{
				gmw_draw_block(pCGI, i, j, BDI_VALUE_BLANK, bdi_normal);
			}
		}
	}
	/* ����б��( / ) */
	for (i = x, j = y; (i < n_r) && (j >= 0); i++, j--)
	{
		if (p1[x][y] != p1[i][j])
			break;
		sum[4]++;
	}
	for (i = x, j = y; (i >= 0) && (j < n_c); i--, j++)
	{
		if (p1[x][y] != p1[i][j])
			break;
		sum[4]++;
	}
	if (sum[4] > 5)
	{
		for (i = x + 1, j = y - 1; (i < n_r) && (j >= 0); i++, j--)
		{
			if (p1[x][y] != p1[i][j])
				break;
			p1[i][j] = 0;
			if (mood == 7)
			{
				gmw_draw_block(pCGI, i, j, BDI_VALUE_BLANK, bdi_normal);
			}
		}
		for (i = x - 1, j = y + 1; (i >= 0) && (j < n_c); i--, j++)
		{
			if (p1[x][y] != p1[i][j])
				break;
			p1[i][j] = 0;
			if (mood == 7)
			{
				gmw_draw_block(pCGI, i, j, BDI_VALUE_BLANK, bdi_normal);
			}
		}
	}
	if ((sum[1] > 5) || (sum[2] > 5) || (sum[3] > 5) || (sum[4] > 5))
	{
		p1[x][y] = 0;
		if (mood == 7)
		{
			gmw_draw_block(pCGI, x, y, BDI_VALUE_BLANK, bdi_normal);
		}
		for (int k = 1; k < 5; k++)
		{
			if (sum[k] > 5)
				sum_total += sum[k] - 1;
		}
		return sum_total + 1;
	}
	else
		return 0;
}
/*==============================================================================
������ݹ��ƶ�����(�������)(����*�ķֲ�)//����֮���ʵ��
==============================================================================*/
void movechess(CONSOLE_GRAPHICS_INFO *pCGI, int color, int(*p2)[9], int n_r, int n_c, int x_qi, int y_qi, int xx, int yy, const BLOCK_DISPLAY_INFO *const bdi_normal)
{
	if (x_qi == xx && y_qi == yy)
		return;
	if ((p2[x_qi][y_qi - 1] == int('*')) && (x_qi >= 0) && (y_qi >= 1))//��
	{
		p2[x_qi][y_qi] = int('0');
		gmw_move_block(pCGI, x_qi, y_qi, color, 0, bdi_normal, RIGHT_TO_LEFT, 1);
		y_qi--;
		movechess(pCGI, color, p2, n_r, n_c, x_qi, y_qi, xx, yy, bdi_normal);
		return;
	}
	if ((p2[x_qi - 1][y_qi] == int('*') && (x_qi >= 1) && (y_qi >= 0)))//��
	{
		p2[x_qi][y_qi] = int('0');
		gmw_move_block(pCGI, x_qi, y_qi, color, 0, bdi_normal, DOWN_TO_UP, 1);
		x_qi--;
		movechess(pCGI, color, p2, n_r, n_c, x_qi, y_qi, xx, yy, bdi_normal);
		return;
	}
	if ((p2[x_qi][y_qi + 1] == int('*') && (x_qi >= 0) && (y_qi + 1 <= n_c - 1)))//��
	{
		p2[x_qi][y_qi] = int('0');
		gmw_move_block(pCGI, x_qi, y_qi, color, 0, bdi_normal, LEFT_TO_RIGHT, 1);
		y_qi++;
		movechess(pCGI, color, p2, n_r, n_c, x_qi, y_qi, xx, yy, bdi_normal);
		return;
	}
	if ((p2[x_qi + 1][y_qi] == int('*') && (x_qi + 1 <= n_r - 1) && (y_qi >= 0)))//��
	{
		p2[x_qi][y_qi] = int('0');
		gmw_move_block(pCGI, x_qi, y_qi, color, 0, bdi_normal, UP_TO_DOWN, 1);
		x_qi++;
		movechess(pCGI, color, p2, n_r, n_c, x_qi, y_qi, xx, yy, bdi_normal);
		return;
	}
}
/*==============================================================================
ͳ�Ƹ��ֲ������Ŀ
==============================================================================*/
void countnum(int(*p1)[9], int n_r, int n_c, int *pn)//���㲻ͬ�������
{
	for (int i = 0; i < 8; i++)
		pn[i] = 0;
	for (int i = 0; i < n_r; i++)
		for (int j = 0; j < n_c; j++)
		{
			if (p1[i][j] == 1)
				pn[1]++;
			if (p1[i][j] == 2)
				pn[2]++;
			if (p1[i][j] == 3)
				pn[3]++;
			if (p1[i][j] == 4)
				pn[4]++;
			if (p1[i][j] == 5)
				pn[5]++;
			if (p1[i][j] == 6)
				pn[6]++;
			if (p1[i][j] == 7)
				pn[7]++;
			if (p1[i][j] == 0)
				pn[0]++;
		}
}
/*==============================================================================
�÷ּ�������ʵ��
==============================================================================*/
void cal_score(int &nnn, int &grade, int *del, int *pn, const int kk, const int n_c, const int n_r)
{
	nnn--;
	setcolor(15, 8);
	grade += nnn * (nnn - 1);
	del[kk] += nnn;
	pn[kk] -= nnn;
	pn[0] += nnn;
	gotoxy(4 * n_c + 12, 2);
	cout << setfill('0') << setw(2) << grade;
	gotoxy(4 * n_c + 24, 10 + kk);
	cout << setfill('0') << setw(2) << del[kk];
	gotoxy(4 * n_c + 9, 10 + kk);
	cout << setfill('0') << setw(2) << pn[kk];
	gotoxy(4 * n_c + 13, 10 + kk);
	cout << setiosflags(ios::fixed) << setprecision(2) << double(pn[kk]) / (n_r*n_c);
	gotoxy(4 * n_c + 9, 10);
	cout << setfill('0') << setw(2) << pn[0];
	gotoxy(4 * n_c + 13, 10);
	cout << setiosflags(ios::fixed) << setprecision(2) << double(pn[0]) / (n_r*n_c);
	setcolor();
}
/*==============================================================================
Ԥ���������ʵ��
==============================================================================*/
void predict(int *pn, int *color, const int n_r, const int n_c)
{
	for (int i = 0; i < 3; i++)
	{
		pn[color[i]]++;
		pn[0]--;
		setcolor(15, 8);
		gotoxy(4 * n_c + 9, 10 + color[i]);
		cout << setfill('0') << setw(2) << pn[color[i]];
		gotoxy(4 * n_c + 13, 10 + color[i]);
		cout << setiosflags(ios::fixed) << setprecision(2) << double(pn[color[i]]) / (n_r*n_c);
		gotoxy(4 * n_c + 9, 10);
		cout << setfill('0') << setw(2) << pn[0];
		gotoxy(4 * n_c + 13, 10);
		cout << setiosflags(ios::fixed) << setprecision(2) << double(pn[0]) / (n_r*n_c);
		setcolor();
	}
}
/*==============================================================================
ʵ�����˵�
==============================================================================*/
int caidan(int(*p1)[9], int(*p2)[9], CONSOLE_GRAPHICS_INFO *pCGI, const int mood)
{
	/* ����1-7�������ú�����ʽ��ʾ�ڽ����ϣ�����״̬�� */
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //0����ʾ���ÿո���伴��
		{1, 1, 15, "��"},
		{2, 2, 15, "��"},
		{3, 3, 15, "��"},
		{4, 4, 15, "��"},
		{5, 5, 15, "��"},
		{6, 6, 15, "��"},
		{7, 7, 15, "��"},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL
	};
	/* ����1-7�������ú�����ʽ��ʾ�ڽ����ϣ�ѡ��״̬�� */
	const BLOCK_DISPLAY_INFO bdi_selected[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //�հ�
		{1, 1, 15, "��"},
		{2, 2, 15, "��"},
		{3, 3, 15, "��"},
		{4, 4, 15,"��"},
		{5, 5, 15,"��"},
		{6, 6, 15,"��"},
		{7, 7, 15, "��"},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL
	};

	int color[3];
	for (int i = 0; i < 3; i++)
		color[i] = rand() % 7 + 1;//Ԥ�����������ɫ
	system("cls");
	gotoxy(0, 0);
	int n_r, n_c;
	n_r = get_int("����������", 7, 9);
	n_c = get_int("����������", 7, 9);//��������
	buildchess(bdi_normal,pCGI,p1, p2, color, n_r, n_c, 5, 0);//���ɲ���

	//��ʼ��
	gmw_init(pCGI, n_r, n_c, COLOR_BLACK, COLOR_WHITE);

	//����
	setcursor(CURSOR_INVISIBLE);//�������
	gmw_set_font(pCGI, "������", 28, 28);//��������
	gmw_set_ext_rowcol(pCGI, 0, 0, 0, 31);//����Ŀռ�����
	gmw_set_frame_color(pCGI, 15, 8);//����ܵı�����ǰ����ɫ
	gmw_set_delay(pCGI,DELAY_OF_BLOCK_MOVED, 20);//�ƶ���ʱ����

	/* ��ʾ��� */
	gmw_draw_frame(pCGI);

	/* ��״̬����ʾ���� */
	char temp[256];
	temp[0] = '\0';
	sprintf(temp, "��Ļ:%d��%d��", pCGI->lines, pCGI->cols);
	gmw_status_line(pCGI, TOP_STATUS_LINE, temp);

	/* ��״̬����ʾ���� */
	gmw_status_line(pCGI, LOWER_STATUS_LINE, "[��ǰ���] 01��0A��");

	setcolor(15, 8);//���Ƶ÷ֿ����
	border_info bor0 = { 1,4 * n_c + 4,1,5,0,1,0 };
	draw_border(bor0);
	gotoxy(4 * n_c + 6, 2);
	setcolor(15, 8);
	cout << "�÷�:0    ";
	setcolor();//���Ƶ÷ֿ����

	border_info bor1 = { 5,4 * n_c + 4,1,3,0,1,1 };//Ԥ��������������
	draw_border(bor1);
	for (int i = 0; i < 3; i++) {
		ball_info ball_0 = { 0,15,color[i],0,0,0,1,2,n_c,i };
		draw_ball(ball_0);
	}
	setcolor();//Ԥ��������������

	int grade = 0;//�÷����ͳ��  //���Ʋ��������
	int pn[8];//ͳ�Ʋ�ͬ��ɫ���������
	int del[8] = { 0 };//ͳ���������������
	countnum(p1, n_r, n_c, pn);//���㲻ͬ��ɫ������Ŀ
	setcolor(15, 8);
	border_info bor2 = { 9,4 * n_c + 4,8,11,0,1,0 };
	draw_border(bor2);
	getchar();
	gotoxy(4 * n_c + 8, 10);//λ��:(4*n_c+9,10+i),(4*n_c+13,10+i),(4*n_c+24,10+i)
	setcolor(15, 8);
	cout << ":" << setfill('0') << setw(2) << pn[0] << "/(" << setiosflags(ios::fixed) << setprecision(2) << double(pn[0]) / (n_r*n_c) << "%) del-" << del[0];
	for (int i = 1; i <= 7; i++)
	{
		ball_info ball_1 = { 0,15,i,0,0,0,1,1,n_c,i };
		draw_ball(ball_1);
		setcolor(15, 8);
		cout << ":" << setfill('0') << setw(2) << pn[i] << "/(" << setiosflags(ios::fixed) << setprecision(2) << double(pn[i]) / (n_r*n_c) << "%) del-" << del[i];
	}
	setcolor();//���Ʋ��������

	/* ���ڲ�����չ�ֵ���Ļ�� */
	for (int i = 0; i < n_r; i++)
		for (int j = 0; j < n_c; j++) {
			gmw_draw_block(pCGI, i, j, p1[i][j], bdi_normal);
		}

	int is_out = 1;
	while ((boardfull(p1, n_r, n_c) == 2) && (is_out))
	{
		int nnn = 0;
		int color[3];
		for (int i = 0; i < 3; i++)
			color[i] = rand() % 7 + 1;//Ԥ�����������ɫ
		setcolor();
		for (int i = 0; i < 3; i++)
		{
			ball_info ball_3 = { 0,15,color[i],0,0,0,1,2,n_c,i };
			setcursor(CURSOR_INVISIBLE);
			draw_ball(ball_3);
		}
		setcolor();
		int mrow = 0, mcol = 0;
		int ret, maction;
		int keycode1, keycode2;
		int loop = 1;
		for (int i = 0; i < n_r; i++)
			for (int j = 0; j < n_c; j++)
				p2[i][j] = int('0');
		int flag[9][9] = { 0 };
		while (loop)
		{
			ret = gmw_read_keyboard_and_mouse(pCGI, maction, mrow, mcol, keycode1, keycode2);
			if (ret == CCT_MOUSE_EVENT)
			{
				if (maction == MOUSE_RIGHT_BUTTON_CLICK)
				{
					loop = 0;
					is_out = 0;
				}
				else if (maction == MOUSE_LEFT_BUTTON_CLICK)
				{
					if (p1[mrow][mcol] != 0)
					{
						for (int i = 0; i < n_r; i++)
							for (int j = 0; j < n_c; j++)
								if (flag[i][j] == 1)
								{
									flag[i][j] = 0;
									gmw_draw_block(pCGI, i, j, p1[i][j], bdi_normal);
								}
						gmw_draw_block(pCGI, mrow, mcol, p1[mrow][mcol], bdi_selected);
						flag[mrow][mcol] = 1;
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
						if ((x_qi == -1) && (y_qi == -1))
							continue;
						int kk = p1[x_qi][y_qi];
						p1[x_qi][y_qi] = 0;//������ʼ�������ɫ
						int begin = 0;
						if (canmove(begin, p1, p2, x_qi, y_qi, mrow, mcol, n_r, n_c))
						{
							p1[mrow][mcol] = kk;
							movechess(pCGI, p1[mrow][mcol], p2, n_r, n_c, x_qi, y_qi, mrow, mcol, bdi_normal);
							if (nnn = delchess(bdi_normal, pCGI,p1, p2, mrow, mcol, n_r, n_c, 7))
								cal_score(nnn, grade, del, pn, kk, n_c, n_r);
							else
							{
								p1[mrow][mcol] = kk;
								predict(pn, color, n_r, n_c);
								buildchess(bdi_normal, pCGI,p1, p2, color, n_r, n_c, 3, 3);
							}
							loop = 0;
						}
						else
						{
							p1[x_qi][y_qi] = kk;
							temp[0] = '\0';
							sprintf(temp, "[����] �޷���%c%d�ƶ���%c%d", char('A' + x_qi), y_qi, char('A' + mrow), mcol);
							gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
						}
					}
				}
			}
		}//end of loop
	}//end of while
	gmw_status_line(pCGI, LOWER_STATUS_LINE, "��Ϸ������,����һ�ְ�~~~");
	setcursor(CURSOR_INVISIBLE);
	return 0;
}