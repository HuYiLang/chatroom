#include "../../include/myhead.h"

struct message msg = {0};
int status = 0;                  //登录成功的状态
int ban_flag = 0;                  //禁言的状态
char save_name[MAX_SIZE];

int main()
{
    int sockfd;

    struct sockaddr_in ser_addr;

    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket create error!");
	exit(1);
    }

    bzero(&ser_addr,sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = inet_addr("192.168.2.17");
    ser_addr.sin_port = htons(SER_PORT);
    
    
    if(connect(sockfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr)) < 0)
    {
        perror("connect error!");
	exit(1);
    }
  
   lts_main(sockfd);

   close(sockfd);

   return 0;
}
