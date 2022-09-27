#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include "DataReaderBase.h"

class DataReaderMushroom : public DataReaderBase
{
public:
	DataReaderMushroom(const std::string& fPath) : filePath(fPath)	{ }
	~DataReaderMushroom();

	bool OpenDataSource() override;
	bool GetNext(std::vector<int>& trans) override;
	std::vector<char> GetEdibility();

private:
	std::unordered_map<int, int> mapObjToIdx;
	const std::string filePath;
	std::ifstream file;
	int maxIdx = 0;
};

