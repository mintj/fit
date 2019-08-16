#include <iostream>
#include "TString.h"
#include "addpdf.h"
#include "datahist.h"
#include "dataset.h"
#include "pdf.h"
#include "plot.h"
#include "plotcmd.h"
#include "projpdf.h"

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

void plot::fill(TH1F * h, dataset * d, size_t dim)
{
	datahist * dh = dynamic_cast<datahist *>(d);
	if (!dh) {
		for (size_t u = 0; u < d->size(); ++u) {
			h->Fill(d->at(u)[dim], d->weight(u));
		}
	}
}

void plot::fill(TH1F * h, pdf * p, size_t dim)
{
	dataset * normset = p->normset();
	projpdf * pp = dynamic_cast<projpdf *>(p);
	int dim_shift = 0;
	if (pp) {
		size_t pdim = pp->proj_dim();
		for (size_t u = 0; u < normset->size(); ++u) {
			int bin = h->FindBin(normset->at(u)[pdim]);
			if (bin > 0 && bin <= h->GetNbinsX()) {
				h->Fill(normset->at(u)[pdim], pp->func_weight(normset->at(u)) * normset->weight(u));
			}
		}
	}
	else {
		for (size_t u = 0; u < normset->size(); ++u) {
			int bin = h->FindBin(normset->at(u)[dim]);
			if (bin > 0 && bin <= h->GetNbinsX()) {
				h->Fill(normset->at(u)[dim], p->evaluate_for_plot(normset->at(u)) * normset->weight(u));
			}
		}
	}
	h->Scale(normalized_nevt() / h->Integral());

	for (int u = 1; u <= h->GetNbinsX(); ++u) {
		h->SetBinError(u, 0);
	}
}

TH1F * plot::generate_hist(dataset * d, size_t dim)
{
	TH1F * h;
	datahist * dh = dynamic_cast<datahist *>(d);
	if (dh) {
		h = (TH1F *)dh->source_hist()->Clone();
	}
	else {
		int nbin = d->nbin() ? d->nbin() : 100;
		double xmin = d->min(dim);
		double xmax = d->max(dim);
		h = new TH1F("unnamed", "", nbin, xmin, xmax);
	}

	h->SetName(Form("%p", h));
	set_normhist(h);
	set_option(h, "e same");
	
	return h;
}

TH1F * plot::generate_hist(pdf * p, size_t dim)
{
	TH1F * h;
	projpdf * pp = dynamic_cast<projpdf *>(p);
	if (pp) {
		auto & binning = pp->get_binning();
		h = new TH1F("unnamed", "", binning.size()-1, &binning.at(0));
	}
	else {
		if (m_normhist) {
			h = (TH1F *)m_normhist->Clone();
			h->Reset();
		}
		else {
			double xmin = p->normset()->min(dim);
			double xmax = p->normset()->max(dim);
			h = new TH1F("unnamed", "", 100, xmin, xmax);
		}
	}

	h->SetName(Form("%p", h));
	set_option(h, "hist same");
	
	return h;
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
