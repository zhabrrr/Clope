#include "Processor.h"

bool Processor::Process(int maxIterations)
{
	if (!dataReader.OpenDataSource())
		throw std::exception("Cannot open the specified data source!");

	DoInitialDistribution();

	int iterationsCount = 0;
	while (++iterationsCount < maxIterations && CorrectClusters() > 0)
		;
	return true;
}

void Processor::GetResults(std::vector<std::vector<size_t>>& result) const
{
	result.resize(clusters.size());
	for (size_t i = 0; i < transClusterIdxs.size(); ++i)
	{
		result[transClusterIdxs[i]].push_back(i);
	}
}

void Processor::DoInitialDistribution()
{
	int transIdx = 0;
	std::vector<int> trans;
	while (dataReader.GetNext(trans))
	{
		double deltaMax = 0;
		size_t idxMax = 0;
		for (size_t i = 0; i < clusters.size(); i++)
		{
			double delta = clusters[i].DeltaAdd(trans);
			if (delta > deltaMax)
			{
				deltaMax = delta;
				idxMax = i;
			}
		}
		if (deltaMax > pow(trans.size(), 1 - Settings::r))
		{
			clusters[idxMax].Add(trans);
			transClusterIdxs.push_back(idxMax);
		}
		else
		{
			transClusterIdxs.push_back(clusters.size());
			clusters.push_back(Cluster(trans));
		}
		++transIdx;
	}
}

int Processor::CorrectClusters()
{
	dataReader.OpenDataSource();
	size_t transIdx = 0;
	std::vector<int> trans;
	size_t changedCnt = 0;

	while (dataReader.GetNext(trans))
	{
		double deltaMax = -clusters[transClusterIdxs[transIdx]].DeltaSub(trans);
		size_t idxMax = transClusterIdxs[transIdx];
		for (size_t i = 0; i < clusters.size(); i++)
			if (i != transClusterIdxs[transIdx] && !clusters[i].empty())
			{
				double delta = clusters[i].DeltaAdd(trans);
				if (delta > deltaMax)
				{
					deltaMax = delta;
					idxMax = i;
				}
			}
		if (idxMax != transClusterIdxs[transIdx])
		{
			clusters[transClusterIdxs[transIdx]].Sub(trans);
			clusters[idxMax].Add(trans);
			transClusterIdxs[transIdx] = idxMax;
			++changedCnt;
		}
		++transIdx;
	}
	return changedCnt;
}
