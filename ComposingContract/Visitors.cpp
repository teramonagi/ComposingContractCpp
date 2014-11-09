#include "Visitors.h"
void Evaluator::visit(Zero & zero)
{
	value_ += bigK(0.0);
}
void Evaluator::visit(One & one)
{
	value_ += model_.exch(currency_, one.getCurrency());
}
void Evaluator::visit(Scale & scale)
{
	Obs<double> & o = scale.get_obs();
	ContractPtr & c = scale.get_contract();
	value_ += evalO(o) * evalC(model_, currency_, c);
}
void Evaluator::visit(When & when)
{
	value_ += model_.disc(currency_, evalO(when.get_obs()), evalC(model_, currency_, when.get_contract()));
}
void Evaluator::visit(Give & give)
{
	value_ -= evalC(model_, currency_, give.get_contract());
}
void Evaluator::visit(Or & or)
{
	PR<double> p1 = evalC(model_, currency_, or.get_contract1());
	PR<double> p2 = evalC(model_, currency_, or.get_contract2());
	const std::size_t size = p1.unPr.size();
	for (std::size_t i = 0; i < size; ++i)
	{
		value_.unPr[i] += std::max(p1.unPr[i], p1.unPr[i]);
	}
}
void Evaluator::visit(And & and)
{
	PR<double> p1 = evalC(model_, currency_, and.get_contract1());
	PR<double> p2 = evalC(model_, currency_, and.get_contract2());
	value_ += (p1 + p2);
}