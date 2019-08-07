#ifndef NLL_H__
#define NLL_H__

#include <vector>
#include "Minuit2/FCNBase.h"

class dataset;
class pdf;

class nll: public ROOT::Minuit2::FCNBase
{
	public:
		nll(pdf * p, dataset * d);
		virtual ~nll();
		virtual double operator()(const std::vector<double> & par) const;
		virtual double Up() const { return 0.5; }
		dataset * target_data() { return m_data; }
		pdf * target_pdf() { return m_pdf; }

	protected:
		dataset * m_data;
		pdf * m_pdf;
};

#endif
