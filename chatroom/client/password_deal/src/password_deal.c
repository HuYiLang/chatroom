#include "../../include/myhead.h"

char Getch()      
{    
    int c = 0;      
    int res = 0;        
    struct termios org_opts, new_opts;     
    res = tcgetattr(STDIN_FILENO, &org_opts);   //用于获取与终端相关的参数    
    if(res == -1)  
    {  
        printf("tcgetattr error! Error code: %d，Error message: %s\n", errno, strerror(errno));   
        c = '\n';  
        return c;  
    }  
      
    memcpy(&new_opts, &org_opts, sizeof(new_opts));    
    new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);   
      
    res = tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);   //用于设置终端参数  
    if(res == -1)  
    {  
        printf("tcsetattr error! Error code: %d，Error message: %s\n", errno, strerror(errno));   
        c = '\n';  
        return c;  
    }  
    c = getchar();        
    res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);   
    if(res == -1)  
    {  
        printf("tcsetattr error! Error code: %d，Error message: %s\n", errno, strerror(errno));   
        c = '\n';  
        return c;  
    }  
      
    return c;    
}    
  
  
char *GetPasswd()  
{  
    char *strRet = (char *)malloc(64);  
    memset(strRet, 0, 64);  
      
    unsigned char c;  
    int i = 0;  
      
    while ((c = Getch()) != '\n')  
    {  
        if (i<64 && isprint(c))  
        {  
            strRet[i++] = c;  
            putchar('*');  
        }  
        else if ((i>0) && (c == '\b'))  
        {  
            --i;  
            putchar('\b');  
            putchar(' ');  
            putchar('\b');  
        }  
    }  
    strRet[i] = '\0';  
      
    return strRet;  
}  
  
 
int shift_passwd(char *str)  
{  
    printf("\n请输入你的密码：%s",GetPasswd()); 
    strcpy(str,GetPasswd());   
    printf("\n");
    return 0;   
}  
