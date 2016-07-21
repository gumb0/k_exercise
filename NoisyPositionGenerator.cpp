#include "NoisyPositionGenerator.h"

#include <random>

namespace
{
    class NoisyPositionObserver : public PositionObserver
    {
    public:
        NoisyPositionObserver(const std::vector<PositionObserverPtr>& originalObservers, double amplitude) :
            mOriginalObservers(originalObservers),
            mRandomEngine(std::random_device()()),
            mDistribution(-amplitude, amplitude)            
        {
        }

        void onPositionUpdate(int sensorId, double x, double y, const TimePoint& timestamp) override
        {
            const double noisyX = addNoise(x);
            const double noisyY = addNoise(y);

            for (PositionObserverPtr observer : mOriginalObservers)
                observer->onPositionUpdate(sensorId, noisyX, noisyY, timestamp);
        }

    private:
        double addNoise(double value)
        {
            return value + mDistribution(mRandomEngine);
        }

    private:
        const std::vector<PositionObserverPtr>& mOriginalObservers;
        std::default_random_engine mRandomEngine;
        std::uniform_real_distribution<double> mDistribution;
    };


    class NoisyPositionGenerator : public PositionGenerator
    {
    public:
        NoisyPositionGenerator(PositionGeneratorPtr generator, double amplitude) :
            mGenerator(generator),
            mNoisyObserver(std::make_shared<NoisyPositionObserver>(mObservers, amplitude))
        {
            mGenerator->addPositionObserver(mNoisyObserver);
        }

        void addPositionObserver(PositionObserverPtr observer) override
        {
            mObservers.push_back(observer);
        }

        void start() override
        {
            mGenerator->start();
        }

        void stop() override
        {
            mGenerator->stop();
        }

    private:
        PositionGeneratorPtr mGenerator;
        std::vector<PositionObserverPtr> mObservers;
        PositionObserverPtr mNoisyObserver;
    };
}

PositionGeneratorPtr CreateNoisyPositionGenerator(PositionGeneratorPtr generator, double amplitudeMeters)
{
    return std::make_shared<NoisyPositionGenerator>(generator, amplitudeMeters);
}
