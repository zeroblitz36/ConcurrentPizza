#pragma once
#include <queue>
#include <mutex>
#include <optional>

template <class T>
class ConcurrentQueue
{
public:
	ConcurrentQueue() {};
	virtual ~ConcurrentQueue() {};

	int push(const T& t) {
		std::lock_guard<std::mutex> guard(mQueueMutex);
		mQueue.push(t);
		return mQueue.size();
	}

	std::optional<T> getAndPop() {
		std::lock_guard<std::mutex> guard(mQueueMutex);
		if (mQueue.empty()) {
			return {};
		}
		else {
			T t = mQueue.front();
			mQueue.pop();
			return t;
		}
	}

	bool isEmpty() {
		std::lock_guard<std::mutex> guard(mQueueMutex);
		return mQueue.empty();
	}

protected:
	std::queue<T> mQueue;
	std::mutex mQueueMutex;
};

