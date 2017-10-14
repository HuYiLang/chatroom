#include "../../include/myhead.h"

void * read_msg(void *arg);
extern int produce_random_num;
extern long random_num;

extern struct message msg;
extern int status;                  //登录成功的状态
extern int ban_flag;                  //禁言的状态

extern char save_name[MAX_SIZE];

int lts_main(int sockfd)
{
   char cmd[MAX_SIZE];                       //命令字符数组

   char confirm_passwd[MAX_SIZE];                 //确认密码数组
   char temp[MAX_SIZE];

   pthread_t id;
   pthread_create(&id,NULL,read_msg,(void *)&sockfd);
   
   log_face();                         //显示登录界面
   get_time();

   int k = 0;                        //临时变量，用于illegal
 
   /*主程序*/
   while(1)
   {   
       scanf("%s",cmd);
    
       if(produce_random_num == 1)                   //注册
       {
           msg.action = update_id;
           sprintf(msg.id,"%ld",random_num);         //发送对应的id 给服务器
           write(sockfd,&msg,sizeof(msg));
           produce_random_num = 0;        
       }

       if(status == 0)
       {
           memset(&msg,0,sizeof(struct message));
       }
        
       if(status == 1 && strlen(save_name) > 0)     //保证每次都是自己的名字:msg.name
       {
           strcpy(msg.name,save_name);
       } 
 
       if(strcmp(cmd,"exit") == 0 && status == 0) 
       { 
           printf("\n");
           exit(0);  
       } 
               
       else if(strcmp(cmd,"reg") == 0 && status == 0) 
       {
           struct message msg;
           memset(&msg,0,sizeof(msg));

           printf("\n输入你要注册的昵称: ");
           scanf("%s",msg.name);

           if((k = illegal(msg.name)) == 0)                //名称里不能包含  :
           {
               continue;
           }

           printf("\n输入你要注册的密码: ");
           scanf("%s",msg.passwd);
           printf("\n请确认你要注册的密码: ");
           bzero(confirm_passwd,sizeof(confirm_passwd));
           scanf("%s",confirm_passwd);
           
           if(strcmp(confirm_passwd,msg.passwd) != 0)
           {
               printf("\n密码不一致\n");
               continue;
           }
  
           msg.action = reg;
           write(sockfd,&msg,sizeof(msg));
           
       }      
       else if(strcmp(cmd,"log") == 0 && status == 0) 
       {   
           printf("\n请输入你要登录的id: ");
           scanf("%s",msg.id);
           shift_passwd(msg.passwd);            //输入密码，并转换成*

           msg.action = log;
           write(sockfd,&msg,sizeof(msg));
       }      

       
 
       else if(strcmp(cmd,"show") == 0 && status == 1) 
       {   
           
           msg.action = show;
           write(sockfd,&msg,sizeof(msg));
       }      
       else if(strcmp(cmd,"say") == 0 && status == 1) 
       {
            if(ban_flag == 1)
            {
               printf("你已经被禁言，不可以发消息\n");
               continue;
            }

            bzero(msg.msg,sizeof(msg.msg)); 
            msg.action = say;
            bzero(msg.toname,sizeof(msg.toname)); //更新好友
            printf("\n输入你要发送的人的名字: ");
	    scanf("%s",msg.toname);
            getchar();
            printf("\n信息:");
	    scanf("%[^\n]",msg.msg);

            if((k = illegal(msg.msg)) == 0)                //名称里不能包含  :
            {
               continue;
            }

            write(sockfd,&msg,sizeof(msg));
       }
       else if(strcmp(cmd,"stoall") == 0 && status == 1) 
       {
           if(ban_flag == 1)
           {
               printf("你已经被禁言，不可以发消息\n");
               continue;
           }

           msg.action = stoall;
           bzero(msg.msg,sizeof(msg.msg)); //更新好友状态
 
           printf("你对全体在线成员说: ");
           getchar();
	   scanf("%[^\n]",msg.msg);
           
	   write(sockfd,&msg,sizeof(msg));
       }
       else if(strcmp(cmd,"off") == 0 && status == 1) 
       {
           msg.action = off;

           write(sockfd,&msg,sizeof(msg));
          
       }
       else if(strcmp(cmd,"chg_psd") == 0 && status == 1) 
       {
           msg.action = cg_pd;
           printf("\n请输入更改的密码: ");
           scanf("%s",msg.passwd);
           write(sockfd,&msg,sizeof(msg));
       
       }
       else if(strcmp(cmd,"chg_nam") == 0 && status == 1) 
       {
           
           if(strcmp("root",msg.name) == 0)
           {
               printf("不允许更改管理员的昵称\n");
               continue;
           }
           
           msg.action = cg_ne;
           strcpy(msg.msg,msg.name);      //保存原来的，用于数据库的更新用户名
           bzero(msg.name,sizeof(msg.name));
           bzero(temp,sizeof(temp));
           printf("\n请输入更改的昵称: ");
           scanf("%s",temp);
           
           if((k = illegal(temp)) == 0)                //名称里不能包含  :
           {
               continue;
           }

           if(strcmp("root",temp) == 0)
           {
               printf("不允许更改该昵称\n");
               continue;
           }
           else
           {
               strcpy(msg.name,temp);
               strcpy(save_name,temp);
                
           }
           
           write(sockfd,&msg,sizeof(msg));

       }
       else if(strcmp(cmd,"help") == 0 && status == 1) 
       {
           get_time();
           face(msg.name);
           continue;
       }
       else if(strcmp(cmd,"exit") == 0 && status == 1) 
       {
           msg.action = exit_;
           write(sockfd,&msg,sizeof(msg));
           
           
       }
       else if(strcmp(cmd,"history_msg") == 0 && status == 1) 
       {
           msg.action = historymsg;
           bzero(msg.toname,sizeof(msg.toname));
           printf("\n请输入要查看的好友: ");
           scanf("%s",msg.toname);
           write(sockfd,&msg,sizeof(msg));
       }
       else if((strcmp(cmd,"out") == 0) && (strcmp(msg.name,"root") == 0) && status == 1) 
       {
           msg.action = out;
           bzero(msg.toname,sizeof(msg.toname));
           printf("\n请输入要踢出的好友: ");
           scanf("%s",msg.toname);

           if(strcmp("root",msg.toname) == 0)
           {
               printf("管理员不允许踢出自己\n");
               continue;
           }

           write(sockfd,&msg,sizeof(msg));
          
       }    
       else if((strcmp(cmd,"ban") == 0) && (strcmp(msg.name,"root") == 0) && status == 1) 
       {
           msg.action = ban;

           bzero(msg.toname,sizeof(msg.toname));
           printf("\n请输入要禁言的好友: ");
           scanf("%s",msg.toname);

           if(strcmp("root",msg.toname) == 0)
           {
               printf("管理员不允许禁言自己\n");
               continue;
           }

           write(sockfd,&msg,sizeof(msg));
          
       }    

       else if((strcmp(cmd,"unban") == 0) && (strcmp(msg.name,"root") == 0) && status == 1) 
       {
           msg.action = unban;

           bzero(msg.toname,sizeof(msg.toname));
           printf("\n请输入要解除禁言的好友: ");
           scanf("%s",msg.toname);
  
           if(strcmp("root",msg.toname) == 0)
           {
               printf("管理员不允许该操作\n");
               continue;
           }

           write(sockfd,&msg,sizeof(msg));
          
       }    
       else
       {
           printf("\n输入有误，请重新输入：\n");
       }
   }

   return 0;
}
