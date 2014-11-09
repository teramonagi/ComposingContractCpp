#include <iostream>
#include <map>
#include <vector>
#include <numeric>
#include <iterator>
#include "Currency.h"
#include "Date.h"
#include "Contract.h"
#include "Visitors.h"


template<class T_> void show(const std::vector<T_> &v)
{
	if (!v.empty())
	{
		std::copy(v.begin(), v.end(), std::ostream_iterator<T_>(std::cout, " "));
	}
	std::cout << std::endl;
}
template<class T_>
std::vector<T_> mean(PR<T_> & pr)
{
	std::vector<T_> res;
	for (auto rv : pr.unPr)
	{
		res.push_back(std::accumulate(rv.begin(), rv.end(), 0.0) / static_cast<double>(rv.size()));
	}
	return res;
}
void eval_and_show(ContractPtr c)
{
	const int size_path = 100;
	PR<double> value = evalC(Model(size_path), Currency::JPY, c);
	show(mean(value));
}
//Main
int main()
{
	//One unit
	{
		ContractPtr c = one(JPY);
		eval_and_show(c);
	}
	//Zero Coupon bond
	{
		ContractPtr c = zcb(mkDate(365), 10.0, JPY);
		eval_and_show(c);
	}
	//Zero Coupon bond(port)
	{
		ContractPtr c1 = zcb(mkDate(365), 10.0, JPY);
		ContractPtr c2 = zcb(mkDate(365 * 2), 10.0, JPY);
		eval_and_show(and(c1, c2));
	}
	//European option
	{
		ContractPtr c = european(mkDate(365), zcb(mkDate(365 * 2), 10.0, JPY));
		eval_and_show(c);
	}
	return 0;
}