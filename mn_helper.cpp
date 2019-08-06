#include "mn_helper.h"

//mn_helper::mn_helper(nll & fcn):
//	m_fcn(fcn)
mn_helper::mn_helper()
{
	m_mn_upar = new ROOT::Minuit2::MnUserParameters;
	//for (size_t u = 0; u < fcn.target_pdf().npar(); ++u) {
	//	auto v = fcn.target_pdf().get_par(u);
	//	m_mn_upar->Add(v.name(), v.value(), v.err());
	//	m_mn_upar->SetLimits(v.name(), v.down_limit(), v.up_limit());
	//}

	//m_mn_migrad = new ROOT::Minuit2::MnMigrad(m_fcn, *m_mn_upar);
	m_mn_min = 0;
	m_mn_minos = 0;
}

mn_helper::~mn_helper()
{
	//if (m_mn_upar) delete m_mn_upar;
	//if (m_mn_migrad) delete m_mn_migrad;
	//if (m_mn_minos) delete m_mn_minos;
	//if (m_mn_min) delete m_mn_min;
}

void mn_helper::migrad()
{
	//if (!m_mn_min) m_mn_min = new ROOT::Minuit2::FunctionMinimum(m_mn_migrad->operator()());
	//*m_mn_min = m_mn_migrad->operator()();
}

void mn_helper::minos()
{
	//if (m_mn_minos) delete m_mn_minos;
	//if (!m_mn_min) migrad();
	//m_mn_minos = new ROOT::Minuit2::MnMinos(m_fcn, *m_mn_min);
}

void mn_helper::minimize(bool minos_err)
{
	migrad();
	if (minos_err) minos();
}
