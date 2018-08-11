#ifndef __SOLID_H__
#define __SOLID_H__
class SolidGroup
{
	void *hLib;
	void *proc;
	char *server_addr;
public:
	static const unsigned error_open_socket = -1;    //�� ������ tcp socket
	static const unsigned sortoscope_not_reply = -2; //�� ��������
	static const unsigned error_recv = -3; //��� ������
	static const unsigned sortoscope_not_connect = -4; //��� ����������� � �������
	SolidGroup();
	~SolidGroup();
	bool Init(char *path);
	void SetAddr(char *server_addr);
	void Destroy();
	unsigned Get();
};
#endif