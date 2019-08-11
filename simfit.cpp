#include "dataset.h"
#include "nll.h"
#include "pdf.h"
#include "simfit.h"

simfit::simfit(const std::vector<pdf *> & pdflist, const std::vector<dataset *> & datalist):
	m_plist(pdflist),
	m_dlist(datalist)
{
}

simfit::~simfit()
{
}

nll * simfit::create_nll()
{
	m_nll.reset(new nll(m_plist, m_dlist));
	return m_nll.get();
}

void simfit::fit(bool minos_err)
{
	nll * fcn = create_nll();
	ROOT::Minuit2::MnUserParameters upar;
	for (variable * v: fcn->get_var_list()) {
		//cout << "aaa: " << v->name() << " " << v->value() << " (" << v->limit_down() << ", " << v->limit_up() << ") " << v->err() << endl;
		upar.Add(v->name(), v->value(), v->err());
		upar.SetLimits(v->name(), v->limit_down(), v->limit_up());
	}
	ROOT::Minuit2::MnMigrad migrad(*fcn, upar);
	ROOT::Minuit2::FunctionMinimum min = migrad();
	cout << min << endl;
	if (minos_err) {
		ROOT::Minuit2::MnMinos minos(*fcn, min);
		cout << "1-sigma minos errors: " << endl;
		for (size_t u = 0; u < fcn->get_var_list().size(); ++u) {
			pair<double, double> e = minos(u);
			const char * name = fcn->get_var(u)->name();
			cout << name << " " << min.UserState().Value(name) << " " << e.first << " " << e.second << endl;
		}
	}
}
