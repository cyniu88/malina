#include "idomtools.h"
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

void m(){
    struct utsname u;
    uname (&u);
    printf ("%s release %s (version %s) on %s\n", u.sysname, u.release,
            u.version, u.machine);
}
void print_cpu_time()
{
    struct rusage usage;
    getrusage (RUSAGE_SELF, &usage);
    printf ("CPU time: %ld.%06ld sec user, %ld.%06ld sec system\n",
            usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,
            usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
}
std::string iDomTOOLS::getSystemInfo()
{
    double load[3];
    if (getloadavg(load, 3) != -1)
    {
        printf("load average : %f , %f , %f\n", load[0],load[1],load[2]);
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
    ret <<  "System uptime: " << days <<" day " << hours
         <<" hours " << minutes << " minutes "
        << seconds << " seconds " << "\n" << "Load: "
        << (info.loads[0]/65536) << "% - 1 min, " <<(info.loads[1]/65536)
            << "% - 5 min, "<<(info.loads[2]/65536) << "% - 15 min.\n"
                                                    << "process count : " << info.procs << "\n"
                                                    << "total RAM   : "<< (info.totalram / megabyte)<< "MB"<< std::endl
                                                    << "free RAM   : " << (info.freeram / megabyte) << "MB"<< std::endl;
    std::cout  << std::endl;
    print_cpu_time();
    m();
    std::cout  << std::endl;
    return ret.str();
}
