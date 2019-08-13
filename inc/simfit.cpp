#include <iostream>
#include "chi2fcn.h"
#include "datahist.h"
#include "dataset.h"
#include "nllfcn.h"
#include "pdf.h"
#include "simfit.h"
#include "variable.h"

simfit::simfit()
{
}

simfit::~simfit()
{
}

void simfit::add(pdf & p, dataset & d)
{
	m_plist.push_back(&p);
	m_dlist.push_back(&d);
}

void simfit::chi2fit(bool minos_err)
{
	chi2fcn * chi2 = create_chi2();
	if (chi2) chi2->minimize(minos_err);
}

nllfcn * simfit::create_nll()
{
	m_nll.reset(new nllfcn);
	for (size_t u = 0; u < m_plist.size(); ++u) {
		m_nll.get()->add(m_plist[u], m_dlist[u]);
	}
	return m_nll.get();
}

chi2fcn * simfit::create_chi2()
{
	m_chi2.reset(new chi2fcn);
	for (size_t u = 0; u < m_plist.size(); ++u) {
		datahist * d = dynamic_cast<datahist *>(m_dlist[u]);
		if (!d) {
			std::cout << "[simfit] error: chi2fit are restricted to datahist only" << std::endl;
			return 0;
		}
		m_chi2.get()->add(m_plist[u], d);
	}
	return m_chi2.get();
}

void simfit::fit(bool minos_err)
{
	nllfcn * nll = create_nll();
	nll->minimize(minos_err);
}
