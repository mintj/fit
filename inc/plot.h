#ifndef PLOT_H__
#define PLOT_H__

#include <vector>
#include <map>
#include <functional>
#include "TH1F.h"

class dataset;
class pdf;

class plot
{
	public:
		plot();
		virtual ~plot();
		
		template <typename ... T> void add(TH1F * h, const T & ... action);
		void draw();
		TH1F * generate_hist(dataset * d, size_t dim);
		TH1F * generate_hist(pdf * p, size_t dim);
		TH1F * get_hist(const char * name);
		TH1F * normhist() { return m_normhist; }
		double normalized_nevt() { return m_nevt; }
		size_t proj_dim();
		void set_normhist(TH1F * h);
		void set_option(TH1F * h, const char * option) { m_option[h] = option; }
		void set_currpdf(pdf * p) { m_currpdf = p; }
	
	private:
		double m_nevt;
		std::vector<TH1F *> m_hist;
		std::map<TH1F *, const char *> m_option;
		TH1F * m_normhist;
		pdf * m_currpdf;
};

#endif
