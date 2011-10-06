#ifndef TYPES_H_
#define TYPES_H_

#ifndef EOF
#define EOF -1
#define O_RDONLY  0x0000
#define O_WRONLY  0x0001
#define O_RDWR    0x0002
#define O_CREAT   0x0200
#define O_TRUNC   0x0400
#endif

typedef long FILE;
typedef long unsigned int size_t;

typedef unsigned int time_t;
struct timespec {
	time_t tv_sec;   /* seconds */
	time_t tv_nsec;  /* nanoseconds */
};
struct tm {
	int tm_sec;         /* seconds */
	int tm_min;         /* minutes */
	int tm_hour;        /* hours */
	int tm_mday;        /* day of the month */
	int tm_mon;         /* month */
	int tm_year;        /* year */
	int tm_wday;        /* day of the week */
	int tm_yday;        /* day in the year */
	int tm_isdst;       /* daylight saving time */
};


#endif // TYPES_H_