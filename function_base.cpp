#include <iostream>
#include "function_base.h"
#include "dataset.h"

function_base::function_base(size_t d, const std::vector<std::string> & varname):
	m_dim(d),
	m_varname(varname.cbegin(), varname.cend())
{
}
		
function_base::~function_base()
{
}
		
double function_base::sum_on(const dataset & d) const
{
	if (dim() != d.dim()) {
		std::cout << "[function_base] error: dimension of function (" << dim() << ") does not match dimension of data (" << d.dim() << ")" << std::endl;
		return 0;
	}

	double sum = 0;
	for (size_t u = 0; u < d.size(); ++u) {
		if (d.at(u)) {
			sum += this->evaluate(*d.at(u));
		}
		else {
			std::cout << "[function_base] warning: invalid data point will be ignored" << std::endl;
		}
	}
	return sum;
}
		
void function_base::read_from_pars(const std::vector<double> & par)
{
	if (npar() == par.size()) {
		for (size_t u = 0; u < par.size(); ++u) {
			get_par(u).set_val(par[u]);
		}
	}
	else {
		std::cout << "[function_base] error: npar of function (" << npar() << ") does not match size of parameters (" << par.size() << ")" << std::endl;
	}
}

void function_base::read_from_pars(const double * par)
{
	for (size_t u = 0; u < npar(); ++u) {
		get_par(u).set_val(par[u]);
	}
}

void function_base::write_to_pars(std::vector<double> & par) const
{
	if (npar() == par.size()) {
		for (size_t u = 0; u < par.size(); ++u) {
			par[u] = get_par(u).value();
		}
	}
	else {
		std::cout << "[function_base] error: npar of function (" << npar() << ") does not match size of parameters (" << par.size() << ")" << std::endl;
	}
}

void function_base::write_to_pars(double * par) const
{
	for (size_t u = 0; u < npar(); ++u) {
		par[u] = get_par(u).value();
	}
}
