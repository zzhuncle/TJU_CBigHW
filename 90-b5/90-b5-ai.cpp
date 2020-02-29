/* 1853790 ׯ�� ��ǿ */
#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <cmath>
#include "90-b5.h"
#include "../common/cmd_net_tools.h"
#include "../common/md5.h"
#include "../common/cmd_gmw_tools.h"
#include <fstream>
const int Delay = 0;
const int PlaneShape[5][5] = {
		{0, 0, 4, 0, 0},
		{1, 1, 1, 1, 1},
		{0, 0, 1, 0, 0},
		{0, 1, 1, 1, 0},
		{0, 0, 0, 0, 0}
};

/***************************************************************************
  �������ƣ�
  ��    �ܣ���̬�����ʼ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void Init3d(int**** Methods, int**** Store)
{
	(*Methods) = new(nothrow) int**[N];
	for (int i = 0; i < N; i++) {
		(*Methods)[i] = new(nothrow) int*[Size];
		for (int j = 0; j < Size; j++) {
			(*Methods)[i][j] = new(nothrow) int[Size];
			for (int k = 0; k < Size; k++)
				(*Methods)[i][j][k] = 0;
		}
	}
	(*Store) = new(nothrow) int**[N];
	for (int i = 0; i < N; i++) {
		(*Store)[i] = new(nothrow) int*[4];
		for (int j = 0; j < 4; j++) {
			(*Store)[i][j] = new(nothrow) int[4];
			for (int k = 0; k < 4; k++)
				(*Store)[i][j][k] = 0;
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ά���鶯̬������ͷ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void free3d(int**** Methods, int**** Store)
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < Size; j++) {
			delete ((*Methods)[i][j]);
		}
		delete ((*Methods)[i]);
	}
	delete (*Methods);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < 4; j++) {
			delete ((*Store)[i][j]);
		}
		delete ((*Store)[i]);
	}
	delete (*Store);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����������Ϣȷ��˫���귨����β����
  ���������cnt������ţ�i����ڼ��ܷɻ�
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void FindOnly(int*** Store, int cnt, char &head_row, char &tail_row, int &head_col, int &tail_col, int i)
{
	if (Store[cnt][i][3] == 0) {
		head_row = Store[cnt][i][1] + 'A';
		head_col = Store[cnt][i][2] + 2;
		tail_row = Store[cnt][i][1] + 3 + 'A';
		tail_col = Store[cnt][i][2] + 2;
	}
	if (Store[cnt][i][3] == 1) {
		head_row = Store[cnt][i][1] + 2 + 'A';
		head_col = Store[cnt][i][2];
		tail_row = Store[cnt][i][1] + 2 + 'A';
		tail_col = Store[cnt][i][2] + 3;
	}
	if (Store[cnt][i][3] == 2) {
		head_row = Store[cnt][i][1] + 3 + 'A';
		head_col = Store[cnt][i][2] + 2;
		tail_row = Store[cnt][i][1] + 'A';
		tail_col = Store[cnt][i][2] + 2;
	}
	if (Store[cnt][i][3] == 3) {
		head_row = Store[cnt][i][1] + 2 + 'A';
		head_col = Store[cnt][i][2] + 3;
		tail_row = Store[cnt][i][1] + 2 + 'A';
		tail_col = Store[cnt][i][2];
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����������ĳ�������״̬
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int CheckInfo(int discovered[Size][Size], int plane[Size][Size], int x, int y) {
	if ((0 <= x && x < Size) && (0 <= y && y < Size)) {
		if (discovered[x][y])
			return plane[x][y];
		else
			return -1;//δ�鿴���������
	}
	else
		return -2;//��������
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��õ���ת��ķɻ�ģ��
  ���������
  �� �� ֵ��
  ˵    ����������ת�Ƕȷ���
***************************************************************************/
int SwitchPlane(int direction, int x, int y)
{
	switch (direction) {
	case 0: //ԭ��
		return PlaneShape[x][y];
	case 1: //��ת90��
		return PlaneShape[y][x];
	case 2: //��ת180
		if (3 - x >= 0)
			return PlaneShape[3 - x][y];
		else
			return 0;
	case 3: //��ת270
		if (3 - y >= 0)
			return PlaneShape[3 - y][x];
		else
			return 0;
	default:
		return 0;
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����������Ƿ���Է��÷ɻ�
  ���������x��y ��ʾ�ɻ����ڵ������У������Ͻǵ�λ��(�յ�)
  �� �� ֵ��
  ˵    ����
***************************************************************************/
bool CanSetPlane(int plane[Size][Size], int x, int y, int dir)
{
	dir %= 4; // dir=0,1,2,3 ��ʾ���ɻ���ʱ����ת 0��90��180��270 
	for (int i = 0; i <= 4; i++) {
		for (int j = 0; j <= 4; j++) {
			int xn = x + i, yn = y + j; // ��ǰ������� 
			if (SwitchPlane(dir, i, j) > 0)// ���ǿյصĵط�����Ҫ���� 
			{
				if (!(0 <= xn && xn <= Size - 1) || !(0 <= yn && yn <= Size - 1))
					return false; // �������� 
				if (plane[xn][yn] != 0)
					return false; // ���������ɻ� 
			}
		}
	}
	return true; // ���Է��÷ɻ� 
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����÷ɻ�
  ���������
  �� �� ֵ��
  ˵    ������ǰһ��Ҫȷ�����Է���
***************************************************************************/
void SetPlane(int plane[Size][Size], int x, int y, int dir) {
	dir %= 4;
	for (int i = 0; i <= 4; i++) {
		for (int j = 0; j <= 4; j++) {
			int xn = x + i, yn = y + j;     // ��ǰ������� 
			if (SwitchPlane(dir, i, j) > 0) {    // ���ǿյصĵط�����Ҫ���� 
				plane[xn][yn] = SwitchPlane(dir, i, j);
			}
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����÷ɻ��ġ�����������
  ���������
  �� �� ֵ��
  ˵    �������������õķɻ�
***************************************************************************/
void UnSetPlane(int plane[Size][Size], int x, int y, int dir) {
	dir %= 4;
	for (int i = 0; i <= 4; i++) {
		for (int j = 0; j <= 4; j++) {
			int xn = x + i, yn = y + j; // ��ǰ������� 
			if (SwitchPlane(dir, i, j) > 0) {
				plane[xn][yn] = 0;
			}
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����������ʽ�ĵ÷ּ�����
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void ai_single_deal(const string spack, int &score, CONSOLE_GRAPHICS_INFO * pCGI, const BLOCK_DISPLAY_INFO bdi[], int plane[][Size],
	const int xn, const int yn)
{
	if (spack == "HitFail") {//��ѡ����Ϊ��
		score -= 1;
		plane[xn][yn] = 0;
		gmw_draw_block(pCGI, xn, yn, -2, bdi);
	}
	if (spack == "HitBody") {//��ѡ����Ϊ����
		score += 1;
		plane[xn][yn] = 1;
		gmw_draw_block(pCGI, xn, yn, 1, bdi);
	}
	if (spack == "HitHead") {//��ѡ����Ϊ��ͷ
		score += 2;
		plane[xn][yn] = 4;
		gmw_draw_block(pCGI, xn, yn, 4, bdi);
	}
	if (spack == "HitRepeat")
		score -= 10;
	if (spack == "InvalidCoordinate")
		score -= 10;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����˫������ʽ�ĵ÷ּ�����
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void ai_double_deal(const string spack, int &score, CONSOLE_GRAPHICS_INFO * pCGI, const BLOCK_DISPLAY_INFO bdi[], int plane[][Size],
	const char head_row, const char tail_row, const int head_col, const int tail_col, int*** Methods, const int n)
{
	if (spack == "HitFail") {//��ѡ����Ϊ��
		score -= 3;
		gmw_draw_block(pCGI, int(head_row - 'A'), head_col, -2, bdi);
		gmw_draw_block(pCGI, int(tail_row - 'A'), tail_col, -2, bdi);
		Sleep(200);
		gmw_draw_block(pCGI, int(head_row - 'A'), head_col, plane[int(head_row - 'A')][head_col], bdi);
		gmw_draw_block(pCGI, int(tail_row - 'A'), tail_col, plane[int(head_row - 'A')][head_col], bdi);
	}
	if (spack == "HitPlane") {//��ѡ����Ϊ�ɻ�
		CalScore(int(head_row - 'A'), head_col, int(tail_row - 'A'), tail_col, plane, score);
		paint_whole__plane(int(head_row - 'A'), head_col, int(tail_row - 'A'), tail_col, pCGI, bdi, plane, 3);
	}
	if (spack == "HitRepeat")
		score -= 10;
	if (spack == "InvalidCoordinate")
		score -= 10;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ݹ��ҵ����еķ������Ҵ洢
  ���������
  �� �� ֵ��
  ˵    ������֧������������������һ�����Ѿ����������е����̷���(������ʽ:���к���)��
			��һ�����Ѿ��ҵ�һ�ֽ������
***************************************************************************/
void FindAllSolutions(int x, int y, int &left, int &cnt, int plane[Size][Size], int*** Methods, int*** Store, int X[4], int Y[4], int dir[4])
{
	if (left == 0) {
		cnt++;
		for (int i = 0; i < Size; i++)
			memcpy(Methods[cnt][i], plane[i], Size * sizeof(int));
		for (int i = 1; i <= 3; i++) { // �������ܷɻ������� 
			Store[cnt][i][1] = X[i];
			Store[cnt][i][2] = Y[i];
			Store[cnt][i][3] = dir[i];
		}
		return;
	}
	if (x == 7)
		return;

	// ��ǰλ�÷ŷɻ� 
	for (int i = 0; i <= 3; i++)
	{
		if (CanSetPlane(plane, x, y, i))
		{
			SetPlane(plane, x, y, i);
			// ���浱ǰ���� 
			X[left] = x;//�����Ͻǵĺ�����
			Y[left] = y;//�����Ͻǵ�������
			dir[left] = i;//�ɻ��ķ���
			left--;
			if (y == 7)
				FindAllSolutions(x + 1, 0, left, cnt, plane, Methods, Store, X, Y, dir);
			else
				FindAllSolutions(x, y + 1, left, cnt, plane, Methods, Store, X, Y, dir);
			//���㣬ȷ���������еķ���
			left++;
			UnSetPlane(plane, x, y, i);
		}
	}

	// ��ǰλ�ò��ŷɻ� 
	if (y == 7)
		FindAllSolutions(x + 1, 0, left, cnt, plane, Methods, Store, X, Y, dir);
	else
		FindAllSolutions(x, y + 1, left, cnt, plane, Methods, Store, X, Y, dir);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ÿһ�ε�����ŷ�������ȷ�����ܷɻ��ķֲ�
  ���������
  �� �� ֵ��
  ˵    ���������Ѿ�֪�����п��ܷ����ķֲ������ֻ��Ҫÿ�ε㿪һ����������ų����Ϳ��Եõ����Ψһ�ķ���
			Ϊ�˽Ͽ���ų��Լ��ϸߵĵ÷��ʣ�ÿ�ζ��㿪���ŵķ����������ĵ÷ָ�����ӽ�0.94
***************************************************************************/
int GetPosition(int& Xn, int& Yn, int &last_x, int &last_y, int discovered[Size][Size], int plane[Size][Size], int*** Methods, bool Unavailable[N])
{
	int avai_cnt = 0; // ���㵱ǰ���ܳ�Ϊ�𰸵ķ����� 
	int last_method_cnt = 0; // ��¼��ǰ���з����е����һ��
	// ���ڴ���ǰ���з���ֻʣ��һ�ֵ���� 
	if (last_x != 0 && last_y != 0) { // ������ǵ�һ�β�ѯ 
		int tmp = CheckInfo(discovered, plane, last_x, last_y); // �õ���һ�β�ѯ��λ�õ�ֵ 
		if (tmp != -1) {
			for (int i = 1; i <= M; i++) {
				if (Methods[i][last_x][last_y] != tmp)
					Unavailable[i] = true;
				if (!Unavailable[i]) { // ���з��� 
					avai_cnt++;
					last_method_cnt = i;
				}
			}
		}
	}
	else
		avai_cnt = M; // ��һ�β�ѯ

	if (avai_cnt == 0) {
		cout << "�������û��ȫ,�����ˣ�����";//Ӧ�ò����ܣ�����д���Է���һ
		exit(0);
	}

	if (avai_cnt != 1) { // ����δ���� 
		int position[Size][Size] = { 0 }; // ��¼���� 
		int total[Size][Size] = { 0 };    // ��¼����
		for (int i = 1; i <= M; i++) {
			if (!Unavailable[i]) {
				for (int x = 0; x < Size; x++) {
					for (int y = 0; y < Size; y++) {
						if (Methods[i][x][y] != 4) {
							position[x][y] += (Methods[i][x][y] != 0);
							total[x][y]++;
						}
						else {
							position[x][y] += 2;
							total[x][y] += 2;
						}
					}
				}
			}
		}
		double cur_rate = 0;
		int cur_x = 0, cur_y = 0; // ��ǰѰ�ҵ������Ų�ѯλ�� 
		for (int x = 0; x < Size; x++) {
			for (int y = 0; y < Size; y++) {
				if (CheckInfo(discovered, plane, x, y) == -1) { /// ���λ�û�δ����ѯ�� 
					double rate = (double)position[x][y] / total[x][y];
					if (fabs(rate - 0.94) < fabs(cur_rate - 0.94)) {
						cur_rate = rate;
						cur_x = x;
						cur_y = y;
					}
					if (fabs(rate - 1) < 1e-6) {//�������ֱ��ȷ������Ȼ���
						cur_x = x;
						cur_y = y;
						break;
					}
				}
			}
		}
		last_x = cur_x;
		Xn = last_x;
		last_y = cur_y;
		Yn = last_y;
		return N;
	}

	else
		return last_method_cnt;
	return -1;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ܰ���Ϸ���̵�ʵ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int console_ai_game_progress(cmd_tcp_socket &client, CONSOLE_GRAPHICS_INFO * pCGI, const BLOCK_DISPLAY_INFO bdi[],
	int plane[][Size], int score, int*** Methods, int*** Store)
{
	int discovered[Size][Size] = { 0 };
	for (int i = 0; i < Size; i++)
		for (int j = 0; j < Size; j++)
			plane[i][j] = 0;

	int last_x = 0, last_y = 0; // ��¼��һ�β�ѯ��λ�� 
	int Xn = 0, Yn = 0;
	bool Unavailable[N] = { false }; // �ж�ĳ�ַ��� ��ǰ�Ƿ񻹿��ܳ�Ϊ��

	char row, head_row, tail_row;
	int col, head_col = 0, tail_col = 0;
	bool recv_startgame = false;

	string spack;
	if (client.get_gameprogress_string(spack) < 0)
		return -1;
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
			int n = GetPosition(Xn, Yn, last_x, last_y, discovered, plane, Methods, Unavailable);
			if (n == N) { //δȷ��Ψһ�ķ���
				row = Xn + 'A', col = Yn;
				client.send_coordinate(row, col);
				discovered[Xn][Yn] = 1;//��ʾ�˷����Ѿ��������������ٷ�������۷�����
				if (client.get_gameprogress_string(spack) < 0)
					return -1;
				gotoxy(20, 1);
				cout << spack << "    ";
				ai_single_deal(spack, score, pCGI, bdi, plane, Xn, Yn);
				if (spack == "StartGame")
					recv_startgame = true;
				if (!recv_startgame)
					return -1;
				Sleep(Delay);
				output_top_info(pCGI, score);//ʵʱ���µ÷�
			}
			else { //�ҵ�Ψһ����������ֱ��˫���귨ȷ��
				for (int i = 1; i <= 3; i++) {
					FindOnly(Store, n, head_row, tail_row, head_col, tail_col, i);
					//���õ������û����Ļ�ͷ���ˣ��Ի�ȡ��ߵ÷�
					for (int j = 0; j <= 4; j++) {
						for (int k = 0; k <= 4; k++) {
							int xn = Store[n][i][1] + j, yn = Store[n][i][2] + k; // ��ǰ������� 
							if (SwitchPlane(Store[n][i][3], j, k) > 0 && discovered[xn][yn] == 0 &&
								(xn == int(head_row - 'A') && yn == head_col))
							{
								row = xn + 'A', col = yn;
								client.send_coordinate(row, col);

								discovered[xn][yn] = 1;//��ʾ�˷����Ѿ��������������ٷ�������۷�����

								if (client.get_gameprogress_string(spack) < 0)
									return -1;
								gotoxy(20, 1);
								cout << spack << "    ";
								ai_single_deal(spack, score, pCGI, bdi, plane, xn, yn);
								if (spack == "StartGame")
									recv_startgame = true;
								if (!recv_startgame)
									return -1;
								Sleep(Delay);
								output_top_info(pCGI, score);//ʵʱ���µ÷�
							}
						}
					}
					//����˫������о�ɱ
					client.send_plane_coordinates(head_row, head_col, tail_row, tail_col);
					if (client.get_gameprogress_string(spack) < 0)
						return -1;
					gotoxy(20, 1);
					cout << spack << "    ";
					ai_double_deal(spack, score, pCGI, bdi, plane, head_row, tail_row, head_col, tail_col, Methods, n);
					if (spack == "StartGame")
						recv_startgame = true;
					if (!recv_startgame)
						return -1;
					if (spack == "GameOver") {//��Ϸ����Ҳ�����˸��µ÷������
						CalScore(int(head_row - 'A'), head_col, int(tail_row - 'A'), tail_col, plane, score);
						paint_whole__plane(int(head_row - 'A'), head_col, int(tail_row - 'A'), tail_col, pCGI, bdi, plane, 3);
						output_top_info(pCGI, score);
						gotoxy(0, 2);
						cout << "����GameID:" << client.get_gameid() << "  ���ε÷�:" << client.get_score();
						return 0;
					}
					Sleep(Delay);
					output_top_info(pCGI, score);
				}
			}
		}
	}
}