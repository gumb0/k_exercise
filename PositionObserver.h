#ifndef __position_observer_h__
#define __position_observer_h__

#include "Common.h"

#include <memory>

class PositionObserver : NonCopyable
{
public:
    virtual ~PositionObserver() {}

    virtual void onPositionUpdate(int sensorId, double x, double y, const TimePoint& timestamp) = 0;
};

using PositionObserverPtr = std::shared_ptr<PositionObserver>;

#endif