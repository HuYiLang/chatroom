/*************************************************
  Function:       // 函数说明：               print_main_window()
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


int log_face()
{ 
    system("clear");

	printf("                                                 \n");
	printf("        ======================================== \n");
	printf("       ||                                      ||\n");
	printf("       ||          WELCOME TO CHATROOM         ||\n");
	printf("       ||                                      ||\n");
	printf("       ||======================================||\n");
	printf("       ||                                      ||\n");
	printf("       ||        LOG       REG       EXIT      ||\n");
	printf("       ||                                      ||\n");
    printf("        ======================================== \n");
    printf("                                                 \n");
    printf("\n");
    printf("           请你选择:退出(exit)/登录(log)/注册(reg)：");
    printf("\n");

    return 0;
}

