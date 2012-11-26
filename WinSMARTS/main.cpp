#include "stdafx.h"
#include "timer.h"
using namespace std;

void test()
{
	cout << "test!" <<endl;
}

int main()
{
	setSigTimer(100, test);

	while(1) ;
	return 0;
}