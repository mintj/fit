#ifndef DATA_H__
#define DATA_H__

#include <vector>
#include <map>
#include <string>
#include "variable.h"

class data
{
	public:
		data(const std::string & name);
		data(const std::vector<std::string> & names);
		data(const variable & var);
		data(const std::vector<variable> & vars);
		virtual ~data();
		bool add(const std::vector<double> & vec, double w = 1, double e = 1);
		bool add(double * arr, double w = 1, double e = 1);
		double get(const std::string & var_name, int n) const;
		const std::vector<double> * at(int n) const;
		double err_at(int n) const { return m_err.at(n); }
		double err_down_at(int n) const { return m_err_down.at(n); }
		double err_up_at(int n) const { return m_err_up.at(n); }
		double weight_at(int n) const { return m_weight.at(n); }
		size_t dim() const { return m_varmap.size(); }
		size_t size() const { return m_point.size(); }
	
	protected:
		std::map<std::string, int> m_varmap;
		std::vector<std::vector<double>> m_point;
		std::vector<double> m_weight;
		std::vector<double> m_err;
		std::vector<double> m_err_up;
		std::vector<double> m_err_down;
};

#endif
