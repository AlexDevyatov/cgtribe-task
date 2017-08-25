#include "ScopedLock.h"

ScopedLock::ScopedLock(std::mutex * mtx) : _mtx(mtx)
{
	_mtx->lock();
}

ScopedLock::~ScopedLock()
{
	_mtx->unlock();
}