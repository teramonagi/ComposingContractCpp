#pragma once
#include <vector>
#include <algorithm>
//Random Variable & Value process and operators(too slow to use for real...)
//A random variable RV a describes the possible values for a value process at a particular time step
template <typename T_> using RV = std::vector<T_>;
template<class BinaryOperator_>
inline RV<double> & binary_operation(RV<double> & self, const RV<double> & rhs)
{
	const std::size_t size_self = self.size();
	const std::size_t size_rhs  = rhs.size();
	if (size_self == size_rhs)
	{
		std::transform(self.begin(), self.end(), rhs.begin(), self.begin(), BinaryOperator_());
	}
	else if (size_self == 1) {
		const double value = self.front();
		self = rhs;
		binary_operation<BinaryOperator_>(self, value);
	}
	else if (size_rhs == 1) {
		const double value = rhs.front();
		binary_operation<BinaryOperator_>(self, value);
	}
	return self;
}
template<class BinaryOperator_>
inline RV<double> & binary_operation(RV<double> & self, double rhs)
{
	std::transform(self.begin(), self.end(), self.begin(), std::bind2nd(BinaryOperator_(), rhs));
	return self;
}
template<class RHS_> inline RV<double> & operator+=(RV<double> & self, const RHS_ & rhs){ return binary_operation<std::plus<double>>(self, rhs); }
template<class RHS_> inline RV<double> & operator*=(RV<double> & self, const RHS_ & rhs){ return binary_operation<std::multiplies<double>>(self, rhs); }
template<class RHS_> inline RV<double> & operator-=(RV<double> & self, const RHS_ & rhs){ return binary_operation<std::minus<double>>(self, rhs); }
template<class RHS_> inline RV<double> & operator/=(RV<double> & self, const RHS_ & rhs){ return binary_operation<std::divides<double>>(self, rhs); }
template<class RHS_> inline RV<double> operator+(const RV<double> & lhs, const RHS_ & rhs){ RV<double> r(lhs); return(r += rhs); }
template<class RHS_> inline RV<double> operator*(const RV<double> & lhs, const RHS_ & rhs){ RV<double> r(lhs); return(r *= rhs); }
template<class RHS_> inline RV<double> operator-(const RV<double> & lhs, const RHS_ & rhs){ RV<double> r(lhs); return(r -= rhs); }
template<class RHS_> inline RV<double> operator/(const RV<double> & lhs, const RHS_ & rhs){ RV<double> r(lhs); return(r /= rhs); }
inline RV<double> operator+(double lhs, const RV<double> & rhs){ RV<double> r(rhs); return(r += lhs); }
inline RV<double> operator*(double lhs, const RV<double> & rhs){ RV<double> r(rhs); return(r *= lhs); }
inline RV<double> operator-(double lhs, const RV<double> & rhs){ RV<double> r(rhs); std::transform(r.begin(), r.end(), r.begin(), [](double &x){return    -x; }); return(r += lhs); }
inline RV<double> operator/(double lhs, const RV<double> & rhs){ RV<double> r(rhs); std::transform(r.begin(), r.end(), r.begin(), [](double &x){return 1.0 / x; }); return(r *= lhs); }

template<class T_>
struct PR
{
	PR(){}
	PR(T_ x) : unPr(1, RV<T_>(1, x)){}
	PR(const RV<T_> & rv) : unPr(1, rv){}
	PR(const std::vector<RV<T_>> & rv) : unPr(rv){}

	std::vector<RV<T_>> unPr;
};

template<class BinaryOperator_>
inline PR<double> & binary_operation(PR<double> & self, const PR<double> & rhs)
{
	const std::size_t size_self = self.unPr.size();
	const std::size_t size_rhs = rhs.unPr.size();
	for (std::size_t i = 0; i < size_self; ++i)
	{
		self.unPr[i] = binary_operation<BinaryOperator_>(self.unPr[i], rhs.unPr[i]);
	}
	return self;
}
template<class BinaryOperator_>
inline PR<double> & binary_operation(PR<double> & self, double rhs)
{
	const std::size_t size_self = self.unPr.size();
	for (std::size_t i = 0; i < size_self; ++i)
	{
		std::transform(self.unPr[i].begin(), self.unPr[i].end(), self.unPr[i].begin(), std::bind2nd(BinaryOperator_(), rhs));
	}
	return self;
}
template<class RHS_> inline PR<double> & operator+=(PR<double> & self, const RHS_ & rhs){ return binary_operation<std::plus<double>>(self, rhs); }
template<class RHS_> inline PR<double> & operator*=(PR<double> & self, const RHS_ & rhs){ return binary_operation<std::multiplies<double>>(self, rhs); }
template<class RHS_> inline PR<double> & operator-=(PR<double> & self, const RHS_ & rhs){ return binary_operation<std::minus<double>>(self, rhs); }
template<class RHS_> inline PR<double> & operator/=(PR<double> & self, const RHS_ & rhs){ return binary_operation<std::divides<double>>(self, rhs); }
template<class RHS_> inline PR<double> operator+(const PR<double> & lhs, const RHS_ & rhs){ PR<double> r(lhs); return(r += rhs); }
template<class RHS_> inline PR<double> operator*(const PR<double> & lhs, const RHS_ & rhs){ PR<double> r(lhs); return(r *= rhs); }
template<class RHS_> inline PR<double> operator-(const PR<double> & lhs, const RHS_ & rhs){ PR<double> r(lhs); return(r -= rhs); }
template<class RHS_> inline PR<double> operator/(const PR<double> & lhs, const RHS_ & rhs){ PR<double> r(lhs); return(r /= rhs); }
inline PR<double> operator+(double lhs, const PR<double> & rhs){ PR<double> r(rhs); return(r += lhs); }
inline PR<double> operator*(double lhs, const PR<double> & rhs){ PR<double> r(rhs); return(r *= lhs); }