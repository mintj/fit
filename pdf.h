#ifndef PDF_H__
#define PDF_H__

#include <vector>
#include <map>
#include <memory>

class datahist;
class dataset;
class fitresult;
class nllfcn;
class variable;

class pdf
{
	public:
		pdf(size_t dim, const std::vector<variable *> & var, dataset * normset = 0);
		pdf(const pdf & p) = default;
		pdf & operator=(const pdf & p) = default;
		virtual ~pdf();
		nllfcn * create_nll(dataset * data);
		size_t dim() { return m_dim; }
		virtual double evaluate(const double * x) = 0;
		fitresult * fit(dataset * data, bool minos_err = false);
		double get_lastvalue(int n);
		std::vector<double> & get_lastvalues();
		double get_par(int n);
		variable * get_var(int n);
		std::vector<variable *> & get_vars();
		virtual double integral(double a, double b, int n = 0);
		virtual double log_sum(dataset * data);
		virtual double nevt() { return 1; }
		virtual double norm();
		size_t npar() { return m_varlist.size(); }
		virtual double operator()(const double * x);
		virtual void set_normset(dataset * normset);
		virtual double sum(dataset * data);

	protected:
		pdf();
		bool updated();
		int normalize();

	protected:
		bool m_updated;
		size_t m_dim;
		int m_status;
		double m_norm;
		std::vector<double> m_lastvalue;
		std::vector<variable *> m_varlist;
		std::shared_ptr<nllfcn> m_nll;
		dataset * m_normset;
};

#endif
