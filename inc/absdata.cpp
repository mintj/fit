#include <cstring>
#include "TH1.h"
#include "TH2.h"
#include "absdata.h"
#include "variable.h"
#include "plot.h"

absdata::absdata(const char * name, variable & x, size_t size):
	m_dim(1), m_name(name)
{
	m_varlist.push_back(variable::var(x.name()));
	m_max.push_back(x.limit_up());
	m_min.push_back(x.limit_down());
	init(size);
}

absdata::absdata(const char * name, const std::vector<variable> & xvec, size_t size):
	m_dim(xvec.size()), m_name(name)
{
	for (auto & x: xvec) {
		m_varlist.push_back(variable::var(x.name()));
		m_max.push_back(x.limit_up());
		m_min.push_back(x.limit_down());
	}
	init(size);
}

absdata::~absdata()
{
}

void absdata::fill_hist(TH1 * h, size_t dim)
{
	if (dim < m_dim) {
		for (size_t u = 0; u < size(); ++u) {
			h->Fill(at(u)[dim], weight(u));
		}
	}
}

void absdata::fill_hist(TH2 * h, size_t dimx, size_t dimy)
{
	if (dimx < m_dim && dimy < m_dim) {
		for (size_t u = 0; u < size(); ++u) {
			h->Fill(at(u)[dimx], at(u)[dimy], weight(u));
		}
	}
}

int absdata::get_dim(const char * vname)
{
	for (int u = 0; u < m_varlist.size(); ++u) {
		if (!strcmp(vname, m_varlist[u]->name())) return u;
	}
	return -1;
}

int absdata::get_dim(variable * v)
{
	return get_dim(v->name());
}

void absdata::init(size_t size)
{
	for (size_t u = 0; u < size; ++u) {
		m_weight.push_back(0);
		for (size_t v = 0; v < m_dim; ++v) {
			m_arr.push_back(0);
		}
	}
}

double absdata::nevt()
{
	double ntot = 0;
	for (size_t u = 0; u < size(); ++u) {
		ntot += m_weight[u];
	}
	return ntot;
}

template <typename...T>
void absdata::plot_on(plot * frame, T...command)
{
	frame->add(this);
}

void absdata::project(TH1 * h, size_t dim)
{
	if (dim < m_dim) {
		h->Reset();
		fill_hist(h, dim);
	}
}

void absdata::project(TH2 * h, size_t dimx, size_t dimy)
{
	if (dimx < m_dim && dimy < m_dim) {
		h->Reset();
		fill_hist(h, dimx, dimy);
	}
}
