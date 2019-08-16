#ifndef DATASET_H__
#define DATASET_H__

#include <vector>
#include "TH2.h"
#include "TTree.h"

class pdf;
class plot;

class dataset
{
	public:
		dataset(size_t s, size_t d);
		dataset(TTree * t, const std::vector<const char *> & varname);
		dataset(TTree * t, const std::vector<const char *> & varname, const char * wname);
		dataset(const dataset & d) = delete;
		dataset & operator=(const dataset & d) = delete;
		virtual ~dataset();
		
		double * at(size_t n) { return m_arr+n*m_dim; }
		size_t dim() { return m_dim; }
		double nevt() { return m_wsize; }
		template<typename... T> void plot1d(size_t dim, plot * frame, T... action);
		void plot2d(size_t dimx, size_t dimy, TH2 * h);
		void set_nbin(size_t n) { m_nbin = n; }
		void set_val(size_t n, size_t d, double v) { m_arr[n*m_size+d] = v; }
		void set_weight(size_t n, double w) { m_weight[n] = w; }
		size_t size() { return m_size; }
		double weight(size_t n) { return m_weight[n]; }
		
		virtual double max(int n = 0);
		virtual double min(int n = 0);

	private:
		void acquire_resourse();
		bool init_from_tree(TTree * t, const std::vector<const char *> & varname);
		bool init_from_tree(TTree * t, const std::vector<const char *> & varname, const char * wname);
		void release_resourse();
	
	protected:
		size_t m_nbin;
		size_t m_dim;
		size_t m_size;
		double m_wsize;
		double * m_arr;
		double * m_weight;
};

#endif
