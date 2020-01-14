#pragma once
#include <vector>
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
			if (int i = std::find(data.begin(), data.end(), newinformationtype) != data.end())
			{
				//first finds the value in the vector, then updates the size in the vector then inserts the newinformation in the correct area
				std::vector<std::string>::iterator it = data.begin();
				advance(it, i);
				++it;
				std::string a = data[i+1];
				int repl = stoi(a);
				int a = 0;
				while (i != repl) {
					++it;
				}
				data[i + 1] = repl;
				data.insert(it, newinformation);
			}
			else {
				//adds a head as newinformationtype and adds its sub values
				data.push_back(newinformationtype);
				int sizeAddin = 1;
				data.push_back(std::to_string(sizeAddin));
				data.push_back(newinformation);
			}
			lock.unlock_shared();
		}
		//return "Data parsed didn't contain valid sintax";
	};
	std::string ShowData(std::string type, int number,bool isValid) {
		std::string output;
		if (isValid == true) {
			//waits for the any other functions in threads do use it
			simpleBarrier.count_down_and_wait();
			bool foundData = false;
			//makes sure any other threads can access at the moment
			lock.lock_shared();
			if (int i = std::find(data.begin(), data.end(), type) != data.end())
			{
				output = data[i + number + 1];
			}
			//unlocks since finished reading the data
			lock.unlock_shared();
			//if found returns
			return output;
			//else returns "data not found"
		}
		return output = "Data parsed didn't contain valid sintax";
	};
	std::string CountData(std::string &countdata,bool isValid) {
		std::string datasize;
		if (isValid == true) {
			bool foundData = false;
			std::string output;
			//finds the size of the type of data in the vector
			//if found returns
			simpleBarrier.count_down_and_wait();
			lock.lock_shared();
			if (int i = std::find(data.begin(), data.end(), countdata) != data.end())
			{
				output = data[i+1];
			}
			lock.unlock_shared();
			return datasize = output;
			//else returns "data not found"
		}
		return datasize = "Data parsed didn't contain valid sintax";
	}
	std::string ListData(bool isValid) {
		std::string returnData;
		if (isValid == true) {
			simpleBarrier.count_down_and_wait();
			lock.lock_shared();
			if (data[2] != "") {
				int countAlong = 0;
				std::string temp = data[1];
				countAlong = stoi(temp);
				++countAlong;
				returnData.append(data[1] +"#");
				int i = 2 + countAlong;
				while (i <= data.size)
				{
					temp = data[countAlong + i];
					countAlong = countAlong + stoi(temp);
					returnData.append(data[countAlong - 1]);
					if ((i + data[countAlong]) < data.size()) {
						returnData.append("#");
					}
					i = countAlong;
				}
			}
			lock.unlock_shared();
			return returnData;
		}
		return returnData = "Data parsed didn't contain valid sintax";
	}
private:
	std::vector<std::string> data;
	std::shared_mutex lock;
	Barrier simpleBarrier;
};

