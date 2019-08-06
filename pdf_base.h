#ifndef PDF_BASE_H__
#define PDF_BASE_H__

#include <vector>
#include <string>
#include "function_base.h"

class pdf_base: public function_base
{
	public:
		pdf_base(size_t d, const std::vector<std::string> & varname, data & data_norm);
		virtual ~pdf_base();
		virtual double evaluate(const std::vector<double> & point) const = 0;
		virtual double value(const std::vector<double> & point) const;
		virtual double log_value(const std::vector<double> & point) const;
		virtual double sum_on(const data & d) const;
		virtual double log_sum_on(const data & d) const;
		void set_nevt(variable & nevt) { m_nevt_name = nevt.name(); }

	protected:
		int normalize();

	protected:
		data & m_data_norm;
		double m_norm;
		std::string m_nevt_name;
		int m_status;
};

#endif
