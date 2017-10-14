#include "../../include/myhead.h"

void * read_msg(void *arg);

int main()
{
    system("ifconfig eth0 192.168.2.17");

    int s_len;
    int sockfd;
    int cfd;
    pthread_t p_id;

    struct sockaddr_in ser_addr;
    struct sockaddr_in client_addr;

    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == - 1)
    {
        perror("socket creat error!");
	exit(1);
    }
    
    printf("sockfd success!\n");

    bzero(&ser_addr,sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    //ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_addr.s_addr = inet_addr("192.168.2.17");
    ser_addr.sin_port = htons(SER_PORT);

    if(bind(sockfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr)) == -1)
    {
        perror("bind error!");
	exit(1);
    }

    printf("bind success!\n");
     
    if(listen(sockfd,5) == -1)
    {
        perror("listen error!");
	exit(1);
    }
    
    printf("listen success!\n");
    
    while(1)
    {
        s_len = sizeof(client_addr);
        if((cfd = accept(sockfd,(struct sockaddr *)&client_addr,&s_len)) < 0)
        {
            perror("accept error!");
	    exit(1);
        }
        printf("accept success = %s\n",inet_ntoa(client_addr.sin_addr));

        printf("server cfd = %d\n",cfd);

        pthread_create(&p_id,NULL,read_msg,(void *)&cfd);

    }

    close(sockfd);

    return 0;
}
