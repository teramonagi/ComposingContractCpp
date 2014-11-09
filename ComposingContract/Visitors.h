#pragma once
#include "PR.h"
#include "Model.h"
#include "Contract.h"
/*
Contract implementation
*/
class Visitor
{
public:
	virtual void visit(Zero & zero) = 0;
	virtual void visit(One & one) = 0;
	virtual void visit(Scale & scale) = 0;
	virtual void visit(When & when) = 0;
	virtual void visit(Give & give) = 0;
	virtual void visit(Or & or) = 0;
	virtual void visit(And & and) = 0;
};
class Evaluator : public Visitor
{
public:
	Evaluator(Model & model, Currency & currency) : model_(model), currency_(currency), value_(PR<double>(RV<double>(model.getSize(), 0.0))){}
	void visit(Zero & zero);
	void visit(One & one);
	void visit(Scale & scale);
	void visit(When & when);
	void visit(Give & give);
	void visit(Or & or);
	void visit(And & and);
	PR<double> getPR(){ return value_; }
private:
	Model model_;
	Currency currency_;
	PR<double> value_;
};
//eval functions
template<class T_> inline PR<T_> evalO(Obs<T_> & o){ return o(time0); }
inline PR<double> evalC(Model & model, Currency currency, ContractPtr & contract)
{
	auto evaluator = std::make_unique<Evaluator>(model, currency);
	contract->accept(*evaluator);
	return evaluator->getPR();
}