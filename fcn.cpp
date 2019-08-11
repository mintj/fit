#include <iostream>
#include <cmath>
#include "datahist.h"
#include "dataset.h"
#include "extpdf.h"
#include "fcn.h"
#include "pdf.h"
#include "variable.h"

fcn::fcn(pdf * p, dataset * d):
	m_pdflist({p}),
	m_datalist({d})
{
	init();
}

fcn::fcn(const std::vector<pdf *> plist, const std::vector<dataset *> dlist):
	m_pdflist(plist.cbegin(), plist.cend()),
	m_datalist(dlist.cbegin(), dlist.cend())
{
	init();
}

fcn::fcn(const std::vector<extpdf *> plist, const std::vector<datahist *> dlist):
	m_pdflist(plist.cbegin(), plist.cend()),
	m_datalist(dlist.cbegin(), dlist.cend())
{
	init();
}

fcn::~fcn()
{
}

void fcn::init()
{
	for (pdf * p: m_pdflist) {
		for (variable * v: p->get_vars()) {
			if (m_vcount.find(v) == m_vcount.end()) {
				m_varlist.push_back(v);
			}
			++m_vcount[v];
		}
	}
}
