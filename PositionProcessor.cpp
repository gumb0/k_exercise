#include "PositionProcessor.h"

#include <assert.h>
#include <math.h>
#include <vector>

namespace
{
    inline double GetDistance(double x1, double y1, double x2, double y2)
    {
        return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }

    inline double GetSpeed(double distance, double duration)
    {
        return distance / duration;
    }


    class PositionProcessorImpl : public PositionProcessor
    {
    public:
        explicit PositionProcessorImpl(int sensorId) : 
            mSensorId(sensorId),
            mLastX(0),
            mLastY(0),
            mDistance(0)
        {
        }

        void addStatisticsObserver(StatisticsObserverPtr observer) override
        {
            mObservers.push_back(observer);
        }

        void onPositionUpdate(int sensorId, double x, double y, const TimePoint& timestamp) override
        {
            assert(sensorId == mSensorId);

            // first position update is skipped, it's not enough data to update statistics yet
            if (mLastX > 0 && mLastY > 0)
                updateStatistics(x, y, timestamp);

            mLastX = x;
            mLastY = y;
            mLastTimestamp = timestamp;
        }

    private:
        void updateStatistics(double x, double y, const TimePoint& timestamp)
        {
            const std::chrono::duration<double> duration = timestamp - mLastTimestamp;
            const double durationSeconds = duration.count();

            const double distance = GetDistance(x, y, mLastX, mLastY);
            mDistance += distance;

            const double speed = GetSpeed(distance, durationSeconds);

            notifyObservers(speed, mDistance, timestamp);
        }

        void notifyObservers(double speed, double distance, const TimePoint& timestamp)
        {
            for (StatisticsObserverPtr observer : mObservers)
                observer->onStatisticsUpdate(mSensorId, speed, distance, timestamp);
        }

    private:
        const int mSensorId;
        std::vector<StatisticsObserverPtr> mObservers;
        double mLastX;
        double mLastY;
        TimePoint mLastTimestamp;
        double mDistance;
    };
}

PositionProcessorPtr CreatePositionProcessor(int sensorId)
{
    return std::make_shared<PositionProcessorImpl>(sensorId);
}
