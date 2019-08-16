#ifndef PDF_H__
#define PDF_H__

#include <vector>
#include <memory>
#include "TH1.h"
#include "TH2.h"

class chi2fcn;
class datahist;
class dataset;
class nllfcn;
class plot;
class variable;

class pdf
{
	public:
		pdf(size_t dim, const std::vector<variable *> & vlist, dataset & normset);
		pdf(const pdf & p) = default;
		pdf & operator=(const pdf & p) = default;
		virtual ~pdf();
		
		void chi2fit(datahist & data, bool minos_err = false);
		chi2fcn * create_chi2(datahist * data);
		nllfcn * create_nll(dataset * data);
		size_t dim() { return m_dim; }
		void fit(dataset & data, bool minos_err = false);
		double get_lastvalue(int n);
		std::vector<double> & get_lastvalues();
		double get_par(int n);
		variable * get_var(int n);
		std::vector<variable *> & get_vars();
		dataset * normset() { return m_normset; }
		size_t npar() { return m_varlist.size(); }
		double operator()(double * x);
		template<typename... T> void plot_on(plot * frame, T... action);
		
		virtual double evaluate(const double * x) = 0;
		virtual double evaluate_for_plot(const double * x) { return evaluate(x); }
		virtual double integral(double a, double b, int n = 0);
		virtual double log_sum(dataset * data);
		virtual double nevt() { return 1; }
		virtual double norm();
		virtual void set_normset(dataset & normset);
		virtual double sum(dataset * data);
		virtual bool updated(); // check whether parameters' values are changed or not since last call
		
		static double calculate_area(TH1 * h);

	protected:
		pdf();
		virtual void update_lastvalue();
		int normalize();

	protected:
		bool m_normalized;
		size_t m_dim;
		int m_status;
		double m_norm;
		std::vector<double> m_lastvalue;
		std::vector<variable *> m_varlist;
		std::shared_ptr<chi2fcn> m_chi2;
		std::shared_ptr<nllfcn> m_nll;
		dataset * m_normset;
};

#endif
