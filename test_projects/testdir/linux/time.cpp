#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
int main(int argc,char * argv[])
{

	struct timeval tv;
	//while(1)
	//{
		gettimeofday(&tv,NULL);
		printf("time %u:%u\n",tv.tv_sec,tv.tv_usec);
		sleep(2);
	//}
	time_t t2;
	struct tm * tm2;
	//t2 = (time_t)1539162310;
	t2 = (time_t)tv.tv_sec;
	tm2 = gmtime(&t2);
	printf("zhd-debug tm.tm_year is %d\n",tm2->tm_year);
	printf("zhd-debug tm.tm_mon is %d\n",tm2->tm_mon);
	printf("zhd-debug tm.tm_mday is %d\n",tm2->tm_mday);
	printf("zhd-debug tm.tm_hour is %d\n",tm2->tm_hour);
	printf("zhd-debug tm.tm_min  is %d\n",tm2->tm_min);
	printf("zhd-debug tm.tm_sec is %d\n",tm2->tm_sec);
	printf("zhd-debug tm.tm_wday is %d\n",tm2->tm_wday);
	printf("zhd-debug tm.tm_yday is %d\n",tm2->tm_yday);
	printf("zhd-debug tm.tm_tm_isdst is %d\n",tm2->tm_isdst);

	
	return 0;
}
