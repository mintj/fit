#ifndef ABSPDF_H__
#define ABSPDF_H__

#include <vector>
#include <memory>
#include <functional>

class absdata;
//class chi2fcn;
class datahist;
class dataset;
class nllfcn;
class plot;
class variable;

class abspdf
{
	public:
		abspdf(const char * name, const std::vector<const char *> & varlist, const std::vector<const char *> & paralist);
		virtual ~abspdf();
		
		// access to internal member
		size_t dim() { return m_dim; }
		int get_dim(const char * vname);
		int get_dim(variable * v);
		double get_para(int n);
		double max(size_t dim) { return m_max[dim]; }
		double min(size_t dim) { return m_min[dim]; }
		const char * name() { return m_name; }
		size_t npara() { return m_npara; }
		std::vector<variable *> & para_list() { return m_paralist; }
		std::vector<variable *> & var_list() { return m_varlist; }
		bool updated(); // check whether parameters' values are changed or not since last call
		
		// calculate value
		virtual double evaluate(const double * x) = 0;
		virtual double integral1d(double a, double b, double eps = 1e-6);
		virtual double norm() = 0;
		double recursive_simpson(double a, double b, double eps = 1e-6);
		double simpson(double a, double b);
		double operator()(const double * x);
		
		//// interface for chi2fit
		//void chi2fit(datahist & data, bool minos_err = false);
		//chi2fcn * create_chi2(datahist * data);

		// interface for nllfit
		virtual nllfcn * create_nll(absdata * data);
		virtual void fit(absdata & data, bool minos_err = false);
		
		// generate dataset
		dataset * generate(variable & x, size_t nevt = 1000, size_t ns = 100000);
		dataset * generate(const std::vector<variable> & xvec, size_t nevt = 1000, size_t ns = 100000);
		
		// plot related
		template<typename...T> void plot_on(plot * frame, T...action);

	protected:
		dataset * generate_eliminate_ambiguity(const std::vector<variable> & xvec, size_t nevt, size_t ns = 100000);
		virtual void update_lastvalue();

	protected:
		size_t m_dim;
		size_t m_npara;
		const char * m_name;
		std::vector<double> m_max;
		std::vector<double> m_min;
		std::vector<double> m_lastvalue;
		std::vector<variable *> m_varlist;
		std::vector<variable *> m_paralist;
		std::shared_ptr<nllfcn> m_nll;
};

#endif
