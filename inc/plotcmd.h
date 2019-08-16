#ifndef PLOTCMD_H__
#define PLOTCMD_H__

#include "TROOT.h"

namespace msfit
{
	void * components(int n);
	//template <typename T> void components(int n, T ... rest);
	void * linecolor(Color_t n);
	void * linestyle(Style_t n);
	void * linewidth(Width_t n);
	void * markercolor(Color_t n);
	void * markersize(Size_t n);
	void * markerstyle(Style_t n);
	void * name(const char * name);
}

class plotcmd
{
	public:
		static void clear_actions() { sm_hist_actions.clear(); }
		static void clear_components() { sm_components.clear(); }
		static std::set<int> & components_to_draw() { return sm_components; }
		static std::vector<std::function<void(TH1F *)>> & hist_actions() { return sm_hist_actions; }
	
	private:
		static std::set<int> sm_components;
		static std::vector<std::function<void(TH1F *)>> sm_hist_actions;
};

#endif
