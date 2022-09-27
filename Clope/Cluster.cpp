#include "Cluster.h"

Cluster::Cluster(const std::vector<int>& trans)
{
	objs.resize(Settings::numObjs);
	//To gain some performance I inserted the code below instead of just Add(trans). Maybe the game's not worth the candle.
	for (size_t i = 0; i < trans.size(); ++i)
	{
		objs[trans[i]] = 1;
	}
	wCur = sCur = trans.size();
	transCnt = 1;
	profitCur = pow(sCur, 1 - Settings::r);
}

double Cluster::DeltaAdd(const std::vector<int>& trans) const
{
	size_t w = wCur;
	for (size_t i = 0; i < trans.size(); ++i)
	{
		if (objs[trans[i]] == 0)
			++w;
	}
	return (sCur + trans.size()) * (transCnt + 1) / std::pow(w, Settings::r) - profitCur;
}

double Cluster::DeltaSub(const std::vector<int>& trans) const
{
	if (transCnt == 1)
		return -profitCur;

	size_t w = wCur;
	for (size_t i = 0; i < trans.size(); ++i)
	{
		if (objs[trans[i]] == 1)
			--w;
	}
	return (sCur - trans.size()) * (transCnt - 1) / std::pow(w, Settings::r) - profitCur;
}

void Cluster::Add(const std::vector<int>& trans)
{
	for (size_t i = 0; i < trans.size(); ++i)
	{
		if (++objs[trans[i]] == 1)
			++wCur;
	}
	sCur += trans.size();
	++transCnt;
	profitCur = CalcProfitCur();
}

void Cluster::Sub(const std::vector<int>& trans)
{
	for (size_t i = 0; i < trans.size(); ++i)
	{
		if (--objs[trans[i]] == 0)
			--wCur;
	}
	sCur -= trans.size();
	--transCnt;
	profitCur = CalcProfitCur();
}

bool Cluster::empty() const
{
	return transCnt == 0;
}

double Cluster::CalcProfitCur() const
{
	return sCur * transCnt / std::pow(wCur, Settings::r);
}
