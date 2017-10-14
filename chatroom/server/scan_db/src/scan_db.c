#include "../../include/myhead.h"

int handle_result(void *para, int column_count, char **column_value, char **column_name)
{

    struct message *msg = (struct message *)para;
    strcat(msg->onlinename,column_value[0]);
    strcat(msg->onlinename," ");
    
    return 0;
}

int scan_db(struct message *msg)
{
    int ret;
    char *errmsg;


    sqlite3 *db;
    
    ret = sqlite3_open("haoyu.db",&db);

    if(ret != SQLITE_OK)
    {
        perror("sqlite open error!");
	exit(1);
    }

    bzero(msg->onlinename,sizeof(msg->onlinename));

    sqlite3_exec(db,"select * from login where online = 1",handle_result,msg,&errmsg);

    printf("str = %s\n",msg->onlinename);

    return 0;

}
