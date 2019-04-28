#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>


typedef struct _event_log_ {
	uint64_t	next_addr;	// physical address of next event
	uint32_t	log_type;
	uint32_t	etype;
	uint32_t	log_data[0];	// log data, format depending on
} event_log_t;


int main(int argc,char * argv[])
{

    event_log_t tt;
    printf("tt size = %lu \n", sizeof(tt)); 
    return 0;
}
