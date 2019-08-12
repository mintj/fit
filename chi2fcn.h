#ifndef CHI2FCN_H__
#define CHI2FCN_H__

#include <vector>
#include <map>
#include "TMath.h"
#include "fcn.h"

class datahist;
class extpdf;
class variable;

class chi2fcn: public fcn
{
	public:
		chi2fcn(extpdf * p, datahist * d);
		chi2fcn(const std::vector<extpdf *> plist, const std::vector<datahist *> dlist);
		virtual ~chi2fcn();
		virtual double operator()(const std::vector<double> & par) const;
		virtual double Up() const { return 1.0; }
};

#endif
