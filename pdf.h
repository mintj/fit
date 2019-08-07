#ifndef PDF_H__
#define PDF_H__

#include <vector>
#include <map>

class dataset;
class datahist;
class fitr;
class variable;

class pdf
{
	public:
		pdf(size_t dim, const std::vector<variable *> & var, dataset * normset);
		virtual ~pdf();
		size_t dim() { return m_dim; }
		virtual double evaluate(const double * x, const std::vector<double> & par) = 0;
		variable * get_var(int n) { return m_var[n]; }
		variable * get_var(const char * name);
		double log_sum(dataset * data, const std::vector<double> & par);
		double norm(const std::vector<double> & par);
		size_t npar() { return m_npar; }
		double sum(dataset * data, const std::vector<double> & par);

	protected:
		size_t m_dim;
		size_t m_npar;
		std::vector<variable *> m_var;
		std::map<const char *, int> m_vmap;
		dataset * m_normset;
};

#endif
