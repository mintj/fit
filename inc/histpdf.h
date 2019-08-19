#ifndef HISTPDF_H__
#define HISTPDF_H__

#include "TH1.h"
#include "abspdf.h"

class variable;

class histpdf: public abspdf
{
	public:
		histpdf(const char * name, variable & x, TH1 * h);
		virtual ~histpdf();

	protected:
		TH1F * m_hist;

		abspdf(const char * name, const std::vector<const char *> & varlist, const std::vector<const char *> & paralist);
		size_t m_dim;
		size_t m_npara;
		const char * m_name;
		std::vector<double> m_max;
		std::vector<double> m_min;
		std::vector<double> m_lastvalue;
		std::vector<variable *> m_varlist;
		std::vector<variable *> m_paralist;
};

#endif
