#include "../../include/myhead.h"

extern int status;         //在线的状态
extern int ban_flag;            //禁言的状态
extern char save_name[MAX_SIZE];

int produce_random_num = 0;
long random_num = 0;
             
extern struct message msg;

void * read_msg(void *arg)
{
    int cfd = *((int *)arg);
    int k = 0;
    int i = 0;
    
    char history_msg[1024][1024];    //历史消息

    int n_read;

    char temp[MAX_SIZE];     //临时变量

    while(1)
    {           
	        n_read = read(cfd,&msg,sizeof(struct message)); 
            
	        if(n_read == 0)
	        {
	            printf("服务器已关闭\n");
		    break;
	        }

		switch(msg.action)
		{
		    case show:
		    {
                        printf("\n当前在线人: %s\n",msg.onlinename);
			break;
		    }
		    case say:
		    {    
                         strcpy(temp,msg.name);
                         strcpy(msg.name,msg.toname);
                         strcpy(msg.toname,temp);
                         
                         printf("\n(在线消息)%s对你说: %s.\n",msg.toname,msg.msg);
			 break;
		    }
                    case -show:
                    {
                       printf("好友离线,以转为离线消息发送\n");
                       break;  
                       
                    }
                    case -say:
                    {  
                       printf("没有该好友\n");
                       break;  
                      
                    }
		    case off:
		    {
                         status = 0;
                         log_face();
                         break;
		    }
		    case exit_:
		    {
                         status = 0;
                         exit(0);
                         break;
		    }
		    case cg_pd:
		    {
                         printf("\n更改密码成功:\n");
			 break;
		    }
		    case cg_ne:
		    {
                       printf("\n更改昵称成功:\n");
                       strcpy(save_name,msg.name);
                       face(msg.name);
                       break;
		    }
		    case -cg_ne:
		    {
                         printf("\n更改昵称失败，该昵称已被注册:\n");
                         break;
     		    }
		    case stoall:
		    {    
                         printf("\n%s对全体在线成员说: %s\n",msg.name,msg.msg);
			 break;
		    }
                    case historymsg:
                    {
                        if(strlen(msg.msg) > 0)
                        { 
                             i = 0;
                             bzero(history_msg,sizeof(history_msg));
                             k = get_name_offlinemsg(history_msg,msg.msg);
       
                             while(i + 1 <= k)
                             {
                                  printf("(历史消息)%s 说 %s\n",history_msg[i],history_msg[i + 1]);
                                  i += 2;
                             }
                             printf("\n");
                        }
                        else
                        {
                           printf("没有历史消息\n");
                        }
                        break; 
                    }
                    case 0:
                    {  
                       printf("操作失败\n");
                       break;  
                    }
                    case log:
                    {
                       printf("登录成功;\n");
                       strcpy(save_name,msg.name);
                       
                       sleep(1);

                       face(msg.name);
                       i = 0;
                       bzero(history_msg,sizeof(history_msg));
                       k = get_name_offlinemsg(history_msg,msg.msg);
       
                       while(i + 1 <= k)
                       {
                           printf("(离线消息)%s 说 %s\n",history_msg[i],history_msg[i + 1]);
                           i += 2;
                       }
                       printf("\n");
                
                       status = 1;             //更新登录状态
                       break;
                    } 
                    case -log:
                    {
                       printf("不可以重复登录\n");
                       break;
                    }   
                    case -reg:
                    {
                       printf("注册失败,该昵称已经有人注册\n");
                       break;  
                    }
                    case reg:
                    {
                       random_num = create_random_num();

                       printf("注册成功,你的id: ");
                       printf("\033[42m\033[31m%ld""\033[m",random_num);
                       produce_random_num = 1;
                       break;
                       
                    }               
                    case out:
                    {
                       status = 0;
                       log_face();
                       printf("你已经被踢出聊天室\n");
                       break;
                    }     
                    case -ban:
                    {
                       printf("操作无效，当前好友不在线\n");
                       break;
                    }        
                    case ban:
                    {
                       ban_flag = 1;
                       printf("你已经被禁言\n");
                       break;
                    }     
                    case unban:
                    {
                       ban_flag = 0;
                       printf("你已经解除禁言\n");
                       break;
                    }     
		}
    
       printf("\n");
       get_time();
    }
}
