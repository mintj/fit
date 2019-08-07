#ifndef GAUS_FUNC_H__
#define GAUS_FUNC_H__

#include "function_base.h"

class gaus_func: public function_base
{
	public:
		gaus_func(variable & x, variable & area, variable & mean, variable & sigma);
		virtual ~gaus_func();
		virtual double evaluate(const std::vector<double> & point) const;
};

#endif
