#ifndef NLL_H__
#define NLL_H__

#include <vector>
#include <map>
#include "TMath.h"
#include "Minuit2/FCNBase.h"

class dataset;
class extpdf;
class pdf;
class variable;

class nll: public ROOT::Minuit2::FCNBase
{
	public:
		nll(pdf * p, dataset * d);
		nll(const std::vector<pdf *> plist, const std::vector<dataset *> dlist);
		virtual ~nll();
		dataset * get_data(int n) { return m_datalist[n]; }
		std::vector<dataset *> & get_data_list() { return m_datalist; }
		pdf * get_pdf(int n) { return m_pdflist[n]; }
		std::vector<pdf *> & get_pdf_list() { return m_pdflist; }
		variable * get_var(int n) { return m_varlist[n]; }
		std::vector<variable *> & get_var_list() { return m_varlist; }
		virtual double operator()(const std::vector<double> & par) const;
		virtual double Up() const { return 0.5; }

	protected:
		void init();

	protected:
		std::vector<dataset *> m_datalist;
		std::vector<pdf *> m_pdflist;
		std::vector<variable *> m_varlist;
		std::map<variable *, int> m_vcount;
};

#endif
