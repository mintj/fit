#include <iostream>
#include "chi2fcn.h"
#include "datahist.h"
#include "dataset.h"
#include "extpdf.h"
#include "nllfcn.h"
#include "pdf.h"
#include "simfit.h"
#include "variable.h"

simfit::simfit(const std::vector<pdf *> & pdflist, const std::vector<dataset *> & datalist):
	m_plist(pdflist),
	m_dlist(datalist)
{
}

simfit::~simfit()
{
}

fitresult * simfit::chi2fit(bool minos_err)
{
	chi2fcn * chi2 = create_chi2();
	ROOT::Minuit2::MnUserParameters upar;
	for (variable * v: chi2->get_var_list()) {
		//std::cout << "aaa: " << v->name() << " " << v->value() << " (" << v->limit_down() << ", " << v->limit_up() << ") " << v->err() << std::endl;
		upar.Add(v->name(), v->value(), v->err());
		upar.SetLimits(v->name(), v->limit_down(), v->limit_up());
	}
	ROOT::Minuit2::MnMigrad migrad(*chi2, upar);
	ROOT::Minuit2::FunctionMinimum min = migrad();
	std::cout << min << std::endl;
	if (minos_err) {
		ROOT::Minuit2::MnMinos minos(*chi2, min);
		std::cout << "1-sigma minos errors: " << std::endl;
		for (size_t u = 0; u < chi2->get_var_list().size(); ++u) {
			std::pair<double, double> e = minos(u);
			const char * name = chi2->get_var(u)->name();
			std::cout << name << " " << min.UserState().Value(name) << " " << e.first << " " << e.second << std::endl;
		}
	}

	return 0;
}

nllfcn * simfit::create_nll()
{
	m_nll.reset(new nllfcn(m_plist, m_dlist));
	return m_nll.get();
}

chi2fcn * simfit::create_chi2()
{
	std::vector<extpdf *> plist;
	for (pdf * p: m_plist) {
		extpdf * extp = dynamic_cast<extpdf *>(p);
		if (!extp) {
			std::cout << "[simfit] error: chi2fit only supports (extended pdf + hist data)" << std::endl;
			return 0;
		}
		plist.push_back(extp);
	}

	std::vector<datahist *> dlist;
	for (dataset * d: m_dlist) {
		datahist * dh = dynamic_cast<datahist *>(d);
		if (!dh) {
			std::cout << "[simfit] error: chi2fit only supports (extended pdf + hist data)" << std::endl;
			return 0;
		}
		dlist.push_back(dh);
	}
	m_chi2.reset(new chi2fcn(plist, dlist));
	return m_chi2.get();
}

fitresult * simfit::fit(bool minos_err)
{
	nllfcn * nll = create_nll();
	ROOT::Minuit2::MnUserParameters upar;
	for (variable * v: nll->get_var_list()) {
		//cout << "aaa: " << v->name() << " " << v->value() << " (" << v->limit_down() << ", " << v->limit_up() << ") " << v->err() << endl;
		upar.Add(v->name(), v->value(), v->err());
		upar.SetLimits(v->name(), v->limit_down(), v->limit_up());
	}
	ROOT::Minuit2::MnMigrad migrad(*nll, upar);
	ROOT::Minuit2::FunctionMinimum min = migrad();
	cout << min << endl;
	if (minos_err) {
		ROOT::Minuit2::MnMinos minos(*nll, min);
		cout << "1-sigma minos errors: " << endl;
		for (size_t u = 0; u < nll->get_var_list().size(); ++u) {
			pair<double, double> e = minos(u);
			const char * name = nll->get_var(u)->name();
			cout << name << " " << min.UserState().Value(name) << " " << e.first << " " << e.second << endl;
		}
	}

	return 0;
}
