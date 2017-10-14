/*************************************************
  Function:       // 函数说明：               face()
  Description:    // 函数功能、性能等的描述:  显示主界面
  Calls:          // 被本函数调用的函数清单： printf
  Called By:      // 调用本函数的函数清单:    no
  Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）:no
  Table Updated:  // 被修改的表（此项仅对于牵扯到数据库操作的程序）:no
  Input:          // 输入参数说明，包括每个参数的作
                  // 用、取值说明及参数间关系。:                    no
  Output:         // 对输出参数的说明。                            :界面
  Return:         // 函数返回值的说明                              :no
  Others:         // 其它说明                                      :no
*************************************************/
#include "../../include/myhead.h"


int face(char *str_login_name)
{ 
    int i;
    system("clear");
    printf("                    +=============================================+\n");
    printf("                    |                                             |\n");
    printf("                    |                 多功能聊天室                |\n");
    printf("                    |                                             |\n");
    printf("                    |                                             |\n");
    printf("                    |                                     版本:1.0|\n");
    printf("                    |---------------------------------------------|\n");
    printf("                    |   1:查看在线用户(show)  2:进行聊天(say)     |\n");
    printf("                    |---------------------------------------------|\n");
    printf("                    |   3:群发消息(stoall)    4:退出登录(off)     |\n");
    printf("                    |---------------------------------------------|\n");
    printf("                    |   5:修改密码(chg_psd)   6:修改昵称(chg_nam) |\n");
    printf("                    |---------------------------------------------|\n");
    printf("                    |   7:查看帮助(help)      8:退出聊天室(exit)  |\n");
    printf("                    |---------------------------------------------|\n");
    printf("                    |   9:查看聊天记录(history_msg)               |\n");
    printf("                    |---------------------------------------------|\n");
    printf("                    |---------------------------------------------|\n");
    printf("                    |  当前用户: %s                               ",str_login_name);
    for(i = 0;i < 33-strlen(str_login_name); i++)
    {
       printf(" ");
    }
    printf("\n");
    printf("                    +=============================================+ \n");

    if(strcmp(str_login_name,"root") == 0)
    {
       root_face();
    }
    printf("\n\n");

    printf("                    tip: 请输入你的选择(show/say/stoall/off/chg_psd/chg_nam/help/history_msg/exit) \n");

    printf("\n");
    
    return 0;
}

