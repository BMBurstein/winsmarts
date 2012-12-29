#pragma once
#ifndef SCHEDALGO_H
#define SCHEDALGO_H

class schedAlgo
{
public:
	virtual int schedule() const = 0;
	WinSMARTS* smarts;
	virtual ~schedAlgo() { }
};

class RR : public schedAlgo
{
	int schedule() const;
};


#endif // SCHEDALGO_H