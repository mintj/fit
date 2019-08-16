#include <iostream>
#include "plot.h"

plot::plot():
	m_normhist(0)
{
}

plot::~plot()
{
	for (auto h: m_hist) {
		if (h) delete h;
	}
}

void plot::add(TH1F * h)
{
	m_hist.push_back(h);
	m_option[h] = "same";
	h->SetDirectory(0);
}

template <typename ... T> void plot::add(TH1F * h, const T & ... action)
{
	m_hist.push_back(h);
	m_option[h] = "same";
	h->SetDirectory(0);
	if (sizeof...(action)) set_hist_attribute(h, action...); // TODO: why???
}

void plot::draw()
{
	for (auto h: m_hist) {
		h->Draw(m_option[h]);
	}
}

TH1F * plot::get(const char * name)
{
	for (auto h: m_hist) {
		if (!strcmp(name, h->GetName())) return h;
	}
	std::cout << "[plot] error: there is no histogram named " << name << " on this plot" << std::endl;
	return 0;
}

template <typename T> void plot::set_hist_attribute(TH1F * h, const T & action)
{
	action(h);
}

template <typename T, typename ... TT> void plot::set_hist_attribute(TH1F * h, const T & action, const TT & ... rest)
{
	action(h);
	set_hist_attribute(h, rest...);
}

void plot::set_option(TH1F * h, const char * option)
{
	m_option[h] = option;
}

namespace msfit
{
	std::function<void(TH1F *)> linecolor(Color_t n)
	{
		return [n](TH1F * h) { h->SetLineColor(n); };
	}
	
	std::function<void(TH1F *)> linestyle(Style_t n)
	{
		return [n](TH1F * h) { h->SetLineStyle(n); };
	}
	
	std::function<void(TH1F *)> linewidth(Width_t n)
	{
		return [n](TH1F * h) { h->SetLineWidth(n); };
	}
	
	std::function<void(TH1F *)> markercolor(Color_t n)
	{
		return [n](TH1F * h) { h->SetMarkerColor(n); };
	}
	
	std::function<void(TH1F *)> markersize(Size_t n)
	{
		return [n](TH1F * h) { h->SetMarkerSize(n); };
	}
	
	std::function<void(TH1F *)> markerstyle(Style_t n)
	{
		return [n](TH1F * h) { h->SetMarkerStyle(n); };
	}
	
	std::function<void(TH1F *)> name(const char * name)
	{
		return [name](TH1F * h) { h->SetName(name); };
	}
}
