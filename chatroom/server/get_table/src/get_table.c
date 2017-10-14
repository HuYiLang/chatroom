#include "../../include/myhead.h"

static  sqlite3 *db=NULL;
static char **Result=NULL;
static char *errmsg=NULL;


int get_table(struct message *msg)
{
   int rc, i, j;
   int nrow;
   int ncolumn;
   char sql[MAX_SIZE];
   char buffer[MAX_SIZE];

   rc= sqlite3_open("haoyu.db", &db);

   printf("msg->id = %s\n",msg->id);
   printf("msg->passwd = %s\n",msg->passwd);

   sprintf(sql,"select * from login where id = '%s' and password = '%s'",msg->id,msg->passwd);

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
       strcpy(msg->name,Result[ncolumn]);
       memset(buffer,0,sizeof(buffer));
       strcpy(buffer,Result[ncolumn + 3]);
       
       printf("buffer = %s\n",buffer);       
       msg->online = atoi(buffer);

   }

   printf("name = %s passwd = %s id = %s online = %d msg = %s\n",msg->name,msg->passwd,msg->id,msg->online,msg->msg);

   sqlite3_free_table(Result);
   return 0;
}

