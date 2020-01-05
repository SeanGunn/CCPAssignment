#pragma once
#include <list>
#include <algorithm>
#include <shared_mutex>
#include <string>
#include "Barrier.h"
class Datastorage
{
public:
	Datastorage();
	~Datastorage();
	void AddData(std::string &newinformationtype, std::string &newinformation) {
		simpleBarrier.count_down_and_wait();
		lock.lock_shared();
		if (std::find(data.begin(), data.end(), newinformationtype) != data.end())
			data.insert(newinformation);
		else
			data.push_back(newinformationtype, newinformation);
		whatsasbout.insert(newinformationtype);
		data.insert(newinformationtype, newinformation);
		lock.unlock_shared();
	};
	std::string ShowData(std::string type, std::string number) {
		simpleBarrier.count_down_and_wait();
		lock.lock_shared();
		std::string output = data.at(type,number,);
		lock.unlock_shared();
		return output;
	};
private:
	std::list<std::list<std::string>> data;
	std::shared_mutex lock;
	Barrier simpleBarrier;
};

