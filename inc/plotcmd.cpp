#ifndef PLOTCOMMOND_H__
#define PLOTCOMMOND_H__

#include "TROOT.h"

void * msfit::components(int n)
{
	plotcmd::components_to_draw().insert(n);
	return 0;
}

template <typename ... T> void * msfit::components(int n, T ... rest)
{
	plotcmd::components_to_draw().insert(n);
	msfit::components(rest...);
	return 0;
}

void * msfit::linecolor(Color_t n)
{
	plotcmd::hist_actions().push_back([n](TH1F * h) { h->SetLineColor(n); });
	return 0;
}

void * msfit::linestyle(Style_t n)
{
	plotcmd::hist_actions().push_back([n](TH1F * h) { h->SetLineStyle(n); });
	return 0;
}

void * msfit::linewidth(Width_t n)
{
	plotcmd::hist_actions().push_back([n](TH1F * h) { h->SetLineWidth(n); });
	return 0;
}

void * msfit::markercolor(Color_t n)
{
	plotcmd::hist_actions().push_back([n](TH1F * h) { h->SetMarkerColor(n); });
	return 0;
}

void * msfit::markersize(Size_t n)
{
	plotcmd::hist_actions().push_back([n](TH1F * h) { h->SetMarkerSize(n); });
	return 0;
}

void * msfit::markerstyle(Style_t n)
{
	plotcmd::hist_actions().push_back([n](TH1F * h) { h->SetMarkerStyle(n); });
	return 0;
}

void * msfit::name(const char * name)
{
	plotcmd::hist_actions().push_back([name](TH1F * h) { h->SetName(name); });
	return 0;
}

void * msfit::project(int n)
{
	plotcmd::project_dim() = n;
	return 0;
}

void plotcmd::clear()
{
	sm_hist_actions.clear();
	sm_components.clear();
	sm_projdim = 0;
}

size_t plotcmd::sm_projdim;

std::set<int> plotcmd::sm_components;

std::vector<std::function<void(TH1F *)>> plotcmd::sm_hist_actions;

#endif
