#ifndef VARIABLE_H__
#define VARIABLE_H__

#include <map>

class plot;

class variable
{
	public:
		variable(const char * name, double v = 0);
		variable(const char * name, double d, double u);
		variable(const char * name, double v, double d, double u);
		virtual ~variable();
		
		size_t bins() const { return m_bins; }
		bool constant() const { return m_constant; }
		double err() const { return m_err; }
		double err_down() const { return m_err_down; }
		double err_up() const { return m_err_up; }
		plot * frame();
		double limit_down() const { return m_limit_down; }
		double limit_up() const { return m_limit_up; }
		const char * name() const { return m_name; }
		double value() const { return m_value; }
		
		void set_constant(bool flag) { m_constant = flag; }
		void set_err(double v) { m_err = v; }
		void set_err_down(double v) { m_err_down = v; }
		void set_err_up(double v) { m_err_up = v; }
		void set_limit_down(double v) { m_limit_down = v; }
		void set_limit_up(double v) { m_limit_up = v; }
		void set_bins(size_t n) { m_bins = n; }
		void set_value(double v) { m_value = v; }
		
		static variable * var(const char * name);
	
	private:
		void add_to_pool();

	private:
		size_t m_bins;
		bool m_constant;
		double m_err;
		double m_err_down;
		double m_err_up;
		double m_limit_down;
		double m_limit_up;
		double m_value;
		const char * m_name;
		
		static std::map<const char *, variable *> var_pool;
};

#endif
