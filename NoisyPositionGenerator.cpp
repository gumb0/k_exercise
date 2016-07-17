#include "NoisyPositionGenerator.h"

#include <random>

namespace
{
    class NoisyPositionObserver : public PositionObserver
    {
    public:
        NoisyPositionObserver(PositionObserverPtr observer, double amplitude, unsigned int seed) : 
            mObserver(observer),
            mRandomEngine(seed),
            mDistribution(-amplitude, amplitude)            
        {
        }

        void onPositionUpdate(int sensorId, double x, double y, const TimePoint& timestamp) override
        {
            mObserver->onPositionUpdate(sensorId, addNoise(x), addNoise(y), timestamp);
        }

    private:
        double addNoise(double value)
        {
            return value + mDistribution(mRandomEngine);
        }

    private:
        PositionObserverPtr mObserver;
        std::default_random_engine mRandomEngine;
        std::uniform_real_distribution<double> mDistribution;
    };


    class NoisyPositionGenerator : public PositionGenerator
    {
    public:
        NoisyPositionGenerator(PositionGeneratorPtr generator, double amplitude) : 
            mGenerator(generator),
            mAmplitude(amplitude)
        {
        }

        void addPositionObserver(PositionObserverPtr observer) override
        {
            PositionObserverPtr decoratedObserver(std::make_shared<NoisyPositionObserver>(observer, mAmplitude, mRandomDevice()));
            mGenerator->addPositionObserver(decoratedObserver);
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
        const double mAmplitude;
        std::random_device mRandomDevice;
    };
}

PositionGeneratorPtr CreateNoisyPositionGenerator(PositionGeneratorPtr generator, double amplitudeMeters)
{
    return std::make_shared<NoisyPositionGenerator>(generator, amplitudeMeters);
}
