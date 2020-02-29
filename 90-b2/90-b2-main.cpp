/* 学号 姓名 班级 */
#include <iostream>
#include "../common/cmd_console_tools.h"
using namespace std;

int main()
{
	//重复显示5次"90-b2-main.cpp"，最长30字节
	showstr(5, 5, "90-b2-main.cpp", COLOR_HYELLOW, COLOR_HBLUE, 5, 30);

	//恢复初始颜色
	setcolor();

	return 0;
}
