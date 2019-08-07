#include "TLeaf.h"
#include "dataset.h"

dataset::dataset(size_t s, size_t d):
	m_size(s),
	m_dim(d)
{
	aquire_resourse();
}

dataset::dataset(size_t s, const std::vector<const char *> & varname):
	m_size(s),
	m_dim(varname.size())
{
	aquire_resourse();
	for (size_t u = 0; u < m_dim; ++u) {
		m_varname[u] = varname.at(u);
	}
}

dataset::dataset(TTree * t, const std::vector<const char *> & varname):
	m_size(t->GetEntries()),
	m_dim(varname.size())
{
	aquire_resourse();
	if (!init_from_tree(t, varname)) release_resourse();
}

dataset::dataset(TTree * t, const std::vector<const char *> & varname, const char * wname):
	m_size(t->GetEntries()),
	m_dim(varname.size())
{
	aquire_resourse();
	if (!init_from_tree(t, varname, wname)) release_resourse();
}

dataset::~dataset()
{
	release_resourse();
}

void dataset::aquire_resourse()
{
	m_arr = new double[m_size*m_dim];
	m_weight = new double[m_size];
	m_varname = new std::string[m_dim];
}

bool dataset::init_from_tree(TTree * t, const std::vector<const char *> & varname)
{
	std::map<std::string, double> dmap;
	std::map<std::string, float> fmap;
	for (size_t u = 0; u < m_dim; ++u) {
		m_varname[u] = varname[u];
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

	double * curr = m_arr;
	for (size_t u = 0; u < m_size; ++u) {
		t->GetEntry(u);
		for (size_t v = 0; v < m_dim; ++v) {
			if (dmap.find(varname[u]) != dmap.end()) {
				*curr = dmap[varname[u]];
				++curr;
			}
			else {
				*curr = fmap[varname[u]];
				++curr;
			}
		}
		m_weight[u] = 1;
	}
	return true;
}

bool dataset::init_from_tree(TTree * t, const std::vector<const char *> & varname, const char * wname)
{
	if (init_from_tree(t, varname)) {
		double wd;
		float wf;
		auto * leaf = t->FindLeaf(wname);
		if (leaf) {
			if (!strcmp(leaf->GetTypeName(), "Double_t")) {
				t->SetBranchAddress(wname, &wd);
				for (size_t u = 0; u < m_size; ++u) {
					t->GetEntry(u);
					m_weight[u] = wd; 
				}
			}
			else if (!strcmp(leaf->GetTypeName(), "Float_t")) {
				t->SetBranchAddress(wname, &wf);
				for (size_t u = 0; u < m_size; ++u) {
					t->GetEntry(u);
					m_weight[u] = wf; 
				}
			}
			else return false;
		}
		else return false;
	}
	return true;
}

void dataset::release_resourse()
{
	if (m_arr) delete[] m_arr;
	if (m_weight) delete [] m_weight;
	if (m_varname) delete [] m_varname;
}
