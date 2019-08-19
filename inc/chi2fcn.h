#ifndef CHI2FCN_H__
#define CHI2FCN_H__

#include <vector>
#include "absfcn.h"

class abspdf;
class datahist;

class chi2fcn: public absfcn
{
	public:
		chi2fcn() = default;
		chi2fcn(abspdf * p, datahist * d);
		virtual ~chi2fcn();
		
		void minimize(bool minos_err = false);
		
		virtual double operator()(const std::vector<double> & par) const;
		virtual double Up() const { return 1.0; }

	protected:
		bool init();

	protected:
		mutable std::vector<double> m_arr_chi2;
};

#endif
