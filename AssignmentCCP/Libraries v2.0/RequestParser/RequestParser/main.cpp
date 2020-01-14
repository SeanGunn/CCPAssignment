#include <conio.h>
#include <vector>
#include "Datastorage.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include "RequestParser.h"

int main()
{
	//Datastorage* dataUser = new Datastorage();
	//std::vector<std::thread> threadVector;
	std::vector<std::string> requestList =
	{
		"POST@Solar system#The dark side of the @moon is #dark.",
		"POST@#x",
		"POST@night_sky#cloudy!",
		"POST@Solar system#Mercury, Venus, Earth, Mars, Jupiter, Saturn, Neptune, Uranus",
		"LIST",
		"COUNT@Solar system",
		"COUNT@solar_system",
		"READ@#0",
		"READ@Solar system#1",
		"READ@night_sky#2",
		"READ@solar_system#0",
		"AAA"
	};

	for (std::string & request : requestList)
	{
		PostRequest post = PostRequest::parse(request);
		if (post.valid)
		{
			std::cout << "Post request: " << post.toString() << std::endl;
			std::cout << "Post topic: " << post.getTopicId() << std::endl;
			std::cout << "Post message: " << post.getMessage() << std::endl;
			//threadVector.emplace_back(dataUser->AddData, post.getTopicId(), post.getMessage(), post.valid);
			continue;
		}

		ReadRequest read = ReadRequest::parse(request);
		if (read.valid)
		{
			std::cout << "Read request" << read.toString() << std::endl;
			std::cout << "Read topic: " << read.getTopicId() << std::endl;
			std::cout << "Read post id: " << read.getPostId() << std::endl;
			//threadVector.emplace_back(dataUser->ShowData,read.getTopicId(),read.getPostId(),read.valid);
			//dataUser->ShowData(read.getTopicId(), read.getPostId(), read.valid);
			continue;
		}

		CountRequest count = CountRequest::parse(request);
		if (count.valid)
		{
			std::cout << "Count request: " << count.toString() << std::endl;
			std::cout << "Count topic: " << count.getTopicId() << std::endl;
			//threadVector.emplace_back(dataUser->CountData,count.getTopicId(),count.valid);
			//dataUser->CountData(count.getTopicId(), count.valid);
			continue;
		}

		ListRequest list = ListRequest::parse(request);
		if (list.valid)
		{
			std::cout << "List request: " << list.toString() << std::endl;
			continue;
		}

		std::cout << "Unknown request: " << request << std::endl;
		std::cout << std::endl;
	}
	/*for (int i = 0; i < threadVector.size(); i++) {
		threadVector[i].join;
	}*/
	_getch();


	return 0;
}
