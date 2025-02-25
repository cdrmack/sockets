# CPPFLAGS = `pkg-config --cflags gtk4`
CFLAGS = -std=c23 -g -Wall -Werror
# LDFLAGS = `pkg-config --libs gtk4`

%.o:%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

all: server client
	@echo build complete

server: server.o
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o server server.o

client: client.o
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o client client.o

clean:
	rm -f server client server.o client.o
