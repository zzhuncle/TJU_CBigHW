/* 1853790 庄镇华 数强 */
#pragma once
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
int caidan(int(*p1)[9],int(*p2)[9],CONSOLE_GRAPHICS_INFO *pCGI,const int mood);
int canmove(int &begin, int(*p1)[9], int(*p2)[9], int x, int y, int to_x, int to_y, int n_r, int n_c);
int check_pos_valid(int(*p1)[9], int(*p2)[9], int x, int y, int n_r, int n_c);
void buildchess(const BLOCK_DISPLAY_INFO bdi[],CONSOLE_GRAPHICS_INFO *pCGI,int(*p1)[9], int(*p2)[9], int *p3, int lines, int cols,
	const int n_chess = 3,const int mood = 0);//没有改变*
int delchess(const BLOCK_DISPLAY_INFO bdi[], CONSOLE_GRAPHICS_INFO *pCGI, int(*p1)[9], int(*p2)[9], int x, int y, int n_r, int n_c, int mood);//没有改变*
int boardfull(int(*p)[9], int n_r, int n_c);//根据p1判断的