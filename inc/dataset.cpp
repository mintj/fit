#include "TLeaf.h"
#include "TString.h"
#include "dataset.h"
#include "util.h"
#include "variable.h"

dataset::dataset(const char * name, variable & x):
	absdata(name, x, 0)
{
}
dataset::dataset(const char * name, const std::vector<variable> & xvec):
	absdata(name, xvec, 0)
{
}

dataset::dataset(const char * name, TTree * t, variable & x, const char * wname):
	absdata(name, x, 0)
{
	init(t, wname);
}

dataset::dataset(const char * name, TTree * t, const std::vector<variable> & xvec, const char * wname):
	absdata(name, xvec, 0)
{
	init(t, wname);
}

dataset::~dataset()
{
}

void dataset::add(const double * x, double w)
{
	bool sel = true;
	std::vector<double> dp;
	for (size_t u = 0; u < m_dim; ++u) {
		if (x[u] < m_min[u] || x[u] > m_max[u]) {
			sel = false;
			break;
		}
		dp.push_back(x[u]);
	}
	if (sel) {
		m_arr.insert(m_arr.end(), dp.begin(), dp.end());
		m_weight.push_back(w);
	}
}

bool dataset::init(TTree * t, const char * wname)
{
	std::map<const char *, double> dmap;
	std::map<const char *, float> fmap;
	for (auto * v: m_varlist) {
		const char * name = v->name();
		auto * leaf = t->FindLeaf(name);
		bool status = false;
		if (leaf) {
			if (!strcmp(leaf->GetTypeName(), "Double_t")) {
				t->SetBranchAddress(name, &dmap[name]);
				status = true;
			}
			else if (!strcmp(leaf->GetTypeName(), "Float_t")) {
				t->SetBranchAddress(name, &fmap[name]);
				status = true;
			}
		}
		if (!status) {
			MSG_ERROR("cannot find float/double leaf named '", wname, "'");
			return false;
		}
	}
	
	double wd;
	float wf;
	int wcase = 0;
	if (wname) {
		auto * leaf = t->FindLeaf(wname);
		if (leaf) {
			if (!strcmp(leaf->GetTypeName(), "Double_t")) {
				t->SetBranchAddress(wname, &wd);
				wcase = 1;
			}
			else if (!strcmp(leaf->GetTypeName(), "Float_t")) {
				t->SetBranchAddress(wname, &wf);
				wcase = 2;
			}
		}
		if (!wcase) {
			MSG_WARNING("cannot find float/double leaf named '", wname, "'");
		}
	}

	for (size_t u = 0; u < t->GetEntries(); ++u) {
		t->GetEntry(u);

		std::vector<double> dp;
		for (size_t v = 0; v < m_dim; ++v) {
			const char * name = m_varlist[v]->name();
			
			double x;
			if (dmap.find(name) != dmap.end()) x = dmap[name];
			else x = fmap[name];
			
			if (x < m_min[v] || x > m_max[v]) dp.push_back(x);
		}

		if (dp.size() == m_dim) {
			m_arr.insert(m_arr.end(), dp.begin(), dp.end());
			double w = 1;
			if (wcase == 1) w = wd;
			else if (wcase == 2) w = wf;
			m_weight.push_back(w);
		}
	}
	return true;
}
