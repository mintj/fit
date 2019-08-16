# msfit

# 0. Introduction

In hadron physics, an model consists of (at least) three parts: dynamic, kinematic, and selection efficiency. Usually in practice only the dynamic part is of one's concern, while the latter two parts are hard to describe (parameterize) but cannot be ignored (espectially for physics near phase space's margin.

A widely adopted solution of this kind of problem is to construct PDF (dynamic) that is normalized on a Monte-Carlo (MC) sample. This MC sample is generated uniformly distributed in phase space (kinematic), and then applied event selection (efficiency).

This package aims at providing relative simple and easy-to-use C++ functions for users to construct PDFs described above.
It works within the ROOT enviroment, while its interface is designed to look like RooFit's.


# 1. Variable

    variable::variable(const char * name, double v = 0); 
    
    variable::variable(const char * name, double d, double u);
    
    variable::variable(const char * name, double v, double d, double u);


# 2. Data   

2.1 dataset
  
    dataset::dataset(TTree * t, const std::vector<const char *> & varname);
    
    dataset::dataset(TTree * t, const std::vector<const char *> & varname, const char * wname);
    
    void dataset::draw(TH1 * h, const char * option = "e", size_t x = 0, pdf * p = 0);
	
    void dataset::draw(TH1 * h, std::function<double(double *)> weight_func, const char * option = "e", size_t x = 0);
	
    void dataset::draw(TH2 * h, const char * option = "e", size_t x = 0, size_t y = 1, pdf * p = 0);
	
    void dataset::draw(TH2 * h, std::function<double(double *)> weight_func, const char * option = "e", size_t x = 0, size_t y = 1);

2.2 datahist

  _1d hist data, notice that 'datahist' is based on 'dataset', so an instance of this class can be passed to interfaces that need a dataset instance_
  
  _'draw' method is re-implemented in 'datahist':_
  
  _a) binning of input TH1 * object does not actually make sense, in any case, a copy of the source TH1 * object (that is used to initialize datahist) will be cloned to this input TH1 * object_
  
  _b) 'datahist' does not support 2d plot_
  
    datahist::datahist(TH1 * h);
    
    void datahist::draw(TH1 * h, const char * option = "", size_t x = 0, pdf * p = 0);
	
    void datahist::draw(TH1 * h, std::function<double(double *)> weight_func, const char * option = "", size_t x = 0);
	
    void datahist::draw(TH2 * h, const char * option = "", size_t x = 0, size_t y = 1, pdf * p = 0) = delete;
	
    void datahist::draw(TH2 * h, std::function<double(double *)> weight_func, const char * option = "", size_t x = 0, size_t y = 1) = delete;
    
# 3. PDF

3.1 pdf

  _abstract base of all PDFs:_
  
  _a) to use this class user must complete the 'evaluate' method_
  
  _b) this class also provides two interfaces fit (MLE) and chi2fit (LS), notice that chi2fit is restricted to datahist only_

    pdf::pdf(size_t dim, const std::vector<variable *> & vlist, dataset & normset);
    
    void pdf::chi2fit(datahist & data, bool minos_err = false);
    
    void pdf::draw(TH1 * h, TH1 * hnorm = 0, const char * option = "hist same");
    
    void pdf::draw(TH2 * h, TH2 * hnorm = 0, const char * option = "hist same");

    virtual double pdf::evaluate(const double * x) = 0;
    
    void pdf::fit(dataset & data, bool minos_err = false);
    
3.2 gaussian/breitwigner

  _two 1d examples for user defined PDFs_
  
    gaussian::gaussian(variable & m, variable & s, dataset & normset):
        pdf(1, {&m, &s}, normset)
    {
    }

    double gaussian::evaluate(const double * x)
    {
       double t = x[0];
        double m = get_par(0);
        double s = get_par(1);
        return exp(-(t-m)*(t-m)/2/s/s);
    }
     
    breitwigner::breitwigner(variable & m, variable & w, dataset & normset):
        pdf(1, {&m, &w}, normset)
    {
    }

    double breitwigner::evaluate(const double * x)
    {
        double t = x[0];
        double m = get_par(0);
        double w = get_par(1);
        return 1.0/((t-m)*(t-m)+0.25*w*w);
    }

3.3 addpdf

  _https://github.com/mintj/fit/issues/9#issuecomment-521021085_
  
    addpdf::addpdf(const std::vector<pdf *> plist, const std::vector<variable *> flist);
    
    void addpdf::draw_comp(TH1 * h, size_t n, TH1 * hnorm = 0, const char * option = "hist same");
	
    void addpdf::draw_comp(TH2 * h, size_t n, TH2 * hnorm = 0, const char * option = "hist same");
 
3.4 projpdf
    
  _abstract base of projection pdf, to use this class user must complete the 'func_weight' method, which is similar to the 'evaluate' method of base pdf_
  
  _another thing worthy be mentioned is that 'draw' method is re-implemented in 'projpdf', for the same reason as 'draw' of 'datahist' is re-implemented from its origin version in 'dataset'

    projpdf::projpdf(const std::vector<variable *> & vlist, dataset & normset, size_t pdim, size_t nbin, double lo, double hi);
    
    projpdf::projpdf(const std::vector<variable *> & vlist, dataset & normset, size_t pdim, size_t nbin, const double * binning);
    
    void projpdf::draw(TH1 * h, TH1 * hnorm = 0, const char * option = "hist same");
	
    void projpdf::draw(TH2 * h, TH2 * hnorm = 0, const char * option = "hist same") = delete;
    
    virtual double projpdf::func_weight(double x) = 0;

3.5 user defined pdf

  _refer to '3.2 gaussian/breitwigner'_


# 4. Simultaneous Fit

    simfit();
    
    simfit::add(pdf & p, dataset & d);
    
    void simfit::chi2fit(bool minos_err = false);
    
    void simfit::fit(bool minos_err = false);
    
    
# 5. Examples
  
  _df01_fit.cpp: fit a 1d-dataset with a 1d-pdf_
  
  _df02_addfit.cpp: fit a 1d-dataset with sum of two 1d-pdfs_

  _df03_simfit.cpp: simultaneous fit_

  _df04_chi2fit.cpp: chi2fit a 1d-datahist_

  _df05_chi2simfit.cpp: simultaneous chi2fit_

  _df06_weightedfit.cpp: fit a dataset with weight_

  _df07_2dfit.cpp: 2d fit_

  _df08_projpdf1.cpp: example of fit using project pdf_

  _df09_projpdf2.cpp: another example of fit using project pdf_

    
