#ifndef PLOT_H__
#define PLOT_H__

#include <vector>
#include <map>
#include <functional>
#include "TH1F.h"

class plot
{
	public:
		plot();
		virtual ~plot();
		void add(TH1F * h);
		template <typename ... T> void add(TH1F * h, const T & ... action);
		void draw();
		TH1F * get(const char * name);
		TH1F * normhist() { return m_normhist; }
		void set_normhist(TH1F * h) { m_normhist = h; }
		void set_option(TH1F * h, const char * option);
		
		template <typename T> static void set_hist_attribute(TH1F * h, const T & action);
		template <typename T, typename ... TT> static void set_hist_attribute(TH1F * h, const T & action, const TT & ... rest);
	
	private:
		TH1F * m_normhist;
		std::vector<TH1F *> m_hist;
		std::map<TH1F *, const char *> m_option;
};

namespace msfit
{
	std::function<void(TH1F *)> linecolor(Color_t n);
	std::function<void(TH1F *)> linestyle(Style_t n);
	std::function<void(TH1F *)> linewidth(Width_t n);
	std::function<void(TH1F *)> markercolor(Color_t n);
	std::function<void(TH1F *)> markersize(Size_t n);
	std::function<void(TH1F *)> markerstyle(Style_t n);
	std::function<void(TH1F *)> name(const char * name);
}

#endif
