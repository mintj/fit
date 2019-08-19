#include "inc/header.h"

void df01_fit()
{
	variable x("x", -10, 10);
	variable m0("m0", -3.2);
	variable s0("s0", 2.7);
	gaussian truth("truth", x, m0, s0);

	dataset * data = truth.generate(x, 10000);

	variable m("m", 0, -10, 10);
	variable s("s", 5, 0.1, 10);
	gaussian model("model", x, m, s);

	model.fit(*data, true);
	
	plot * frame = x.frame();
	data->plot_on(frame);
	model.plot_on(frame, msfit::linecolor(2));
	frame->draw();
}
