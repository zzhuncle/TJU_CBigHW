/* 1853790 ׯ�� ��ǿ */
#pragma once
#include<fstream>
#include<cstring>
#include <ctime>
#include<iostream>
#include <iomanip>
#include <cmath>
#include <cstdio>
#include <conio.h>
#include "../common/cmd_gmw_tools.h"
#include"../common/cmd_console_tools.h"
using namespace std;

char menu(const char *menu[], const char *choice);
//������������ֵ
void tj_swap(int &a, int &b);
//�ƶ�����
int move_ball(struct move_ball_info &move);
//���Ʋ���
void draw_ball(struct ball_info &ball);
//�˵�����
int caidan(int(*p1)[9], int(*p2)[9], CONSOLE_GRAPHICS_INFO *pCGI, const int mood = 1);
//�ж�����֮���Ƿ���Խ���
int is_exchange(int(*p1)[9], int(*p2)[9], int x, int y, int x_, int y_, int n_r, int n_c, const int mood = 1);
//��ֵ���
void fill(const BLOCK_DISPLAY_INFO bdi_normal[], CONSOLE_GRAPHICS_INFO *pCGI, int(*p1)[9], int(*p2)[9], int n_r, int n_c, const int mood = 1);
//�����0
void delzero(const BLOCK_DISPLAY_INFO bdi_normal[], CONSOLE_GRAPHICS_INFO *pCGI, int(*p1)[9], int(*p2)[9], int n_r, int n_c, const int mood = 1);
//��������
int delchess(int(*p1)[9], int(*p2)[9], int n_r, int n_c, const int mood = 1);
//��ʾ���������
int mark_remove(int(*p1)[9], int(*p2)[9], int n_r, int n_c, const int mood = 1);