#include "PositionGeneratorRunner.h"

#include <algorithm>
#include <iterator>
#include <thread>

namespace
{
    // TODO rename to group ?
    class ParallelPositionGeneratorRunner : public PositionGeneratorRunner
    {
    public:
        explicit ParallelPositionGeneratorRunner(const std::vector<PositionGeneratorPtr>& generators) : mGenerators(generators)
        {
        }

        // TODO don't leak threads, join in destructor

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
        std::vector<PositionGeneratorPtr> mGenerators;
        std::vector<std::thread> mThreads;
    };
}

std::unique_ptr<PositionGeneratorRunner> CreatePositionGeneratorRunner(const std::vector<PositionGeneratorPtr>& generators)
{
    return std::unique_ptr<PositionGeneratorRunner>(new ParallelPositionGeneratorRunner(generators));
}
