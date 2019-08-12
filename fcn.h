#ifndef FCN_H__
#define FCN_H__

#include <vector>
#include <map>
#include "TMath.h"
#include "Minuit2/FCNBase.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnMinos.h"

class datahist;
class dataset;
class extpdf;
class pdf;
class variable;

class fcn: public ROOT::Minuit2::FCNBase
{
	public:
		fcn(pdf * p, dataset * d);
		fcn(const std::vector<pdf *> plist, const std::vector<dataset *> dlist);
		fcn(const std::vector<extpdf *> plist, const std::vector<datahist *> dlist);
		virtual ~fcn();
		dataset * get_data(int n) { return m_datalist[n]; }
		std::vector<dataset *> & get_data_list() { return m_datalist; }
		pdf * get_pdf(int n) { return m_pdflist[n]; }
		std::vector<pdf *> & get_pdf_list() { return m_pdflist; }
		variable * get_var(int n) { return m_varlist[n]; }
		std::vector<variable *> & get_var_list() { return m_varlist; }
		void minimize(bool minos_err = false);
		virtual double operator()(const std::vector<double> & par) const = 0;
		virtual double Up() const = 0;

	protected:
		void init();

	protected:
		std::vector<dataset *> m_datalist;
		std::vector<pdf *> m_pdflist;
		std::vector<variable *> m_varlist;
		std::map<variable *, int> m_vcount;
		ROOT::Minuit2::MnMigrad * m_migrad;
		ROOT::Minuit2::MnMinos * m_minos;
		ROOT::Minuit2::FunctionMinimum * m_min;
};

#endif
