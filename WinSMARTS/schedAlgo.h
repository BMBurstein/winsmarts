#pragma once
#ifndef SCHEDALGO_H
#define SCHEDALGO_H

class schedAlgo
{
public:
	virtual void operator()(WinSMARTS*) = 0;
};

#endif // SCHEDALGO_H