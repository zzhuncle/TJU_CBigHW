/* 1853790 庄镇华 数强 */
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
//交换整型数的值
void tj_swap(int &a, int &b);
//移动彩球
int move_ball(struct move_ball_info &move);
//绘制彩球
void draw_ball(struct ball_info &ball);
//菜单函数
int caidan(int(*p1)[9], int(*p2)[9], CONSOLE_GRAPHICS_INFO *pCGI, const int mood = 1);
//判断两两之间是否可以交换
int is_exchange(int(*p1)[9], int(*p2)[9], int x, int y, int x_, int y_, int n_r, int n_c, const int mood = 1);
//新值填充
void fill(const BLOCK_DISPLAY_INFO bdi_normal[], CONSOLE_GRAPHICS_INFO *pCGI, int(*p1)[9], int(*p2)[9], int n_r, int n_c, const int mood = 1);
//下落除0
void delzero(const BLOCK_DISPLAY_INFO bdi_normal[], CONSOLE_GRAPHICS_INFO *pCGI, int(*p1)[9], int(*p2)[9], int n_r, int n_c, const int mood = 1);
//消除彩球
int delchess(int(*p1)[9], int(*p2)[9], int n_r, int n_c, const int mood = 1);
//提示可消除标记
int mark_remove(int(*p1)[9], int(*p2)[9], int n_r, int n_c, const int mood = 1);