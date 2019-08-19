#ifndef NLLFCN_H__
#define NLLFCN_H__

#include <vector>
#include "absfcn.h"

class datahist;
class dataset;
class abspdf;

class nllfcn: public absfcn
{
	public:
		nllfcn() = default;
		nllfcn(abspdf * p, absdata * d);
		virtual ~nllfcn();

		void minimize(bool minos_err = false);
		
		virtual double operator()(const std::vector<double> & par) const;
		virtual double Up() const { return 0.5; }

	private:
		bool init();

	protected:
		mutable std::vector<double> m_arr_logsum;
		mutable std::vector<std::vector<double>> m_arr_rdata;
};

#endif
