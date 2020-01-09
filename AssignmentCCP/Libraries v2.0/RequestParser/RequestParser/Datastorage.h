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
	void AddData(std::string &newinformationtype, std::string &newinformation, bool isValid) {
		if (isValid == true) {
			simpleBarrier.count_down_and_wait();
			lock.lock_shared();
			//set as a carry on of linked list
			if (std::find(data.begin(), data.end(), newinformationtype) != data.end())
				data.insert(newinformation);
			else
				//add to end of linkedlist then a new list with the newinformation
				data.push_back(newinformationtype, newinformation);
			lock.unlock_shared();
		}
		return "Data parsed didn't contain valid sintax";
	};
	std::string ShowData(std::string type, int number,bool isValid) {
		if (isValid == true) {
			simpleBarrier.count_down_and_wait();
			bool foundData = false;
			lock.lock_shared();
			//finds the string data from the nestedlinkedlist
			std::string output = data.at(type, number);
			lock.unlock_shared();
			//if found returns
			return output;
			//else returns "data not found"
		}
		return "Data parsed didn't contain valid sintax";
	};
	std::string CountData(std::string &countdata,bool isValid) {
		if (isValid == true) {
			std::string datasize;
			bool foundData = false;
			//finds the linked list relating to countdata
			//if found returns
			return datasize = data.size();
			//else returns "data not found"
		}
		return "Data parsed didn't contain valid sintax";
	}
private:
	std::list<std::list<std::string>> data;
	std::shared_mutex lock;
	Barrier simpleBarrier;
};

