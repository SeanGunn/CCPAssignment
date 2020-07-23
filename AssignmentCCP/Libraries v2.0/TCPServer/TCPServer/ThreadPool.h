#pragma once
#include <condition_variable>
#include <functional>
#include <vector>
#include <thread>
#include <queue>
#include <future>
class ThreadPool
{
public:
	using Task = std::function<void()>;
	explicit ThreadPool(std::size_t nThreads) {
		start(nThreads);
	};
	~ThreadPool() {
		stop();
	};

	template<class T>
	auto enqueue(T task) ->std::future<decltype(task())>{
		auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(std::move(task));
		{
			std::unique_lock<std::mutex> lock{ nEventMutex };
			nTasks.emplace([=] {
				(*wrapper)();
			});
		}

		nEventVar.notify_one();
		return wrapper->get_future();
	}
private:
	std::vector<std::thread> nThreads;

	std::condition_variable nEventVar;

	std::mutex nEventMutex;
	bool nStopping = false;

	std::queue<Task> nTasks;

	void start(std::size_t numThreads) {
		for (auto i = 0u; i < numThreads; ++i) {
			nThreads.emplace_back([=] {
				while (true) {
					Task task;

					{
					std::unique_lock<std::mutex> lock{ nEventMutex };
					nEventVar.wait(lock, [=] {return nStopping || !nTasks.empty(); });
					if (nStopping && nTasks.empty())
						break;

					task = std::move(nTasks.front());
					nTasks.pop();
					}
					task();
				}
			});
		}
	}

	void stop() noexcept{
		{
			std::unique_lock < std::mutex >lock{ nEventMutex };
			nStopping = true;
		}

		nEventVar.notify_all();

		for(auto &thread : nThreads)
			thread.join();
	}
};

