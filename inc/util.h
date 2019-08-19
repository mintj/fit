#ifndef UTIL_H__
#define UTIL_H__

#include <iostream>
#include "TROOT.h"

#define MSG_DEBUG(...) \
	{\
		std::cout << "[debug] " << __FILE__ << ": " << __LINE__ << ": ";\
		msfit::msg(__VA_ARGS__);\
	}

#define MSG_ERROR(...) \
	{\
		std::cout << "[error] " << __FILE__ << ": " << __LINE__ << ": ";\
		msfit::msg(__VA_ARGS__);\
	}

#define MSG_INFO(...) \
	{\
		std::cout << "[info] " << __FILE__ << ": " << __LINE__ << ": ";\
		msfit::msg(__VA_ARGS__);\
	}

#define MSG_WARNING(...) \
	{\
		std::cout << "[warning] " << __FILE__ << ": " << __LINE__ << ": ";\
		msfit::msg(__VA_ARGS__);\
	}

namespace msfit
{
	// message
	template <typename T> void msg(const T & text);
	template <typename T, typename...R> void msg(const T & text, const R & ...rest);

	// plot command
	//void * components(int n);
	//template <typename ... T> void * components(int n, T ... rest);
	void * linecolor(Color_t n);
	void * linestyle(Style_t n);
	void * linewidth(Width_t n);
	void * markercolor(Color_t n);
	void * markersize(Size_t n);
	void * markerstyle(Style_t n);
	void * name(const char * name);
	void * title(const char * name);
}

#endif
