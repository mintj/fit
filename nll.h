#ifndef NLL_H__
#define NLL_H__

#include <vector>
#include <string>
#include "Minuit2/FCNBase.h"
#include "dataset.h"
#include "pdf_base.h"

class nll: public ROOT::Minuit2::FCNBase 
{
	public:
		nll(pdf_base & p, dataset & d);
		virtual ~nll();
		virtual double Up() const { return 0.5; }
		virtual double operator()(const std::vector<double> & par) const;
		pdf_base & target_pdf() { return m_pdf; }
		dataset & target_data() { return m_data; }

	protected:
		pdf_base & m_pdf;
		dataset & m_data;
};

#endif
