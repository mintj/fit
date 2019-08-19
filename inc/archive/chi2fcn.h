#ifndef CHI2FCN_H__
#define CHI2FCN_H__

#include <vector>
#include <map>
#include "TMath.h"
#include "fcn.h"

class datahist;
class variable;

class chi2fcn: public fcn
{
	public:
		chi2fcn() = default;
		chi2fcn(pdf * p, datahist * d);
		virtual ~chi2fcn();
		
		void add(pdf * p, datahist * d);
		
		virtual double operator()(const std::vector<double> & par) const;
		virtual double Up() const { return 1.0; }

	protected:
		void update_data(pdf * p, datahist * d);

	protected:
		mutable std::vector<std::vector<std::vector<double *>>> m_data;
};

#endif
