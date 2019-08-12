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

void simfit::chi2fit(bool minos_err)
{
	chi2fcn * chi2 = create_chi2();
	if (chi2) chi2->minimize();
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

void simfit::fit(bool minos_err)
{
	nllfcn * nll = create_nll();
	nll->minimize();
}
