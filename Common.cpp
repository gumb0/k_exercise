#include "Common.h"

#include <iomanip>
#include <sstream>

std::string FormatTimePoint(const TimePoint& timePoint)
{
    std::stringstream stream;

    const std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    stream << std::put_time(std::localtime(&time), "%F %T");

    const std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(timePoint.time_since_epoch());
    stream << '.' << ms.count() % 1000;

    return stream.str();
}
