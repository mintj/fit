#ifndef NLL_H__
#define NLL_H__

#include <vector>
#include <string>
#include "Minuit2/FCNBase.h"
#include "data.h"
#include "function_base.h"
#include "pdf_base.h"

class nll: public ROOT::Minuit2::FCNBase 
{
	public:
		nll(pdf_base & p, data & d);
		virtual ~nll();
		virtual double Up() const { return 0.5; }
		virtual double operator()(const std::vector<double> & par) const;
		pdf_base & target_pdf() { return m_pdf; }
		data & target_data() { return m_data; }

	protected:
		pdf_base & m_pdf;
		data & m_data;
};

#endif
