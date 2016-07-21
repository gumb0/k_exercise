#include "NoiseReducingPositionProcessor.h"

#include <deque>

namespace
{
    class NoiseReducingPositionProcessor : public PositionProcessor
    {
    public:
        explicit NoiseReducingPositionProcessor(PositionProcessorPtr processor, std::size_t filterLength) : 
            mProcessor(processor),
            mFilterLength(filterLength),
            mSumX(0),
            mSumY(0)
        {
        }

        void addStatisticsObserver(StatisticsObserverPtr observer) override
        {
            mProcessor->addStatisticsObserver(observer);
        }

        void onPositionUpdate(int sensorId, double x, double y, const TimePoint& timestamp) override
        {
            // Moving Average filter - return average of last mFilterLength values

            mSumX += x;
            mLastX.push_back(x);

            mSumY += y;
            mLastY.push_back(y);

            if (mLastX.size() > mFilterLength)
            {
                mSumX -= mLastX.front();
                mLastX.pop_front();

                mSumY -= mLastY.front();
                mLastY.pop_front();
            }

            const double filteredX = mSumX / mLastX.size();
            const double filteredY = mSumY / mLastY.size();

            mProcessor->onPositionUpdate(sensorId, filteredX, filteredY, timestamp);
        }

    private:
        PositionProcessorPtr mProcessor;
        const std::size_t mFilterLength;
        std::deque<double> mLastX;
        std::deque<double> mLastY;
        double mSumX;
        double mSumY;
    };
}

PositionProcessorPtr CreateNoiseReducingPositionProcessor(PositionProcessorPtr processor, std::size_t filterLength)
{
    return std::make_shared<NoiseReducingPositionProcessor>(processor, filterLength);
}
