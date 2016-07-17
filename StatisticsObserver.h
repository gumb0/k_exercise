#ifndef __statistics_observer_h__
#define __statistics_observer_h__

#include "Common.h"

#include <memory>

class StatisticsObserver : NonCopyable
{
public:
    virtual ~StatisticsObserver() {}

    virtual void onStatisticsUpdate(int sensorId, double speed, double distance, const TimePoint& timestamp) = 0;
};

using StatisticsObserverPtr = std::shared_ptr<StatisticsObserver>;

#endif