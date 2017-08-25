#ifndef SCOPEDLOCK_H
#define SCOPEDLOCK_H

#include <mutex>

class ScopedLock
{
private:
	std::mutex * _mtx;
public:
	ScopedLock(std::mutex * mtx);
	~ScopedLock();
};

#endif