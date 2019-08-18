#ifndef PLOT_H__
#define PLOT_H__

#include <vector>
#include <map>

class TGraph;
class TH1F;
class absdata;
class abspdf;
class dataset;
class variable;

class plot
{
	public:
		plot(variable & x);
		virtual ~plot();

		void add(absdata * d);
		void add(abspdf * p);
		void draw();
		TH1F * get_hist(const char * name);
		TH1F * normhist() { return m_normhist; }
		variable * var() { return m_var; }
		//double normalized_nevt() { return m_nevt; }
		//size_t proj_dim();
		//void set_option(TH1F * h, const char * option) { m_option[h] = option; }
		//void set_currpdf(pdf * p) { m_currpdf = p; }
	
	private:
		double calculate_hist_area(TH1F * h);
		void calculate_y_range();
		TGraph * create_graph_and_fill(abspdf * p);
		TH1F * create_hist_and_fill(absdata * d);
		void set_normhist(TH1F * h) { m_normhist = h; }
	
	private:
		double m_max_height;
		double m_min_height;
		std::vector<TGraph *> m_graph;
		std::vector<TH1F *> m_hist;
		std::map<TH1F *, const char *> m_option;
		TH1F * m_normhist;
		//pdf * m_currpdf;
		variable * m_var;
};

#endif
