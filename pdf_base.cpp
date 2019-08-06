#include <iostream>
#include <cmath>
#include "data.h"
#include "pdf_base.h"

pdf_base::pdf_base(size_t d, const std::vector<std::string> & varname, data & data_norm):
	function_base(d, varname),
	m_data_norm(data_norm),
	m_norm(1)
{
}

pdf_base::~pdf_base()
{
}

double pdf_base::value(const std::vector<double> & point) const
{
	double v = evaluate(point);
	if (v < 0) {
		std::cout << "[pdf_base] warning: negative pdf value will be set to 0 by force" << std::endl;
		return 0;
	}
	return v*m_norm;
}

double pdf_base::log_value(const std::vector<double> & point) const
{
	double v = evaluate(point);
	if (v <= 0) {
		std::cout << "[pdf_base] warning: non-positive pdf value will return log_value = 0" << std::endl;
		return 0;
	}
	return log(v*m_norm);
}

double pdf_base::sum_on(const data & d) const
{
	if (dim() != d.dim()) {
		std::cout << "[pdf_base] error: dimension of pdf (" << dim() << ") does not match dimension of data (" << d.dim() << ")" << std::endl;
		return 0;
	}

	double sum = 0;
	for (size_t u = 0; u < d.size(); ++u) {
		if (d.at(u)) {
			sum += value(*d.at(u));
		}
		else {
			std::cout << "[pdf_base] warning: invalid data point will be ignored" << std::endl;
		}
	}
	return sum;
}

double pdf_base::log_sum_on(const data & d) const
{
	if (dim() != d.dim()) {
		std::cout << "[pdf_base] error: dimension of pdf (" << dim() << ") does not match dimension of data (" << d.dim() << ")" << std::endl;
		return 0;
	}

	double log_sum = 0;
	for (size_t u = 0; u < d.size(); ++u) {
		if (d.at(u)) {
			log_sum += log_value(*d.at(u));
		}
		else {
			std::cout << "[pdf_base] warning: invalid data point will be ignored" << std::endl;
		}
	}
	return log_sum;
}
		
int pdf_base::normalize()
{
	size_t size = m_data_norm.size();
	double sum = sum_on(m_data_norm);
	
	if (!size) {
		m_norm = 1;
		return 1; // null norm data
	}
	else if (sum == 0) {
		m_norm = 1;
		return 2; // sum on norm data is 0
	}
	else {
		m_norm = sum/size;
		return 0; // valid
	}
}
