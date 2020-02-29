/* 1853790 ׯ�� ��ǿ */
#define _CRT_SECURE_NO_WARNINGS
#include<iomanip>
#include <iostream>
#include <climits>
#include <conio.h>
#include "cmd_console_tools.h"
#include "cmd_gmw_tools.h"
using namespace std;

/* --------------------------------------------------
		�˴����Ը�����Ҫ�ľ�̬ȫ�ֱ������������٣����û�У�����̬ȫ��ֻ������/�궨�壨�������ޣ���
   -------------------------------------------------- */

/* --------------------------------------------------
   �˴����Ը�����Ҫ���ڲ��������ߺ���
   1�����������ޣ�����Ϊ gmw_inner_*
   2����������
   3��������static������ȷ��ֻ�ڱ�Դ�ļ���ʹ��
 -------------------------------------------------- */

//�����
static void gmw_inner_check_string(char *change,const char * pstring)
{
	if (pstring == NULL || strlen(pstring) == 0)
		strcpy(change, "  ");
	if (strlen(pstring) >= 3)
		strncpy(change, pstring, 2);
	if (strlen(pstring) == 1) {
		strcpy(change,pstring);
		strcpy(change, " ");
	}
	else
		strcpy(change,pstring);
}

//�������
static int gmw_inner_output(const char *s, int l, const bool is_full = true)
{
	int i;
	if (s == NULL)
		return 0;
	for (i = 0; i < l && s[i]; i++) {
		if (s[i] < 0) {
			if (i + 1 < l) {
				cout << s[i] << s[i+1];
				i = i + 1;
				continue;
			}
			else
				break;
		}
		else
			cout << s[i];
	}
	if (is_full)
		for (; i < l; i++)
			cout << " ";
	return 0;
}

//�ı���ֵ
static void gmw_inner_change_value(CONSOLE_GRAPHICS_INFO *const pCGI)
{
	pCGI->lines = pCGI->extern_up_lines + pCGI->extern_down_lines
		+ (pCGI->CFI.block_high * pCGI->row_num + pCGI->CFI.separator * (pCGI->row_num - 1))
		+ pCGI->top_status_line + pCGI->lower_status_line + pCGI->draw_frame_with_row_no + 6;
	pCGI->cols = pCGI->extern_left_cols + pCGI->extern_right_cols
		+ (pCGI->CFI.block_width * pCGI->col_num + pCGI->CFI.separator * (pCGI->col_num - 1) * 2)
		+ 2 * pCGI->draw_frame_with_col_no + 5;
	pCGI->start_x = pCGI->extern_left_cols + pCGI->draw_frame_with_row_no * 2;
	pCGI->start_y = pCGI->extern_up_lines + pCGI->top_status_line + pCGI->draw_frame_with_col_no;
	pCGI->SLI.top_start_x = (pCGI->start_x - 2 * pCGI->draw_frame_with_col_no > 0 ? pCGI->start_x - 2 * pCGI->draw_frame_with_col_no : 0);
	pCGI->SLI.top_start_y = (pCGI->start_y - pCGI->draw_frame_with_row_no - 1 > 0 ? pCGI->start_y - pCGI->draw_frame_with_row_no - 1 : 0);
	pCGI->SLI.lower_start_x = pCGI->SLI.top_start_x;
	pCGI->SLI.lower_start_y = pCGI->start_y + (pCGI->CFI.block_high * pCGI->row_num +
		pCGI->CFI.separator * (pCGI->row_num - 1)) + 2;
	if (pCGI->SLI.lower_start_y < 0)
		pCGI->SLI.lower_start_y = 0;
}

//���㻭ͼ
static int gmw_inner_piant(const CONSOLE_GRAPHICS_INFO *const pCGI, const int x, const int y,
	const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
	int fgcolor, bgcolor;
	for (int i = 0; bdi[i].value != BDI_VALUE_END; i++) {
		if (bdi[i].value == bdi_value) {
			fgcolor = (bdi[i].fgcolor == -1 ? pCGI->CFI.fgcolor : bdi[i].fgcolor);
			bgcolor = (bdi[i].bgcolor == -1 ? pCGI->CFI.bgcolor : bdi[i].bgcolor);
			setcolor(bgcolor, fgcolor);
			//������
			if (bdi_value != BDI_VALUE_BLANK) {
				//�������
				if (pCGI->CBI.block_border){
				gotoxy(x, y);
				cout << (pCGI->CBI.block_border ? pCGI->CBI.top_left : "  ");
				//getchar();
				for (int i = 0; i < pCGI->CFI.block_width / 2 - 2; i++)
					cout << (pCGI->CBI.block_border ? pCGI->CBI.h_normal : "  ");
				cout << (pCGI->CBI.block_border ? pCGI->CBI.top_right : "  ");
				//getchar();

				for (int i = 1; i < pCGI->CFI.block_high - 1; i++) {
					gotoxy(x, y + i);
					cout << (pCGI->CBI.block_border ? pCGI->CBI.v_normal : "  ");
					//getchar();
					for (int j = 0; j < pCGI->CFI.block_width / 2 - 2; j++)
						cout << "  ";
					cout << (pCGI->CBI.block_border ? pCGI->CBI.v_normal : "  ");
					//getchar();
				}
				gotoxy(x, y + pCGI->CFI.block_high - 1);
				cout << (pCGI->CBI.block_border ? pCGI->CBI.lower_left : "  ");
				//getchar();
				for (int i = 0; i < pCGI->CFI.block_width / 2 - 2; i++)
					cout << (pCGI->CBI.block_border ? pCGI->CBI.h_normal : "  ");
				cout << (pCGI->CBI.block_border ? pCGI->CBI.lower_right : "  ");
				//getchar();
				}
				else 
					for (int i = 0; i < pCGI->CFI.block_high; i++) {
						gotoxy(x, y + i);
						for (int j = 0; j < pCGI->CFI.block_width / 2; j++)
							cout << "  ";
					}
				//��������
				//�����Ǵ�����
				int nl = 1, n;
				gotoxy(x + 2 * pCGI->CBI.block_border +
					(pCGI->CFI.block_width - 4 * pCGI->CBI.block_border - nl) / 2,
					y + pCGI->CBI.block_border +
					(pCGI->CFI.block_high - 2 * pCGI->CBI.block_border) / 2);
				if (bdi[i].content == NULL) {
					//������ֵ�λ��
					n = bdi[i].value;
					while (n /= 10)
						nl++;
					cout << bdi[i].value;
				}
				//������ͼ��
				else {
					nl = (int)(strlen(bdi[i].content));
					cout << bdi[i].content;
				}
			}
			//������
			else {
				for (int i = 0; i < pCGI->CFI.block_high; i++) {
					gotoxy(x, y + i);
					for (int j = 0; j < pCGI->CFI.block_width / 2; j++)
						cout << "  ";
				}
			}
		}
	}
	return 0;
}

//�ƶ�һ��
static void gmw_inner_move(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no,
	const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction)
{
	int x, y;
	x = pCGI->start_x + col_no * 2 * pCGI->CFI.separator + 2 + col_no * pCGI->CFI.block_width;
	y = pCGI->start_y + row_no * pCGI->CFI.separator + 1 + row_no * pCGI->CFI.block_high;
	if (direction == LEFT_TO_RIGHT) {
		for (int i = 1; i <= pCGI->CFI.block_width; i++) {
			gmw_inner_piant(pCGI, x + i, y, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_high; j++) {
				gotoxy(x + i - 1, y + j);
				cout << " ";
				//getchar();
			}
			Sleep(pCGI->delay_of_block_moved);
		}
		if (pCGI->CFI.separator) {
			gmw_inner_piant(pCGI, x + pCGI->CFI.block_width + 2, y, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_high; j++) {
				gotoxy(x + pCGI->CFI.block_width + 2 - 2, y + j);
				cout << pCGI->CFI.v_normal;
				Sleep(pCGI->delay_of_block_moved);
				//getchar();
			}
		}
	}
	if (direction == RIGHT_TO_LEFT) {
		for (int i = 1; i <= pCGI->CFI.block_width; i++) {
			gmw_inner_piant(pCGI, x - i, y, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_high; j++) {
				gotoxy(x - i + pCGI->CFI.block_width, y + j);
				cout << " ";
				//getchar();
			}
			Sleep(pCGI->delay_of_block_moved);
		}
		if (pCGI->CFI.separator) {
			gmw_inner_piant(pCGI, x - pCGI->CFI.block_width - 2, y, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_high; j++) {
				gotoxy(x - 2, y + j);
				cout << pCGI->CFI.v_normal;
				Sleep(pCGI->delay_of_block_moved);
				//getchar();
			}
		}
	}
	if (direction == UP_TO_DOWN) {
		for (int i = 1; i <= pCGI->CFI.block_high; i++) {
			gmw_inner_piant(pCGI, x, y + i, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_width; j++) {
				gotoxy(x + j, y + i - 1);
				cout << " ";
				//getchar();
			}
			Sleep(pCGI->delay_of_block_moved);
		}
		if (pCGI->CFI.separator) {
			gmw_inner_piant(pCGI, x, y + pCGI->CFI.block_high + 1, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_width; j += 2) {
				gotoxy(x + j, y + pCGI->CFI.block_high + 1 - 1);
				cout << pCGI->CFI.h_normal;
				Sleep(pCGI->delay_of_block_moved);
				//getchar();
			}
		}
	}
	if (direction == DOWN_TO_UP) {
		for (int i = 1; i <= pCGI->CFI.block_high; i++) {
			gmw_inner_piant(pCGI, x, y - i, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_width; j++) {
				gotoxy(x + j, y - i + pCGI->CFI.block_high);
				cout << " ";
				//getchar();
			}
			Sleep(pCGI->delay_of_block_moved);
		}
		if (pCGI->CFI.separator) {
			gmw_inner_piant(pCGI, x, y - pCGI->CFI.block_high - 1, bdi_value, bdi);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			for (int j = 0; j < pCGI->CFI.block_width; j += 2) {
				gotoxy(x + j, y - 1);
				cout << pCGI->CFI.h_normal;
				Sleep(pCGI->delay_of_block_moved);
				//getchar();
			}
		}
	}
}

/* -----------------------------------------------
		ʵ����������ĺ���������������׼����
   ----------------------------------------------- */

   /***************************************************************************
	 �������ƣ�
	 ��    �ܣ�������Ϸ����ܵ�������
	 ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			   const int row						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
			   const int col						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
	 �� �� ֵ��
	 ˵    ����1��ָ��������Ϸ�ľ������������ֵ
			   2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
   ***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	pCGI->row_num = (row > 0 ? row : 0);
	pCGI->col_num = (col > 0 ? col : 0);
	gmw_inner_change_value(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸ġ�������
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������������ڣ�����Ϸ�������������ڵ�����cmd���ڣ�����ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int bg_color					��ǰ��ɫ��ȱʡCOLOR_BLACK��
		   const int fg_color					������ɫ��ȱʡCOLOR_WHITE��
		   const bool cascade					���Ƿ�����ȱʡΪtrue-������
  �� �� ֵ��
  ˵    ����1��cascade = trueʱ
				ͬ���޸���Ϸ���������ɫ
				ͬ���޸�����״̬���������ı��ı���ɫ��ǰ��ɫ����Ŀ�ı��ı���ɫ��ǰ��ɫ���䣩
			2���������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15
					ǰ��ɫ����ɫ��ֵһ�µ����޷���������
					ǰ��ɫ������״̬����Ŀǰ��ɫ�������޷�������Ŀ��ʾ
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	pCGI->area_bgcolor = bgcolor;
	pCGI->area_fgcolor = fgcolor;
	setcolor(bgcolor, fgcolor);
	if (cascade) {
		pCGI->CFI.bgcolor = bgcolor;
		pCGI->CFI.fgcolor = fgcolor;
		pCGI->SLI.lower_normal_bgcolor = bgcolor;
		pCGI->SLI.lower_normal_fgcolor = fgcolor;
		pCGI->SLI.top_normal_bgcolor = bgcolor;
		pCGI->SLI.top_normal_fgcolor = fgcolor;
		pCGI->SLI.lower_catchy_bgcolor = bgcolor;
		pCGI->SLI.top_catchy_bgcolor = bgcolor;
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ô��ڵ�����
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *fontname					���������ƣ�ֻ����"Terminal"��"������"���֣������򷵻�-1�����ı����壩
		   const int fs_high					������߶ȣ�ȱʡ������Ϊ16�������������ƣ���Ϊ��֤��
		   const int fs_width					������߶ȣ�ȱʡ������Ϊ8�������������ƣ���Ϊ��֤��
  �� �� ֵ��
  ˵    ����1����cmd_console_tools�е�setfontsize���ƣ�Ŀǰֻ֧�֡��������塱�͡������塱
			2������������������ֱ�ӷ��أ�����ԭ�������ò���
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	if (strcmp(fontname, "Terminal") && strcmp(fontname, "������"))
		return -1;
	strcpy((pCGI->CFT).font_type, fontname);
	pCGI->CFT.font_size_high = fs_high;
	pCGI->CFT.font_size_width = fs_width;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������ʱ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type						����ʱ�����ͣ�ĿǰΪ3�֣�
		   const int delay_ms					����msΪ��λ����ʱ
			   ���߿����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ��ɫ�����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ɫ���ƶ�����ʱ��BLOCK_MOVED_DELAY_MS ~ �������ޣ���Ϊ��֤��ȷ�� <BLOCK_MOVED_DELAY_MS ���� BLOCK_MOVED_DELAY_MS��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	if (type == DELAY_OF_DRAW_FRAME)
		pCGI->delay_of_draw_frame = (delay_ms > 0 ? delay_ms : 0);
	if (type == DELAY_OF_DRAW_BLOCK)
		pCGI->delay_of_draw_block = (delay_ms > 0 ? delay_ms : 0);
	if (type == DELAY_OF_BLOCK_MOVED)
		pCGI->delay_of_block_moved = (delay_ms > BLOCK_MOVED_DELAY_MS ? delay_ms : BLOCK_MOVED_DELAY_MS);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ���������������Ϸ����ܽṹ֮����Ҫ�����Ķ�������
  ��    �ܣ�CONSOLE_GRAPHICS_INFO *const pCGI	��
		   const int up_lines					���ϲ�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int down_lines				���²�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int left_cols					����߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int right_cols				���ұ߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
  �� �� ֵ��
  ˵    �����������еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����

***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	pCGI->extern_left_cols = left_cols;
	pCGI->extern_right_cols = right_cols;
	pCGI->extern_up_lines = up_lines;
	pCGI->extern_down_lines = down_lines;
	gmw_inner_change_value(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	/*type.1:�T�U�X�[�^�a�d�g�m�j�p
	type.2:���������������ȩةЩ�
	type.3:�T���V�Y�\�_�b�e�k�h�n
	type.4:���U�W�Z�]�`�c�f�l�i�o*/

	switch (type)
	{
	case 2:
	{
		strcpy(pCGI->CFI.h_normal, "��");
		strcpy(pCGI->CFI.v_normal, "��");
		strcpy(pCGI->CFI.top_left, "��");
		strcpy(pCGI->CFI.top_right, "��");
		strcpy(pCGI->CFI.lower_left, "��");
		strcpy(pCGI->CFI.lower_right, "��");
		strcpy(pCGI->CFI.v_left_separator, "��");
		strcpy(pCGI->CFI.v_right_separator, "��");
		strcpy(pCGI->CFI.h_lower_separator, "��");
		strcpy(pCGI->CFI.h_top_separator, "��");
		strcpy(pCGI->CFI.mid_separator, "��");
		break;
	}
	case 3:
	{
		strcpy(pCGI->CFI.h_normal, "�T");
		strcpy(pCGI->CFI.v_normal, "��");
		strcpy(pCGI->CFI.top_left, "�V");
		strcpy(pCGI->CFI.top_right, "�Y");
		strcpy(pCGI->CFI.lower_left, "�\");
		strcpy(pCGI->CFI.lower_right, "�_");
		strcpy(pCGI->CFI.v_left_separator, "�b");
		strcpy(pCGI->CFI.v_right_separator, "�e");
		strcpy(pCGI->CFI.h_lower_separator, "�k");
		strcpy(pCGI->CFI.h_top_separator, "�h");
		strcpy(pCGI->CFI.mid_separator, "�n");
		break;
	}
	case 4:
	{
		strcpy(pCGI->CFI.h_normal, "��");
		strcpy(pCGI->CFI.v_normal, "�U");
		strcpy(pCGI->CFI.top_left, "�W");
		strcpy(pCGI->CFI.top_right, "�Z");
		strcpy(pCGI->CFI.lower_left, "�]");
		strcpy(pCGI->CFI.lower_right, "�`");
		strcpy(pCGI->CFI.v_left_separator, "�c");
		strcpy(pCGI->CFI.v_right_separator, "�f");
		strcpy(pCGI->CFI.h_lower_separator, "�l");
		strcpy(pCGI->CFI.h_top_separator, "�i");
		strcpy(pCGI->CFI.mid_separator, "�o");
		break;
	}
	default:
	{
		strcpy(pCGI->CFI.h_normal, "�T");
		strcpy(pCGI->CFI.v_normal, "�U");
		strcpy(pCGI->CFI.top_left, "�X");
		strcpy(pCGI->CFI.top_right, "�[");
		strcpy(pCGI->CFI.lower_left, "�^");
		strcpy(pCGI->CFI.lower_right, "�a");
		strcpy(pCGI->CFI.v_left_separator, "�d");
		strcpy(pCGI->CFI.v_right_separator, "�g");
		strcpy(pCGI->CFI.h_lower_separator, "�m");
		strcpy(pCGI->CFI.h_top_separator, "�j");
		strcpy(pCGI->CFI.mid_separator, "�p");
		break;
	}//ȱʡ�������Ҳ�������ͳһȫ˫��
	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const char *...						����11�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right,
	const char *lower_right, const char *h_normal, const char *v_normal, const char *h_top_separator,
	const char *h_lower_separator, const char *v_left_separator, const char *v_right_separator, const char *mid_separator)
{
	gmw_inner_check_string(pCGI->CFI.h_normal, h_normal);
	gmw_inner_check_string(pCGI->CFI.v_normal, v_normal);
	gmw_inner_check_string(pCGI->CFI.top_left, top_left);
	gmw_inner_check_string(pCGI->CFI.top_right, top_right);
	gmw_inner_check_string(pCGI->CFI.lower_left, lower_left);
	gmw_inner_check_string(pCGI->CFI.lower_right, lower_right);
	gmw_inner_check_string(pCGI->CFI.v_left_separator, v_left_separator);
	gmw_inner_check_string(pCGI->CFI.v_right_separator, v_right_separator);
	gmw_inner_check_string(pCGI->CFI.h_lower_separator, h_lower_separator);
	gmw_inner_check_string(pCGI->CFI.h_top_separator, h_top_separator);
	gmw_inner_check_string(pCGI->CFI.mid_separator, mid_separator);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�ɫ��������С���Ƿ���Ҫ�ָ��ߵ�
  ������������������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int block_width						����ȣ�����ȱʡ2����ΪԼ�������Ϊ�����Ʊ�����������������Ҫ+1��
			const int block_high						���߶ȣ�����ȱʡ1��
			const bool separator						���Ƿ���Ҫ�ָ��ߣ�ȱʡΪtrue����Ҫ�ָ��ߣ�
  �� �� ֵ��
  ˵    ������ܴ�С/�Ƿ���Ҫ�ָ��ߵȵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	pCGI->CFI.block_high = block_high;
	pCGI->CFI.block_width = (block_width % 2 == 0 ? block_width : (block_width + 1));
	pCGI->CFI.separator = separator;
	gmw_inner_change_value(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BORDER_TYPE �ṹ�е���ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int bg_color					������ɫ��ȱʡ -1��ʾ�ô��ڱ���ɫ��
			const int fg_color					��ǰ��ɫ��ȱʡ -1��ʾ�ô���ǰ��ɫ��
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	pCGI->CFI.fgcolor = (fgcolor == -1 ? pCGI->area_fgcolor : fgcolor);
	pCGI->CFI.bgcolor = (bgcolor == -1 ? pCGI->area_bgcolor : bgcolor);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ˫�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	switch (type)
	{
	case 2:
	{
		strcpy(pCGI->CBI.h_normal, "��");
		strcpy(pCGI->CBI.v_normal, "��");
		strcpy(pCGI->CBI.top_left, "��");
		strcpy(pCGI->CBI.top_right, "��");
		strcpy(pCGI->CBI.lower_left, "��");
		strcpy(pCGI->CBI.lower_right, "��");
		break;
	}
	case 3:
	{
		strcpy(pCGI->CBI.h_normal, "�T");
		strcpy(pCGI->CBI.v_normal, "��");
		strcpy(pCGI->CBI.top_left, "�V");
		strcpy(pCGI->CBI.top_right, "�Y");
		strcpy(pCGI->CBI.lower_left, "�\");
		strcpy(pCGI->CBI.lower_right, "�_");
		break;
	}
	case 4:
	{
		strcpy(pCGI->CBI.h_normal, "��");
		strcpy(pCGI->CBI.v_normal, "�U");
		strcpy(pCGI->CBI.top_left, "�W");
		strcpy(pCGI->CBI.top_right, "�Z");
		strcpy(pCGI->CBI.lower_left, "�]");
		strcpy(pCGI->CBI.lower_right, "�`");
		break;
	}
	default:
	{
		strcpy(pCGI->CBI.h_normal, "�T");
		strcpy(pCGI->CBI.v_normal, "�U");
		strcpy(pCGI->CBI.top_left, "�X");
		strcpy(pCGI->CBI.top_right, "�[");
		strcpy(pCGI->CBI.lower_left, "�^");
		strcpy(pCGI->CBI.lower_right, "�a");
		break;
	}
	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *...					����6�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	gmw_inner_check_string(pCGI->CBI.h_normal, h_normal);
	gmw_inner_check_string(pCGI->CBI.v_normal, v_normal);
	gmw_inner_check_string(pCGI->CBI.top_left, top_left);
	gmw_inner_check_string(pCGI->CBI.top_right, top_right);
	gmw_inner_check_string(pCGI->CBI.lower_left, lower_left);
	gmw_inner_check_string(pCGI->CBI.lower_right, lower_right);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ÿ����Ϸɫ��(����)�Ƿ���ҪС�߿�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡfalse��
  �� �� ֵ��
  ˵    �����߿�Լ��Ϊ�����Ʊ����˫��
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->CBI.block_border = on_off;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ����״̬��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡtrue��
  �� �� ֵ��
  ˵    ����1��״̬�������Լ�����£�
			   1.1����״̬��ֻ��һ�У������������Ϸ�����/�б�����棬Ϊ��������ʼһ�У�����������Ͻ����������״̬�������꣩
			   1.2����״̬��ֻ��һ�У������������·����ߵ�����
			   1.3��״̬���Ŀ��Ϊ�������ȣ������Ϣ������ض�
		   2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	if (type == TOP_STATUS_LINE) {
		pCGI->top_status_line = on_off;
		gmw_inner_change_value(pCGI);
	}
	if (type == LOWER_STATUS_LINE) {
		pCGI->lower_status_line = on_off;
		gmw_inner_change_value(pCGI);
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���������״̬������ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const int normal_bgcolor			�������ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int normal_fgcolor			�������ı�ǰ��ɫ��ȱʡ -1��ʾʹ�ô���ǰ��ɫ��
			const int catchy_bgcolor			����Ŀ�ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int catchy_fgcolor			����Ŀ�ı�ǰ��ɫ��ȱʡ -1��ʾʹ������ɫ��
  ���������
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor,
	const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	if (type == TOP_STATUS_LINE) {
		pCGI->SLI.top_normal_bgcolor = (normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor);
		pCGI->SLI.top_normal_fgcolor = (normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor);
		pCGI->SLI.top_catchy_bgcolor = (catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor);
		pCGI->SLI.top_catchy_fgcolor = (catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor);
	}
	if (type == LOWER_STATUS_LINE) {
		pCGI->SLI.lower_normal_bgcolor = (normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor);
		pCGI->SLI.lower_normal_fgcolor = (normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor);
		pCGI->SLI.lower_catchy_bgcolor = (catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor);
		pCGI->SLI.lower_catchy_fgcolor = (catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor);
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�к�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���к�Լ��Ϊ��ĸA��ʼ�������У��������26�����a��ʼ������52��ͳһΪ*��ʵ��Ӧ�ò����ܣ�
			2���Ƿ���ʾ�кŵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->draw_frame_with_row_no = on_off;
	gmw_inner_change_value(pCGI);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�б�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���б�Լ��Ϊ����0��ʼ�������У�����0-99������99ͳһΪ**��ʵ��Ӧ�ò����ܣ�
			2���Ƿ���ʾ�б�ı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->draw_frame_with_col_no = on_off;
	gmw_inner_change_value(pCGI);
	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡ CONSOLE_GRAPHICS_INFO �ṹ���еĸ���Աֵ
  ���������
  �� �� ֵ��
  ˵    ����1�����������ã���ӡ��ʽ�Զ���
			2������������������δ���ù������Բ�ʵ��
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��� CONSOLE_GRAPHICS_INFO �ṹ����ȱʡֵ���г�ʼ��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI������ṹָ��
		   const int row					����Ϸ����ɫ��������ȱʡ10��
		   const int col					����Ϸ����ɫ��������ȱʡ10��
		   const int bgcolor				���������ڱ���ɫ��ȱʡ COLOR_BLACK��
		   const int fgcolor				���������ڱ���ɫ��ȱʡ COLOR_WHITE��
  �� �� ֵ��
  ˵    �������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ��
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	gmw_set_rowcol(pCGI, row, col);
	//ȫ�ֱ�������
	gmw_set_color(pCGI, bgcolor, fgcolor, true);
	//��������
	gmw_set_font(pCGI, "Terminal", 16, 8);
	//��ʱ����
	pCGI->delay_of_draw_frame = 0;
	pCGI->delay_of_draw_block = 0;
	pCGI->delay_of_block_moved = 3;
	//����Ŀռ�����
	gmw_set_ext_rowcol(pCGI);
	//״̬������
	gmw_set_status_line_switch(pCGI, TOP_STATUS_LINE, true);
	gmw_set_status_line_switch(pCGI, LOWER_STATUS_LINE, true);
	//ɫ��߿�����//��Ϸ����ָ�������
	gmw_set_frame_style(pCGI);
	//״̬����Ŀ����ǰ������
	pCGI->SLI.lower_catchy_fgcolor = 14;
	pCGI->SLI.top_catchy_fgcolor = 14;
	//���б������
	gmw_set_rowno_switch(pCGI, false);
	gmw_set_colno_switch(pCGI, false);
	//555
	gmw_set_frame_default_linetype(pCGI, 1);
	//ɫ��߿�����
	gmw_set_block_border_switch(pCGI, false);
	//555
	gmw_set_block_default_linetype(pCGI, 1);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ���
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ��������ɲο�demo��Ч��
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	//"�T�U�X�[�^�a�d�g�m�j�p"
	char string[60] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	//������Ϣ
	setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
	setconsoleborder(pCGI->cols, pCGI->lines, pCGI->cols, pCGI->lines);
	setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	//�к�
	if (pCGI->draw_frame_with_col_no == true) {
		int middle = pCGI->CFI.block_width + pCGI->CFI.separator * 2, m;
		m = (middle > 4 ? middle / 2 : 2);
		for (int i = 0; i < pCGI->col_num; i++) {
			gotoxy(pCGI->start_x + i * middle + m, pCGI->start_y - 1);
			if (i < 99)
				cout << i;
			else
				cout << "**";
		}
	}
	//�б�
	setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	if (pCGI->draw_frame_with_row_no) {
		int middle = pCGI->CFI.block_high + pCGI->CFI.separator;
		int m = (middle == 1 ? 1 : middle / 2);
		for (int i = 0; i < pCGI->row_num; i++) {
			gotoxy(pCGI->start_x - 1, pCGI->start_y + m + i * middle);
			cout << (i < 52 ? string[i] : '*');
		}
	}
	//��һ��ɫ��ĵ�һ��
	setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
	gotoxy(pCGI->start_x, pCGI->start_y);
	cout << pCGI->CFI.top_left;//"�X"
	for (int i = 0; i < pCGI->col_num; i++) {
		for (int j = 0; j < pCGI->CFI.block_width / 2; j++) {
			cout << pCGI->CFI.h_normal;//"�T"
		}
		if (i < pCGI->col_num - 1 && pCGI->CFI.separator) {
			cout << pCGI->CFI.h_top_separator;//"�j"
		}
	}
	cout << pCGI->CFI.top_right;//"�["
	Sleep(pCGI->delay_of_draw_frame);

	for (int i = 0; i < pCGI->row_num; i++) {

		//ɫ��ķ����һ��
		for (int j = 1; j <= pCGI->CFI.block_high; j++) {
			gotoxy(pCGI->start_x, pCGI->start_y + i * (pCGI->CFI.block_high + pCGI->CFI.separator) + j);
			cout << pCGI->CFI.v_normal;//"�U"
			int k;
			for (k = 1; k < pCGI->col_num; k++) {
				for (int t = 0; t < pCGI->CFI.block_width / 2; t++)
					cout << "  ";
				if (pCGI->CFI.separator) {
					gotoxy(pCGI->start_x + k * (pCGI->CFI.block_width + pCGI->CFI.separator * 2),
						pCGI->start_y + i * (pCGI->CFI.block_high + pCGI->CFI.separator) + j);
					cout << pCGI->CFI.v_normal;//"�U"
				}
			}
			for (int t = 0; t < pCGI->CFI.block_width / 2; t++)
				cout << "  ";
			gotoxy(pCGI->start_x + k * (pCGI->CFI.block_width + pCGI->CFI.separator * 2) + 2 * (!pCGI->CFI.separator),
				pCGI->start_y + i * (pCGI->CFI.block_high + pCGI->CFI.separator) + j);
			cout << pCGI->CFI.v_normal;//"�U"
		}
		Sleep(pCGI->delay_of_draw_frame);

		//ɫ������һ��
		gotoxy(pCGI->start_x, pCGI->start_y + i * (pCGI->CFI.block_high + pCGI->CFI.separator) + pCGI->CFI.block_high + 1);
		if (i < pCGI->row_num - 1) {
			cout << pCGI->CFI.v_left_separator;//"�d"
			for (int k = 1; k <= pCGI->col_num; k++) {
				if (pCGI->CFI.separator) {
					for (int t = 0; t < pCGI->CFI.block_width / 2; t++) {
						cout << pCGI->CFI.h_normal;//"�T"
					}
					if (k < pCGI->col_num) {
						cout << pCGI->CFI.mid_separator;//"�p"
					}
				}
			}
			gotoxy(pCGI->start_x + pCGI->col_num*(pCGI->CFI.block_width + 2 * pCGI->CFI.separator), pCGI->start_y + i * (pCGI->CFI.block_high + pCGI->CFI.separator) + pCGI->CFI.block_high + 1);
			if (pCGI->CFI.separator) {
				cout << pCGI->CFI.v_right_separator;//"�g"
			}
			Sleep(pCGI->delay_of_draw_frame);
		}
	}

	//���һ��ɫ������һ��
	gotoxy(pCGI->start_x, pCGI->start_y + (pCGI->row_num - 1) * (pCGI->CFI.block_high + pCGI->CFI.separator) + pCGI->CFI.block_high + 1);
	cout << pCGI->CFI.lower_left;//"�^"
	for (int i = 0; i < pCGI->col_num; i++) {
		for (int j = 0; j < pCGI->CFI.block_width / 2; j++) {
			cout << pCGI->CFI.h_normal;//"�T"
		}
		if (i < pCGI->col_num - 1 && pCGI->CFI.separator)
			cout << pCGI->CFI.h_lower_separator;//"�m"
	}
	cout << pCGI->CFI.lower_right;//"�a"

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���״̬������ʾ��Ϣ
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type							��ָ������/��״̬��
		   const char *msg						��������Ϣ
		   const char *catchy_msg					����Ҫ�ر��ע����Ϣ����������Ϣǰ��ʾ��
  �� �� ֵ��
  ˵    ����1���������޶�Ϊ����ܵĿ�ȣ����к��б�λ�ã����������ȥ
			2��������һ���ַ���ĳ���ֵ�ǰ������ᵼ�º������룬Ҫ����
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg)
{
	int len = (catchy_msg == NULL ? 0 : strlen(catchy_msg));
	if ((type == TOP_STATUS_LINE) && pCGI->top_status_line)
	{
		gotoxy(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y);
		setcolor(pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor);
		gmw_inner_output(catchy_msg, pCGI->cols - pCGI->extern_left_cols - pCGI->extern_right_cols,false);
		setcolor(pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor);
		gmw_inner_output(msg, pCGI->cols - pCGI->extern_left_cols - pCGI->extern_right_cols - len);
	}
	else if ((type == LOWER_STATUS_LINE) && pCGI->lower_status_line)
	{
		gotoxy(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y);
		setcolor(pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor);
		gmw_inner_output(catchy_msg, pCGI->cols - pCGI->extern_left_cols - pCGI->extern_right_cols,false);
		setcolor(pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);
		gmw_inner_output(msg, pCGI->cols - pCGI->extern_left_cols - pCGI->extern_right_cols - len);
	}
	return 0; //�˾�ɸ�����Ҫ�޸�*/
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾĳһ��ɫ��(����Ϊ�ַ���������Ϊrow/col)
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const BLOCK_DISPLAY_INFO *const bdi		����Ÿ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
  �� �� ֵ��
  ˵    ����1��BLOCK_DISPLAY_INFO �ĺ����ͷ�ļ����÷��ο���������
			2��bdi_valueΪ BDI_VALUE_BLANK ��ʾ�հ׿飬Ҫ���⴦��
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{
	int x, y;
	x = pCGI->start_x + col_no * 2 * pCGI->CFI.separator + 2 + col_no * pCGI->CFI.block_width;
	y = pCGI->start_y + row_no * pCGI->CFI.separator + 1 + row_no * pCGI->CFI.block_high;
	gmw_inner_piant(pCGI,x,y,bdi_value,bdi);
	Sleep(pCGI->delay_of_draw_block);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ƶ�ĳһ��ɫ��
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const int blank_bdi_value				���ƶ����������ڶ���Ч����ʾʱ���ڱ�ʾ�հ׵�ֵ
		   ��һ��Ϊ0���˴���Ϊ�������룬�ǿ��ǵ����ܳ��ֵ����������
		   const BLOCK_DISPLAY_INFO *const bdi		�������ʾֵ/�հ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
		   const int direction						���ƶ�����һ�����֣������cmd_gmw_tools.h
		   const int distance						���ƶ����루��1��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
	int i = 0;
	if (direction == LEFT_TO_RIGHT) 
		for (; i < distance; i++) 
			gmw_inner_move(pCGI, row_no, col_no + i, bdi_value, blank_bdi_value, bdi, direction);
	if (direction == RIGHT_TO_LEFT)
		for (; i < distance; i++) 
		gmw_inner_move(pCGI, row_no, col_no - i, bdi_value, blank_bdi_value, bdi, direction);
	if (direction == UP_TO_DOWN) 
		for (; i < distance; i++)
			gmw_inner_move(pCGI, row_no + i, col_no, bdi_value, blank_bdi_value, bdi, direction);
	if (direction == DOWN_TO_UP) 
		for (; i < distance; i++)
			gmw_inner_move(pCGI, row_no - i, col_no, bdi_value, blank_bdi_value, bdi, direction);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������̻����
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   int &MAction							��������� CCT_MOUSE_EVENT�����ֵ��Ч��Ϊ MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK ����֮һ
													   ������� CCT_KEYBOARD_EVENT�����ֵ��Ч
		   int &MRow								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &MCol								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode1							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ��һ����
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode2							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ�ڶ���������ǵ����룬��Ϊ0��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   const bool update_lower_status_line		������ƶ�ʱ���Ƿ�Ҫ�ڱ���������ʾ"[��ǰ���] *��*��"����Ϣ��true=��ʾ��false=����ʾ��ȱʡΪtrue��
  �� �� ֵ����������Լ��
			1�����������ƶ��������أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] *��*��"
		   2������ǰ������������ҵ�ǰ���ָ��ͣ��������Ϸ�����*��*���ϣ��򷵻� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow Ϊ�кţ�MCol Ϊ�б�
								  �ҵ�ǰ���ָ��ͣ���ڷǷ����򣨷���Ϸ������Ϸ�����еķָ��ߣ����򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
		   3������ǰ�������Ҽ������ж����ָ��ͣ�������Ƿ�Ϸ���ֱ�ӷ��� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow��MCol������
		   4��������¼����ϵ�ĳ������˫���밴��������ֱ�ӷ��� CCT_KEYBOARD_EVENT��KeyCode1/KeyCode2��Ϊ��Ӧ�ļ���ֵ
 ˵    ����ͨ������ cmd_console_tools.cpp �е� read_keyboard_and_mouse ����ʵ��
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	enable_mouse();
	int X = 0, Y = 0,x=-1,y=-1;
	int ret, maction;
	int keycode1, keycode2;
	KeyCode1 = 0;
	KeyCode2 = 0;
	while (1) {
		MRow = -1;
		MCol = -1;
		ret = read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		x = (X - 2 - pCGI->start_x) % (2 * pCGI->CFI.separator + pCGI->CFI.block_width);
		y = (Y - 1 - pCGI->start_y) % (pCGI->CFI.separator + pCGI->CFI.block_high);
		if (ret == CCT_KEYBOARD_EVENT) {
			KeyCode1 = keycode1;
			KeyCode2 = keycode2;
			disable_mouse();
			return ret; 
		}
		if (ret == CCT_MOUSE_EVENT) {
			MAction = maction;
			if (x >= 0 && x < pCGI->CFI.block_width && y >= 0 && y < pCGI->CFI.block_high) {
				MCol = (X - 2 - pCGI->start_x) / (2 * pCGI->CFI.separator + pCGI->CFI.block_width);
				MRow = (Y - 1 - pCGI->start_y) / (pCGI->CFI.separator + pCGI->CFI.block_high);
			}
			if (update_lower_status_line) {
				char temp[256];
				if (MRow >= 0 && MRow < pCGI->row_num && MCol >= 0 && MCol < pCGI->col_num)
					sprintf(temp, "[��ǰ���] %c��%d��", char('A' + (Y - 1 - pCGI->start_y) / (pCGI->CFI.separator + pCGI->CFI.block_high)),
					(X - 2 - pCGI->start_x) / (2 * pCGI->CFI.separator + pCGI->CFI.block_width));
				else
					sprintf(temp, "[��ǰ���] λ�÷Ƿ�");
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
			}
			//gotoxy(0, 0);
			//cout << setw(3) << MRow << "   " << setw(3) << MCol<<"   "<< pCGI->row_num<<"   "<< pCGI->col_num;
			//gotoxy(0, 1);
			//cout << setw(3) << x << "   " << setw(3) << y<< "   "<<pCGI->CFI.block_width<<"   "<< pCGI->CFI.block_high;
			if ((maction == MOUSE_LEFT_BUTTON_CLICK || maction == MOUSE_RIGHT_BUTTON_CLICK) &&
				(MRow >= 0 && MRow < pCGI->row_num && MCol >= 0 && MCol < pCGI->col_num)){
				disable_mouse();
				return ret;
			}
		}
	}
}
