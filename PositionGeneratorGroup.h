#ifndef __position_generator_group_h__
#define __position_generator_group_h__

#include "PositionGenerator.h"

#include <memory>
#include <vector>

class PositionGeneratorGroup : NonCopyable
{
public:
    virtual ~PositionGeneratorGroup() {}

    virtual void start() = 0;
    virtual void stop() = 0;
};

std::unique_ptr<PositionGeneratorGroup> CreatePositionGeneratorGroup(const std::vector<PositionGeneratorPtr>& generators);

#endif