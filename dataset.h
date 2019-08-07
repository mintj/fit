#ifndef DATASET_H__
#define DATASET_H__

#include <vector>
#include <string>
#include "TTree.h"

class dataset
{
	public:
		dataset(size_t s, size_t d);
		dataset(size_t s, const std::vector<const char *> & varname);
		dataset(TTree * t, const std::vector<const char *> & varname);
		dataset(TTree * t, const std::vector<const char *> & varname, const char * wname);
		dataset(const dataset & d) = delete;
		dataset & operator=(const dataset & d) = delete;
		virtual ~dataset();
		const double * at(size_t n) { return m_arr+n; }
		size_t dim() { return m_dim; }
		const char * name(size_t n) { return m_varname[n].c_str(); }
		void set_name(size_t n, const char * name) { m_varname[n] = name; }
		void set_val(size_t n, double v, double w) { m_arr[n] = v; m_weight[n] = w; }
		size_t size() { return m_size; }
		double weight_at(size_t n) { return m_weight[n]; }

	private:
		void aquire_resourse();
		bool init_from_tree(TTree * t, const std::vector<const char *> & varname);
		bool init_from_tree(TTree * t, const std::vector<const char *> & varname, const char * wname);
		void release_resourse();
	
	protected:
		std::string * m_varname;
		double * m_arr;
		double * m_weight;
		size_t m_size;
		size_t m_dim;
};

#endif
