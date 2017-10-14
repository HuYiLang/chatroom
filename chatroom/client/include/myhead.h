/*
 * myhead.h
 *
 *  Created on: 2011-7-27
 *      Author: root
 */

#ifndef MYHEAD_H_
#define MYHEAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <fcntl.h>
#include <sqlite3.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <termios.h> 
#include <time.h>

#define SER_PORT 3333
#define BUFMAX 256
#define MAX_SIZE 1024

struct message
{
    int action;
    char name[MAX_SIZE];
    char passwd[MAX_SIZE];
    char id[MAX_SIZE];
    char toname[MAX_SIZE];
    char msg[MAX_SIZE];
    int online;
    int cfd;
    char onlinename[MAX_SIZE];
};

enum Rusult{exitall = 0,log,reg,update_id,cg_pd = 4,cg_ne,off,show,say,stoall,historymsg,out = 11,ban,unban,exit_};

#endif /* MYHEAD_H_ */
