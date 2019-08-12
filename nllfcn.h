#ifndef NLLFCN_H__
#define NLLFCN_H__

#include <vector>
#include <map>
#include "TMath.h"
#include "fcn.h"

class dataset;
class extpdf;
class pdf;
class variable;

class nllfcn: public fcn
{
	public:
		nllfcn(pdf * p, dataset * d);
		nllfcn(const std::vector<pdf *> plist, const std::vector<dataset *> dlist);
		virtual ~nllfcn();
		virtual double operator()(const std::vector<double> & par) const;
		virtual double Up() const { return 0.5; }
};

#endif
