#pragma once
#ifndef SCHEDALGO_H
#define SCHEDALGO_H

class schedAlgo
{
public:
	virtual int schedule() const = 0;
	WinSMARTS* smarts;
	virtual schedAlgo* clone() const = 0; // http://www.parashift.com/c++-faq-lite/virtual-ctors.html
	virtual ~schedAlgo() { }
};

#endif // SCHEDALGO_H