#include "idomtools.h"
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

std::string iDomTOOLS::getSystemInfo()
{
    double load[3];
    if (getloadavg(load, 3) != -1)
    {
#ifdef BT_TEST
        printf("load average : %f , %f , %f\n", load[0],load[1],load[2]);
#endif
    }
    const double megabyte = 1024 * 1024;
    struct sysinfo info;
    sysinfo(&info);

    long input_seconds = info.uptime;
    auto days = input_seconds / 60 / 60 / 24;
    auto hours = (input_seconds / 60 / 60) % 24;
    auto minutes = (input_seconds / 60) % 60;
    auto seconds = input_seconds % 60;

    std::stringstream  ret;
    ret << "System uptime: " << days <<" day " << hours
        << " hours " << minutes << " minutes "
        << seconds << " seconds " << std::endl << "Load: "
        << (info.loads[0]/65536) << "% - 1 min, " <<(info.loads[1]/65536)
        << "% - 5 min, "<<(info.loads[2]/65536) << "% - 15 min." << std::endl
        << "process count : " << info.procs << std::endl
        << "total RAM   : "<< (info.totalram / megabyte)<< "MB" << std::endl
        << "free RAM   : " << (info.freeram / megabyte) << "MB" << std::endl;

    return ret.str();
}

