/* 1853790 ׯ�� ��ǿ */
#define _CRT_SECURE_NO_WARNINGS
#include "90-b5.h"
#include "../common/cmd_gmw_tools.h"
/***************************************************************************
  �������ƣ�
  ��    �ܣ����µ÷����
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void output_top_info(CONSOLE_GRAPHICS_INFO * pCGI, const int score)
{
	char temp[256];
	temp[0] = '\0';
	sprintf(temp, "��ǰ����:%d", score);
	gmw_status_line(pCGI, TOP_STATUS_LINE, temp);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����������ɻ�
  ���������
  �� �� ֵ��0���������� 1������ɻ��ᳬ���߿���������������
  ˵    ����
***************************************************************************/
int paint_whole__plane(const int row1, const int col1, const int row2, const int col2, CONSOLE_GRAPHICS_INFO * pCGI,
	const BLOCK_DISPLAY_INFO bdi[], int plane[][Size], int mood)
{
	//mood 1���հ�ֵ��� 2�������� 3�������ɻ����
	int color;
	if (mood == 1)
		color = 0;
	else if (mood == 2)
		color = -2;
	else if (mood == 3)
		color = 1;
	if (row1 == row2) {
		int min = (col1 < col2 ? col1 : col2);
		int max = (col1 > col2 ? col1 : col2);
		if (abs(col1 - col2) != 3 || min < 0 || max > 9 || row1 - 2 < 0 || row1 + 2 > 9)
			return -1;
		for (int i = min; i < min + 4; i++)
			gmw_draw_block(pCGI, row1, i, color, bdi);
		for (int i = row1 - 2; i < row1 + 3; i++)
			gmw_draw_block(pCGI, i, col1 + (col1 < col2 ? 1 : -1), color, bdi);
		for (int i = row2 - 1; i != row2 + 2; i++)
			gmw_draw_block(pCGI, i, col2, color, bdi);
		if (mood == 3) {
			plane[row1][col1] = 4;
			plane[row2][col2] = 1;
			gmw_draw_block(pCGI, row1, col1, 4, bdi);
		}
		return 0;
	}
	else if (col1 == col2)
	{
		int min = (row1 < row2 ? row1 : row2);
		int max = (row1 > row2 ? row1 : row2);
		if (abs(row1 - row2) != 3 || min < 0 || max > 9 || col1 - 2 < 0 || col1 + 2 > 9)
			return -1;
		for (int i = min; i < min + 4; i++)
			gmw_draw_block(pCGI, i, col1, color, bdi);
		for (int i = col1 - 2; i < col1 + 3; i++)
			gmw_draw_block(pCGI, row1 + (row1 < row2 ? 1 : -1), i, color, bdi);
		for (int i = col2 - 1; i != col2 + 2; i++)
			gmw_draw_block(pCGI, row2, i, color, bdi);
		if (mood == 3) {
			plane[row1][col1] = 4;
			plane[row2][col2] = 1;
			gmw_draw_block(pCGI, row1, col1, 4, bdi);
		}
		return 0;
	}
	else
		return -1;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�˫������һ�ܷɻ��ĵ÷ּ���
  ���������
  �� �� ֵ��
  ˵    ������ͷ����֣����ڻ���ͻ�β������õ��������ˣ�Ҳ����֣�û�б�����ģ���1��
***************************************************************************/
void CalScore(const int row1, const int col1, const int row2, const int col2, int plane[][Size], int &score)
{
	if (row1 == row2) {
		int min = (col1 < col2 ? col1 : col2);
		for (int i = min; i < min + 4; i++)
			if (plane[row1][i] == 0)
				score++;
		for (int i = row1 - 2; i < row1; i++)
			if (plane[i][col1 + (col1 < col2 ? 1 : -1)] == 0)
				score++;
		for (int i = row1 + 1; i < row1 + 3; i++)
			if (plane[i][col1 + (col1 < col2 ? 1 : -1)] == 0)
				score++;
		for (int i = row2 - 1; i < row2; i++)
			if (plane[i][col2] == 0)
				score++;
		for (int i = row2 + 1; i < row2 + 2; i++)
			if (plane[i][col2] == 0)
				score++;
		if (plane[row1][col1] == 0)
			score--;
	}
	else if (col1 == col2)
	{
		int min = (row1 < row2 ? row1 : row2);
		for (int i = min; i < min + 4; i++)
			if (plane[i][col1] == 0)
				score++;
		for (int i = col1 - 2; i < col1; i++)
			if (plane[row1 + (row1 < row2 ? 1 : -1)][i] == 0)
				score++;
		for (int i = col1 + 1; i < col1 + 3; i++)
			if (plane[row1 + (row1 < row2 ? 1 : -1)][i] == 0)
				score++;
		for (int i = col2 - 1; i < col2; i++)
			if (plane[row2][i] == 0)
				score++;
		for (int i = col2 + 1; i < col2 + 2; i++)
			if (plane[row2][i] == 0)
				score++;
		if (plane[row1][col1] == 0)
			score--;
	}
}