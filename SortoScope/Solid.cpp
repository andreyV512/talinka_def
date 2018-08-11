#include "Solid.h"
#include "SortoScopeDLL.h"

SolidGroup::SolidGroup()
	: hLib(NULL)
	, proc(NULL)
	, server_addr(NULL)
{}

SolidGroup::~SolidGroup()
{
	Destroy();
}
#pragma warning(disable: 4996)
bool SolidGroup::Init(char *path)
{
	hLib = (void *)LoadLibraryA(path);
	proc = ::GetProcAddress((HMODULE)hLib, "_GetSolidGroup@4");
	return NULL != hLib;
}

void SolidGroup::SetAddr(char *addr)
{
	if(NULL != server_addr) delete[] server_addr;
	server_addr = new char[1 + strlen(addr)];
	strcpy(server_addr, addr);
}

void SolidGroup::Destroy()
{
	if(NULL != hLib) {FreeLibrary((HMODULE)hLib); hLib = NULL;}
	if(NULL != server_addr) {delete[] server_addr; server_addr = NULL;}
}
unsigned SolidGroup::Get()
{
	return ((unsigned(WINAPI *)(char *))proc)(server_addr);
}