#include "../../include/myhead.h"

static  sqlite3 *db=NULL;
static char **Result=NULL;
static char *errmsg=NULL;

int amend_column_name(struct message * msg)
{
   printf("更改列名中...\n");
   
   printf("\n更改列名函数:action = %d id = %s name = %s passwd = %s online = %d msg = %s cfd= %d msg->toname = %s\n",msg->action,msg->id,msg->name,msg->passwd,msg->online,msg->msg,msg->cfd,msg->toname);
   int rc, i, j;
   int nrow;
   int ncolumn;

   char sql[1024];

   rc= sqlite3_open("haoyu.db", &db);
   sprintf(sql,"select * from chatmsg where username = '%s'","root");
   rc= sqlite3_get_table(db, sql, &Result, &nrow, &ncolumn,&errmsg); 

    if(rc)
    {
       printf("query fail!\n");
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

    sqlite3_exec(db,"alter table chatmsg rename to chatmsg_temp",NULL,NULL,&errmsg); 
    sqlite3_exec(db,"create table chatmsg(username text)",NULL,NULL,&errmsg); 
    
    for(i = 1; i < ncolumn; i++)
    {
       if(strcmp(msg->msg,Result[i]) != 0)
       {
           sprintf(sql,"alter table chatmsg add column '%s' text",Result[i]);
           rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);
           if(rc == SQLITE_OK)
           {
               printf("success %s\n",sql);
           }
           else
           {
               printf("fail %s\n",sql);
           }
       }
       else
       {
           sprintf(sql,"alter table chatmsg add column '%s' text",msg->name);
           rc = sqlite3_exec(db,sql,NULL,NULL,&errmsg);       
           if(rc == SQLITE_OK)
           {
               printf("success %s\n",sql);
           }
           else
           {
               printf("fail %s\n",sql);
           }
       }
    }
    
    rc = sqlite3_exec(db,"insert into chatmsg select * from chatmsg_temp",NULL,NULL,&errmsg); 
      
    if(rc == SQLITE_OK)
    {
       printf("success insert \n");
    }
    else
    {
       printf("fail insert\n");
    }

    sqlite3_exec(db,"drop table chatmsg_temp",NULL,NULL,&errmsg); 
   
    sqlite3_free_table(Result);

    return 0;
}

