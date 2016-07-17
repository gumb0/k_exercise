#ifndef __common_h__
#define __common_h__

#include <chrono>
#include <string>

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;


std::string FormatTimePoint(const TimePoint& timestamp);


class NonCopyable
{
protected:
    NonCopyable() {}
    virtual ~NonCopyable() {}

private:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

#endif