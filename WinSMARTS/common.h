#pragma once
#ifndef COMMON_H
#define COMMON_H

typedef void (* taskProc)(void *);

typedef void* taskAsm;
extern "C" { taskAsm newTask(taskProc fn, void* fnParam, char* stack, taskProc ret, void* retParam); }
extern "C" { void contextSwitch(taskAsm* oldContext, taskAsm newContext); }

#endif // COMMON_H