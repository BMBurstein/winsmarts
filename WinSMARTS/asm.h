#pragma once
#ifndef ASM_H
#define ASM_H

class WinSMARTS;

typedef void (__stdcall * TaskProc) (WinSMARTS *); //??

typedef void* TaskObj;

extern "C" { TaskObj __cdecl newTask(TaskProc fn, void* fnParam, char* stack, TaskProc ret, void* retParam); }
extern "C" { void __cdecl contextSwitch(TaskObj* oldContext, TaskObj newContext); }

#endif // ASM_H