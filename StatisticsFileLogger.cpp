#include "Common.h"
#include "StatisticsFileLogger.h"

#include <fstream>
#include <string>

namespace
{
    const std::string FileNamePrefix = "statistics";
    const std::string FileNameSuffix = ".log";

    std::string GetFileName(int sensorId)
    {
        return FileNamePrefix + std::to_string(sensorId) + FileNameSuffix;
    }


    class StatisticsFileLogger : public StatisticsObserver
    {
    public:
        explicit StatisticsFileLogger(int sensorId) : mFile(GetFileName(sensorId))
        {
            if (!mFile)
                throw std::runtime_error("Failed to open position statisics file for write.");
        }

        void onStatisticsUpdate(int sensorId, double speed, double distance, const TimePoint& timestamp) override
        {
            // assuming several updates from the same sensor won't happen at the same time, so writing to file is safe

            mFile << FormatTimePoint(timestamp) << ' ' << sensorId << ' ' << speed << ' ' << distance << '\n';
            if (!mFile)
                throw std::runtime_error("Failed write to statistics log file.");
        }

    private:
        std::ofstream mFile;
    };
}

StatisticsObserverPtr CreateStatisticsFileLogger(int sensorId)
{
    return std::make_shared<StatisticsFileLogger>(sensorId);
}
