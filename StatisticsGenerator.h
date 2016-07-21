#ifndef __statistics_generator_h__
#define __statistics_generator_h__

#include "StatisticsObserver.h"

class StatisticsGenerator
{
public:
    virtual ~StatisticsGenerator() {}

    virtual void addStatisticsObserver(StatisticsObserverPtr observer) = 0;
};

#endif