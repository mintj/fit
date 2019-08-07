#ifndef CHI2_H__
#define CHI2_H__

#include <vector>
#include <string>
#include "Minuit2/FCNBase.h"
#include "dataset.h"
#include "function_base.h"

class chi2: public ROOT::Minuit2::FCNBase
{
	public:
		chi2(function_base & f, dataset & d);
		virtual ~chi2();
		virtual double Up() const { return 1; }
		virtual double operator()(const std::vector<double> & par) const;
		function_base & target_func() { return m_func; }
		dataset & target_data() { return m_data; }

	protected:
		function_base & m_func;
		dataset & m_data;
};

#endif
