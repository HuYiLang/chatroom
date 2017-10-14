#include "../../include/myhead.h"

int illegal(char *t)
{
    char *s = t;

    while(*s != '\0')
    {
       if(*s == ':')
       {
           printf("含有非法字符，请重新输入\n");
           return 0;
       }
       s++;
    }
    
    if(s = '\0')
    {
         return 1;
    }
}
