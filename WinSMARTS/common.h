#pragma once
#ifndef COMMON_H
#define COMMON_H

class WinSMARTS;

typedef void (__stdcall * TaskProc) (WinSMARTS *); //??

typedef void* TaskObj;

extern "C" { TaskObj __cdecl newTask(TaskProc fn, void* fnParam, char* stack, TaskProc ret, void* retParam); }
extern "C" { void __cdecl contextSwitch(TaskObj* oldContext, TaskObj newContext); }

#endif // COMMON_H