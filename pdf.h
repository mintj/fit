#ifndef PDF_H__
#define PDF_H__

#include <vector>
#include <map>
#include <memory>

class dataset;
class datahist;
class nll;
class variable;

class pdf
{
	public:
		pdf(size_t dim, const std::vector<variable *> & var, dataset * normset = 0);
		pdf(const pdf & p) = default;
		pdf & operator=(const pdf & p) = default;
		virtual ~pdf();
		nll * create_nll(dataset * data);
		size_t dim() { return m_dim; }
		virtual double evaluate(const double * x) = 0;
		//fitresult * fit(dataset * data, bool minos_err = false);
		void fit(dataset * data, bool minos_err = false);
		double get_par_ext(int n);
		double get_par_int(int n);
		variable * get_var_ext(int n) { return m_var_ext[n]; }
		variable * get_var_int(int n) { return m_var_int[n]; }
		virtual double log_sum(dataset * data);
		virtual double norm();
		size_t npar_ext() { return m_var_ext.size(); }
		size_t npar_int() { return m_var_int.size(); }
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
		std::vector<double> m_var_int_lastvalue;
		std::vector<variable *> m_var_ext;
		std::vector<variable *> m_var_int;
		std::map<variable *, int> m_vcount;
		std::shared_ptr<nll> m_nll;
		dataset * m_normset;
};

#endif
