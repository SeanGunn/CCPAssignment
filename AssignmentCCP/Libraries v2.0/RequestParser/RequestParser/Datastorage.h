#pragma once
#include <map>
#include <vector>
#include <algorithm>
class Datastorage
{
public:
	Datastorage();
	~Datastorage();
	void AddData(std::string &newinformation) {
		
		std::replace(newinformation.begin(), newinformation.end(), ' ', '¬');
		std::replace(newinformation.begin(), newinformation.end(), '#', ' ');
		std::vector<std::string> array;
		std::stringstream ss(newinformation);
		std::string temp;
		while (ss >> temp)
			array.push_back(temp);

		std::istringstream iss(newinformation);
		char c; // dummy character for the colon
		int a[8];
		iss >> a[0];
		for (int i = 1; i < 8; i++)
			iss >> c >> a[i];

		data.insert("", "");
		data.insert(newinformation)
	};
	string ShowData(std::string type, int number) {
		return data;
	};
private:
	std::map<std::string,std::string> data;
	std::vector<std::string> splitStrings;
};

