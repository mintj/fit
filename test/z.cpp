#include <memory>

int main()
{
	std::shared_ptr<double> a = std::make_shared<double>(5);
	while (1) {
		a.reset(new double(5));
	}
}
