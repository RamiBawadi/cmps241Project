CC = gcc
CFLAGS = 
TARGET = gameOut

SRCS = main.c game.c ai.c server.c client.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Dependencies
main.o: main.c game.h ai.h
game.o: game.c game.h
ai.o: ai.c ai.h game.h
server.o: server.c server.h
client.o: client.c client.h


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
.PHONY: clean
.PHONY: gitclean
clean:
	rm -f $(OBJS) $(TARGET)

gitclean:
	git rm $(OBJS) $(TARGET)

	

