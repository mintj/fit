#include <iostream>
#include "TH1F.h"
#include "TString.h"
#include "datahist.h"
#include "util.h"
#include "variable.h"

datahist::datahist(const char * name, variable & x, TH1 * h):
	absdata(name, x, h->GetNbinsX()),
	m_err(h->GetNbinsX(), 0),
	m_err_down(h->GetNbinsX(), 0),
	m_err_up(h->GetNbinsX(), 0),
	m_edge(h->GetNbinsX()+1, 0)
{
	int nbin = h->GetNbinsX();
	if (x.limit_down() != h->GetBinLowEdge(1) || x.limit_up() != h->GetBinLowEdge(nbin) + h->GetBinWidth(nbin)) {
		MSG_ERROR("range of variable '", x.name(), "' does not match that of hist '", h->GetName(), "'");
		m_hist = 0;
	}
	else {
		m_hist = (TH1F *)h->Clone();
		m_hist->SetName(Form("%p", m_hist));
		m_hist->SetDirectory(0);
		m_nevt = 0;
		for (size_t u = 0; u < h->GetNbinsX(); ++u) {
			m_arr[u] = h->GetBinCenter(u+1);
			m_weight[u] = h->GetBinContent(u+1);
			m_edge[u] = h->GetBinLowEdge(u+1);
			m_err[u] = h->GetBinError(u+1);
			m_err_down[u] = h->GetBinErrorLow(u+1);
			m_err_up[u] = h->GetBinErrorUp(u+1);
			m_nevt += m_weight[u];
		}
		m_edge[m_size] = m_edge[m_size-1] + h->GetBinWidth(m_size);
		m_min[0] = m_edge[0];
		m_max[0] = m_edge[m_size];
	}
}

datahist::~datahist()
{
	if (m_hist) delete m_hist;
}

int datahist::find_bin(double x) 
{
	return m_hist->FindBin(x)-1;
}
