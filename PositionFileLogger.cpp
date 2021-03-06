#include "Common.h"
#include "PositionFileLogger.h"

#include <fstream>
#include <string>

namespace
{
    const std::string FileNamePrefix = "position";
    const std::string FileNameSuffix = ".log";

    std::string GetFileName(int sensorId)
    {
        return FileNamePrefix + std::to_string(sensorId) + FileNameSuffix;
    }


    class PositionFileLogger : public PositionObserver
    {
    public:
        explicit PositionFileLogger(int sensorId) : mFile(GetFileName(sensorId))
        {
            if (!mFile)
                throw std::runtime_error("Failed to open position log file for write.");
        }

        void onPositionUpdate(int sensorId, double x, double y, const TimePoint& timestamp) override
        {
            // assuming several updates from the same sensor won't happen at the same time, so writing to file is safe

            mFile << FormatTimePoint(timestamp) << ' ' << sensorId << ' ' << x << ' ' << y << '\n';
            if (!mFile)
                throw std::runtime_error("Failed write to position log file.");
        }

    private:
        std::ofstream mFile;
    };
}

PositionObserverPtr CreatePositionFileLogger(int sensorId)
{
    return std::make_shared<PositionFileLogger>(sensorId);
}
