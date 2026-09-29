#include <cstdint>
#include <fstream>
#include <string>
#include <sstream>
#include <optional>
#include "systMon.h"


//getCPUStats() implementation
CPUStats SystemMonitoring::getCPUStats()
{
    CPUStats stats{0, 0, 0, 0, std::nullopt};


    std::ifstream cpuinfo("/proc/stat");

    if (!cpuinfo.is_open()) {
        return stats;
    }

    std::string line;

    while (std::getline(cpuinfo, line))
    {
        std::istringstream fin(line);

        std::string key;
        fin >> key;

        if (key == "cpu")
        {
            uint64_t user;
            uint64_t nice;
            uint64_t system;
            uint64_t idle;
            uint64_t iowait;
            uint64_t irq;
            uint64_t softirq;
            uint64_t steal;

            fin >> user
                >> nice
                >> system
                >> idle
                >> iowait
                >> irq
                >> softirq
                >> steal;

            uint64_t total = user + nice + system + idle
                            + iowait + irq + softirq + steal;

            uint64_t idleTime = idle + iowait;

            if (previousTotal != 0)
            {
                uint64_t totalDiff = total - previousTotal;
                uint64_t idleDiff = idleTime - previousIdle;

                if (totalDiff > 0)
                {
                    stats.usage = static_cast<int>(
                        100.0 * (totalDiff - idleDiff) / totalDiff
                    );
                }
            }

            previousTotal = total;
            previousIdle = idleTime;
        }
        else if (key == "processes")
        {
            int processes;
            fin >> processes;

            stats.processesCount = processes;
        }
    }


    std::ifstream uptimeFile("/proc/uptime");

    if (uptimeFile.is_open())
    {
        double uptimeSeconds;
        uptimeFile >> uptimeSeconds;

        stats.uptime = static_cast<uint64_t>(uptimeSeconds);
    }

    //threadscount
    std::ifstream loadavgFile("/proc/loadavg");
    if (loadavgFile.is_open()) {
        std::string line;
        std::getline(loadavgFile,line);
        std::istringstream fin(line);
        double load1,load5,load15;
        std::string runningThreads;
        fin>>load1>>load5>>load15>>runningThreads;
        std::istringstream threadsStream(runningThreads);
        int running;
        int total;
        char separator;
        threadsStream >> running >> separator >> total;

        stats.threadsCount = total;
    }
  //temperature
    std::ifstream tempFile("/sys/class/thermal/thermal_zone0/temp");

    if (tempFile.is_open()) {
        int temperatureRaw;
        tempFile >> temperatureRaw;

        stats.temperature = temperatureRaw / 1000.0;
    }
    return stats;
}

//getMemStats() implemenation
MemStats SystemMonitoring::getMemStats() {
    MemStats stats{0,0,0};
    std::ifstream meminfo("/proc/meminfo");

    if (!meminfo.is_open())
    {
        return stats;
    }
    std::string line;
    uint64_t memTotal =0;
    uint64_t MemFree =0;
    uint64_t memAvailable=0;
   while (std::getline(meminfo,line)) {
       std::istringstream fin(line);
       std::string key;
       uint64_t value;
       fin>>key>>value;
       if (key=="MemTotal:") {
           memTotal=value;
       }else if (key=="MemFree:") {
           MemFree=value;
       }else if (key=="MemAvailable:") {
           memAvailable=value;
       }
   }
    stats.total=memTotal;
    if (memAvailable!=0) {
        stats.mfree=memAvailable;
    }else {
        stats.mfree=MemFree;
    }
    stats.used=stats.total-stats.mfree;
    return stats;

}