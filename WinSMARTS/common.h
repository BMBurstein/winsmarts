#pragma once
#ifndef COMMON_H
#define COMMON_H

class WinSMARTS;

typedef void (__stdcall * TaskProc) (WinSMARTS *);

typedef void* TaskAsm;
extern "C" { TaskAsm __cdecl newTask(TaskProc fn, void* fnParam, char* stack, TaskProc ret, void* retParam); }
extern "C" { void __cdecl contextSwitch(TaskAsm* oldContext, TaskAsm newContext); }

#endif // COMMON_H