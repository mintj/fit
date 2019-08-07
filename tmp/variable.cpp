#include <iostream>
#include "variable.h"

variable::variable(const std::string & name):
	m_name(name)
{
	m_down = 0;
	m_up = 0;
	m_value = 0;
	m_err = 0.1 * (m_up - m_down);
	m_err_down = m_err;
	m_err_up = m_err;
	m_is_const = false;
	add_to_pool();
}
		
variable::variable(const std::string & name, double v):
	m_name(name),
	m_value(v),
	m_down(v),
	m_up(v)
{
	m_err = 0.1 * (m_up - m_down);
	m_err_down = m_err;
	m_err_up = m_err;
	m_is_const = true;
	add_to_pool();
}

variable::variable(const std::string & name, double down, double up):
	m_name(name),
	m_down(down),
	m_up(up)
{
	m_value = (down+up)/2;
	m_err = 0.1 * (m_up - m_down);
	m_err_down = m_err;
	m_err_up = m_err;
	m_is_const = false;
	add_to_pool();
}

variable::variable(const std::string & name, double value, double down, double up):
	m_name(name),
	m_value(value),
	m_down(down),
	m_up(up)
{
	m_err = 0.1 * (m_up - m_down);
	m_err_down = m_err;
	m_err_up = m_err;
	m_is_const = false;
	add_to_pool();
}
		
variable::~variable()
{
}

void variable::add_to_pool()
{
	if (var_pool.find(m_name) != var_pool.end()) {
		std::cout << "warning: variable named [" << m_name << "] already exists, the old one will be overwritten" << std::endl;
	}
	var_pool[m_name] = this;
}

variable & variable::var(const std::string & name)
{
	if (var_pool.find(name) == var_pool.end()) {
		std::cout << "warning: variable named [" << name << "] does not exist, a new one will be created" << std::endl;
		variable tmp(name);
	}
	return *var_pool.find(name)->second;
}

std::map<std::string, variable *> variable::var_pool;
