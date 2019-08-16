#include <iostream>
#include "TString.h"
#include "datahist.h"
#include "plot.h"

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

template<typename... T> void datahist::plot_on(plot * frame, T... action)
{
	size_t dim = frame->proj_dim();
	if (dim) {
		std::cout << "[datahist] warning: 'datahist' is 1d data, dimension argument in 'plot_on' method is ignored" << endl;
	}
	TH1F * h = frame->generate_hist(this, dim);
	frame->add(h, std::forward<T>(action)...);
	frame->fill(h, this, dim);
}

void datahist::release_resourse()
{
	if (m_edge) delete[] m_edge;
	if (m_err) delete [] m_err;
	if (m_err_down) delete [] m_err_down;
	if (m_err_up) delete [] m_err_up;
}
