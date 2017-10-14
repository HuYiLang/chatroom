CC := gcc
CFLAGS := -Wall -O3
Libs = -lpthread -lsqlite3
Target := server
Source := $(wildcard src/*.c)
Objs := $(patsubst %.c,%.o,$(Source))
Modules +=  amend_column_name get_online_cfd get_table get_table_name history_msg main offline_msg scan_db pth_run
AllObjs := $(addsuffix /src/*.o,$(Modules))

