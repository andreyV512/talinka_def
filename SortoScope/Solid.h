#ifndef __SOLID_H__
#define __SOLID_H__
class SolidGroup
{
	void *hLib;
	void *proc;
	char *server_addr;
public:
	static const unsigned error_open_socket = -1;    //не открыл tcp socket
	static const unsigned sortoscope_not_reply = -2; //не отвечает
	static const unsigned error_recv = -3; //нет ответа
	static const unsigned sortoscope_not_connect = -4; //нет подключения к серверу
	SolidGroup();
	~SolidGroup();
	bool Init(char *path);
	void SetAddr(char *server_addr);
	void Destroy();
	unsigned Get();
};
#endif