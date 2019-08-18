#ifndef DATAHIST_H__
#define DATAHIST_H__

#include "absdata.h"

class TH1F;
class variable;

class datahist: public absdata
{
	public:
		datahist(const char * name, variable & x, TH1 * h);
		virtual ~datahist();
		
		double edge_lo(int n) { return m_edge[n]; }
		double edge_hi(int n) { return m_edge[n+1]; }
		double err(int n) { return m_err[n]; }
		double err_down(int n) { return m_err_down[n]; }
		double err_up(int n) { return m_err_up[n]; }
		int find_bin(double x);
		TH1F * hist() { return m_hist; }
		double width(int n) { return edge_hi(n) - edge_lo(n); }
		
		void fill_hist(TH1 * h, size_t dim) = delete;
		void fill_hist(TH2 * h, size_t dimx, size_t dimy) = delete;
		void project(TH1 * h, size_t dim) = delete;
		void project(TH2 * h, size_t dimx, size_t dimy) = delete;

	private:
		void init();
	
	protected:
		TH1F * m_hist;
		std::vector<double> m_edge;
		std::vector<double> m_err;
		std::vector<double> m_err_down;
		std::vector<double> m_err_up;
};

#endif
