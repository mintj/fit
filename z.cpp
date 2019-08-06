#include "Minuit2/FCNBase.h"
#include "Minuit2/MnUserParameterState.h"
#include "Minuit2/MnMigrad.h"

class ri: public ROOT::Minuit2::FCNBase
{
	public:
		ri() {}
		virtual ~ri() {}
		virtual double Up() const { return 0.5; }
		virtual double operator()(const std::vector<double> & par) const;
};

double ri::operator()(const std::vector<double> & par) const
{
	double x = par[0];
	double y = par[1];
	return (x-0.5)*(x-0.5) + (y-2.5)*(y-2.5);
}

int main()
{
	ROOT::Minuit2::MnUserParameters upar;
	ROOT::Minuit2::MnMigrad migrad(ri, upar);
}
