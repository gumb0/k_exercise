#include "RandomPositionGenerator.h"

#include <atomic>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

namespace
{
    const double Pi = 3.141592653589793238463;
    const double SpeedDurationMinSeconds = 1;
    const double SpeedDurationMaxSeconds = 30;


    inline std::chrono::milliseconds GetPeriod(double frequencyHz)
    {
        const std::chrono::duration<double> period(1.0 / frequencyHz); // double seconds
        return std::chrono::duration_cast<std::chrono::milliseconds>(period);
    }


    // This implementation emulates moving in random direction with random speed, 
    // changing it after random period of time

    class RandomPositionGenerator : public PositionGenerator
    {
    public:
        RandomPositionGenerator(int sensorId, double frequencyHz, double playingFieldSizeMeters, double maxSpeedMetersPerSec) : 
            mSensorId(sensorId), 
            mSensorPeriod(GetPeriod(frequencyHz)),
            mPlayingFieldSize(playingFieldSizeMeters),
            mIsStopped(true),
            mRandomEngine(std::random_device()()),
            mSpeedDistribution(0, maxSpeedMetersPerSec),
            mDirectionDistribution(0, 2 * Pi),
            mSpeedDurationDistribution(SpeedDurationMinSeconds, SpeedDurationMaxSeconds),
            mX(0),
            mY(0),
            mSpeedX(0),
            mSpeedY(0),
            mSpeedDuration(0),
            mLastTimestamp(std::chrono::system_clock::now()),
            mLastSpeedChange(std::chrono::system_clock::now())
        {
            generateRandomPosition();
            generateRandomSpeed();
        }

        void addPositionObserver(PositionObserverPtr observer) override
        {
            mObservers.push_back(observer);
        }

        void start() override
        {
            // Ignore start request if it's already started 
            if (!mIsStopped.exchange(false))
                return;

            try
            {
                while (!mIsStopped)
                {
                    update();

                    std::this_thread::sleep_for(mSensorPeriod);
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error in sensor " << mSensorId << " occured: " << e.what() << '\n';
                mIsStopped = true;
            }
        }

        void stop() override
        {
            mIsStopped = true;
        }

    private:
        void generateRandomPosition()
        {
            std::uniform_real_distribution<double> coordinateDistribution(0, mPlayingFieldSize);

            mX = coordinateDistribution(mRandomEngine);
            mY = coordinateDistribution(mRandomEngine);
        }

        void generateRandomSpeed()
        {
            const double speed = mSpeedDistribution(mRandomEngine);
            const double direction = mDirectionDistribution(mRandomEngine);
            mSpeedX = speed * cos(direction);
            mSpeedY = speed * sin(direction);

            // random duration for this speed to remain constant
            mSpeedDuration = std::chrono::duration<double>(mSpeedDurationDistribution(mRandomEngine));

            mLastSpeedChange = std::chrono::system_clock::now();
        }

        void update()
        {
            const TimePoint timestamp = std::chrono::system_clock::now();
            updatePosition(timestamp);
            notifyObservers(mX, mY, timestamp);
            
            mLastTimestamp = timestamp;
        }

        void updatePosition(const TimePoint& timestamp)
        {
            // change speed & direction randomly once in a while
            if (timestamp - mLastSpeedChange > mSpeedDuration)
                generateRandomSpeed();

            const std::chrono::duration<double> duration = timestamp - mLastTimestamp;
            const double durationSeconds = duration.count();

            double newX = mX + mSpeedX * durationSeconds;
            double newY = mY + mSpeedY * durationSeconds;
            
            // change speed & direction if we hit the bounds
            while (newX < 0 || newX > mPlayingFieldSize || newY < 0 || newY > mPlayingFieldSize)
            {
                generateRandomSpeed();
                newX = mX + mSpeedX * durationSeconds;
                newY = mY + mSpeedY * durationSeconds;
            }

            mX = newX;
            mY = newY;
        }

        void notifyObservers(double x, double y, const TimePoint& timestamp)
        {
            for (PositionObserverPtr observer : mObservers)
                observer->onPositionUpdate(mSensorId, x, y, timestamp);
        }

    private:
        const int mSensorId;
        const std::chrono::milliseconds mSensorPeriod;
        const double mPlayingFieldSize;
        std::vector<PositionObserverPtr> mObservers;
        std::atomic_bool mIsStopped;
        std::default_random_engine mRandomEngine;
        std::uniform_real_distribution<double> mSpeedDistribution;
        std::uniform_real_distribution<double> mDirectionDistribution;
        std::uniform_real_distribution<double> mSpeedDurationDistribution;
        double mX;
        double mY;
        double mSpeedX;
        double mSpeedY;
        std::chrono::duration<double> mSpeedDuration;
        TimePoint mLastTimestamp;
        TimePoint mLastSpeedChange;
    };
}

PositionGeneratorPtr CreateRandomPositionGenerator(int sensorId, double frequencyHz, double playingFieldSizeMeters, 
    double maxSpeedMetersPerSec)
{
    return std::make_shared<RandomPositionGenerator>(sensorId, frequencyHz, playingFieldSizeMeters, maxSpeedMetersPerSec);
}
