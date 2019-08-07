#ifndef FUNCTION_BASE_H__
#define FUNCTION_BASE_H__

#include <vector>
#include <string>
#include "variable.h"

class dataset;

class function_base
{
	public:
		function_base(size_t d, const std::vector<std::string> & varname);
		virtual ~function_base();
		virtual double evaluate(const std::vector<double> & point) const = 0;
		double value(const std::vector<double> & point) const { return evaluate(point); }
		double sum_on(const dataset & d) const;
		size_t dim() const { return m_dim; }
		size_t npar() const { return m_varname.size(); }
		variable & get_par(int n) const { return variable::var(m_varname[n]); }
		virtual void read_from_pars(const std::vector<double> & par);
		virtual void read_from_pars(const double * par);
		virtual void write_to_pars(std::vector<double> & par) const;
		virtual void write_to_pars(double * par) const;

	protected:
		size_t m_dim;
		std::vector<std::string> m_varname;
};

#endif
