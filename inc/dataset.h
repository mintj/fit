#ifndef DATASET_H__
#define DATASET_H__

#include "TTree.h"
#include "absdata.h"

class variable;

class dataset: public absdata
{
	public:
		dataset(const char * name, variable & x);
		dataset(const char * name, const std::vector<variable> & xvec);
		dataset(const char * name, TTree * t, variable & x, const char * wname = 0);
		dataset(const char * name, TTree * t, const std::vector<variable> & xvec, const char * wname = 0);
		virtual ~dataset();
		
		void add(const double * x, double w = 1);

	private:
		bool init(TTree * t, const char * wname = 0);
};

#endif
