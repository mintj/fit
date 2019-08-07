#ifndef PDF_BASE_H__
#define PDF_BASE_H__

#include <vector>
#include <string>
#include "function_base.h"

class pdf_base: public function_base
{
	public:
		pdf_base(size_t d, const std::vector<std::string> & varname, dataset & data_norm);
		virtual ~pdf_base();
		virtual double evaluate(const std::vector<double> & point) const = 0;
		double value(const std::vector<double> & point, bool norm = true) const;
		double log_value(const std::vector<double> & point, bool norm = true) const;
		double sum_on(const dataset & d) const;
		double log_sum_on(const dataset & d) const;
		double norm() const { return m_norm; }
		void set_nevt(variable & nevt) { m_nevt_name = nevt.name(); }
		virtual void read_from_pars(const std::vector<double> & par);
		virtual void read_from_pars(const double * par);

	protected:
		int normalize();

	protected:
		dataset & m_data_norm;
		double m_norm;
		std::string m_nevt_name;
		int m_status;
};

#endif
