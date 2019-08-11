#ifndef SIMFIT_H__
#define SIMFIT_H__

#include <vector>
#include <memory>

class dataset;
class nll;
class pdf;

class simfit
{
	public:
		simfit(const std::vector<pdf *> & pdflist, const std::vector<dataset *> & datalist);
		virtual ~simfit();
		nll * create_nll();
		void fit(bool minos_err = false);

	protected:
		std::vector<dataset *> m_dlist;
		std::vector<pdf *> m_plist;
		std::shared_ptr<nll> m_nll;
};

#endif
