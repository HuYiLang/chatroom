#include "../../include/myhead.h"

char* getDateTime();

int get_time()
{
	char* nowtime = getDateTime();
        printf("当前时间: ");
	printf("%s\n", nowtime);
	return 0;
}

char* getDateTime()
{
	static char nowtime[20];
	time_t rawtime;
	struct tm* ltime;
	time(&rawtime);
	ltime = localtime(&rawtime);
	strftime(nowtime, 20, "%Y-%m-%d %H:%M:%S", ltime);
	return nowtime;
}
