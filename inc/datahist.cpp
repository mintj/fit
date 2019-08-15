#include <iostream>
#include "datahist.h"

datahist::datahist(TH1 * h):
	dataset(h->GetNbinsX(), 1),
	m_hist(h)
{
	acquire_resourse();
	if (!init_from_h1d(h)) release_resourse();
}

datahist::~datahist()
{
	release_resourse();
}

void datahist::acquire_resourse()
{
	m_edge = new double[m_size+1];
	m_err = new double[m_size];
	m_err_down = new double[m_size];
	m_err_up = new double[m_size];
}

void datahist::draw(TH1 * h, const char * option, size_t x, pdf * p)
{
	const char * name = h->GetName();
	if (h != m_hist) {
		if (h->IsOnHeap()) delete h;
		h = (TH1F *)m_hist->Clone(name);
	}
	if (p && p->dim() == 1) {
		for (int u = 1; u <= h->GetNbinsX(); ++u) {
			double c = h->GetBinContent(u);
			double e = h->GetBinError(u);
			double v = p->operator()(&m_arr[u-1]);
			h->SetBinContent(u, c*v);
			h->SetBinError(u, e*v);
		}
	}
	h->Draw(option);
}

void datahist::draw(TH1 * h, std::function<double(double *)> weight_func, const char * option, size_t x)
{
	const char * name = h->GetName();
	if (h->IsOnHeap()) delete h;
	h = (TH1F *)m_hist->Clone(name);
	for (int u = 1; u <= h->GetNbinsX(); ++u) {
		double c = h->GetBinContent(u);
		double e = h->GetBinError(u);
		double v = weight_func(&m_arr[u-1]);
		h->SetBinContent(u, c*v);
		h->SetBinError(u, e*v);
	}
	h->Draw(option);
}

bool datahist::init_from_h1d(TH1 * h)
{
	m_wsize = 0;
	for (size_t u = 0; u < h->GetNbinsX(); ++u) {
		m_arr[u] = h->GetBinCenter(u+1);
		m_weight[u] = h->GetBinContent(u+1);
		m_edge[u] = h->GetBinLowEdge(u+1);
		m_err[u] = h->GetBinError(u+1);
		m_err_down[u] = h->GetBinErrorLow(u+1);
		m_err_up[u] = h->GetBinErrorUp(u+1);
		m_wsize += m_weight[u];
	}
	m_edge[m_size] = m_edge[m_size-1] + h->GetBinWidth(m_size);
	return true;
}

double datahist::max(int n)
{
	return m_edge[0];
}

double datahist::min(int n)
{
	return m_edge[m_size];
}

void datahist::release_resourse()
{
	if (m_edge) delete[] m_edge;
	if (m_err) delete [] m_err;
	if (m_err_down) delete [] m_err_down;
	if (m_err_up) delete [] m_err_up;
}
