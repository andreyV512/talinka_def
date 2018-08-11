#pragma once

static const unsigned error_open_socket = -1;    //не открыл tcp socket
static const unsigned sortoscope_not_reply = -2; //не отвечает
static const unsigned error_recv = -3; //нет ответа
static const unsigned sortoscope_not_connect = -4; //нет подключения к серверу
unsigned getSolidGroup(char *);
