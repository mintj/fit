#ifndef PLOTCMD_H__
#define PLOTCMD_H__

#include <vector>
//#include <set>
#include <functional>

class TNamed;
class TAttLine;
class TAttMarker;
class TH1F;
class TGraph;

class plotcmd
{
	public:
		static void add_cmd_for_line(const std::function<void(TAttLine *)> & f);
		static void add_cmd_for_marker(const std::function<void(TAttMarker *)> & f);
		static void add_cmd_for_named(const std::function<void(TNamed *)> & f);
		static void clear();
		static void execute(TGraph * gr);
		static void execute(TH1F * h);

	private:
		static std::vector<std::function<void(TNamed *)>> sm_cmd_for_named;
		static std::vector<std::function<void(TAttLine *)>> sm_cmd_for_line;
		static std::vector<std::function<void(TAttMarker *)>> sm_cmd_for_marker;
};

#endif
