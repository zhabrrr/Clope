#pragma once

#include <vector>
#include <cmath>
#include "Settings.h"

class Cluster
{
public:
	Cluster(const std::vector<int>& trans);

	double DeltaAdd(const std::vector<int>& trans) const;
	double DeltaSub(const std::vector<int>& trans) const;
	void Add(const std::vector<int>& trans);
	void Sub(const std::vector<int>& trans);
	bool empty() const;

private:
	double CalcProfitCur() const;

private:
	std::vector<int> objs;
	size_t sCur = 0;
	size_t wCur = 0;
	size_t transCnt = 0;
	double profitCur = 0;
};

