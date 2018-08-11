#define WIN32_LEAN_AND_MEAN
#include "SortoScope.h"
#include <Windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdint.h>
#define dprint printf
#pragma comment(lib, "Ws2_32.lib")
struct StructSortoskopResults
{
	int16_t number;       // ����� �����
	int16_t count;        // ���������� ���������
	int16_t min_value;    // ����������� �������� ���������� �� ����� ���������
	int16_t max_value;    // ������������ �������� ���������� �� ����� ���������
	int16_t avr_value;    // ������� �������� ���������� �� ����� ���������
	int16_t freq;         // �������
	int16_t empty;        // ����� �����
	int16_t porog[6];     // ������ ��������
	int16_t value[2000];      // ���������� ���������
};

struct InitTcp
{
	InitTcp();
	~InitTcp();
};
InitTcp::InitTcp()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

InitTcp::~InitTcp()
{
	WSACleanup();
}

unsigned getSolidGroup(char *addr_serv)
{
	InitTcp initTcp;
	StructSortoskopResults sortoskopResult;
	struct sockaddr_in addr;
	int sock;
	int prevNumber = 0;
	unsigned GP = 'X';
	//��������� �����
	sock = socket(AF_INET,SOCK_STREAM,0);
	if (sock < 0) {
		dprint("error open socket\n");
		GP = error_open_socket;
	}
	//������������� ��� ������
	addr.sin_family = AF_INET;
	//���� ����������
	addr.sin_port = htons(0x3340);
	//Ip ����������
	addr.sin_addr.s_addr = inet_addr(addr_serv);//"192.168.0.10");
	if(::connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		GP = sortoscope_not_connect;
		printf("sortoscope not connect\n");
	}
	else {
		//��������� �������
		uint32_t cmd = 0x33200003;
		//���������� ��� �������� ������
		uint16_t buf[512];
		//��������� ��� ������� ���������� �����������
		uint8_t *res = (uint8_t *)&sortoskopResult;
		//���������� ������� ����������
		int err = send(sock,(char *)&cmd,sizeof(cmd),0);
		Sleep(1000);
		//���� ������ ������� ���������� �������� ����� ����������
		if (err==4) {
			for (int i = 0; i <4; i++) {
				err = recv(sock,(char *)&buf,1024,0);
				// ���� �� ���� ������� 1024 ����� ����� ������ � ��
				if (err!=1024) {
					dprint("error recv\n");
					GP = error_recv;
				} else {
					//�������� � ���������
					memcpy(&res[i * 1006], &buf[2], 1006);
					break;
				}
			}
			//��������� ��������� ���������
			if ((sortoskopResult.number!=prevNumber) && sortoskopResult.number!=0) {
				if (sortoskopResult.avr_value > sortoskopResult.porog[0]) GP = 'K';
				if ((sortoskopResult.avr_value < sortoskopResult.porog[0]) && (sortoskopResult.avr_value > sortoskopResult.porog[1])) GP = 'K';
				if ((sortoskopResult.avr_value < sortoskopResult.porog[1]) && (sortoskopResult.avr_value > sortoskopResult.porog[2])) GP = 'E';
				if ((sortoskopResult.avr_value < sortoskopResult.porog[2]) && (sortoskopResult.avr_value > sortoskopResult.porog[3])) GP = 'L';
				if ((sortoskopResult.avr_value < sortoskopResult.porog[3]) && (sortoskopResult.avr_value > sortoskopResult.porog[4])) GP = 'M';
				if ((sortoskopResult.avr_value < sortoskopResult.porog[4]) && (sortoskopResult.avr_value > sortoskopResult.porog[5])) GP = 'P';
			} 
		}
	}
	closesocket(sock);
	return GP;
}
#ifndef MAKEDLL
int main()
{
	for(int i = 0; i < 100; ++i)
	{
		char res =  getSolidGroup();
		printf("%d %c\n", i, res);
		Sleep(100);
	}
	getchar();
	return 0;
}
#endif
