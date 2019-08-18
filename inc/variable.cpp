#include <iostream>

#include "variable.h"
#include "plot.h"
#include "util.h"

variable::variable(const char * name, double v)
{
	m_name = name;
	m_value = v;
	m_limit_down = v;
	m_limit_up = v;
	m_err = 0;
	m_err_down = 0;
	m_err_up = 0;
	m_constant = true;
	m_bins = 100;
	add_to_pool();
}

variable::variable(const char * name, double d, double u)
{
	m_name = name;
	m_value = (d+u)/2;
	m_limit_down = d;
	m_limit_up = u;
	m_err = 0.1*(u-d);
	m_err_down = m_err;
	m_err_up = m_err;
	m_constant = false;
	m_bins = 100;
	add_to_pool();
}

variable::variable(const char * name, double v, double d, double u)
{
	m_name = name;
	m_value = v;
	m_limit_down = d;
	m_limit_up = u;
	m_err = 0.1*(u-d);
	m_err_down = m_err;
	m_err_up = m_err;
	m_constant = false;
	m_bins = 100;
	add_to_pool();
}
		
variable::~variable()
{
}

void variable::add_to_pool()
{
	if (var_pool.find(m_name) != var_pool.end()) {
		MSG_WARNING("variable [", m_name, "] already exists, will be overwritten by new one");
	}
	var_pool[m_name] = this;
}

plot * variable::frame()
{
	plot * f = new plot(*this);
	return f;
}

variable * variable::var(const char * name)
{
	if (var_pool.find(name) == var_pool.end()) {
		MSG_ERROR("variable '", name, "' does not exist");
		return 0;
	}
	return var_pool.find(name)->second;
}

std::map<const char *, variable *> variable::var_pool;
