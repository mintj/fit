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
		
		static variable & var(const char * name);
	
	private:
		void add_to_pool();

	public:
		bool is_constant;
		double err;
		double err_down;
		double err_up;
		double limit_down;
		double limit_up;
		double value;
		const char * name;
		
		static std::map<const char *, variable *> var_pool;
};

#endif
