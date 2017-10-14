#include "../../include/myhead.h"

static  sqlite3 *db=NULL;
static char **Result=NULL;
static char *errmsg=NULL;

int about_historymsg_done(struct message *msg,int cfd)
{
   printf("\n关于历史信息的操作\n");
   
   int rc, i, j;
   int nrow;
   int ncolumn;
   char sql[1024];
   char buffer2[1024];
   int ret;

   rc= sqlite3_open("haoyu.db", &db);
   printf("当前查看 %s 的消息记录\n",msg->name);
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
   
    if(nrow > 0 && (msg->action == historymsg))       //将历史消息发给客户端
    {
       printf("历史信息发给客户端中....\n");
       printf("要查找人的姓名:%s\n",msg->toname);

       for(i = 0; i < nrow * ncolumn; i++)
       {
           if(strcmp(msg->toname,Result[i]) == 0)
           {
               if(Result[ncolumn + i] != NULL)
               {
                   printf("对应的历史消息:%s\n",Result[ncolumn + i]);
                   strcpy(msg->msg,Result[ncolumn + i]);
               }
               printf("msg->msg[%d] = %s\n",msg->action,msg->msg);
               write(cfd,msg,sizeof(struct message)); 
               printf("\n历史信息已经发给客户端:action = %d id = %s name = %s passwd = %s online = %d msg = %s cfd= %d msg->toname = %s\n",msg->action,msg->id,msg->name,msg->passwd,msg->online,msg->msg,msg->cfd,msg->toname);    
               return 0;
           }
       }
       if(i == nrow * ncolumn)
       {
           msg->action = 0;
           write(cfd,msg,sizeof(struct message)); 
       }
    }

    if(nrow > 0 && msg->action == say)     //将历史消息保存在数据库
    {
       printf("将新的信息保存在数据库中...\n");
       printf("\n收到的历史消息:action = %d id = %s name = %s passwd = %s online = %d msg = %s cfd= %d msg->toname = %s\n",msg->action,msg->id,msg->name,msg->passwd,msg->online,msg->msg,msg->cfd,msg->toname);    
       for(i = 0; i < nrow * ncolumn; i++)
       {
           if(strcmp(msg->toname,Result[i]) == 0)
           {
                   memset(buffer2,0,sizeof(buffer2));
                   sprintf(buffer2,"%s:%s:",msg->name,msg->msg);
                   
                   printf("要添加的消息:%s\n",buffer2);
                   if(Result[ncolumn + i] != NULL)                //原来有历史消息
                   {
                       printf("原来的历史消息:%s\n",Result[ncolumn + i]);
                       strcat(buffer2,Result[ncolumn + i]);                  //将该用户发送的消息附加到后面
                   }
                    
                   /*更新自己行的历史消息*/
                   sprintf(sql,"update chatmsg set '%s' = '%s' where username = '%s'",msg->toname,buffer2,msg->name);
                   ret = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
                   /*更新对方行的历史消息*/
                   sprintf(sql,"update chatmsg set '%s' = '%s' where username = '%s'",msg->name,buffer2,msg->toname);
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
              break; 
           }
       }
       printf("msg->msg[%d] = %s\n",msg->action,msg->msg);
       
    }

    sqlite3_free_table(Result);
    return 0;
}

