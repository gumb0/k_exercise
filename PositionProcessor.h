#ifndef __position_processor_h__
#define __position_processor_h__

#include "PositionObserver.h"
#include "StatisticsObserver.h"

#include <memory>

class PositionProcessor : public PositionObserver
{
public:
    virtual ~PositionProcessor() {}

    virtual void addStatisticsObserver(StatisticsObserverPtr observer) = 0;

};

using PositionProcessorPtr = std::shared_ptr<PositionProcessor>;

#endif