#include <iostream>
#include "plot.h"

plot::plot()
{
}

plot::~plot()
{
}

template<typename... T>
TH1F * plot::add(TH1F * h, T... action)
{
	m_hist.push_back(h);
	set_attribute(h, action...);
	m_option[h] = "same";
	return h;
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
		
void plot::set_attribute(TH1F * h, std::function<void(TH1F *)> action)
{
	action(h);
}

template<typename...T>
void plot::set_attribute(TH1F * h, std::function<void(TH1F *)> action, T... rest)
{
	action(h);
	set_attribute(h, rest...);
}

void plot::set_option(TH1F * h, const char * option)
{
	m_option[h] = option;
}

std::function<void(TH1F *)> msfit::linecolor(Color_t n)
{
	return [n](TH1F * h){ h->SetLineColor(n); };
}

std::function<void(TH1F *)> msfit::linestyle(Style_t n)
{
	return [n](TH1F * h){ h->SetLineStyle(n); };
}

std::function<void(TH1F *)> msfit::linewidth(Width_t n)
{
	return [n](TH1F * h){ h->SetLineWidth(n); };
}

std::function<void(TH1F *)> msfit::markercolor(Color_t n)
{
	return [n](TH1F * h){ h->SetMarkerColor(n); };
}

std::function<void(TH1F *)> msfit::markersize(Size_t n)
{
	return [n](TH1F * h){ h->SetMarkerSize(n); };
}

std::function<void(TH1F *)> msfit::markerstyle(Style_t n)
{
	return [n](TH1F * h){ h->SetMarkerStyle(n); };
}

std::function<void(TH1F *)> msfit::name(const char * name)
{
	return [name](TH1F * h){ h->SetName(name); };
}
