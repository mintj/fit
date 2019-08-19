#ifndef ABSFCN_H__
#define ABSFCN_H__

#include <vector>
#include <map>
#include "Minuit2/FCNBase.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnMinos.h"

class absdata;
class abspdf;
class variable;

class absfcn: public ROOT::Minuit2::FCNBase
{
	public:
		absfcn() = default;
		absfcn(abspdf * p, absdata * d);
		virtual ~absfcn();
		
		void add(abspdf * p, absdata * d);
		void minimize(bool minos_err = false);
		
		virtual double operator()(const std::vector<double> & par) const = 0;
		virtual double Up() const = 0;

	protected:
		std::vector<absdata *> m_datalist;
		std::vector<abspdf *> m_pdflist;
		std::vector<variable *> m_paralist;
		std::map<variable *, int> m_count;
		ROOT::Minuit2::MnMigrad * m_migrad;
		ROOT::Minuit2::MnMinos * m_minos;
		ROOT::Minuit2::FunctionMinimum * m_min;
};

#endif
