#ifndef __position_processor_h__
#define __position_processor_h__

#include "PositionObserver.h"
#include "StatisticsGenerator.h"

class PositionProcessor : public PositionObserver, public StatisticsGenerator
{
};

using PositionProcessorPtr = std::shared_ptr<PositionProcessor>;


PositionProcessorPtr CreatePositionProcessor(int sensorId);

#endif