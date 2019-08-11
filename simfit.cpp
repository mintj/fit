#include "dataset.h"
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

nllfcn * simfit::create_nll()
{
	m_nll.reset(new nllfcn(m_plist, m_dlist));
	return m_nll.get();
}

void simfit::fit(bool minos_err)
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
}
