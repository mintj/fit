#ifndef GAUSSIAN_H__
#define GAUSSIAN_H__

#include "abspdf.h"

class variable;

class gaussian: public abspdf
{
	public:
		gaussian(const char * name, variable & x, variable & m, variable & s);
		virtual ~gaussian();
		
		// implement pure method
		double evaluate(const double * x);
		double norm();
};

#endif
