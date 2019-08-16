#include <iostream>
#include "plot.h"
#include "plotcmd.h"

plot::plot():
	m_nevt(1),
	m_normhist(0),
	m_currpdf(0)
{
}

plot::~plot()
{
	for (auto h: m_hist) {
		if (h) delete h;
	}
}

template <typename ... T> void plot::add(TH1F * h, const T & ... action)
{
	m_hist.push_back(h);
	m_option[h] = "same";
	h->SetDirectory(0);
	for (auto act: plotcmd::hist_actions()) {
		act(h);
	}

	m_nevt = m_normhist ? m_normhist->Integral() : 1;
	auto & comp_set = plotcmd::components_to_draw();
	addpdf * apdf = dynamic_cast<addpdf *>(m_currpdf);
	if (apdf) apdf->calculate_frac();
	if (comp_set.size() && apdf) {
		double ftot = 0;
		for (size_t u = 0; u < apdf->number_of_pdfs(); ++u) {
			if (!(comp_set.find(u) == comp_set.end())) {
				ftot += apdf->frac(u);
			}
		}
		if (m_normhist) m_nevt = m_normhist->Integral() * ftot;
		else m_nevt = ftot;

		for (size_t u = 0; u < apdf->number_of_pdfs(); ++u) {
			if (comp_set.find(u) == comp_set.end()) {
				apdf->set_frac_for_plot(u, 0);
			}
		}
	}
	plotcmd::clear();
}

void plot::draw()
{
	double max_height = 0;
	double min_height = 0;
	for (auto h: m_hist) {
		if (h->GetMaximum() > max_height) max_height = h->GetMaximum();
		if (h->GetMinimum() < max_height) min_height = h->GetMinimum();
	}

	if (max_height < 0) max_height = 0;
	if (min_height > 0) min_height = 0;
	for (auto h: m_hist) {
		h->SetMaximum(1.1*max_height);
		h->SetMinimum(min_height);
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

size_t plot::proj_dim()
{
	return plotcmd::project_dim();
}

void plot::set_normhist(TH1F * h)
{
	m_normhist = h;
	m_nevt = h->Integral();
}
