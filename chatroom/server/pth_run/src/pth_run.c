#include "../../include/myhead.h"

void * read_msg(void *arg)
{
   int cfd = *((int *)arg);
   int ret;
   char *errmsg;
   char sql[MAX_SIZE];
   sqlite3 *db;
   int to_fd;
   int n_read;

   struct message *msg = (struct message *)malloc(sizeof(struct message));

   ret = sqlite3_open("haoyu.db",&db);

   if(ret != SQLITE_OK)
   {
      perror("sqlite open error!");
      exit(1);
   }

   while(1)
   {   
       printf("\n\033[42m\033[31m服务器线程运行中...\033[m\n");

       bzero(sql,sizeof(sql));

       memset(msg,0,sizeof(struct message));

       n_read = read(cfd,msg,sizeof(struct message));

       if(n_read == 0)
       {
	   printf("\033[42m\033[31m客户端已下线！\033[m\n"); 
           break;
       }

       printf("\n\033[40m\033[36m收到客户端的信息\033[m:action = %d id = %s name = %s passwd = %s online = %d msg = %s cfd= %d msg->toname = %s msg->onlinename = %s\n",msg->action,msg->id,msg->name,msg->passwd,msg->online,msg->msg,msg->cfd,msg->toname,msg->onlinename);

	    if(msg->action == off || msg->action == exit_)                        //退出登录
            {      
                   printf("\n退出中，改变cfd,online\n");
                      
                   msg->online = 0;
                   
		   sprintf(sql,"update login set online='%d' where username='%s'",msg->online,msg->name);
                   ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
                   
		   sprintf(sql,"update login set cfd='%d' where username='%s'",cfd + 1024,msg->name);
                   ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
               
                   if(ret != SQLITE_OK)
                   {
		      printf("update online = 0 error\n");
		   }
                   else
                   {
                      printf("update online = 0 success:\n");
		   }   
            }

	    if(msg->action == update_id)                        // 更新登录是id
            {
                   printf("\n更新登录状态\n");
              
		   sprintf(sql,"update login set id='%s' where username='%s'",msg->id,msg->name);
                
                   ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);

                   if(ret != SQLITE_OK)
                   {
		      printf("update error\n");
		      exit(1);
		   }

                   printf("update success:\n");
                   continue;
	    }

	    if(msg->action == cg_pd)                     //改密码
            {
                   printf("\n改密码中\n");
              

		   sprintf(sql,"update login set password='%s' where id='%s'",msg->passwd,msg->id);
                
                   ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);

                   
                   if(ret != SQLITE_OK)
                   {
		      printf("update error\n");
		      exit(1);
		   }
                   printf("update success:\n");
	    }

	    if(msg->action == cg_ne)                      //改名
            {
                   printf("\n改昵称中\n");

		   sprintf(sql,"update login set username='%s' where id='%s'",msg->name,msg->id);
                   ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);

                   if(ret != SQLITE_OK)
                   {
		      printf("update error\n");
		      msg->action = 0 - cg_ne;
		   }
                   else
                   {
                      printf("update success:\n");
		      sprintf(sql,"update chatmsg set username='%s' where username='%s'",msg->name,msg->msg);//更新聊天记录用户名
                      ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
                      amend_column_name(msg);
                     
		   }   
	       
	    }

            if(msg->action == show)                //展示
            {
               printf("\n查看所以在线好友\n");
               
               scan_db(msg);                       //获取在线人名
               printf("在线人: %s\n",msg->onlinename);
            }

            if(msg->action == say)                 //发消息
            {
               printf("\n发信息检测中...\n");
               to_fd = get_table_name(msg);
               if(msg->action == 0)
               {
                   printf("没有该好友\n");
               }
               else if(msg->online == 1)            //在线
               {
                  about_historymsg_done(msg,cfd);             //保存聊天消息
                  printf("to_cfd= %d\n",to_fd);
                  write(to_fd,msg,sizeof(struct message));
                  printf("\n发消息给客户端:action = %d id = %s name = %s passwd = %s online = %d msg = %s cfd= %d msg->toname = %s msg->onlinename = %s\n",msg->action,msg->id,msg->name,msg->passwd,msg->online,msg->msg,to_fd,msg->toname,msg->onlinename);
                  continue;
               }
               else                             //不在线
               {
                   about_historymsg_done(msg,cfd);             //保存聊天消息

                   about_offlinemsg_done(msg,cfd);             //保存离线消息

                   msg->action = -show;
               }
            }
         
          
            if(msg->action == stoall)
            {
                 printf("\n群发在线好友\n");

		 printf("cfd = %d\n",cfd);
                 printf("msg->cfd = %d\n",msg->cfd);
                 
                 get_online_cfd(msg,cfd);             //获取在线人的cfd,并发送
                 continue;
            }
            
            if(msg->action == log)                  //登录
            {     
                   printf("\n登录中\n");
                   get_table(msg);

                   if(strlen(msg->name) > 0 && (msg->online == 0))
                   {      
                       msg->online = 1;                    //登录成功
		       sprintf(sql,"update login set online='%d' where username='%s'",msg->online,msg->name);
                       ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
		       sprintf(sql,"update login set cfd='%d' where username='%s'",cfd,msg->name);
                       ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
                       msg->cfd = cfd;
                       printf("login success! and update online = 1 cfd= %d\n",cfd);

                       about_offlinemsg_done(msg,cfd);             //发送离线消息

                       continue;
                   }
                   else if(msg->online == 1)
                   {
                       msg->action = -1;             //不可以重复登录
                   }
                   else                      //登录失败
                   {
                       msg->action = 0;
                       printf("login fail!\n");
                   }
            }

            if(msg->action == reg)                  //注册
            {
                  
                   printf("\n注册中\n");
                   msg->cfd = 0;
                   msg->online = 0;
                   
                   sprintf(sql,"insert into login(username,password,id,online,cfd) values('%s','%s','%s','%d','%d')",msg->name,msg->passwd,msg->id,msg->online,msg->cfd);

                   ret =  sqlite3_exec(db,sql,NULL,NULL,&errmsg);

                   if(ret != SQLITE_OK)
                   {
		       printf("insert fail\n");
	               msg->action = -reg;          //注册失败
                   }  
                   else
                   {
                       msg->action = reg;       //注册成功
                       
                       /*创建聊天记录表格*/
                       sprintf(sql,"insert into chatmsg(username) values('%s')",msg->name);
                       sqlite3_exec(db,sql,NULL,NULL,&errmsg);
                       sprintf(sql,"alter table chatmsg add column '%s' text",msg->name);
                       sqlite3_exec(db,sql,NULL,NULL,&errmsg);

                   } 
            }
             
            /*获得聊天记录*/
            if(msg->action ==  historymsg)
            {
                 bzero(msg->msg,sizeof(msg->msg));
                 about_historymsg_done(msg,cfd);            //获得聊天记录

                 continue;                 
            }
              
            /*踢人*/
            if(msg->action == out)
            {
               printf("\n踢人检测中...\n");
               to_fd = get_table_name(msg);
               if(msg->action == 0)
               {
                   printf("没有该好友\n");
               }
               else if(msg->online == 1)            //在线
               {
                  printf("to_cfd= %d\n",to_fd);

		  sprintf(sql,"update login set online='%d' where username='%s'",0,msg->toname);
                  ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
                   
		  sprintf(sql,"update login set cfd='%d' where username='%s'",cfd + 1024,msg->toname);
                  ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);

                  write(to_fd,msg,sizeof(struct message));
                  printf("\n发消息给客户端:action = %d id = %s name = %s passwd = %s online = %d msg = %s cfd= %d msg->toname = %s\n",msg->action,msg->id,msg->name,msg->passwd,msg->online,msg->msg,to_fd,msg->toname);
                  continue;
               }
               else                             //不在线
               {
                   msg->action = -ban;
               }
                
            }
            if(msg->action == ban || msg->action == unban)
            {
               printf("\n禁言检测中...\n");
               to_fd = get_table_name(msg);
               if(msg->action == 0)
               {
                   printf("没有该好友\n");
               }
               else if(msg->online == 1)            //在线
               {
                  printf("to_cfd= %d\n",to_fd);

                  write(to_fd,msg,sizeof(struct message));
                  printf("\n发消息给客户端:action = %d id = %s name = %s passwd = %s online = %d msg = %s cfd= %d msg->toname = %s\n",msg->action,msg->id,msg->name,msg->passwd,msg->online,msg->msg,to_fd,msg->toname);
                  continue;
               }
               else                             //不在线
               {
                   msg->action = -ban;
               }
                
            }
          
       printf("\n\n服务器发送给客户端:action = %d id = %s name = %s passwd = %s online = %d msg = %s cfd= %d msg->toname = %s\n",msg->action,msg->id,msg->name,msg->passwd,msg->online,msg->msg,msg->cfd,msg->toname);

       write(cfd,msg,sizeof(struct message));    //发送
   }
}
