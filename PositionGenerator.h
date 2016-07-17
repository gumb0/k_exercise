#ifndef __position_generator_h__
#define __position_generator_h__

#include "Common.h"
#include "PositionObserver.h"

#include <memory>

class PositionGenerator : NonCopyable
{
public:
    virtual ~PositionGenerator() {}

    virtual void addPositionObserver(PositionObserverPtr observer) = 0;

    virtual void start() = 0;
    virtual void stop() = 0;
};

using PositionGeneratorPtr = std::shared_ptr<PositionGenerator>;

#endif