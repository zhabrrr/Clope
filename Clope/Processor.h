#pragma once
#include <vector>
#include "Settings.h"
#include "DataReaderBase.h"
#include "Cluster.h"

class Processor
{
public:
	Processor(DataReaderBase& dr) : dataReader(dr)	{ }

	bool Process(int maxIterations);
	void GetResults(std::vector<std::vector<size_t>>& result) const;

private:
	void DoInitialDistribution();
	int CorrectClusters();

private:
	DataReaderBase& dataReader;
	std::vector<Cluster> clusters;
	std::vector<size_t> transClusterIdxs;
};

