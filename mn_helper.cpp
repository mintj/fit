#include <iostream>
#include "Minuit2/MnPrint.h"
#include "mn_helper.h"
#include "pdf.h"
#include "variable.h"

mn_helper::mn_helper(nll & fcn):
	m_fcn(fcn)
{
	m_mn_upar = new ROOT::Minuit2::MnUserParameters;
	for (size_t u = 0; u < fcn.target_pdf()->npar_int(); ++u) {
		auto v = fcn.target_pdf()->get_var_int(u);
		cout << "aaa: " << v->name() << " " << v->value() << " (" << v->limit_down() << ", " << v->limit_up() << ") " << v->err() << endl;
		m_mn_upar->Add(v->name(), v->value(), v->err());
		m_mn_upar->SetLimits(v->name(), v->limit_down(), v->limit_up());
	}

	m_mn_migrad = new ROOT::Minuit2::MnMigrad(m_fcn, *m_mn_upar);
	m_mn_minos = 0;
	m_mn_min = 0;
}
		
//mn_helper::mn_helper(chi2 & fcn):
//	m_fcn(fcn)
//{
//	m_mn_upar = new ROOT::Minuit2::MnUserParameters;
//	for (size_t u = 0; u < fcn.target_func()->npar_int(); ++u) {
//		auto v = fcn.target_func()->get_var_int(u);
//		m_mn_upar->Add(v->name, v->value, v->err);
//		m_mn_upar->SetLimits(v->name, v->limit_down, v->limit_up);
//	}
//
//	m_mn_migrad = new ROOT::Minuit2::MnMigrad(m_fcn, *m_mn_upar);
//	m_mn_min = 0;
//	m_mn_minos = 0;
//}

mn_helper::~mn_helper()
{
	if (m_mn_upar) delete m_mn_upar;
	if (m_mn_migrad) delete m_mn_migrad;
	if (m_mn_minos) delete m_mn_minos;
	if (m_mn_min) delete m_mn_min;
}

void mn_helper::migrad()
{
	if (!m_mn_min) m_mn_min = new ROOT::Minuit2::FunctionMinimum(m_mn_migrad->operator()());
	*m_mn_min = m_mn_migrad->operator()();
	print();
}

void mn_helper::minos()
{
	if (m_mn_minos) delete m_mn_minos;
	if (!m_mn_min) migrad();
	m_mn_minos = new ROOT::Minuit2::MnMinos(m_fcn, *m_mn_min);
	// output
	print();
	cout << "1-sigma minos errors: " << endl;
	nll * wtf = reinterpret_cast<nll *>(&m_fcn);
	for (size_t u = 0; u < wtf->target_pdf()->npar_int(); ++u) {
		pair<double, double> e = m_mn_minos->operator()(u);
		const char * name = wtf->target_pdf()->get_var_int(u)->name();
		cout << name << " " << m_mn_min->UserState().Value(name) << " " << e.first << " " << e.second << endl;
	}
}

void mn_helper::minimize(bool minos_err)
{
	migrad();
	if (minos_err) minos();
}
		
void mn_helper::print()
{
	std::cout << *m_mn_min << std::endl;
	//std::cout << "**************************************************" << std::endl;
	//if (m_mn_min) {
	//	printf("FCN = %.6f, EDM = %.6e\n", m_mn_min.Fval(), m_mn_min.Edm());
	//}
	//else {
	//	std::cout << "(NOT MINIMIZED)" << std::endl;
	//}
	//std::cout << "**************************************************" << std::endl;
	//printf("%15s%15s%15s"
}
