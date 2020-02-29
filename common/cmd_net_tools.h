/* 1853790 庄镇华 数强 */
#pragma once
#include <Winsock2.h>
#include <string>
using namespace std;

#define SENDBUF_LEN		1024
#define RECVBUF_LEN		1024

#define ID_MAXLEN		68

class cmd_tcp_socket {
protected:
	SOCKET sockid;
	char sendbuf[SENDBUF_LEN];
	char recvbuf[RECVBUF_LEN];
	int  sendbuf_len;
	int  recvbuf_len;

	int  get_full_packet(string &pack);		//内部函数，从接收到的数据中取一个完整包（已实现）
	int  get_send_packet_len() const;		//内部函数，取一个完整包的长度（已实现）

	int  debug_switch;						//调试开关，设为true则打印错误提示信息，否则只返回，由调用者处理
	int  score;
	char gameid[ID_MAXLEN];
public:
	cmd_tcp_socket();						//完成网络应用的初始化（已实现）
	~cmd_tcp_socket();						//完成网络应用退出时的收尾工作（已实现）
	int create();							//创建网络id（已实现）
	int connect();							//向服务器发起连接（已实现）
	int close();							//关闭连接（已实现）
	int recv_from_server(string &spack);	//从Server中取数据（已实现）
	int send_to_server();					//将数据发送给Server（已实现）

	int packet_dump(const char *prompt, const char *buffer, const int buflen) const; //将给定的字符串打印成hex格式（已实现）

	int get_security_string(string &s);		//从Server端发送来的数据中取40字节的异或加密串（已实现）
	int tmake_register_string(char *send_regstr, const char *no, const char *password, const char *security_str); //用学号、密码、异或串生成要发送的认证串
	int send_register_string(const char *reg_str);		//向Server端发送认证串（加密后的用户名和密码，已实现）
	int send_coordinate(const char row, const int col);	//向Server端发送单坐标（已实现）
	int send_plane_coordinates(const char head_row, const int head_col, const char tail_row, const int tail_col);	//向Server端发送双坐标（已实现）
	void set_debug_switch(bool on_off);		//打开debug开关（已实现，true则打开，会打印出错及提示信息，false则只返回，需要根据返回值判断错误）
	int   get_score() const;
	const char *get_gameid() const;


	/* 最后两个函数需要实现 */
	int get_gameprogress_string(string &s);	//从Server端取游戏反馈信息（从StartGame到GameOver，未实现完整，需自行进一步处理）
	int make_register_string(char *send_regstr, const char *stu_no, const char *stu_password, const char *security_str, const char *secure_crt = "*"); //用学号、密码、异或串生成要发送的认证串，需实现
};
