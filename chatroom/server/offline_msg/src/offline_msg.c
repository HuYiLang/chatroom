#include "../../include/myhead.h"

static  sqlite3 *db=NULL;
static char **Result=NULL;
static char *errmsg=NULL;

int about_offlinemsg_done(struct message *msg,int cfd)
{
   printf("\n关于离线信息的操作\n");
   
   int rc, i = 0, j = 0;
   int nrow;
   int ncolumn;
   char sql[MAX_SIZE];
   char buffer2[MAX_SIZE];
   int ret;

   rc= sqlite3_open("haoyu.db", &db);
   printf("当前查看 %s 的离线消息记录\n",msg->name);
   sprintf(sql,"select * from chatmsg where username = '%s'",msg->name);
   rc= sqlite3_get_table(db, sql, &Result, &nrow, &ncolumn,&errmsg); 

    if(rc)
    {
       printf("query fail!\n");
       msg->action = 0;
    }
    else
    {
       printf("query success!\n");
       for(i = 0; i <= nrow; i++)
       {
          for(j = 0; j < ncolumn; j++)
          {
             printf("%s | ", Result[i * ncolumn + j]);
          }
          printf("\n");
          for(j = 0; j < ncolumn; j++)
          {
             printf("Result[%d] | ", i * ncolumn + j);
          }
          printf("\n");
       }
    }
   
   if(nrow > 0 && (msg->action == log))       //离线消息发给客户端
   {
       bzero(msg->msg,sizeof(msg->msg));
       printf("\n收到的客户端消息:action = %d id = %s name = %s passwd = %s online = %d msg = %s cfd= %d msg->toname = %s\n",msg->action,msg->id,msg->name,msg->passwd,msg->online,msg->msg,msg->cfd,msg->toname);  
         
       printf("离线信息发给客户端中....\n");
       printf("要发送人的姓名:%s\n",msg->name);

       if(Result[ncolumn + 1] != NULL)
       {
           printf("对应的离线消息:%s\n",Result[ncolumn + 1]);

           strcpy(msg->msg,Result[ncolumn + 1]);
       }
       else
       {
           printf("没有离线消息\n");
       }
             
       write(cfd,msg,sizeof(struct message)); 
       sprintf(sql,"update chatmsg set offline_msg=NULL where username='%s'",msg->name);//清空离线消息
       ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);

       printf("\n离线信息已经发给客户端:action = %d id = %s name = %s passwd = %s online = %d msg = %s cfd= %d msg->toname = %s\n",msg->action,msg->id,msg->name,msg->passwd,msg->online,msg->msg,msg->cfd,msg->toname);    
       return 0;
   }

   if(nrow > 0 && msg->action == say)     //将离线消息保存在客户端
   {
       printf("将新的离线信息保存在数据库中...\n");
       printf("\n收到的离线消息:action = %d id = %s name = %s passwd = %s online = %d msg = %s cfd= %d msg->toname = %s\n",msg->action,msg->id,msg->name,msg->passwd,msg->online,msg->msg,msg->cfd,msg->toname);    

       printf("1\n");
       memset(buffer2,0,sizeof(buffer2));
       sprintf(buffer2,"%s:%s:",msg->name,msg->msg);                  
       printf("要添加的消息:%s\n",buffer2);

       if(Result[ncolumn + 1] != NULL)                //原来有历史消息
       {
           printf("原来的历史消息:%s\n",Result[ncolumn + i]);
           strcat(buffer2,Result[ncolumn + i]);                  //将该用户发送的消息附加到后面
       }
       printf("2\n");
                    
       sprintf(sql,"update chatmsg set offline_msg = '%s' where username = '%s'",buffer2,msg->toname);
                   
       ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
       printf("更新数据库中...\n");

       if(ret != SQLITE_OK)
       {
           printf("update history error\n");
       }
       else
       {
           printf("update history success:\n");
       }   
    }

    sqlite3_free_table(Result);

    return 0;
}

