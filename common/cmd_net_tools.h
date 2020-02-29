/* 1853790 ׯ�� ��ǿ */
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

	int  get_full_packet(string &pack);		//�ڲ��������ӽ��յ���������ȡһ������������ʵ�֣�
	int  get_send_packet_len() const;		//�ڲ�������ȡһ���������ĳ��ȣ���ʵ�֣�

	int  debug_switch;						//���Կ��أ���Ϊtrue���ӡ������ʾ��Ϣ������ֻ���أ��ɵ����ߴ���
	int  score;
	char gameid[ID_MAXLEN];
public:
	cmd_tcp_socket();						//�������Ӧ�õĳ�ʼ������ʵ�֣�
	~cmd_tcp_socket();						//�������Ӧ���˳�ʱ����β��������ʵ�֣�
	int create();							//��������id����ʵ�֣�
	int connect();							//��������������ӣ���ʵ�֣�
	int close();							//�ر����ӣ���ʵ�֣�
	int recv_from_server(string &spack);	//��Server��ȡ���ݣ���ʵ�֣�
	int send_to_server();					//�����ݷ��͸�Server����ʵ�֣�

	int packet_dump(const char *prompt, const char *buffer, const int buflen) const; //���������ַ�����ӡ��hex��ʽ����ʵ�֣�

	int get_security_string(string &s);		//��Server�˷�������������ȡ40�ֽڵ������ܴ�����ʵ�֣�
	int tmake_register_string(char *send_regstr, const char *no, const char *password, const char *security_str); //��ѧ�š����롢�������Ҫ���͵���֤��
	int send_register_string(const char *reg_str);		//��Server�˷�����֤�������ܺ���û��������룬��ʵ�֣�
	int send_coordinate(const char row, const int col);	//��Server�˷��͵����꣨��ʵ�֣�
	int send_plane_coordinates(const char head_row, const int head_col, const char tail_row, const int tail_col);	//��Server�˷���˫���꣨��ʵ�֣�
	void set_debug_switch(bool on_off);		//��debug���أ���ʵ�֣�true��򿪣����ӡ������ʾ��Ϣ��false��ֻ���أ���Ҫ���ݷ���ֵ�жϴ���
	int   get_score() const;
	const char *get_gameid() const;


	/* �������������Ҫʵ�� */
	int get_gameprogress_string(string &s);	//��Server��ȡ��Ϸ������Ϣ����StartGame��GameOver��δʵ�������������н�һ������
	int make_register_string(char *send_regstr, const char *stu_no, const char *stu_password, const char *security_str, const char *secure_crt = "*"); //��ѧ�š����롢�������Ҫ���͵���֤������ʵ��
};
