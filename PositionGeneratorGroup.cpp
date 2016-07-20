#include "PositionGeneratorGroup.h"

#include <algorithm>
#include <iterator>
#include <thread>

namespace
{
    class ParallelPositionGeneratorGroup : public PositionGeneratorGroup
    {
    public:
        explicit ParallelPositionGeneratorGroup(const std::vector<PositionGeneratorPtr>& generators) : mGenerators(generators)
        {
        }

        ~ParallelPositionGeneratorGroup()
        {
            try
            {
                stop();
            }
            catch (const std::exception&)
            {
            }
        }


        void start() override
        {
            if (!mThreads.empty())
                throw std::runtime_error("Generators are already started.");

            std::transform(mGenerators.begin(), mGenerators.end(), std::back_inserter(mThreads), 
                [](PositionGeneratorPtr generator){ return std::thread(&PositionGenerator::start, generator); });
        }

        void stop() override
        {
            for (PositionGeneratorPtr generator : mGenerators)
                generator->stop();

            for (std::thread& thread : mThreads)
                thread.join();

            mThreads.clear();
        }

    private:
        const std::vector<PositionGeneratorPtr> mGenerators;
        std::vector<std::thread> mThreads;
    };
}

std::unique_ptr<PositionGeneratorGroup> CreatePositionGeneratorGroup(const std::vector<PositionGeneratorPtr>& generators)
{
    return std::unique_ptr<PositionGeneratorGroup>(new ParallelPositionGeneratorGroup(generators));
}
