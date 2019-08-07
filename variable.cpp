#include <iostream>
#include "variable.h"

variable::variable(const char * name, double v)
{
	value = v;
	limit_down = value;
	limit_up = value;
	err = 0;
	err_down = 0;
	err_up = 0;
	is_constant = true;
	add_to_pool();
}

variable::variable(const char * name, double d, double u)
{
	value = (d+u)/2;
	limit_down = d;
	limit_up = u;
	err = 0.1*(u-d);
	err_down = err;
	err_up = err;
	is_constant = false;
	add_to_pool();
}

variable::variable(const char * name, double v, double d, double u)
{
	value = v;
	limit_down = d;
	limit_up = u;
	err = 0.1*(u-d);
	err_down = err;
	err_up = err;
	is_constant = false;
	add_to_pool();
}
		
variable::~variable()
{
}

void variable::add_to_pool()
{
	if (var_pool.find(name) != var_pool.end()) {
		std::cout << "warning: variable named [" << name << "] already exists, the old one will be overwritten" << std::endl;
	}
	var_pool[name] = this;
}
	
variable & variable::var(const char * name)
{
	if (var_pool.find(name) == var_pool.end()) {
		std::cout << "warning: variable named [" << name << "] does not exist, a new one will be created" << std::endl;
		variable tmp(name);
	}
	return *var_pool.find(name)->second;
}

std::map<const char *, variable *> variable::var_pool;
