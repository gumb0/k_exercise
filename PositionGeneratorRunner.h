#ifndef __position_generator_runner_h__
#define __position_generator_runner_h__

#include "PositionGenerator.h"
#include "PositionObserver.h"

#include <functional>
#include <memory>
#include <vector>

// TODO generator runner ?

class PositionGeneratorRunner : NonCopyable
{
public:
    virtual ~PositionGeneratorRunner() {}

    virtual void start() = 0;
    virtual void stop() = 0;
};

std::unique_ptr<PositionGeneratorRunner> CreatePositionGeneratorRunner(const std::vector<PositionGeneratorPtr>& generators);

#endif