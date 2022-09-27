#pragma once
#include <vector>

class DataReaderBase
{
public:
	virtual ~DataReaderBase() {}
	virtual bool OpenDataSource() = 0;
	virtual bool GetNext(std::vector<int>& trans) = 0;
};

