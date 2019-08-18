#ifndef GAUSSIAN_H__
#define GAUSSIAN_H__

#include "abspdf.h"
#include <functional>
#include "TF1.h"

class variable;

class gaussian: public abspdf
{
	public:
		gaussian(const char * name, variable & x, variable & m, variable & s);
		virtual ~gaussian();
		
		// implement pure method
		double evaluate(const double * x);
		double norm();

		TF1 * fff()
		{
			gaussian * pp = this;
			std::function<double(double * x, double * p)> f = [&pp](double * x, double * p)->double{return pp->evaluate(x);};
			TF1 * f1 = new TF1("f1", f, -10, 10, 0);
			return f1;
		}
};

#endif
