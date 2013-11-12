#pragma once
#ifndef ASM_H
#define ASM_H

class WinSMARTS;

typedef void (__stdcall * TaskProc) (WinSMARTS *); // pointer to __stdcall function

typedef void* contextHnd;

extern "C" { contextHnd __cdecl newTask(TaskProc fn, void* fnParam, char* stack, TaskProc ret, void* retParam); }
extern "C" { void __cdecl contextSwitch(contextHnd* oldContext, contextHnd newContext); }

#endif // ASM_H