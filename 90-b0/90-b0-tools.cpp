#include"90-b0.h"
void enter()
{
	cout << endl << "按回车键显示图形...";
	while (_getch() != '\r')
		;
}
