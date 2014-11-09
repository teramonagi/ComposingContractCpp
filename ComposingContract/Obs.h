#pragma once
#include <functional>
#include "PR.h"
#include "Date.h"
//type Obs
template <class T_> using ObsFunc = std::function<PR<T_>(Date t)>;
template<class T_>
struct Obs
{
	Obs(ObsFunc<T_> func) : func_(func){}
	PR<T_> operator()(Date t) const{ return func_(t); }
	ObsFunc<T_> func_;
};
