/* 1853790 ׯ�� ��ǿ */
#include <iostream>
#include <conio.h>
#include <iomanip>
#include "cmd_console_tools.h"
using namespace std;
const int filenum_maxl = 50;
const int filename_maxl = 100;
const int _s = 20;
const int N = 100;
const int cols = 36;
const int lines = 20;
const int base_x = 4;
const int base_y = 2;
/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ʽ�˵�ʵ��
  ����������˵���ѡ������ѡ�����
  �� �� ֵ������ѡ������
  ˵    ������ӡʱ����ÿ2�ֽ�һ��
***************************************************************************/
int pop_menu(char file_name[][filename_maxl], int cnt)
{
	int fflag = 1;
	setcursor(CURSOR_INVISIBLE);
	setcolor(15, 6);

	//��ʼ��ӡ
	for (int i = 0; i < (cnt > lines ? lines : cnt); i++) {
		gotoxy(base_x + 2, base_y + 1 + i);
		cout << file_name[i];
	}

	int flag = 0;
	if (cnt <= lines)
		flag = 1;

	enable_mouse();
	int X = 0, Y = 0;
	int ret, maction;
	int keycode1, keycode2;
	int loop = 1;
	int real_order = 0, screen_order = 0;
	while (loop == 1) {
		ret = read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		if (ret == CCT_KEYBOARD_EVENT) {
			switch (keycode1) {
			case 81:
			case 113://Q(q)��
			{
				setcolor(15, 0);
				gotoxy(0, base_y + lines + 10);
				exit(0);
				break;
			}
			case 13:   //enter
				fflag = 0;
				break;
			case 224:
				switch (keycode2) {
				case KB_ARROW_UP://�ϼ�ͷ��
				{
					real_order = (real_order - 1 < 0 ? 0 : real_order - 1);
					if (screen_order - 1 < 0 && flag == 0) {
						for (int i = 0; i < lines; i++) {
							if (i == 0)
								setcolor(0, 6);
							gotoxy(base_x + 2, base_y + 1 + i);
							cout << file_name[real_order + i];
							setcolor(15, 6);
						}
					}
					else {
						if (screen_order - 1 >= 0)
							screen_order--;
						gotoxy(base_x + 2, base_y + 2 + screen_order);
						cout << file_name[real_order + 1];
						gotoxy(base_x + 2, base_y + 1 + screen_order);
						setcolor(0, 6);
						cout << file_name[real_order];
						setcolor(15, 6);
					}
					break;
				}
				case KB_ARROW_DOWN://�¼�ͷ��
				{
					if (flag == 1)
						real_order = (real_order + 1 > cnt - 1 ? real_order : real_order + 1);
					if (screen_order + 1 > lines - 1 && flag == 0) {
						real_order = (real_order - screen_order + lines > cnt - 1 ? real_order : real_order + 1);
						for (int i = 0; i < lines; i++) {
							if (i == lines - 1)
								setcolor(0, 6);
							gotoxy(base_x + 2, base_y + 1 + i);
							cout << file_name[real_order - screen_order + i];
							setcolor(15, 6);
						}
					}
					else if (flag == 0) {
						if (screen_order + 1 < cnt)
							screen_order++;
						real_order = (real_order - screen_order + lines > cnt - 1 ? real_order : real_order + 1);
						gotoxy(base_x + 2, base_y + screen_order);
						cout << file_name[real_order - 1];
						gotoxy(base_x + 2, base_y + 1 + screen_order);
						setcolor(0, 6);
						cout << file_name[real_order];
						setcolor(15, 6);
					}
					break;
				}
				default:
					break;
				}
			}
			if (fflag == 0)
				break;
		}
	}
	return real_order;
}
/*==============================================================================
�������·ָ���
==============================================================================*/
static void draw_line_break(const int num)
{
	int i;
	char line[100 * N] = { 0 };
	for (i = 0; i < num; i++)
		line[i] = '-';
	cout << line << endl;
}
/*==============================================================================
�˵�
==============================================================================*/
char menu(const char *menu[], const char *choice)
{
	char sel, **pitem;
	int line_maxlen = 0, line_len, march = 0;
	for (pitem = (char **)menu; *pitem; pitem++) {
		line_len = int(strlen(*pitem));
		if (line_len > line_maxlen)
			line_maxlen = line_len;
	}
	draw_line_break(line_maxlen);
	for (pitem = (char **)menu; *pitem; pitem++)
		cout << *pitem << endl;
	draw_line_break(line_maxlen);
	cout << "[��ѡ��]:";
	while (!march)
	{
		sel = _getch();
		if (strchr(choice, sel) != NULL)//ԭʼƥ��
			march = 1;
		else if ((sel >= 'a'&&sel <= 'z') &&
			(strchr(choice, sel - 32) != NULL)) {//����Сд��ƥ���д
			sel -= 32;
			march = 1;
		}
		else if ((sel >= 'A'&&sel <= 'Z') &&
			(strchr(choice, sel + 32) != NULL)) {//����Сд��ƥ���д
			sel += 32;
			march = 1;
		}
		if (march) {
			putchar(sel);
			putchar('\n');
			Sleep(50);
		}
	}//end of while(1)
	return sel;
}
/*==============================================================================
��ӡ�ڲ�����
==============================================================================*/
void drawarray(int(*p1)[9], int(*p2)[9], int lines, int cols, const int mood)
{
	cout << "  |";
	for (int i = 1; i <= cols; i++)
		cout << "  " << i;
	cout << endl;
	cout << "--+";
	for (int i = 1; i <= cols; i++)
		cout << "---";
	cout << endl;
	for (int i = 0; i < lines; i++)
	{
		cout << char('A' + i) << " |";
		for (int j = 0; j < cols; j++)
		{
			setcolor();
			if (mood == 1 || mood == 11)
			{
				if (p1[i][j] == 0)
					cout << "  0";
				else if (p1[i][j] == int('*') || p1[i][j] == int('0'))
					cout << "  " << char(p1[i][j]);
				else
				{
					cout << "  ";
					if (mood == 1)
						setcolor(14, p1[i][j]);
					cout << p1[i][j];
				}
			}
			if (mood == 2 || mood == 12)
			{
				if (p2[i][j] == int('0'))
					cout << "  " << p1[i][j];
				else
				{
					cout << "  ";
					setcolor(14, p1[i][j]);
					cout << p1[i][j];
				}
			}
			setcolor();
		}
		cout << endl;
	}
}
/*==============================================================================
��������״̬������ʾ
==============================================================================*/
void dispaly(struct model_info &model)
{
	setcolor();
	gotoxy(model.x, model.y);
	if (model.is_up == 1)
		cout << "��Ļ:" << model.n_r << "��" << model.n_c << "��";
	else
		cout << "[��ǰ���] " << setw(2) << char(model.n_r) << "��" << setw(2) << model.n_c << "��                     ";
}
/*==============================================================================
��ʾ��ʼ��߿�
==============================================================================*/
void draw_border(struct border_info &bor)
{
	setcolor(15, 8);
	if (bor.is_gapline == 0)
	{
		if (bor.is_mark == 1)
		{
			setcolor();
			gotoxy(bor.y - 2, bor.x - 1);
			cout << "    ";
			for (int i = 1; i <= bor.n_c; i++)
				cout << " " << i;
		}
		if (bor.is_mark == 1)
		{
			gotoxy(bor.y - 2, bor.x);
			cout << "  ";
			setcolor(0, 8);
		}
		gotoxy(bor.y, bor.x);
		cout << (bor.is_empty ? "�X" : "��");
		for (int i = 1; i <= bor.n_c; i++)
			cout << (bor.is_empty ? "�T" : "��");
		cout << (bor.is_empty ? "�[" : "��");
		for (int i = 1; i <= bor.n_r; i++)
		{
			gotoxy(bor.y, bor.x + i);
			if (bor.is_mark == 1)
			{
				setcolor();
				gotoxy(bor.y - 2, bor.x + i);
				cout << " " << char('A' + i - 1);
				setcolor(0, 8);
			}
			cout << (bor.is_empty ? "�U" : "��");
			for (int j = 1; j <= bor.n_c; j++)
				cout << "  ";
			cout << (bor.is_empty ? "�U" : "��");
		}//�м伸��
		gotoxy(bor.y, bor.x + bor.n_r + 1);
		if (bor.is_mark == 1)
		{
			setcolor();
			gotoxy(bor.y - 2, bor.x + bor.n_r + 1);
			cout << "  ";
			setcolor(0, 8);
		}
		cout << (bor.is_empty ? "�^" : "��");
		for (int i = 1; i <= bor.n_c; i++)
			cout << (bor.is_empty ? "�T" : "��");
		cout << (bor.is_empty ? "�a" : "��") << endl;          //���һ��
	}
	else if (bor.is_gapline == 1)
	{
		gotoxy(bor.y, bor.x);
		if (bor.is_mark == 1)
		{
			setcolor();
			gotoxy(bor.y - 2, bor.x - 1);
			cout << "    ";
			for (int i = 1; i <= bor.n_c; i++)
				cout << " " << i << "  ";

		}
		if (bor.is_mark == 1)
		{
			gotoxy(bor.y - 2, bor.x);
			cout << "  ";
			setcolor(0, 8);
		}
		cout << (bor.is_empty ? "�X" : "��");
		for (int i = 1; i <= bor.n_c - 1; i++)
			cout << (bor.is_empty ? "�T�j" : "����");
		cout << (bor.is_empty ? "�T�[" : "����");//��һ��
		for (int i = 1; i <= 2 * bor.n_r - 2; )
		{
			gotoxy(bor.y, bor.x + i++);
			if (bor.is_mark == 1)
			{
				setcolor();
				gotoxy(bor.y - 2, bor.x + i - 1);
				cout << " " << char('A' + i / 2 - 1);
				setcolor(0, 8);
			}
			cout << (bor.is_empty ? "�U" : "��");
			for (int j = 1; j <= bor.n_c; j++)
				cout << "  " << (bor.is_empty ? "�U" : "��");//�ڶ���
			gotoxy(bor.y, bor.x + i++);
			if (bor.is_mark == 1)
			{
				setcolor();
				gotoxy(bor.y - 2, bor.x + i - 1);
				cout << "  ";
				setcolor(0, 8);
			}
			cout << (bor.is_empty ? "�d" : "��");
			for (int j = 1; j <= bor.n_c - 1; j++)
				cout << (bor.is_empty ? "�T�p" : "����");
			cout << (bor.is_empty ? "�T�g" : "����");    //������
		}
		gotoxy(bor.y, bor.x + 2 * bor.n_r - 1);
		if (bor.is_mark == 1)
		{
			setcolor();
			gotoxy(bor.y - 2, bor.x + 2 * bor.n_r - 1);
			cout << " " << char('A' + bor.n_r - 1);
			setcolor(0, 8);
		}
		cout << (bor.is_empty ? "�U" : "��");
		for (int i = 1; i <= bor.n_c; i++)
			cout << "  " << (bor.is_empty ? "�U" : "��");   //�����ڶ���
		gotoxy(bor.y, bor.x + 2 * bor.n_r);
		if (bor.is_mark == 1)
		{
			setcolor();
			gotoxy(bor.y - 2, bor.x + 2 * bor.n_r);
			cout << "  ";
			setcolor(0, 8);
		}
		cout << (bor.is_empty ? "�^" : "��");
		for (int i = 1; i <= bor.n_c - 1; i++)
			cout << (bor.is_empty ? "�T�m" : "����");
		cout << (bor.is_empty ? "�T�a" : "����");//���һ��
	}
	setcolor();
}
/*==============================================================================
��ʾһ��С��
==============================================================================*/
void draw_ball(struct ball_info &ball)
{
	if (ball.extra == 1)
		gotoxy(4 * ball.lines + 6, 10 + ball.i);
	else if (ball.extra == 2)
		gotoxy(4 * ball.lines + 6 + 4 * ball.i, 6);
	else if (ball.extra == 0)
	{
		if (ball.is_mark == 0)
		{
			if (ball.is_gapline == 0)
				gotoxy(2 * (ball.n_c + 1), 2 + ball.n_r);
			else
			{
				gotoxy(4 * ball.n_c + 2, 2 * (ball.n_r + 1));
				if (ball.move_mood == 1)
					gotoxy(4 * ball.n_c + 2, 2 * (ball.n_r + 1) + 1);
				if (ball.move_mood == 2)
					gotoxy(4 * ball.n_c + 2, 2 * (ball.n_r + 1) - 1);
				if (ball.move_mood == 3)
					gotoxy(4 * ball.n_c + 2 + 2, 2 * (ball.n_r + 1));
				if (ball.move_mood == 4)
					gotoxy(4 * ball.n_c + 2 - 2, 2 * (ball.n_r + 1));
			}
		}
		else
		{
			if (ball.is_gapline == 0)
				gotoxy(2 * (ball.n_c + 2), 3 + ball.n_r);
			else
			{
				gotoxy(4 * ball.n_c + 4, 2 * ball.n_r + 3);
				if (ball.move_mood == 1)
					gotoxy(4 * ball.n_c + 4, 2 * ball.n_r + 3 + 1);
				if (ball.move_mood == 2)
					gotoxy(4 * ball.n_c + 4, 2 * ball.n_r + 3 - 1);
				if (ball.move_mood == 3)
					gotoxy(4 * ball.n_c + 4 + 2, 2 * ball.n_r + 3);
				if (ball.move_mood == 4)
					gotoxy(4 * ball.n_c + 4 - 2, 2 * ball.n_r + 3);
			}
		}
	}
	setcolor(ball.back, ball.front);
	if (ball.mode == 0)
		cout << "��";
	if (ball.mode == 1)
		cout << "��";
	if (ball.mode == 2)
		cout << "��";
	if (ball.mode == 3)
		cout << "��";
	if (ball.mode == 4)
		cout << "��";
	if (ball.mode == 5)
		cout << "  ";
	setcolor();
}
/*==============================================================================
�ƶ�һ��С��
==============================================================================*/
int move_ball(struct move_ball_info &move)
{
	if (move.mood == 1)
	{
		int move_x0 = move.x;
		for (; move.x - move_x0 < move.n; move.x++)
		{
			ball_info ball_1 = { 0, (move.is_mark ? 0 : 15), move.color, move.x, move.y, move.is_mark, 1, 0, 0, 0, 1 };
			draw_ball(ball_1);
			Sleep(_s);
			if (move.is_mark == 0)
				gotoxy(4 * move.y + 2, 2 * move.x + 2);
			else
				gotoxy(4 * move.y + 2 + 2, 2 * move.x + 2 + 1);
			setcolor((move.is_mark ? 0 : 15), 8);
			cout << "  ";
			Sleep(_s);
			ball_info ball_2 = { 0, (move.is_mark ? 0 : 15), move.color, move.x + 1, move.y, move.is_mark, 1, 0, 0, 0, 0 };
			draw_ball(ball_2);
			Sleep(_s);
			if (move.is_mark == 0)
				gotoxy(4 * move.y + 2, 2 * move.x + 2 + 1);
			else
				gotoxy(4 * move.y + 2 + 2, 2 * move.x + 2 + 1 + 1);
			setcolor((move.is_mark ? 0 : 15), 8);
			cout << ((!move.is_mark) ? "�T" : "��");
			setcolor();
		}
	}
	if (move.mood == 2)
	{
		int move_x0 = move.x;
		for (; move_x0 - move.x < move.n; move.x--)
		{
			ball_info ball_1 = { 0, (move.is_mark ? 0 : 15), move.color, move.x, move.y, move.is_mark, 1, 0, 0, 0, 2 };
			draw_ball(ball_1);
			Sleep(_s);
			if (move.is_mark == 0)
				gotoxy(4 * move.y + 2, 2 * move.x + 2);
			else
				gotoxy(4 * move.y + 4, 2 * move.x + 3);
			setcolor((move.is_mark ? 0 : 15), 8);
			cout << "  ";
			Sleep(_s);
			ball_info ball_2 = { 0, (move.is_mark ? 0 : 15), move.color, move.x - 1, move.y, move.is_mark, 1, 0, 0, 0, 0 };
			draw_ball(ball_2);
			Sleep(_s);
			if (move.is_mark == 0)
				gotoxy(4 * move.y + 2, 2 * move.x + 2 - 1);
			else
				gotoxy(4 * move.y + 4, 2 * move.x + 2);
			setcolor((move.is_mark ? 0 : 15), 8);
			cout << ((!move.is_mark) ? "�T" : "��");
			setcolor();
		}
	}
	if (move.mood == 3)
	{
		int move_y0 = move.y;
		for (; move.y - move_y0 < move.n; move.y++)
		{
			ball_info ball_1 = { 0, (move.is_mark ? 0 : 15), move.color, move.x, move.y, move.is_mark, 1, 0, 0, 0, 3 };
			draw_ball(ball_1);
			Sleep(_s);
			if (move.is_mark == 0)
				gotoxy(4 * move.y + 2, 2 * move.x + 2);
			else
				gotoxy(4 * move.y + 4, 2 * move.x + 3);
			setcolor((move.is_mark ? 0 : 15), 8);
			cout << "  ";
			Sleep(_s);
			ball_info ball_2 = { 0, (move.is_mark ? 0 : 15), move.color, move.x , move.y + 1, move.is_mark, 1, 0, 0, 0, 0 };
			draw_ball(ball_2);
			Sleep(_s);
			if (move.is_mark == 0)
				gotoxy(4 * move.y + 2 + 2, 2 * move.x + 2);
			else
				gotoxy(4 * move.y + 6, 2 * move.x + 3);
			setcolor((move.is_mark ? 0 : 15), 8);
			cout << ((!move.is_mark) ? "�U" : "��");
			setcolor();
		}
	}
	if (move.mood == 4)
	{
		int move_y0 = move.y;
		for (; move_y0 - move.y < move.n; move.y--)
		{
			ball_info ball_1 = { 0, (move.is_mark ? 0 : 15), move.color, move.x, move.y, move.is_mark, 1, 0, 0, 0, 4 };
			draw_ball(ball_1);
			Sleep(_s);
			if (move.is_mark == 0)
				gotoxy(4 * move.y + 2, 2 * move.x + 2);
			else
				gotoxy(4 * move.y + 4, 2 * move.x + 3);
			setcolor((move.is_mark ? 0 : 15), 8);
			cout << "  ";
			Sleep(_s);
			ball_info ball_2 = { 0, (move.is_mark ? 0 : 15), move.color, move.x , move.y - 1, move.is_mark, 1, 0, 0, 0, 0 };
			draw_ball(ball_2);
			Sleep(_s);
			if (move.is_mark == 0)
				gotoxy(4 * move.y + 2 - 2, 2 * move.x + 2);
			else
				gotoxy(4 * move.y + 2, 2 * move.x + 3);
			setcolor((move.is_mark ? 0 : 15), 8);
			cout << ((!move.is_mark) ? "�U" : "��");
			setcolor();
		}
	}
	return 0;
}
/*==============================================================================
�ض���������
==============================================================================*/
void clear_input_line(int X, int Y, char *filled)
{
	gotoxy(X, Y);
	cout << filled;
	gotoxy(X, Y);
}
/*==============================================================================
��ջ�����
==============================================================================*/
void clear_cin_buf(void)
{
	if (!cin.good()) {
		cin.clear();
		cin.ignore(1024, '\n');
	}
}
/*==============================================================================
��ĳ��ָ����Χ�ڵ�����
==============================================================================*/
int get_int(const char *prompt, const int min_value, const int max_value, const int def_value, int X, int Y)
{
	int value = def_value;
	if (X < 0 && Y < 0)
		getxy(X, Y);
	while (1)
	{
		char blank[] = "                              ";
		clear_input_line(X, Y, blank);
		gotoxy(X, Y);
		cout << prompt << "(" << min_value << "-" << max_value << "):";
		clear_cin_buf();
		cin >> value;
		if (value >= min_value && value <= max_value){
			clear_input_line(X, Y + 1, blank);
			return value;
		}
		else if(def_value!=0x80000000)
		    gotoxy(X, Y + 1);
		cout << "���벻�Ϸ�������������";
		gotoxy(X, Y);
	}
}
/*==============================================================================
�ȴ�����ĳ�ַ���
==============================================================================*/
void wait_for_str(const char *wait_str, const char *prompt, int X, int Y)
{
	char input_str[80];
	if (X < 0 && Y < 0)
		getxy(X, Y);//ȡ��ǰ���λ��
	char blank[] = "	                             ";
	getchar();
	while (1)
	{
		clear_input_line(X, Y, blank);
		clear_cin_buf();
		if (prompt)
			cout << prompt;
		gets_s(input_str);
		if (_strcmpi(input_str, wait_str) == 0) //�����ִ�Сд
			return;
	}
}
/*==============================================================================
�ȴ���ĳ��
==============================================================================*/
void wait_for_key(const char *prompt,
	const int X,
	const int Y,
	const int wait_key_value)
{
	int key, wait_key_value2 = 0;
	if (X >= 0 && Y >= 0)
		gotoxy(X, Y);
	if (prompt)
		cout << prompt;
	if (wait_key_value >= 'A'&&wait_key_value <= 'Z')
		wait_key_value2 = wait_key_value + 32;
	else if (wait_key_value >= 'a'&&wait_key_value <= 'z')
		wait_key_value2 = wait_key_value - 32;
	while (1) {
		key = _getch();
		if (key == wait_key_value || (wait_key_value2&&key == wait_key_value2))
			break;
	}//end of while
}
/***************************************************************************
����a,b��ֵ
***************************************************************************/
void tj_swap(int &a, int &b)
{
	int temp;
	temp = a;
	a = b;
	b = temp;
}
