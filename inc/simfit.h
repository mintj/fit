#ifndef SIMFIT_H__
#define SIMFIT_H__

#include <vector>
#include <memory>

class addpdf;
class chi2fcn;
class dataset;
class nllfcn;
class pdf;

class simfit
{
	public:
		simfit();
		virtual ~simfit();
		void add(pdf & p, dataset & d);
		void chi2fit(bool minos_err = false);
		nllfcn * create_nll();
		chi2fcn * create_chi2();
		void fit(bool minos_err = false);

	protected:
		std::vector<dataset *> m_dlist;
		std::vector<pdf *> m_plist;
		std::shared_ptr<nllfcn> m_nll;
		std::shared_ptr<chi2fcn> m_chi2;
};

#endif
