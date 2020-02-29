/* 1853790 庄镇华 数强 */
#pragma once
const int N = 66820;
const int M = 66816;
const int Size = 10;
const int Num = 3;
void Init3d(int**** Methods, int**** Store);
void free3d(int**** Methods, int**** Store);
void CalScore(const int row1, const int col1, const int row2, const int col2, 
	int plane[][Size], int &score);
void console_manual(const char *num, const char *psw,const int mood);
void FindAllSolutions(int x, int y, int &left, int &cnt, int plane[Size][Size], 
	int*** Methods, int*** Store, int X[4], int Y[4], int dir[4]);


