#include <iostream>
#include "plot.h"
#include "plotcmd.h"

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

//void plot::add(TH1F * h)
//{
//	m_hist.push_back(h);
//	m_option[h] = "same";
//	h->SetDirectory(0);
//}

template <typename ... T> void plot::add(TH1F * h, const T & ... action)
{
	m_hist.push_back(h);
	m_option[h] = "same";
	h->SetDirectory(0);
	for (auto act: plotcmd::hist_actions()) {
		act(h);
	}
	plotcmd::clear_actions();
}

void plot::draw()
{
	for (auto h: m_hist) {
		h->Draw(m_option[h]);
	}
}

TH1F * plot::get_hist(const char * name)
{
	for (auto h: m_hist) {
		if (!strcmp(name, h->GetName())) return h;
	}
	std::cout << "[plot] error: there is no histogram named " << name << " on this plot" << std::endl;
	return 0;
}

//template <typename T> void plot::set_hist_attribute(TH1F * h, const T & action)
//{
//	if (action) action(h);
//}
//
//template <typename T, typename ... TT> void plot::set_hist_attribute(TH1F * h, const T & action, const TT & ... rest)
//{
//	if (action) action(h);
//	set_hist_attribute(h, rest...);
//}

void plot::set_option(TH1F * h, const char * option)
{
	m_option[h] = option;
}
