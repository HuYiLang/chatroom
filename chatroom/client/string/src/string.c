#include "../../include/myhead.h"

int get_name_offlinemsg(char (*p)[1024],char *buffer)
{
   char buffer1[1024];
   int i = 0;
   int j = 0;
   int k = 0;
   
   while(buffer[i] != '\0')
   {
       while(buffer[i] != ':')
       {
           buffer1[j++] = buffer[i++];
       }
       buffer1[j] = '\0';

       j = 0;
       
       strcpy(p[k],buffer1);

       k++;
       bzero(buffer1,sizeof(buffer1));

       i++;
      
   }
   
   return k;
}

