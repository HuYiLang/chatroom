#include "../../include/myhead.h"

static  sqlite3 *db=NULL;
static char **Result=NULL;
static char *errmsg=NULL;

int get_online_cfd(struct message *msg,int cfd1)
{
   int rc, i, j = 0;
   int nrow;
   int ncolumn;
   char sql[MAX_SIZE];
   char buffer[MAX_SIZE];
   int cfd;             //发送在线人对应的cfd

   rc= sqlite3_open("haoyu.db", &db);

   printf("msg->id = %s\n",msg->id);
   printf("msg->passwd = %s\n",msg->passwd);
   printf("msg->id = %d\n",msg->cfd);
   

   sprintf(sql,"select * from login where online = '%d'",1);

   rc= sqlite3_get_table(db,sql, &Result, &nrow, &ncolumn, &errmsg); 
   
   printf("nrow = %d\n",nrow);
   printf("ncolumn = %d\n",ncolumn);

    if(rc)
    {
       printf("query fail!\n");
    }
    else
    {
       printf("query success!\n");
       for(i = 1; i <= nrow; i++)
       {
          for(j = 0; j < ncolumn; j++)
          {
             printf("%s | ", Result[i * ncolumn + j]);
          }
          printf("\n");
       }
    }
   if(nrow > 0)
   {   
       for(i = 1; i <= nrow; i++)
       {
           printf("Result[cfd] = %s\n",Result[i * ncolumn + j - 1]);
           memset(buffer,0,sizeof(buffer));
           strcpy(buffer,Result[i * ncolumn + j - 1]);
       
           printf("buffer = %s\n",buffer);       
           cfd = atoi(buffer);
           printf("msg->cfd = %d",msg->cfd);
           if(cfd != cfd1)
           {
               write(cfd,msg,sizeof(struct message)); //自己不发给自己
               printf("\n发消息给客户端:action = %d id = %s name = %s passwd = %s online = %d msg = %s cfd= %d msg->toname = %s\n",msg->action,msg->id,msg->name,msg->passwd,msg->online,msg->msg,cfd,msg->toname);
           }
       }
   }


    sqlite3_free_table(Result);
    return 0;
}

