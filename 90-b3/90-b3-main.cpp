/* 1853790 庄镇华 数强 */
#include "90-b3.h"
#include "../common/cmd_console_tools.h"
#include "../common/cmd_hdc_tools.h"
using namespace std;

struct file_position {
	char filename[filename_maxl];
	int position;
};

/***************************************************************************
  函数名称：
  功    能：绘制彩圆
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void painting(void)
{
	for (int i = 0; i < 2; i++) {
		int x, y;
		x = cols * 20;
		for (y = 0; y < lines * 20; y += 30) {
			hdc_circle(x, y, 20, 0, 2 + rand() % 5, RGB(rand() % 255, rand() % 255, rand() % 255)); //i%2使空心实心交替
		}
	}
	for (int i = 0; i < 2; i++) {
		int x, y;
		x = 20;
		for (y = lines * 40; y >= lines * 20; y -= 30) {
			hdc_circle(x, y, 20, 0, 2 + rand() % 5, RGB(rand() % 255, rand() % 255, rand() % 255)); //i%2使空心实心交替
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：标准化小说名称
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void standard(char file_name[][filename_maxl], int cnt)
{
	for (int i = 0; i < cnt; i++) {
		int length = (int)strlen(file_name[i]);
		file_name[i][length - 4] = '\0';
		strcat(file_name[i], "    ");
		if (length >= cols - 3) {
			int sum = 0;
			for (int j = 0; j < cols - 3; j++)
				if ((int)file_name[i][j] < 0)
					sum++;
			if (sum % 2 == 0) {
				file_name[i][cols - 3] = '\0';
				strcat(file_name[i], "...");
			}
			else {
				file_name[i][cols - 4] = '\0';
				strcat(file_name[i], " ...");
			}
		}
		else {
			for (int j = length; j < cols; j++)
				file_name[i][j] = ' ';
			file_name[i][cols] = '\0';
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：绘制边框与信息栏
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void mood_border()
{
	//绘制边框
	border_info border0 = { base_y,base_x,lines,cols / 2 + 1,0,0 };
	draw_border(border0);
	border_info mood_border0 = { base_y + lines + 1,base_x,7,cols / 2 + 1,0,0 };
	draw_border(mood_border0);
	const char *p[] = { "功能说明~~~上下箭头键:上下移动一行",
					"Pageup/down:上下翻动一页",
					"S(s)键:阅读进度跳转",
					"N(n)键:结束本次阅读",
					"Q(q)键:完全退出阅读",
					"F(f)键:阅读格式转变",
	};
	setcolor(15, 6);
	gotoxy(base_x + 2, base_y + 2 + lines);
	cout << "当前进度~~~  0.00%";
	for (int i = 0; i < 6; i++) {
		gotoxy(base_x + 2, base_y + lines + 3 + i);
		cout << p[i];
	}
	setcolor();
}

/***************************************************************************
  函数名称：
  功    能：输出小说文本
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void out_book(char **book, int qi, int zh)
{
	for (int i = qi; i < zh; i++) {
		gotoxy(base_x + 2, base_y + 1 + i - qi);
		cout << book[i];
		for (int j = strlen(book[i]); j < cols; j++)
			cout << " ";
	}
}

/***************************************************************************
  函数名称：
  功    能：输入小说文本
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int read_book(ifstream& fnovel, char **&book, int mode)
{
	//清空标志位，防止出错
	if (fnovel.fail()) {
		fnovel.clear();
	}

	//统计文件字节数
	int size;
	fnovel.seekg(-1, ios::end);
	size = (int)fnovel.tellg();
	fnovel.seekg(0, ios::beg);

	int line_num = 0, point = 0, totle_num = size / cols + 1;
	bool last_endl = 0;
	book = (char **)malloc((size / cols + 1) * 4);

	if (book == NULL) {
		gotoxy(0, 0);
		cout << "申请内存错误";
		exit(0);
	}
	for (; fnovel.peek() != EOF; line_num++) {
		if (line_num == totle_num - 1) {
			book = (char **)realloc(book, (totle_num + 10) * 4);
			if (book == NULL) {
				gotoxy(0, 0);
				cout << "申请内存错误";
				exit(0);
			}
			totle_num += 10;
		}
		book[line_num] = (char *)malloc(cols + 2);
		if (book[line_num] == NULL) {
			gotoxy(0, 0);
			cout << "申请内存错误";
			exit(0);
		}
		int i = 0, double_num = 0;
		if ((fnovel.peek() == '\t') || (last_endl && fnovel.peek() != ' ' && fnovel.peek() != '\t' && mode == 1)) {
			book[line_num][0] = ' ';
			book[line_num][1] = ' ';
			book[line_num][2] = ' ';
			book[line_num][3] = ' ';
			i = 4;
			last_endl = 0;
		}
		for (; i < cols - 1; i++) {
			char cha;
			cha = fnovel.get();
			if (cha == '\n') {
				if (mode == 1) {
					book[line_num][i] = 0;
					last_endl = 1;
					break;
				}
				else if (mode == 2) {
					int p;
					if ((p = fnovel.get()) != ' '&&p != '\t' && (p != 0xA1 || fnovel.peek() != 0xA1)) {
						fnovel.seekg(-1, SEEK_CUR);
						i--;
						continue;
					}
					else {
						fnovel.seekg(-1, SEEK_CUR);
						book[line_num][i] = 0;
						break;
					}
				}
			}
			if (cha < 0 || (double_num % 2 == 1 && book[line_num][i - 1] < 0))
				double_num++;
			book[line_num][i] = cha;
		}
		if (i == cols - 1)
			if (double_num % 2 == 1)
				book[line_num][i++] = fnovel.get();
		book[line_num][i] = '\0';
	}
	book[line_num - 1][0] = '\0';
	return line_num;
}

/***************************************************************************
  函数名称：
  功    能：读取位置文件
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int read_position(const char savingpath[], char FileName[], file_position *file_position0, int cnt)
{
	int size = -1;
	char ch;
	char filename[filename_maxl];
	char num[100];
	int position;
	ifstream frsaving(savingpath, ios::in);
	if (frsaving.is_open() == 0) {
		gotoxy(0, 0);
		cout << "打开" << savingpath << "文件错误";
		exit(0);
	}
	while (1) {
		while (frsaving.get() != '[' && (!frsaving.eof()))
			;
		if (frsaving.eof())
			break;
		int i = 0;
		while ((ch = frsaving.get()) != ']' && (!frsaving.eof()) && i < filename_maxl - 1) {
			filename[i++] = ch;
		}
		if (frsaving.eof())
			break;
		filename[i] = 0;

		while ((((ch = frsaving.get()) < '0') || (ch > '9')) && (!frsaving.eof()))
			;
		if (frsaving.eof())
			break;
		num[0] = ch;

		//排除负数
		frsaving.seekg(-2, SEEK_CUR);
		if (frsaving.get() == '-' && (!frsaving.eof()))
			continue;
		frsaving.get();

		int ii = 1;
		while ((ch = frsaving.get()) >= '0' && (ch) <= '9' && (!frsaving.eof()) && ii < filename_maxl - 1) {
			num[ii++] = ch;
		}
		if (frsaving.eof())
			break;
		num[ii] = 0;
		position = atoi(num);

		for (int i = 0; i < cnt; i++) {
			if (strcmp(file_position0[i].filename, filename) == 0) {
				file_position0[i].position = position;
			}
		}

		if (strcmp(FileName, filename) == 0) {
			size = position;
		}

	}
	frsaving.close();
	return size;
}

/***************************************************************************
  函数名称：
  功    能：更新位置文件
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void write_position(const char savingpath[], char FileName[], file_position *file_position0, int position, int cnt)
{
	ofstream fwsaving(savingpath, ios::out);
	for (int i = 0; i < cnt; i++) {
		if (strcmp(file_position0[i].filename, FileName) == 0)
			file_position0[i].position = position;
		fwsaving << "[" << file_position0[i].filename << "]" << " " << file_position0[i].position << endl;
	}
	fwsaving.close();
}

/***************************************************************************
  函数名称：
  功    能：行数转换字节数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int line2byte(char **book, int o_qi)
{
	int bbyte = 0;
	for (int i = 0; i < o_qi; i++) {
		bbyte += strlen(book[i]);
	}
	return bbyte;
}

/***************************************************************************
  函数名称：
  功    能：字节数转换行数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int byte2line(char **book, int byte)
{
	int lline = 0;
	for (lline = 0; byte >= 0; lline++) {
		byte -= strlen(book[lline]);
	}
	return lline;
}

/***************************************************************************
  函数名称：
  功    能：文本阅读按键选择(总体实现)
  输入参数：
  返 回 值：
  说    明：打印时每2字节一组，否则会乱
***************************************************************************/
int key_choose(ifstream& fnovel, char **&book, int linenum, int mood, char filename[], const char savingpath[], file_position *file_position0, int cnt)
{
	int fflag = -1;//退出时使用
	int o_qi = 0, o_zh = (linenum > lines ? lines : linenum);
	double process = 0;

	setcursor(CURSOR_INVISIBLE);
	setcolor(15, 6);

	int bytesize = read_position(savingpath, filename, file_position0, cnt);
	if (bytesize == -1) {
		bytesize = 0;
		write_position(savingpath, filename, file_position0, 0, cnt);
	}

	//初始打印
	for (int i = 0; i < (linenum > lines ? lines : linenum); i++) {
		gotoxy(base_x + 2, base_y + 1 + i);
		cout << book[i];
	}

	//不足20行则简单特殊处理
	int flag = 0;
	if (linenum < lines)
		flag = 1;

	if (flag == 0) {
		o_qi = byte2line(book, bytesize) - 1;
		o_zh = (o_qi + lines > linenum ? linenum : o_qi + lines);
		if (o_qi > linenum || o_qi < 0) {
			o_qi = 0;
			o_zh = lines;
		}
		process = double(o_zh) / linenum;
		out_book(book, o_qi, o_zh);
		if (o_zh - o_qi < lines) {
			for (int i = o_zh - o_qi; i < lines; i++) {
				gotoxy(base_x + 2, base_y + 1 + i);
				for (int j = 0; j < cols; j++)
					cout << " ";
			}
		}
	}

	//进行选择  real_order为选中的字符数组真实存储行数顺序，而screen_order为选中的屏幕行数顺序
	enable_mouse();
	int X = 0, Y = 0;
	int ret, maction;
	int keycode1, keycode2;
	int loop = 1;
	int real_order = 0, screen_order = 0;
	double percent = 0;
	while (loop == 1) {
		percent = double(o_zh) / linenum;
		gotoxy(base_x + 14, base_y + 22);
		cout << setiosflags(ios::fixed) << setiosflags(ios::right) << setprecision(2) << setw(6) << percent * 100 << "%";
		ret = read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		if (ret == CCT_KEYBOARD_EVENT) {
			switch (keycode1) {
			case 70:
			case 102://F(f)键
			{
				int bbyte = line2byte(book, o_qi);
				for (int i = 0; i < linenum; i++) {
					free(book[i]);
				}
				free(book);
				linenum = read_book(fnovel, book, 3 - mood);
				mood = 3 - mood;
				if (linenum < lines) {
					flag = 1;
					for (int i = linenum; i < lines; i++) {
						gotoxy(base_x + 2, base_y + 1 + i);
						for (int j = 0; j < cols; j++)
							cout << " ";
					}
				}
				o_qi = byte2line(book, bbyte) - 1;
				o_zh = (o_qi + lines > linenum ? linenum : o_qi + lines);
				out_book(book, o_qi, o_zh);
				if (o_zh - o_qi < lines) {
					for (int i = o_zh - o_qi; i < lines; i++) {
						gotoxy(base_x + 2, base_y + 1 + i);
						for (int j = 0; j < cols; j++)
							cout << " ";
					}
				}
				break;
			}
			case 83:
			case 115://S(s)键
			{
				gotoxy(base_x + 2, base_y + 22);
				cout << "请输入进度~~~   ";
				setcursor(CURSOR_VISIBLE_FULL);
				cin >> process;
				setcursor(CURSOR_INVISIBLE);
				gotoxy(base_x + 2, base_y + 22);
				cout << "当前进度~~~  0.00%        ";
				if ((process / 100 <= ((double)(lines) / linenum)) || (process > 100))
					continue;
				o_zh = int(linenum*process / 100);
				o_qi = (o_zh - lines < 0 ? 0 : o_zh - lines);
				out_book(book, o_qi, o_zh);
				if (o_zh - o_qi < lines) {
					for (int i = o_zh - o_qi; i < lines; i++) {
						gotoxy(base_x + 2, base_y + 1 + i);
						for (int j = 0; j < cols; j++)
							cout << " ";
					}
				}
				break;
			}
			case 78:
			case 110://N(n)键
			{
				int bbyte = line2byte(book, o_qi);
				write_position(savingpath, filename, file_position0, bbyte, cnt);
				for (int i = 0; i < linenum; i++) {
					free(book[i]);
				}
				free(book);
				fnovel.close();
				fflag = 1;
				break;
			}
			case 81:
			case 113://Q(q)键
			{
				int bbyte = line2byte(book, o_qi);
				write_position(savingpath, filename, file_position0, bbyte, cnt);
				for (int i = 0; i < linenum; i++) {
					free(book[i]);
				}
				free(book);
				fnovel.close();
				fflag = 0;
				break;
			}
			case 224:
				switch (keycode2) {
				case 72://上箭头键
				{
					if (flag == 0) {
						int flag1 = (o_qi - 1 < 0 ? 1 : 0);
						o_qi = (flag1 ? 0 : o_qi - 1);
						o_zh = (flag1 ? lines : o_zh - 1);
						out_book(book, o_qi, o_zh);
					}
					break;
				}
				case 80://下箭头键
				{
					if (flag == 0) {
						if (o_zh + 1 > linenum)
							continue;
						o_qi++;
						o_zh++;
						out_book(book, o_qi, o_zh);
					}
					break;
				}
				case 73://上翻页键
				{
					if (flag == 0) {
						int flag1 = (o_qi - lines + 1 < 0 ? 1 : 0);
						o_qi = (flag1 ? 0 : o_qi - lines + 1);
						o_zh = (flag1 ? lines : o_zh - lines + 1);
						out_book(book, o_qi, o_zh);
					}
					break;
				}
				case 81://下翻页键
				{
					if (flag == 0) {
						if (o_zh + lines - 1 > linenum)
							continue;
						o_qi += lines - 1;
						o_zh += lines - 1;
						out_book(book, o_qi, o_zh);
					}
					break;
				}
				default:
					break;
				}
			}
			if (fflag == 1 || fflag == 0)
				break;
		}
	}
	return fflag;
}

int main()
{
	//初始化窗口，设置时间种子
	srand((unsigned int)time(0));
	hdc_init(6, 0, COLS, LINES);

	//在窗口框架外绘制彩圆
	painting();

	//取book文件夹下所有.txt文件
	struct _finddata_t file;
	int k;
	long HANDLE;
	char filename[400] = { 0 };
	char saving[400] = { 0 };
	saving[0] = '\0';
	filename[0] = '\0';
	strcat(filename, "./book");
	strcat(filename, "/*.txt");
	k = HANDLE = _findfirst(filename, &file);
	char file_name[filenum_maxl][filename_maxl];
	char FileName[filenum_maxl][filename_maxl];
	int k_cnt = 0;
	while (k != -1 && k_cnt < filenum_maxl) {
		if (file.attrib == _A_SUBDIR) {
			k = _findnext(HANDLE, &file);
			continue;
		}
		strcpy(file_name[k_cnt++], file.name);
		file_name[k_cnt - 1][filename_maxl - 1] = '\0';
		k = _findnext(HANDLE, &file);
	}
	_findclose(HANDLE);

	if (k_cnt == 0) {
		gotoxy(0, 0);
		cout << "打开书籍错误";
		return 0;
	}

	//备份
	for (int i = 0; i < k_cnt; i++) {
		strcpy(FileName[i], file_name[i]);
	}
	standard(file_name, k_cnt);

	//创建文件阅读位置信息
	file_position file_position0[60];
	for (int i = 0; i < k_cnt; i++) {
		strcpy(file_position0[i].filename, FileName[i]);
		file_position0[i].position = 0;
	}

	int iii = 0;
	while (1) {
		//绘制框架
		mood_border();
		if (iii != 0)
			painting();
		iii++;

		//进行选择
		int choose = pop_menu(file_name, k_cnt);
		filename[0] = '\0';
		saving[0] = '\0';
		strcat(filename, "./book/");
		strcat(saving, filename);
		strcat(saving, "position.saving");
		strcat(filename, FileName[choose]);

		//读取位置保存文件，若没有则创建空白文件
		ifstream  fsaving(saving, ios::in);
		if (fsaving.is_open() == 0) {
			fsaving.close();
			ofstream  fsaving(saving, ios::out);
			fsaving.close();
		}
		fsaving.close();

		//打开文件
		ifstream  fnovel(filename, ios::in);
		if (fnovel.is_open() == 0) {
			cout << "打开" << filename << "文件错误";
			return 0;
		}

		//绘制框架
		mood_border();

		//存储书籍
		char **book = NULL;
		int linenum = read_book(fnovel, book, 1);

		//过程实现
		int cchoice = key_choose(fnovel, book, linenum, 1, FileName[choose], saving, file_position0, k_cnt);

		if (cchoice == 1)
			continue;
		else if (cchoice == 0)
			break;
	}
	setcolor(15, 0);
	gotoxy(0, base_y + lines + 10);
	return 0;
}
