#ifndef __INTERNAL_H__
#define	__INTERNAL_H__

#include <string>
#include <vector>
#include <map>

#include "../utils/thread_queue.h"
#include "czhtFaceInterface.h"
#include "opencvVideoInterface.h"

#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }
#endif


typedef struct tagSvrHandle
{

};

#endif