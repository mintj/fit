#ifndef VARIABLE_H__
#define VARIABLE_H__

#include <map>

class variable
{
	public:
		variable(const char * name, double v = 0);
		variable(const char * name, double d, double u);
		variable(const char * name, double v, double d, double u);
		virtual ~variable();
		bool constant() { return m_constant; }
		double err() { return m_err; }
		double err_down() { return m_err_down; }
		double err_up() { return m_err_up; }
		double limit_down() { return m_limit_down; }
		double limit_up() { return m_limit_up; }
		const char * name() { return m_name; }
		void set_constant(bool flag) { m_constant = flag; }
		void set_err(double v) { m_err = v; }
		void set_err_down(double v) { m_err_down = v; }
		void set_err_up(double v) { m_err_up = v; }
		void set_limit_down(double v) { m_limit_down = v; }
		void set_limit_up(double v) { m_limit_up = v; }
		void set_value(double v) { m_value = v; }
		double value() { return m_value; }
		
		static variable & var(const char * name);
	
	private:
		void add_to_pool();

	public:
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
