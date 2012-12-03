#include "stdafx.h"
#include "WinSMARTS.h"
using namespace std;

void *ac,*bc,*mc;
#define STACK_SIZE 65536
void a()
{
	for(int i=0; i<10; ++i)
	{
		cout << 'a' << i << endl;
		contextSwitch(&ac,bc);
	}
}
void b()
{
	for(int i=0; i<10; ++i)
	{
		cout << 'b' << i << endl;
		contextSwitch(&bc,ac);
	}
}
void e()
{
	cout << "e\n";
	contextSwitch(&ac,mc);
}

int main()
{
	//char s1[STACK_SIZE], s2[STACK_SIZE];
	//ac=newTask(a,s1+STACK_SIZE,e);
	//bc=newTask(b,s2+STACK_SIZE,NULL);
	//contextSwitch(&mc,ac);
	while(1) ;
	return 0;
}