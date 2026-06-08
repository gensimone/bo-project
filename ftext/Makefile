CC = cc
CFLAGS = -Wall -Wextra -g -pthread -fno-stack-protector
LDFLAGS = -z execstack
DEPS = utils.h alloc.h strutil.h core.h queue.h
OBJ = ftext.o util.o alloc.o strutil.o core.o queue.o
TARGET = ftext

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ -lm

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
