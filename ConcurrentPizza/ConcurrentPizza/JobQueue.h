#pragma once

#include <queue>
#include <mutex>
#include <stdint.h>
#include "ConcurrentQueue.h"

enum JobType {
	INVALID = -1,
};

class Job {
public:
	Job();
protected:
	JobType jobType;
};

class JobQueue
{
public:
	JobQueue();
	virtual ~JobQueue();

	void addJob(const Job& j);
	std::optional<Job> extractJob();
private:
	ConcurrentQueue<Job> mJobQueue;
};

