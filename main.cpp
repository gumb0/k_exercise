#include "NoiseReducingPositionProcessor.h"
#include "NoisyPositionGenerator.h"
#include "PositionFileLogger.h"
#include "PositionGeneratorGroup.h"
#include "PositionProcessorImpl.h"
#include "RandomPositionGenerator.h"
#include "StatisticsFileLogger.h"

#include <iostream>

namespace
{
    const std::string NoiseOption = "--noise";

    const int SensorCount = 20;
    const double SensorFrequencyHz = 20;
    const double PlayingFieldSizeMeters = 100;
    const double MaxSpeedMetersPerSecond = 6;
    const double NoiseAmplitudeMeters = 0.3;
    const int NoiseReductionFilterLength = 21;



    PositionGeneratorPtr CreatePositionGenerator(int sensorId, bool noise)
    {
        PositionGeneratorPtr generator(CreateRandomPositionGenerator(sensorId, SensorFrequencyHz, PlayingFieldSizeMeters, MaxSpeedMetersPerSecond));

        return noise ? CreateNoisyPositionGenerator(generator, NoiseAmplitudeMeters) : generator;
    }

    PositionProcessorPtr CreatePositionProcessor(int sensorId, bool noiseReduction)
    {
        PositionProcessorPtr processor(CreatePositionProcessorImpl(sensorId));

        return noiseReduction ? CreateNoiseReducingPositionProcessor(processor, NoiseReductionFilterLength) : processor;
    }

    PositionGeneratorPtr SetupSensor(int sensorId, bool noise)
    {
        PositionGeneratorPtr generator(CreatePositionGenerator(sensorId, noise));

        // log coordinates to file
        generator->addPositionObserver(CreatePositionFileLogger(sensorId));

        PositionProcessorPtr processor(CreatePositionProcessor(sensorId, noise));

        // link processor to generator
        generator->addPositionObserver(processor);

        // log speed & distance to file
        processor->addStatisticsObserver(CreateStatisticsFileLogger(sensorId));

        return generator;
    }
}

int main(int argc, char *argv[])
{
    // run with --noise to enable noise
    const bool noise = argc >= 2 && argv[1] == NoiseOption;

    try
    {
        std::vector<PositionGeneratorPtr> generators;
        for (int i = 0; i < SensorCount; ++i)
            generators.push_back(SetupSensor(i, noise));

        std::unique_ptr<PositionGeneratorGroup> group(CreatePositionGeneratorGroup(generators));

        group->start();

        std::cout << "Running...\nPress Enter to stop\n";
        std::cin.get();

        group->stop();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occured: " << e.what() << '\n';
        return -1;
    }

    return 0;
}