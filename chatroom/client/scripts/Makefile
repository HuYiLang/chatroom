CC := gcc
CFLAGS := -Wall -O3
Libs = -lpthread -lsqlite3
Target := client
Source := $(wildcard src/*.c)
Objs := $(patsubst %.c,%.o,$(Source))
Modules +=   face illegal log_face main msg_main password_deal pth_read random_num root_face string time
AllObjs := $(addsuffix /src/*.o,$(Modules))

