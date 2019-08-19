#ifndef ABSDATA_H__
#define ABSDATA_H__

#include <vector>

class TH1;
class TH2;
class plot;
class variable;

// actually this is not an abstract class ...
class absdata
{
	public:
		absdata(const char * name, variable & x, size_t size = 0);
		absdata(const char * name, const std::vector<variable> & xvec, size_t size = 0);
		virtual ~absdata();

		// access to data
		double * at(size_t n) { return &m_arr[n]; }
		size_t dim() { return m_dim; }
		int get_dim(const char * vname);
		int get_dim(variable * v);
		size_t size() { return m_arr.size(); }
		double max(size_t dim = 0) { return m_max[dim]; }
		double min(size_t dim = 0) { return m_min[dim]; }
		const char * name() { return m_name; }
		double nevt();
		double weight(size_t n) { return m_weight[n]; }

		// plot related
		template <typename...T> void plot_on(plot * frame, T...command);
		void fill_hist(TH1 * h, size_t dim);
		void fill_hist(TH2 * h, size_t dimx, size_t dimy);
		void project(TH1 * h, size_t dim);
		void project(TH2 * h, size_t dimx, size_t dimy);
	
	private:
		void init(size_t size);

	protected:
		size_t m_dim;
		const char * m_name;
		std::vector<double> m_max;
		std::vector<double> m_min;
		std::vector<double> m_arr;
		std::vector<double> m_weight;
		std::vector<variable *> m_varlist;
};

#endif
