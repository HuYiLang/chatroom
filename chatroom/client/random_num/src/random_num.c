#include "../../include/myhead.h"

int create_random_num()
{
   time_t t;
   srand((unsigned)time(&t));

   long k = rand()%(999999999-100000000) + 100000000;        //生成9位随机数

   return k;
}
