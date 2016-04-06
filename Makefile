# $@--目标文件，$^--所有的依赖文件，$<--第一个依赖文件。

CC                     = cc
#SRC                    = $(wildcard *.c)
SRC                    = log.c test.c item.c str.c config.c tran.c socket.c server.c emul.c utils.c
#OBJ                    = $(patsubst %.c, %.o, $(SRC))
OBJ1                   = log.o item.o str.o config.o tran.o socket.o 
OBJ2                   = utils.o
OBJS                   = $(OBJ1) $(OBJ2)
LIBSERVER              = server
LIBEMULATION           = emulation
LIBTEST                = mytest
MDEFINE                = -DDEBUG
CFLAGS                 = $(MDEFINE)
RM                     = rm -f

all : $(LIBSERVER) $(LIBEMULATION) 

#$(LIBTEST) : $(OBJ1) test.o
#	@echo $(OBJ) "q111"
#	$(CC) -o $@ $^ $(MDEFINE)
#	$(CC) -g -o mytest $(OBJ1) test.o $(MDEFINE) -I./
	
$(LIBSERVER) : $(OBJ1) server.o
	$(CC) -g -o $(LIBSERVER) $(OBJ1) server.o
	
$(LIBEMULATION) : $(OBJS) emul.o
	$(CC) -g -o $(LIBEMULATION) $(OBJS) emul.o $(MDEFINE) -I./ -lpthread
	
.c.o:
	$(CC) $(CFLAGS) -g -c $<
	
clean :
	$(RM) $(LIBSERVER) $(LIBEMULATION) $(OBJS) server.o emul.o
	

