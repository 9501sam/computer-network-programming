#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
    time_t my_time;
    struct tm * timeinfo;
    time (&my_time);
    timeinfo = localtime (&my_time);
    printf("year->%d\n",timeinfo->tm_year+1900);
    printf("month->%d\n",timeinfo->tm_mon+1);
    printf("date->%d\n",timeinfo->tm_mday);
    printf("hour->%d\n",timeinfo->tm_hour);
    printf("minutes->%d\n",timeinfo->tm_min);
    printf("seconds->%d\n",timeinfo->tm_sec);

}

