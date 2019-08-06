#ifndef MN_HELPER_H__
#define MN_HELPER_H__

#include "Minuit2/MnUserParameters.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnMinos.h"
#include "nll.h"

class mn_helper
{
	public:
		//mn_helper(nll & fcn);
		mn_helper();
		virtual ~mn_helper();
		void migrad();
		void minos();
		void minimize(bool minos_err = false);

	protected:
		//ROOT::Minuit2::FCNBase & m_fcn;
		ROOT::Minuit2::MnUserParameters * m_mn_upar;
		ROOT::Minuit2::MnMigrad * m_mn_migrad;
		ROOT::Minuit2::FunctionMinimum * m_mn_min;
		ROOT::Minuit2::MnMinos * m_mn_minos;
};

#endif
