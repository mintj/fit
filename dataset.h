#ifndef DATASET_H__
#define DATASET_H__

#include <vector>
#include "TTree.h"

class dataset
{
	public:
		dataset(size_t s, size_t d);
		dataset(TTree * t, const std::vector<const char *> & varname);
		dataset(TTree * t, const std::vector<const char *> & varname, const char * wname);
		dataset(const dataset & d) = delete;
		dataset & operator=(const dataset & d) = delete;
		virtual ~dataset();
		const double * at(size_t n) { return m_arr+n*m_dim; }
		size_t dim() { return m_dim; }
		void set_val(size_t n, size_t d, double v) { m_arr[n*m_size+d] = v; }
		void set_weight(size_t n, double w) { m_weight[n] = w; }
		size_t size() { return m_size; }
		double weight(size_t n) { return m_weight[n]; }

	private:
		void aquire_resourse();
		bool init_from_tree(TTree * t, const std::vector<const char *> & varname);
		bool init_from_tree(TTree * t, const std::vector<const char *> & varname, const char * wname);
		void release_resourse();
	
	protected:
		size_t m_dim;
		size_t m_size;
		double * m_arr;
		double * m_weight;
};

#endif
