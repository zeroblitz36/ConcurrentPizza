#pragma once

#include "stdafx.h"

template<class T>
class ConcurrentSet
{
public:
	ConcurrentSet() {};
	~ConcurrentSet() {};

	int insert(const T& t) {
		std::lock_guard<std::mutex> guard();
		mSet.insert(t);
		return mSet.size();
	}
	bool tryErase(const T& t) {
		std::lock_guard<std::mutex> guard();
		return 1 == mSet.erase(t);
	}
	
	std::optional<T> extractOne() {
		std::lock_guard<std::mutex> guard(mSetMutex);
		if (mSet.empty()) {
			return {};
		}
		else {
			return { mSet.extract(0) };
		}
	}
	
protected:
	std::set<T> mSet;
	std::mutex mSetMutex;
};