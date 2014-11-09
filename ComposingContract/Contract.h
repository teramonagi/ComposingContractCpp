#pragma once
#include<memory>
#include "Currency.h"
#include "Obs.h"
#include "Date.h"
#include "PR.h"
//Forward-declration
class Visitor;
//Contract
class Contract
{
public:
	virtual void accept(Visitor & visitor) = 0;
};
typedef std::shared_ptr<Contract> ContractPtr;
class Zero : public Contract
{
public:
	void accept(Visitor & visitor);
};
class One : public Contract
{
public:
	One(Currency currency) : currency_(currency){}
	Currency getCurrency(){ return currency_; }
	void accept(Visitor & visitor);
private:
	Currency currency_;
};
class Scale : public Contract
{
public:
	Scale(Obs<double> & obs, ContractPtr contract) : obs_(obs), contract_(contract){}
	ContractPtr get_contract(){ return contract_; }
	Obs<double> & get_obs(){ return obs_; }
	void accept(Visitor & visitor);
private:
	Obs<double> obs_;
	ContractPtr contract_;
};
class When : public Contract
{
public:
	When(Obs<bool> & obs, ContractPtr contract) : obs_(obs), contract_(contract){}
	ContractPtr get_contract(){ return contract_; }
	Obs<bool> & get_obs(){ return obs_; }
	void accept(Visitor & visitor);
private:
	Obs<bool> obs_;
	ContractPtr contract_;
};
class Give : public Contract
{
public:
	Give(ContractPtr contract) : contract_(contract){}
	ContractPtr get_contract(){ return contract_; }
	void accept(Visitor & visitor);
private:
	ContractPtr contract_;
};
class Or : public Contract
{
public:
	Or(ContractPtr c1, ContractPtr c2) : contract1_(c1), contract2_(c2){}
	ContractPtr get_contract1(){ return contract1_; }
	ContractPtr get_contract2(){ return contract2_; }
	void accept(Visitor & visitor);
private:
	ContractPtr contract1_;
	ContractPtr contract2_;
};
class And : public Contract
{
public:
	And(ContractPtr c1, ContractPtr c2) : contract1_(c1), contract2_(c2){}
	ContractPtr get_contract1(){ return contract1_; }
	ContractPtr get_contract2(){ return contract2_; }
	void accept(Visitor & visitor);
private:
	ContractPtr contract1_;
	ContractPtr contract2_;
};
//Unsed...
template<class A_, class B_>
Obs<B_> lift(std::function<B_(A_)> f, Obs<A_> o)
{
	return Obs<B_>([=](Date t){
		PR<A_> & prs = o(t);
		std::vector<RV<B_>> new_unPr;
		for (auto rv : prs.unPr)
		{
			std::vector<B_> new_values(rv.values.size());
			std::transform(rv.values.begin(), rv.values.end(), new_values.begin(), f);
			new_unPr.push_back(RV<B_>(new_values));
		}
		return new_unPr;
	});
}
template<class A_, class B_, class C_>
Obs<C_> lift2(std::function<C_(A_, B_)> f, Obs<A_> & o1, Obs<B_> & o2)
{
	return Obs<C_>([=](Date t){
		PR<A_> & prs1 = o1(t);
		PR<B_> & prs2 = o2(t);
		const std::size_t size_unPr = prs1.unPr.size();
		const std::size_t size_values = prs1.unPr[0].size();
		PR<C_> pr;
		for (std::size_t i = 0; i < size_unPr; ++i)
		{
			std::vector<C_> new_values(prs1.unPr[i].size());
			for (std::size_t j = 0; j < size_values; ++j)
			{
				new_values[j] = f(prs1.unPr[i][j], prs2.unPr[i][j]);
			}
			pr.unPr.push_back(RV<C_>(new_values));
		}
		return pr;
	});
}
//Unsed...
template<class T_>
Obs<bool> operator==(Obs<T_> & o1, Obs<T_> & o2){
	std::function<bool(T_, T_)> f = ([](T_ a, T_ b){return a == b; });
	return lift2(f, o1, o2);
}
template<class T_> inline PR<T_> bigK(T_ x){ return PR<T_>(x); };
template<class T_> inline Obs<T_> konst(T_ val){ return Obs<T_>([=](Date t){return bigK(val); }); }
//Differ from the original implementation(http://contracts.scheming.org/) because I'm not sure how to write lazy/sequential process in C++ easily
static Obs<Date> date = Obs<Date>([](double t){return PR<Date>(mkDate(0));});
//inline Obs<bool> at(Date t){ return (konst<Date>(t) == date); }
inline Obs<bool> at(Date t){ 
	return Obs<bool>([=](Date dummy){
		PR<bool> pr;
		for (Date lt = 0; lt < t; ++lt)
		{
			pr.unPr.push_back(RV<bool>(1, false));
		}
		pr.unPr.push_back(RV<bool>(1, true));
		return pr;
	});
}
inline Obs<Date> mkObsDate(Date t){ return Obs<Date>([=](Date dummy){return PR<Date>(mkDate(t)); }); }
static ContractPtr zero = std::make_shared<Zero>();
inline ContractPtr one(Currency currency){ return std::make_shared<One>(currency); }
inline ContractPtr when(Obs<bool> & o, ContractPtr c){ return std::make_shared<When>(o, c); }
inline ContractPtr or(ContractPtr c1, ContractPtr c2){ return std::make_shared<Or>(c1, c2); }
inline ContractPtr and(ContractPtr c1, ContractPtr c2){ return std::make_shared<And>(c1, c2); }
inline ContractPtr scale(Obs<double> & o, ContractPtr contract){ return std::make_shared<Scale>(o, contract); }
//Compositional valuation semantics for contracts
inline ContractPtr european(Date t, ContractPtr c){return when(at(t), or(c, zero));}
inline ContractPtr zcb(Date t, double x, Currency k){ return when(at(t), scale(konst(x), one(k))); }
