VERSION = 3.14.0

CC = gcc
CPP = g++
AR = ar

BOOST_DIR=/usr/local/include/boost

OUTPUT_DIR = output/
STATIC_LIB_TARGET = $(OUTPUT_DIR)/lib/libselib.a

LFLAGS = -lcrypto

CFLAGS = ${CFLAGS_BASIC} -Werror

CFLAGS_BASIC = -g -O2 -W -Wall -Wpointer-arith -fPIC \
	       -Wno-unused-parameter -Wunused-function -Wunused-variable \
	       -Wunused-value

SRC=./util/
CPPCLASS=$(wildcard $(SRC)/*.cpp)
OBJCLASS=$(CPPCLASS:.cpp=.o)

INCLUDEDIR = -I$(BOOST_DIR) -I./util/
BASE_LDLIB = -lboost_regex -lcurl -liconv -lcrypto -lm -lz -lpthread -lboost_filesystem -lrt -levent

default: dir static mkinclude

.PHONY: dir
dir:
	mkdir -p output/include output/lib

mkinclude:
	-cp util/*.h output/include/

%.o : %.cpp
	$(CPP) -c ${CFLAGS} -c -o $@ $< $(INCLUDEDIR) $(BASE_LDLIB)

static: $(OBJCLASS)
	$(AR) cr $(STATIC_LIB_TARGET) $(OBJCLASS)

.PHONY: clean
clean:
	-rm -rf output $(OBJCLASS)
