#pragma once
#include <optional>
#include <cstdint>
struct CPUStats {
    int usage;
    int processesCount;
    int threadsCount;
    uint64_t uptime;
    std::optional<double> temperature;
};
struct MemStats {
    uint64_t total;
    uint64_t used;
    uint64_t mfree;

};

//class
class SystemMonitoring {
public:
    CPUStats getCPUStats();
    MemStats getMemStats();

private:
    uint64_t previousTotal = 0;
    uint64_t previousIdle = 0;

};
