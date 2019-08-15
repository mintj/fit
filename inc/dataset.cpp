#include <iostream>
#include "TLeaf.h"
#include "dataset.h"
#include "pdf.h"

dataset::dataset(size_t s, size_t d):
	m_size(s),
	m_dim(d)
{
	acquire_resourse();
}

dataset::dataset(TTree * t, const std::vector<const char *> & varname):
	m_size(t->GetEntries()),
	m_dim(varname.size())
{
	acquire_resourse();
	if (!init_from_tree(t, varname)) release_resourse();
}

dataset::dataset(TTree * t, const std::vector<const char *> & varname, const char * wname):
	m_size(t->GetEntries()),
	m_dim(varname.size())
{
	acquire_resourse();
	if (!init_from_tree(t, varname, wname)) release_resourse();
}

dataset::~dataset()
{
	release_resourse();
}

void dataset::acquire_resourse()
{
	m_arr = new double[m_size*m_dim];
	for (size_t u = 0; u < m_size*m_dim; ++u) {
		m_arr[u] = 0;
	}
	
	m_weight = new double[m_size];
	for (size_t u = 0; u < m_size; ++u) {
		m_weight[u] = 0;
	}
}

void dataset::draw(TH1 * h, const char * option, size_t x, pdf * p)
{
	if (x < m_dim) {
		h->Reset();
		double * curr = m_arr;
		if (p && p->dim() <= m_dim) {
			for (size_t u = 0; u < m_size; ++u) {
				h->Fill(curr[x], m_weight[u]*p->operator()(curr));
				curr += m_dim;
			}
		}
		else {
			for (size_t u = 0; u < m_size; ++u) {
				h->Fill(curr[x], m_weight[u]);
				curr += m_dim;
			}
		}
		h->Draw(option);
	}
	else {
		std::cout << "[dataset] error: out of allowed dimension 0 ~ " << m_dim-1 << " (" << x << ")" << std::endl;
	}
}

void dataset::draw(TH1 * h, std::function<double(double *)> weight_func, const char * option, size_t x)
{
	if (x < m_dim) {
		h->Reset();
		double * curr = m_arr;
		for (size_t u = 0; u < m_size; ++u) {
			h->Fill(curr[x], m_weight[u]*weight_func(curr));
			curr += m_dim;
		}
		h->Draw(option);
	}
	else {
		std::cout << "[dataset] error: out of allowed dimension 0 ~ " << m_dim-1 << " (" << x << ")" << std::endl;
	}
}

void dataset::draw(TH2 * h, const char * option, size_t x, size_t y, pdf * p)
{
	if (x < m_dim && y < m_dim) {
		h->Reset();
		double * curr = m_arr;
		if (p && p->dim() <= m_dim) {
			for (size_t u = 0; u < m_size; ++u) {
				h->Fill(curr[x], curr[y], m_weight[u]*p->operator()(curr));
				curr += m_dim;
			}
		}
		else {
			for (size_t u = 0; u < m_size; ++u) {
				h->Fill(curr[x], curr[y], m_weight[u]);
				curr += m_dim;
			}
		}
		h->Draw(option);
	}
	else {
		std::cout << "[dataset] error: out of allowed dimension 0 ~ " << m_dim-1 << " (" << x << ", " << y << ")" << std::endl;
	}
}

void dataset::draw(TH2 * h, std::function<double(double *)> weight_func, const char * option, size_t x, size_t y)
{
	if (x < m_dim) {
		h->Reset();
		double * curr = m_arr;
		for (size_t u = 0; u < m_size; ++u) {
			h->Fill(curr[x], curr[y], m_weight[u]*weight_func(curr));
			curr += m_dim;
		}
		h->Draw(option);
	}
	else {
		std::cout << "[dataset] error: out of allowed dimension 0 ~ " << m_dim-1 << " (" << x << ", " << y << ")" << std::endl;
	}
}

bool dataset::init_from_tree(TTree * t, const std::vector<const char *> & varname)
{
	std::map<std::string, double> dmap;
	std::map<std::string, float> fmap;
	for (size_t u = 0; u < m_dim; ++u) {
		auto * leaf = t->FindLeaf(varname[u]);
		if (leaf) {
			if (!strcmp(leaf->GetTypeName(), "Double_t")) {
				t->SetBranchAddress(varname[u], &dmap[varname[u]]);
			}
			else if (!strcmp(leaf->GetTypeName(), "Float_t")) {
				t->SetBranchAddress(varname[u], &fmap[varname[u]]);
			}
			else return false;
		}
		else return false;
	}

	m_wsize = 0;
	double * curr = m_arr;
	for (size_t u = 0; u < m_size; ++u) {
		t->GetEntry(u);
		for (size_t v = 0; v < m_dim; ++v) {
			if (dmap.find(varname[v]) != dmap.end()) {
				*curr = dmap[varname[v]];
				++curr;
			}
			else {
				*curr = fmap[varname[v]];
				++curr;
			}
		}
		m_weight[u] = 1;
		m_wsize += 1;
	}
	return true;
}

bool dataset::init_from_tree(TTree * t, const std::vector<const char *> & varname, const char * wname)
{
	if (init_from_tree(t, varname)) {
		m_wsize = 0;
		double wd;
		float wf;
		auto * leaf = t->FindLeaf(wname);
		if (leaf) {
			if (!strcmp(leaf->GetTypeName(), "Double_t")) {
				t->SetBranchAddress(wname, &wd);
				for (size_t u = 0; u < m_size; ++u) {
					t->GetEntry(u);
					m_weight[u] = wd; 
					m_wsize += wd;
				}
			}
			else if (!strcmp(leaf->GetTypeName(), "Float_t")) {
				t->SetBranchAddress(wname, &wf);
				for (size_t u = 0; u < m_size; ++u) {
					t->GetEntry(u);
					m_weight[u] = wf; 
					m_wsize += wf;
				}
			}
			else return false;
		}
		else return false;
	}
	return true;
}

double dataset::max(int n)
{
	if (!m_size || n >= m_dim) return 0;
	
	double m = m_arr[n];
	for (size_t u = 1; u < m_size; ++u) {
		if (m < m_arr[u*m_dim+n]) m = m_arr[u*m_dim+n];
	}
	return m;
}

double dataset::min(int n)
{
	if (!m_size || n >= m_dim) return 0;
	
	double m = m_arr[n];
	for (size_t u = 1; u < m_size; ++u) {
		if (m > m_arr[u*m_dim+n]) m = m_arr[u*m_dim+n];
	}
	return m;
}

void dataset::release_resourse()
{
	if (m_arr) delete[] m_arr;
	if (m_weight) delete [] m_weight;
}
