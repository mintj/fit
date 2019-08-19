#ifndef PLOTCMD_H__
#define PLOTCMD_H__

#include <vector>
#include <set>
#include <functional>
#include "TROOT.h"

namespace msfit
{
	void * components(int n);
	template <typename ... T> void * components(int n, T ... rest);
	void * linecolor(Color_t n);
	void * linestyle(Style_t n);
	void * linewidth(Width_t n);
	void * markercolor(Color_t n);
	void * markersize(Size_t n);
	void * markerstyle(Style_t n);
	void * name(const char * name);
	void * project(int n);
}

class plotcmd
{
	public:
		static void clear();
		static std::set<int> & components_to_draw() { return sm_components; }
		static std::vector<std::function<void(TH1F *)>> & hist_actions() { return sm_hist_actions; }
		static size_t & project_dim() { return sm_projdim; }
	
	private:
		static size_t sm_projdim;
		static std::set<int> sm_components;
		static std::vector<std::function<void(TH1F *)>> sm_hist_actions;
};

#endif
