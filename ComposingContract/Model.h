#pragma once
#include <map>
#include "PR.h"
#include "Currency.h"
//Model
class IModel
{
public:
	virtual PR<double> disc(Currency k, PR<bool> & bs, PR<double> & rs) = 0;
	virtual PR<double> exch(Currency k1, Currency k2) = 0;
	virtual std::size_t getSize() = 0;
};
class Model
{
public:
	Model(int size_path) : size_path_(size_path), rates_(){}
	PR<double> disc(Currency k, PR<bool> & bs, PR<double> & ps)
	{
		std::vector<RV<bool>> &b = bs.unPr;
		std::vector<RV<double>> &p = ps.unPr;
		std::size_t index_time = 0;
		for (; index_time < b.size(); ++index_time)
		{
			if (b[index_time].front())
			{
				time_evolution_rate(k, index_time - rates_[k].unPr.size() + 1);
				p.front() *= calc_discount(k, index_time);
			}
		}
		return PR<double>(p);
	}
	PR<double> exch(Currency k1, Currency k2)
	{
		RV<double> exchange_rates = RV<double>(size_path_, 1.0);
		return PR<double>(exchange_rates);
	}
	std::size_t getSize(){ return size_path_; }
private:
	void time_evolution_rate(Currency k, int ts)
	{
		//dummy time evolve
		for (int i = 0; i < ts;  ++i)
		{
			rates_[k].unPr.push_back(RV<double>(size_path_, 0.01));
		}
	}
	RV<double> calc_discount(Currency k, int index_time_stop)
	{
		const PR<double> & rate = rates_[k];
		RV<double> result(size_path_);
		for (int index_path = 0; index_path < size_path_; ++index_path)
		{
			double sum_rate = 0.0;
			for (int index_time = 0; index_time < index_time_stop; ++index_time)
			{
				sum_rate += rate.unPr[index_time][index_path];
			}
			result[index_path] = std::exp(-(sum_rate) / 365.0);
		}
		return result;
	}
	std::map<Currency, PR<double>> rates_;
	int size_path_;
};
