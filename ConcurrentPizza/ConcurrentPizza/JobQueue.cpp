#include "stdafx.h"
#include "JobQueue.h"

JobQueue::JobQueue()
{
}


JobQueue::~JobQueue()
{
}

void JobQueue::addJob(const Job & j)
{
	mJobQueue.push(j);
}

std::optional<Job> JobQueue::extractJob()
{
	return mJobQueue.getAndPop();
}



Job::Job() : jobType(JobType::INVALID)
{
}
