#include <iostream>
#include "data.h"
		
data::data(const std::string & name)
{
	m_varmap[name] = 0;
}

data::data(const std::vector<std::string> & names)
{
	int id = 0;
	for (auto name: names) {
		if (m_varmap.find(name) == m_varmap.end()) {
			m_varmap[name] = id;
			++id;
		}
		else {
			std::cout << "[data] warning: ignore duplicate variable name [" << name << "]" << std::endl; 
		}
	}
}

data::data(const variable & var)
{
	m_varmap[var.name()] = 0;
}

data::data(const std::vector<variable> & vars)
{
	int id = 0;
	for (auto var: vars) {
		auto name = var.name();
		if (m_varmap.find(name) == m_varmap.end()) {
			m_varmap[name] = id;
			++id;
		}
		else {
			std::cout << "[data] warning: ignore duplicate variable name [" << name << "]" << std::endl; 
		}
	}
}

data::~data()
{
}

bool data::add(const std::vector<double> & vec, double w, double e)
{
	int d = dim();
	if (d > vec.size()) {
		std::cout << "[data] error: number of provided values (" << vec.size() << ") is smaller than data's dimension (" << d << ")" << std::endl;
		return false;
	}
	else if (d < vec.size()) {
		std::cout << "[data] error: number of provided values (" << vec.size() << ") is larger than data's dimension (" << d << ")" << std::endl;
	}

	m_point.push_back(std::vector<double>(vec.cbegin(), vec.cend()));
	m_weight.push_back(w);
	m_err.push_back(e);
	return true;
}

bool data::add(double * arr, double w, double e)
{
	if (!arr) return false;

	std::vector<double> p;
	for (int u = 0; u < dim(); ++u) {
		p.push_back(arr[u]);
	}
	m_point.push_back(std::move(p));
	m_weight.push_back(w);
	m_err.push_back(e);
	return true;
}

double data::get(const std::string & var_name, int n) const
{
	if (m_varmap.find(var_name) == m_varmap.end()) {
		std::cout << "[data] error: invalid variable name [" << var_name << "]" << std::endl;
		return 0;
	}
	else if (n >= size()) {
		std::cout << "[data] error: required index (" << n << ") is out of range (0, " << size()-1 << ")" << std::endl;
		return 0;
	}
	else {
		return m_point.at(n).at(m_varmap.find(var_name)->second);
	}
}
		
const std::vector<double> * data::at(int n) const
{
	if (n >= size()) {
		std::cout << "[data] error: required index (" << n << ") is out of range (0, " << size()-1 << ")" << std::endl;
		return 0;
	}
	return &m_point.at(n);
}
