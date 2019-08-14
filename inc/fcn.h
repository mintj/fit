#ifndef FCN_H__
#define FCN_H__

#include <vector>
#include <map>
#include "Minuit2/FCNBase.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/MnMinos.h"

class datahist;
class dataset;
class pdf;
class variable;

class fcn: public ROOT::Minuit2::FCNBase
{
	public:
		fcn() = default;
		fcn(pdf * p, dataset * d);
		virtual ~fcn();
		void add(pdf * p, dataset * d);
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
		void update_varlist(pdf * p, dataset * d);

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
