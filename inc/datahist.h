#ifndef DATAHIST_H__
#define DATAHIST_H__

#include "TH1.h"
#include "dataset.h"

class datahist: public dataset
{
	public:
		datahist(TH1 * h);
		datahist(const datahist & d) = delete;
		datahist & operator=(const datahist & d) = delete;
		virtual ~datahist();
		double edge_lo(int n) { return m_edge[n]; }
		double edge_hi(int n) { return m_edge[n+1]; }
		double err(int n) { return m_err[n]; }
		double err_down(int n) { return m_err_down[n]; }
		double err_up(int n) { return m_err_up[n]; }
		double width(int n) { return edge_hi(n) - edge_lo(n); }

	private:
		void aquire_resourse();
		bool init_from_h1d(TH1 * h1);
		void release_resourse();
	
	protected:
		double * m_edge;
		double * m_err;
		double * m_err_down;
		double * m_err_up;
};

#endif
