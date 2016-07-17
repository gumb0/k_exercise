#include "PositionProcessorImpl.h"

#include <assert.h>
#include <math.h>
#include <vector>

namespace
{
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

            // skip first position update, we don't have enough data yet
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

            // TODO extract
            const double distance = sqrt((x - mLastX) * (x - mLastX) + (y - mLastY) * (y - mLastY));
            const double speed = distance / durationSeconds;

            mDistance += distance;

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

PositionProcessorPtr CreatePositionProcessorImpl(int sensorId)
{
    return std::make_shared<PositionProcessorImpl>(sensorId);
}
