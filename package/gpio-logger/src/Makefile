CC      ?= $(CC)
CFLAGS  ?= -Wall -O2
LDFLAGS ?= -lgpiod

all: gpio-logger

gpio-logger: main.o
	$(echo "ESTOU BUILDANDO")
	$(CC) $(CFLAGS) -o gpio-logger main.o $(LDFLAGS)

clean:
	rm -f *.o gpio-logger

