#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <future>
#include <chrono> 
#include <map>

#include "RequestParser.h"
#include "config.h"
#include "TCPServer.h"
#include "ThreadPool.h"

#ifndef NON_BLOCKING

#define DEFAULT_PORT 12345

unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
ThreadPool pool(concurentThreadsSupported); 
std::map <std::string, std::string> dataMap;
std::map <std::string, int> dataMapAmount;
int main()
{

	TCPServer server(DEFAULT_PORT);
	ReceivedSocketData receivedData;

	std::cout << "Starting server. Send \"exit\" (without quotes) to terminate." << std::endl;

	receivedData = server.accept();

	do {
		server.receiveData(receivedData, 1);
		receivedData.reply = receivedData.request;

		PostRequest post = PostRequest::parse(receivedData.reply);
		if (post.valid)
		{
			
			pool.enqueue([newinformationtype = post.getTopicId(),newinformation = post.getMessage()] {
				bool alreadyCreated = false;
				bool wasFound = false;
				std::map<std::string, int>::iterator it;
				int amountIn = -1;
				for (it = dataMapAmount.begin(); it != dataMapAmount.end(); it++) {
					if (it->first == newinformationtype) {
						amountIn = dataMapAmount[newinformationtype];
						amountIn++;
						dataMapAmount[newinformationtype] = amountIn;
						wasFound = true;
						break;
					}
				}
				std::map<std::string, std::string>::iterator itData;
				if (wasFound == true) {
					std::string temp = newinformationtype + std::to_string(amountIn);
					for (itData = dataMap.begin(); itData != dataMap.end(); itData++) {
						if (itData->first == temp) {
							if (dataMap.size() != dataMap.max_size()) {
								if (itData->second != newinformation) {
									std::string temp2 = "";
									temp2.append(newinformationtype);
									temp2.append(std::to_string(amountIn));
									dataMap[temp2] = newinformation;
								}
								break;
							}
						}
					}
				}
				else {
					std::string temp = "";
					temp.append(newinformationtype);
					temp.append("1");
					dataMap[temp] = newinformation;
					dataMapAmount[newinformationtype] = 1;
				}
			});
			server.sendReply(receivedData);
		}
		
		ReadRequest read = ReadRequest::parse(receivedData.reply);
		if (read.valid)
		{	
			auto f2 = pool.enqueue([type = read.getTopicId(), number = read.getPostId()]{
				std::cout << "received read request\n";
				std::string output = "";
				std::map<std::string, int>::iterator it;
				int amountIn = 0;
				bool wasFound = false;
				for (it = dataMapAmount.begin(); it != dataMapAmount.end(); it++) {
					if (it->first == type) {
						wasFound = true;
						break;
					}
				}
				if (wasFound == true) {
					std::string whereIs = type;
					whereIs.append(std::to_string(number + 1));
					std::map<std::string, std::string>::iterator it;
					for (it = dataMap.begin(); it != dataMap.end(); it++) {
						if (it->first == whereIs) {
							output = it->second;
						}
					}
				}
				else {
					output = "Couldn't find the string.";
				}
				std::cout << "Read got\n";
				std::cout << "Output should be " << output << "\n";
				return output;
			});
			std::string answer = f2.get();
			receivedData.reply = answer;
			server.sendReply(receivedData);
		}

		CountRequest count = CountRequest::parse(receivedData.reply);
		if (count.valid)
		{
			auto f3 = pool.enqueue([countdata = count.getTopicId()]{
				std::string datasize;
				int output = 0;
				std::map<std::string, int>::iterator it;
				for (it = dataMapAmount.begin(); it != dataMapAmount.end(); it++) {
					if (it->first == countdata) {
						output = it->second;
					}
				}
				if (output == 0) {
					return datasize = "This passed doesn't exist.";
				}
				else {
					return datasize = "The size is " + std::to_string(output);
				}
			});
			std::string answer = f3.get();
			receivedData.reply = answer;
			server.sendReply(receivedData);
		}

		ListRequest list = ListRequest::parse(receivedData.reply);
		if (list.valid)
		{
			std::cout << "List request: " << list.toString() << std::endl;
			auto f3 = pool.enqueue([]{
				std::string answer = "";
				std::map<std::string, std::string>::iterator it;
				bool wasFound = false;
				for (it = dataMap.begin(); it != dataMap.end(); it++) {
					if (it->first.back() == 0) {
						size_t size = it->first.size();
						std::string subString1 = it->first.substr(0, size - 1);
						answer.append(subString1 + "#");
					}
				}
				return answer;
			});
			std::string answer = f3.get();
			receivedData.reply = answer;
			server.sendReply(receivedData);
		}
		std::string notCorrect = "Unknown request: " + receivedData.reply + "\n";
		receivedData.reply = notCorrect;
		server.sendReply(receivedData);
	} while (receivedData.request != "exit");
	server.closeClientSocket(receivedData);
	dataMap.clear();
	dataMapAmount.clear();
	std::cout << "Server terminated." << std::endl;
}

#endif //NON_BLOCKING
