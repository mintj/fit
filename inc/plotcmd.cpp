#ifndef PLOTCOMMOND_H__
#define PLOTCOMMOND_H__

#include "TGraph.h"
#include "TH1F.h"
#include "plotcmd.h"

void plotcmd::add_cmd_for_line(const std::function<void(TAttLine *)> & f)
{
	sm_cmd_for_line.push_back(f);
}

void plotcmd::add_cmd_for_marker(const std::function<void(TAttMarker *)> & f)
{
	sm_cmd_for_marker.push_back(f);
}

void plotcmd::add_cmd_for_named(const std::function<void(TNamed *)> & f)
{
	sm_cmd_for_named.push_back(f);
}

void plotcmd::clear()
{
	sm_cmd_for_line.clear();
	sm_cmd_for_marker.clear();
	sm_cmd_for_named.clear();
	//sm_components.clear();
	//sm_projdim = 0;
}

void plotcmd::execute(TGraph * gr)
{
	for (auto & cmd: sm_cmd_for_line) {
		cmd(gr);
	}
	for (auto & cmd: sm_cmd_for_marker) {
		cmd(gr);
	}
	for (auto & cmd: sm_cmd_for_named) {
		cmd(gr);
	}
}

void plotcmd::execute(TH1F * h)
{
	for (auto & cmd: sm_cmd_for_line) {
		cmd(h);
	}
	for (auto & cmd: sm_cmd_for_marker) {
		cmd(h);
	}
	for (auto & cmd: sm_cmd_for_named) {
		cmd(h);
	}
}
//size_t plotcmd::sm_projdim;
//
//std::set<int> plotcmd::sm_components;

std::vector<std::function<void(TAttLine *)>> plotcmd::sm_cmd_for_line;

std::vector<std::function<void(TAttMarker *)>> plotcmd::sm_cmd_for_marker;

std::vector<std::function<void(TNamed *)>> plotcmd::sm_cmd_for_named;

#endif
