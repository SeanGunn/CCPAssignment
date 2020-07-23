#include <iostream>
#include <chrono>
#include <mutex>
#include "TCPClient.h"
#include "ThreadPool.h"

#define DEFAULT_PORT 12345


int main(int argc, char **argv)
{
	double threadTimeAdder = 0;
	int readers = 0;
	int writers = 0;
	int requestsToSend = 0;
	int howManyTests = 10;
	std::cout << "How man writers you want?";
	std::cin >> writers;
	std::cout << "How man readers you want?";
	std::cin >> readers;
	std::cout << "How man request you want to send?";
	std::cin >> requestsToSend;
	int totalAmount = writers + readers;
	ThreadPool pool(totalAmount);
	TCPClient client("127.0.0.1", DEFAULT_PORT);
	client.OpenConnection();
	std::vector<double>timeTakenVector;
	std::vector<double>requestsSentVector;
	for(int i = 0; i< howManyTests; i++){
		int requestSent = 0;
		std::cout << "Test " <<std::to_string(i+1) << ":\n";
		auto startTime = std::chrono::high_resolution_clock::now();
		std::cout << "Starting Writers\n";
		for (int i = 0; i < writers; ++i) {
			auto thread1 = pool.enqueue([&client, i, startTime,requestSent,requestsToSend] {
				std::string request = "POST@Lets#Even";
				double tempTime = 0;
				int amountRan = 0;
				for (int x = 0; x < requestsToSend; ++x) {
					client.send(request);
					++amountRan;
				}
				return amountRan;
			});
			requestSent += thread1.get();
		}
		std::cout << "Starting Readers\n";
		for (int i = 0; i < readers; ++i) {
			auto thread2 = pool.enqueue([&client, i, startTime, requestSent, requestsToSend] {
				std::string request = "READ@Lets#0";
				double tempTime = 0;
				int amountRan = 0;
				for (int x = 0; x < requestsToSend; ++x) {
					std::string reply = client.send(request);
					++amountRan;
				}
				return amountRan;
			});
			requestSent += thread2.get();
		}
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - startTime;
		threadTimeAdder = elapsed_seconds.count();

		timeTakenVector.push_back(threadTimeAdder);
		requestsSentVector.push_back(requestSent);
	}
	double timeOverall = 0;
	double requestsOverall = 0;
	for(int i = 0; i<timeTakenVector.size(); i++){
		std::cout << "Time for thread "<<i<<" is: " << std::to_string(timeTakenVector.at(i)) <<"\n";
		timeOverall += timeTakenVector[i];
		std::cout << "Requests sent: " << requestsSentVector[i] << "\n";
		requestsOverall += requestsSentVector[i];
	}
	std::cout << "Time elapsed on average: " << std::to_string(timeOverall/howManyTests) <<"\n";
	client.send("exit");
	client.CloseConnection();
	system("pause");
	return 0;
}
