#include "TAttLine.h"
#include "TAttMarker.h"
#include "TNamed.h"
#include "plotcmd.h"
#include "util.h"

// message
template <typename T>
void msfit::msg(const T & text)
{
	std::cout << text << std::endl;
}

template <typename T, typename...R>
void msfit::msg(const T & text, const R & ...rest)
{
	std::cout << text << " ";
	msfit::msg(rest...);
}

// plot command
//void * msfit::components(int n)
//{
//	plotcmd::components_to_draw().insert(n);
//	return 0;
//}
//
//template <typename...T>
//void * msfit::components(int n, T ...rest)
//{
//	plotcmd::components_to_draw().insert(n);
//	msfit::components(rest...);
//	return 0;
//}

void * msfit::linecolor(Color_t n)
{
	plotcmd::add_cmd_for_line([n](TAttLine * obj) { obj->SetLineColor(n); });
	return 0;
}

void * msfit::linestyle(Style_t n)
{
	plotcmd::add_cmd_for_line([n](TAttLine * obj) { obj->SetLineStyle(n); });
	return 0;
}

void * msfit::linewidth(Width_t n)
{
	plotcmd::add_cmd_for_line([n](TAttLine * obj) { obj->SetLineWidth(n); });
	return 0;
}

void * msfit::markercolor(Color_t n)
{
	plotcmd::add_cmd_for_marker([n](TAttMarker * obj) { obj->SetMarkerColor(n); });
	return 0;
}

void * msfit::markersize(Size_t n)
{
	plotcmd::add_cmd_for_marker([n](TAttMarker * obj) { obj->SetMarkerSize(n); });
	return 0;
}

void * msfit::markerstyle(Style_t n)
{
	plotcmd::add_cmd_for_marker([n](TAttMarker * obj) { obj->SetMarkerStyle(n); });
	return 0;
}

void * msfit::name(const char * name)
{
	plotcmd::add_cmd_for_named([name](TNamed * obj) { obj->SetName(name); });
	return 0;
}

void * msfit::title(const char * title)
{
	plotcmd::add_cmd_for_named([title](TNamed * obj) { obj->SetTitle(title); });
	return 0;
}
