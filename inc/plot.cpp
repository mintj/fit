#include "TGraph.h"
#include "TH1F.h"
#include "TString.h"

#include "abspdf.h"
#include "datahist.h"
#include "dataset.h"
#include "plot.h"
#include "plotcmd.h"
#include "util.h"
#include "variable.h"

plot::plot(variable & var):
	m_var(&var), m_normhist(0)
{
	TH1F * h = new TH1F(Form("plot_of_%s", var.name()), "", var.bins(), var.limit_down(), var.limit_up());
	h->SetTitle(0);
	m_hist.push_back(h);
	m_option[h] = "";
}

plot::~plot()
{
	for (auto h: m_hist) {
		if (h) delete h;
	}
	for (auto gr: m_graph) {
		if (gr) delete gr;
	}
}

void plot::add(absdata * d)
{
	TH1F * h = create_hist_and_fill(d);
	if (h) {
		plotcmd::execute(h);
		m_hist.push_back(h);
		set_normhist(h);
	}
	plotcmd::clear();
}

void plot::add(abspdf * p)
{
	TGraph * gr = create_graph_and_fill(p);
	if (gr) {
		plotcmd::execute(gr);
		m_graph.push_back(gr);
	}
	plotcmd::clear();
}

double plot::calculate_hist_area(TH1F * h)
{
	double area = 0;
	for (int u = 1; u <= h->GetNbinsX(); ++u) {
		area += h->GetBinContent(u) * h->GetBinWidth(u);
	}
	return area;
}

void plot::calculate_y_range()
{
	double hmax = 0;
	double hmin = 0;
	for (auto gr: m_graph) {
		double * y = gr->GetY();
		for (int u = 0; u < gr->GetN(); ++u) {
			if (hmax < y[u]) hmax = y[u];
			if (hmin > y[u]) hmin = y[u];
		}
	}
	
	for (auto h: m_hist) {
		for (int u = 1; u <= h->GetNbinsX(); ++u) {
			double htmp = h->GetBinContent(u) + h->GetBinError(u);
			if (hmax < htmp) hmax = htmp;
		}
	}
	
	for (auto h: m_hist) {
		if (h->GetMinimum() < hmax) hmin = h->GetMinimum();
	}
	if (hmin > 0) hmin = 0;
	
	m_max_height = hmax;
	m_min_height = hmin;
}

TGraph * plot::create_graph_and_fill(abspdf * p)
{
	TGraph * gr = 0;
	int dim = p->get_dim(m_var);
	if (dim < 0) {
		MSG_ERROR("pdf '", p->name(), "' does not contain variable '", m_var->name(), "'");
	}
	else {
		int bins = 100000;
		std::vector<double> x(bins+1, 0);
		std::vector<double> y(bins+1, 0);
		double xmin = p->min(dim);
		double xmax = p->max(dim);
		double step = (xmax-xmin)/bins;
		double area = m_normhist ? calculate_hist_area(m_normhist) : 1.0;
	
		{
			double ytot = 0;
			for (int u = 0; u <= bins; ++u) {
				x[u] = xmin + u*step;
				y[u] = p->evaluate(&x[u]);
				ytot += y[u];
			}

			double sf = area / (ytot*step);
			for (int u = 0; u <= bins; ++u) {
				y[u] *= sf;
			}
			gr = new TGraph(bins, &(x[0]), &(y[0]));
			gr->SetName(Form("[%s]_project_on_[%s]", p->name(), m_var->name()));
		}
	}
	return gr;
}

TH1F * plot::create_hist_and_fill(absdata * d)
{
	TH1F * h = 0;
	int dim = d->get_dim(m_var);
	if (dim < 0) {
		MSG_ERROR("data '", d->name(), "' does not contain variable '", m_var->name(), "'");
	}
	else {
		datahist * dh = dynamic_cast<datahist *>(d);
		if (dh) {
			h = (TH1F *)dh->hist()->Clone();
		}
		else {
			h = (TH1F *)m_hist[0]->Clone();
			d->project(h, dim);
		}
		h->SetName(Form("[%s]_project_on_[%s]", d->name(), m_var->name()));
		h->SetDirectory(0);
	}
	return h;
}
//{
//	m_hist.push_back(h);
//	h->SetDirectory(0);
//	for (auto act: plotcmd::hist_actions()) {
//		act(h);
//	}
//
//	m_nevt = m_normhist ? m_normhist->Integral() : 1;
//	auto & comp_set = plotcmd::components_to_draw();
//	addpdf * apdf = dynamic_cast<addpdf *>(m_currpdf);
//	if (apdf) apdf->calculate_frac();
//	if (comp_set.size() && apdf) {
//		double ftot = 0;
//		for (size_t u = 0; u < apdf->number_of_pdfs(); ++u) {
//			if (!(comp_set.find(u) == comp_set.end())) {
//				ftot += apdf->frac(u);
//			}
//		}
//		if (m_normhist) m_nevt = m_normhist->Integral() * ftot;
//		else m_nevt = ftot;
//
//		for (size_t u = 0; u < apdf->number_of_pdfs(); ++u) {
//			if (comp_set.find(u) == comp_set.end()) {
//				apdf->set_frac_for_plot(u, 0);
//			}
//		}
//	}
//	plotcmd::clear();
//}
//
void plot::draw()
{
	calculate_y_range();
	
	for (auto h: m_hist) {
		h->SetMaximum(1.1*m_max_height);
		h->SetMinimum(m_min_height);
		h->Draw(m_option[h]);
	}
	for (auto gr: m_graph) {
		gr->Draw("l same");
	}
}
//
//void plot::fill(TH1F * h, dataset * d, size_t dim)
//{
//	datahist * dh = dynamic_cast<datahist *>(d);
//	if (!dh) {
//		for (size_t u = 0; u < d->size(); ++u) {
//			h->Fill(d->at(u)[dim], d->weight(u));
//		}
//	}
//}
//
//void plot::fill(TH1F * h, pdf * p, size_t dim)
//{
//	dataset * normset = p->normset();
//	projpdf * pp = dynamic_cast<projpdf *>(p);
//	int dim_shift = 0;
//	if (pp) {
//		//size_t pdim = pp->proj_dim();
//		//for (size_t u = 0; u < normset->size(); ++u) {
//		//	int bin = h->FindBin(normset->at(u)[pdim]);
//		//	if (bin > 0 && bin <= h->GetNbinsX()) {
//		//		h->Fill(normset->at(u)[pdim], pp->func_weight(normset->at(u)) * normset->weight(u));
//		//	}
//		//}
//	}
//	else {
//		for (size_t u = 0; u < normset->size(); ++u) {
//			int bin = h->FindBin(normset->at(u)[dim]);
//			if (bin > 0 && bin <= h->GetNbinsX()) {
//				h->Fill(normset->at(u)[dim], p->evaluate_for_plot(normset->at(u)) * normset->weight(u));
//			}
//		}
//	}
//	h->Scale(normalized_nevt() / h->Integral());
//
//	for (int u = 1; u <= h->GetNbinsX(); ++u) {
//		h->SetBinError(u, 0);
//	}
//}
//
//TH1F * plot::generate_hist(dataset * d, size_t dim)
//{
//	TH1F * h;
//	datahist * dh = dynamic_cast<datahist *>(d);
//	if (dh) {
//		h = (TH1F *)dh->source_hist()->Clone();
//	}
//	else {
//		int nbin = d->nbin() ? d->nbin() : 100;
//		double xmin = d->min(dim);
//		double xmax = d->max(dim);
//		h = new TH1F("unnamed", "", nbin, xmin, xmax);
//	}
//
//	h->SetName(Form("%p", h));
//	set_normhist(h);
//	set_option(h, "e same");
//	
//	return h;
//}
//
//TH1F * plot::generate_hist(pdf * p, size_t dim)
//{
//	TH1F * h;
//	projpdf * pp = dynamic_cast<projpdf *>(p);
//	if (pp) {
//		pp->normalize();
//		h = (TH1F *)pp->hist()->Clone();
//	}
//	else {
//		if (m_normhist) {
//			h = (TH1F *)m_normhist->Clone();
//			h->Reset();
//		}
//		else {
//			double xmin = p->normset()->min(dim);
//			double xmax = p->normset()->max(dim);
//			h = new TH1F("unnamed", "", 100, xmin, xmax);
//		}
//	}
//
//	h->SetName(Form("%p", h));
//	set_option(h, "hist same");
//	
//	return h;
//}
//
TH1F * plot::get_hist(const char * name)
{
	for (auto h: m_hist) {
		if (!strcmp(name, h->GetName())) return h;
	}
	MSG_ERROR("there is no histogram named", name, "on this plot");
	return 0;
}
//
//size_t plot::proj_dim()
//{
//	return plotcmd::project_dim();
//}
//
//void plot::set_normhist(TH1F * h)
//{
//	m_normhist = h;
//	m_nevt = h->Integral();
//}
