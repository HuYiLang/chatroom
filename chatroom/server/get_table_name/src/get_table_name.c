#include "../../include/myhead.h"

static  sqlite3 *db=NULL;
static char **Result=NULL;
static char *errmsg=NULL;

int get_table_name(struct message *msg)
{
   int rc, i, j;
   int nrow;
   int ncolumn;
   char sql[MAX_SIZE];
   char buffer[MAX_SIZE];

   rc= sqlite3_open("haoyu.db", &db);

   printf("msg->toname = %s\n",msg->toname);
   

   sprintf(sql,"select * from login where username = '%s'",msg->toname);

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

   
   if(nrow == 1)
   {
       printf("Result[] = %s\n",Result[ncolumn + 3]);
       memset(buffer,0,sizeof(buffer));
       strcpy(buffer,Result[ncolumn + 3]);
       
       printf("buffer = %s\n",buffer);       
       msg->online = atoi(buffer); //查看在线状态
       
       memset(buffer,0,sizeof(buffer));
       
       strcpy(buffer,Result[ncolumn + 4]);   //cfd
       
       printf("buffer = %s\n",buffer);       
       //msg->cfd = atoi(buffer);
       
   }
   else
   {
       msg->action = 0;
       printf("db don't have this person!\n"); 
   }

    printf("get_name client msg:action = %d id = %s name = %s passwd = %s online = %d msg = %s cfd = %d toname= %s\n",msg->action,msg->id,msg->name,msg->passwd,msg->online,msg->msg,msg->cfd,msg->toname);

    sqlite3_free_table(Result);
    return atoi(buffer); //cfd
}

