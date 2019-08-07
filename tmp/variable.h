#ifndef VARIABLE_H__
#define VARIABLE_H__

#include <string>
#include <map>

class variable
{
	public:
		variable(const std::string & name);
		variable(const std::string & name, double v);
		variable(const std::string & name, double down, double up);
		variable(const std::string & name, double value, double down, double up);
		virtual ~variable();
		std::string name() const { return m_name; }
		double value() const { return m_value; }
		double down_limit() const { return m_down; }
		double up_limit() const { return m_up; }
		double err() const { return m_err; }
		double err_down() const { return m_err_down; }
		double err_up() const { return m_err_up; }
		bool is_constant() const { return m_is_const; }
		void set_val(double v) { m_value = v; }
		void set_limit(double down, double up) { m_down = down; m_up = up; }
		void set_err(double e) { m_err = e; }
		void set_err_down(double e) { m_err_down = e; }
		void set_err_up(double e) { m_err_up = e; }
		void set_const() { m_is_const = true; }
		
	protected:
		void add_to_pool();
	
	protected:
		std::string m_name;
		double m_value;
		double m_down;
		double m_up;
		double m_err;
		double m_err_down;
		double m_err_up;
		bool m_is_const;

	public:
		static variable & var(const std::string & name);

	private:
		static std::map<std::string, variable *> var_pool;
};

#endif
