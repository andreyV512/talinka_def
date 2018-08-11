#ifndef SOLID_GROUP_H
#define SOLID_GROUP_H
#include <Windows.h>
#ifdef MAKEDLL
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif
extern "C"{
EXPORT unsigned __stdcall GetSolidGroup(char *addr);
}
#endif
