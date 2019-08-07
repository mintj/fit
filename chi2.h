#ifndef CHI2_H__
#define CHI2_H__

#include <vector>
#include "Minuit2/FCNBase.h"

class datahist;
class pdf;

class chi2: public ROOT::Minuit2::FCNBase
{
	public:
		chi2(pdf * p, datahist * d);
		virtual ~nll();
		virtual double operator()(const std::vector<double> & par) const;
		virtual double Up() const { return 0.5; }
		datahist * target_data() { return m_data; }
		pdf * target_pdf() { return m_pdf; }

	protected:
		datahist * m_data;
		pdf * m_pdf;
};

#endif
